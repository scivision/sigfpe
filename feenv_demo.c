// from https://en.cppreference.com/w/c/numeric/fenv/feenv

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>

#ifdef HAVE_PRAGMA_STDC_FENV
#pragma STDC FENV_ACCESS ON
#endif

void show_fe_exceptions(void)
{
    printf("current exceptions raised: ");
    if(fetestexcept(FE_DIVBYZERO))     printf(" FE_DIVBYZERO");
    if(fetestexcept(FE_INEXACT))       printf(" FE_INEXACT");
    if(fetestexcept(FE_INVALID))       printf(" FE_INVALID");
    if(fetestexcept(FE_OVERFLOW))      printf(" FE_OVERFLOW");
    if(fetestexcept(FE_UNDERFLOW))     printf(" FE_UNDERFLOW");
    if(fetestexcept(FE_ALL_EXCEPT)==0) printf(" none");
    printf("\n");
}

void show_fe_rounding_method(void)
{
    printf("current rounding method:    ");
    switch (fegetround()) {
           case FE_TONEAREST:  printf ("FE_TONEAREST");  break;
           case FE_DOWNWARD:   printf ("FE_DOWNWARD");   break;
           case FE_UPWARD:     printf ("FE_UPWARD");     break;
           case FE_TOWARDZERO: printf ("FE_TOWARDZERO"); break;
           default:            printf ("unknown");
    };
    printf("\n");
}

void show_fe_environment(void)
{
    show_fe_exceptions();
    show_fe_rounding_method();
}

int main(void)
{
    fenv_t curr_env;

    /* Show default environment. */
    show_fe_environment();
    printf("\n");

    /* Perform some computation under default environment. */
    printf("+11.5 -> %+4.1f\n", rint(+11.5)); /* midway between two integers */
    printf("+12.5 -> %+4.1f\n", rint(+12.5)); /* midway between two integers */
    show_fe_environment();
    printf("\n");

    /* Save current environment. */
    if(fegetenv(&curr_env)){
        fprintf(stderr, "fegetenv() failed\n");
        return EXIT_FAILURE;
    }

    /* Perform some computation with new rounding method. */
    feclearexcept(FE_ALL_EXCEPT);
    fesetround(FE_DOWNWARD);
#ifndef _MSC_VER
    printf("1.0/0.0 = %f\n", 1.0/0.0);
#endif
    printf("+11.5 -> %+4.1f\n", rint(+11.5));
    printf("+12.5 -> %+4.1f\n", rint(+12.5));
    show_fe_environment();
    printf("\n");

    /* Restore previous environment. */
    if(fesetenv(&curr_env)){
        fprintf(stderr, "fesetenv() failed\n");
        return EXIT_FAILURE;
    }
    show_fe_environment();

    return EXIT_SUCCESS;
}
