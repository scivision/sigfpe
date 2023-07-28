// https://en.cppreference.com/w/c/numeric/fenv

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* before headers, necessary on Linux */

#define __STDC_WANT_IEC_60559_BFP_EXT__
/* before fenv.h, enables SNAN
https://en.cppreference.com/w/c/experimental/fpext1
*/

#pragma STDC FENV_ACCESS ON

#include <signal.h>      // for signal()
#include <fenv.h>       // for fegetenv(), fesetenv()
#include <math.h>
#include <float.h>

#include <stdio.h>
#include <stdlib.h>

/* Needed for print_hexval, below */
#include <string.h>    /* Needed for memcpy */
#include <inttypes.h>  /* Needed for PRIx64 */


void fpe_signal_handler(int sig) {
    fprintf(stderr, "Floating point exception encountered\n");
    /* would like non-zero exit code, but that interferes with CTest */
    switch (sig)
    {
        case FE_INVALID:
            fprintf(stderr, "Invalid value detected\n");
            exit(77);
            break;
        case FE_DIVBYZERO:
            fprintf(stderr, "Division by zero detected\n");
            exit(77);
            break;
        case FE_OVERFLOW:
            fprintf(stderr, "Overflow detected\n");
            exit(77);
            break;
        case FE_UNDERFLOW:
            fprintf(stderr, "Underflow detected\n");
            exit(77);
            break;
        case FE_INEXACT:
            fprintf(stderr, "Inexact detected\n");
            exit(77);
            break;
        default :
            fprintf(stderr, "Unknown exception detected\n");
            exit(77);
            break;
    }
}

void enable_floating_point_exceptions()
{

#ifdef HAVE_FEENABLEEXCEPT /* Linux */
 feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT);
#elif defined(HAVE_FPCR)
// this code was intended for macOS Apple Silicon, but signal() doesn't fire
 fenv_t env;
 if(fegetenv(&env)){
    fprintf(stderr, "fegetenv() failed\n");
    exit(EXIT_FAILURE);
 }

 env.__fpcr = env.__fpcr | FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW | FE_INEXACT;
 printf("set env var __fpcr to %llx\n", env.__fpcr);

 if(fesetenv(&env)){
    fprintf(stderr, "fesetenv() failed\n");
    exit(EXIT_FAILURE);
 }
#else
fprintf(stderr, "FPE signal handling not enabled\n");
return;
#endif

 signal(SIGFPE, fpe_signal_handler);
}


static
void set_qnan(double *f)
{
    /*
     The quiet nan from math.h
    */
    *f = NAN;
}

inline
void set_snan(double* f)
{
#ifndef _MSC_VER
    *f = 0.0 / 0.0;

    // https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
    // *f = SNAN;
#else
    /* From :
      "NaNs, Uninitialized Variables, and C++"
      http://codingcastles.blogspot.fr/2008/12/nans-in-c.html
    */
     *((long long*)f) = 0x7ff0000000000001LL;
#endif
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
        y = sqrt(x);  // Or acos(x-1), log(x)
        printf("1: INVALID :");
        print_hexval(y);
        break;

        case 2:
        x = 100000;
        y = exp(x);
        printf("2: OVERFLOW :");
        print_hexval(y);
        break;

        case 3:
        y = DBL_MIN / 10;
        printf("3: UNDERFLOW :");
        print_hexval(y);
        break;

        case 4:
        x = 0;
        y = 1/x;
        printf("4: DIVBYZERO :");
        print_hexval(y);
        break;

        case 5:
        set_qnan(&x);
        printf("5: Quiet NAN (not trapped) :");
        print_hexval(x);
        y = x + 1;
        break;

        case 6:
        set_snan(&x);
        printf("6: Signaling NAN (trapped) :");
        print_hexval(x);
        y = x + 1;
        break;

        default:
        y = -1;
        printf("Usage : tst_fpe <fpe_id>\n");
        printf("        fpe_id in [1-5]\n\n");
        exit(EXIT_FAILURE);
    }
    return y;
}

void show_fe_exceptions(void)
{
    // from https://en.cppreference.com/w/c/numeric/fenv/feenv
    printf("current exceptions raised: ");
    if(fetestexcept(FE_DIVBYZERO))     printf(" FE_DIVBYZERO");
    if(fetestexcept(FE_INEXACT))       printf(" FE_INEXACT");
    if(fetestexcept(FE_INVALID))       printf(" FE_INVALID");
    if(fetestexcept(FE_OVERFLOW))      printf(" FE_OVERFLOW");
    if(fetestexcept(FE_UNDERFLOW))     printf(" FE_UNDERFLOW");
    if(fetestexcept(FE_ALL_EXCEPT)==0) printf(" none");
    printf("\n");
}

void check_for_fpe(void)
{
    // this method is for systems like macOS Apple Silicon for which the typical FPE trap
    // signal() method doesn't fire because the OS/CPU/microcode works differently than
    // Linux arm64/x86_64 and macOS x86_64.
    // Instead of signal(), the user code would call this function as desired to check
    // if an FPE occurred.
    if(fetestexcept(FE_DIVBYZERO))     fpe_signal_handler(FE_DIVBYZERO);
    if(fetestexcept(FE_INEXACT))       fpe_signal_handler(FE_INEXACT);
    if(fetestexcept(FE_INVALID))       fpe_signal_handler(FE_INVALID);
    if(fetestexcept(FE_OVERFLOW))      fpe_signal_handler(FE_OVERFLOW);
    if(fetestexcept(FE_UNDERFLOW))     fpe_signal_handler(FE_UNDERFLOW);
}


int main(int argc, char** argv)
{
    int fpe_id = 1;
    int use_eh = 0;

    if (argc > 1)
        fpe_id = atoi(argv[1]);
    if (argc > 2)
        use_eh = atoi(argv[2]);

    if (use_eh){
        printf("Enabling exception handling\n");
        enable_floating_point_exceptions();
    }

    show_fe_exceptions();
    double y = create_exception(fpe_id);

    if (use_eh && fetestexcept(FE_ALL_EXCEPT)){
        fprintf(stderr, "Should not get here, using alternate check function\n");
        check_for_fpe();
        return EXIT_FAILURE;
    }

    show_fe_exceptions();

    printf("Result : y = %g\n",y);

    return EXIT_SUCCESS;
}
