// https://en.cppreference.com/w/c/numeric/fenv

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* before headers, necessary on Linux */

#pragma STDC FENV_ACCESS ON

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
    /* would like non-zero exit code, but that interferes with CTest */
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
 feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT);
#elif defined(HAVE_FPCR)
 fenv_t env;
 if(std::fegetenv(&env)){
    std::cerr << "fegetenv() failed\n";
    exit(EXIT_FAILURE);
 }

 env.__fpcr = env.__fpcr | FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT;
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
    if(std::fetestexcept(FE_INEXACT))       std::cout << " FE_INEXACT";
    if(std::fetestexcept(FE_INVALID))       std::cout << " FE_INVALID";
    if(std::fetestexcept(FE_OVERFLOW))      std::cout << " FE_OVERFLOW";
    if(std::fetestexcept(FE_UNDERFLOW))     std::cout << " FE_UNDERFLOW";
    if(std::fetestexcept(FE_ALL_EXCEPT)==0) std::cout << " none";
    std::cout << "\n";
}


int main(int argc, char** argv)
{
    int fpe_id = 1;
    int use_eh = 0;

    if (argc > 1)
        fpe_id = std::atoi(argv[1]);
    if (argc > 2)
        use_eh = std::atoi(argv[2]);

    if (use_eh){
        std::cout << "Enabling exception handling\n";
        enable_floating_point_exceptions();
    }

    show_fe_exceptions();
    double y = create_exception(fpe_id);
    show_fe_exceptions();

    std::cout << "Result : y = " << y << "\n";

    return EXIT_SUCCESS;
}
