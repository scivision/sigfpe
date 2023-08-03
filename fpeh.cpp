// https://en.cppreference.com/w/c/numeric/fenv

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* before headers, necessary on Linux */

#ifdef HAVE_PRAGMA_STDC_FENV
#pragma STDC FENV_ACCESS ON
#endif

#include <csignal>      // for signal()
#include <cfenv>       // for fegetenv(), fesetenv()
#include <limits>
#include <cmath>

#include <iostream>
#include <cstdlib>

#include <cstring> // for memcpy()
#include <cinttypes> // for PRIx64


void fpe_signal_handler(int sig) {
    std::cerr << "Floating point exception encountered\n";
    switch (sig)
    {
        case FE_INVALID:
            std::cerr << "Invalid value detected\n";
            exit(77);
            break;
        case FE_DIVBYZERO:
            std::cerr << "Division by zero detected\n";
            exit(77);
            break;
        case FE_OVERFLOW:
            std::cerr << "Overflow detected\n";
            exit(77);
            break;
        case FE_UNDERFLOW:
            std::cerr << "Underflow detected\n";
            exit(77);
            break;
        case FE_INEXACT:
            std::cerr << "Inexact detected\n";
            exit(77);
            break;
        default :
            std::cerr << "Unknown exception detected\n";
            exit(77);
            break;
    }
}

void enable_floating_point_exceptions()
{

#ifdef HAVE_FEENABLEEXCEPT /* Linux */
 feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW);
 // | FE_INEXACT
 // it's typical to omit inexact as it trips on typical operations
#elif defined(HAVE_FPCR)
 fenv_t env;
 if(std::fegetenv(&env)){
    std::cerr << "fegetenv() failed\n";
    exit(EXIT_FAILURE);
 }

 env.__fpcr = env.__fpcr | FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW;
 std::cout << "set env var __fpcr to " << env.__fpcr  << "\n";

 if(std::fesetenv(&env)){
    std::cerr << "fesetenv() failed\n";
    exit(EXIT_FAILURE);
 }
#else
  std::cerr << "FPE signal handling not enabled\n";
  return;
#endif

 std::signal(SIGFPE, fpe_signal_handler);
}


static
void set_qnan(double *f)
{
    *f = std::numeric_limits<double>::quiet_NaN();
}

static
void set_snan(double* f)
{
    *f = std::numeric_limits<double>::signaling_NaN();
}

static
void print_hexval(double x)
{
    /* Kind of cool - didn't know it was possible to do this */
    uint64_t xn;
    memcpy(&xn, &x, sizeof x);
    printf("x = %16f (%" PRIx64 ")\n", x, xn);
}

static
double create_exception(int choice)
{
    double x,y;
    switch(choice)
    {
        case 1:
        x = -1;
        y = std::sqrt(x);  // Or acos(x-1), log(x)
        std::cout << "1: INVALID :";
        print_hexval(y);
        break;

        case 2:
        x = 100000;
        y = std::exp(x);
        std::cout << "2: OVERFLOW :";
        print_hexval(y);
        break;

        case 3:
        y = std::numeric_limits<double>::min() / 10;
        std::cout << "3: UNDERFLOW :";
        print_hexval(y);
        break;

        case 4:
        x = 0;
        y = 1/x;
        std::cout << "4: DIVBYZERO :";
        print_hexval(y);
        break;

        case 5:
        set_qnan(&x);
        std::cout << "5: Quiet NAN (not trapped) :";
        print_hexval(x);
        y = x + 1;
        break;

        case 6:
        set_snan(&x);
        std::cout << "6: Signaling NAN (trapped) :";
        print_hexval(x);
        y = x + 1;
        break;

        default:
        y = -1;
        std::cout << "Usage : tst_fpe <fpe_id>\n";
        std::cout << "        fpe_id in [1-5]\n\n";
        exit(EXIT_FAILURE);
    }
    return y;
}

void show_fe_exceptions(void)
{
    // from https://en.cppreference.com/w/c/numeric/fenv/feenv
    std::cout << "current exceptions raised: ";
    if(std::fetestexcept(FE_DIVBYZERO))     std::cout << " FE_DIVBYZERO";
    // if(std::fetestexcept(FE_INEXACT))       std::cout << " FE_INEXACT";
    if(std::fetestexcept(FE_INVALID))       std::cout << " FE_INVALID";
    if(std::fetestexcept(FE_OVERFLOW))      std::cout << " FE_OVERFLOW";
    if(std::fetestexcept(FE_UNDERFLOW))     std::cout << " FE_UNDERFLOW";
    if(std::fetestexcept(FE_ALL_EXCEPT)==0) std::cout << " none";
    std::cout << "\n";
}

void check_for_fpe(void)
{
    // this method is for non-Linux systems for which the typical FPE trap
    // signal() doesn't fire because the OS/CPU/microcode works differently than Linux.
    // Instead of signal(), the user code would call this function as desired to check
    // if an FPE occurred.
    if(std::fetestexcept(FE_DIVBYZERO))     fpe_signal_handler(FE_DIVBYZERO);
    if(std::fetestexcept(FE_INEXACT))       fpe_signal_handler(FE_INEXACT);
    if(std::fetestexcept(FE_INVALID))       fpe_signal_handler(FE_INVALID);
    if(std::fetestexcept(FE_OVERFLOW))      fpe_signal_handler(FE_OVERFLOW);
    if(std::fetestexcept(FE_UNDERFLOW))     fpe_signal_handler(FE_UNDERFLOW);
}

int main(int argc, char** argv)
{

    if (argc < 1){
        std::cerr << "specify fpe integer mode (1-6)\n";
        return EXIT_FAILURE;
    }

    int fpe_id = atoi(argv[1]);

    enable_floating_point_exceptions();

    show_fe_exceptions();
    double y = create_exception(fpe_id);

    if (std::fetestexcept(FE_ALL_EXCEPT)){
        std::cerr << "Should not get here, using alternate check function\n";
        check_for_fpe();
        return EXIT_FAILURE;
    }

    show_fe_exceptions();

    std::cout << "Result : y = " << y << "\n";

    return EXIT_SUCCESS;
}
