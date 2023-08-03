include(CheckSymbolExists)
include(CheckSourceCompiles)


if(CMAKE_C_COMPILER_ID MATCHES "(GNU|Clang)")
  add_compile_options(-Wall
  $<$<COMPILE_LANGUAGE:C>:-Werror=implicit-function-declaration>
  )
elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
  # error: '#pragma STDC FENV_ACCESS ON' is illegal when precise is disabled
  # https://www.intel.com/content/www/us/en/docs/cpp-compiler/developer-guide-reference/2021-10/fp-model-fp.html
  add_compile_options($<IF:$<BOOL:${WIN32}>,/fp:precise,-fp-model=precise>)
elseif(CMAKE_C_COMPILER_ID STREQUAL "NVHPC")
  add_compile_options("$<$<COMPILE_LANGUAGE:C,CXX>:--diag_suppress=unrecognized_stdc_pragma>")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-fsignaling-nans>)
endif()

if(CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
 # https://gcc.gnu.org/onlinedocs/gfortran/IEEE-modules.html
 add_compile_options("$<$<COMPILE_LANGUAGE:Fortran>:-fno-unsafe-math-optimizations;-frounding-math;-fsignaling-nans>")
  # add_compile_options("$<$<COMPILE_LANGUAGE:Fortran>:-ffpe-trap=invalid$<COMMA>zero$<COMMA>overflow>")
endif()

# ---- feature checks
# not all libc have the fenv.h exception functions.
# the prototype might be in the header, but not in the library.
# so we need to check if the function is available.
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE -D__STDC_WANT_IEC_60559_BFP_EXT__)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_REQUIRED_FLAGS "-fsignaling-nans")
endif()
if(NOT MSVC)
  set(CMAKE_REQUIRED_LIBRARIES m)  # -lm required for Linux fe* functions
endif()

check_symbol_exists(feenableexcept "fenv.h" HAVE_FEENABLEEXCEPT)

if(NOT HAVE_FEENABLEEXCEPT)
  check_source_compiles(C "#include <fenv.h>
  int main(void) { fenv_t env; env.__fpcr; return 0; }" HAVE_FPCR)
  if(NOT HAVE_FPCR)
    message(WARNING "no FP exception handling available on this platform")
  endif()
endif()

check_symbol_exists(SNAN  "fenv.h" HAVE_SNAN)

# check_symbol_exists(__SUPPORT_SNAN__ "fenv.h" SUPPORT_SNAN)
# __SUPPORT_SNAN__ is often undefined but we can still use signaling NaNs

if(NOT MSVC)
  set(CMAKE_REQUIRED_FLAGS "-Werror -Wall")
endif()

check_source_compiles(C "#pragma STDC FENV_ACCESS ON
int main(void) { return 0; }" HAVE_PRAGMA_STDC_FENV)

set(libm  $<$<NOT:$<BOOL:${MSVC}>>:m>)
