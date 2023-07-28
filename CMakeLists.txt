cmake_minimum_required(VERSION 3.19)

project(fpeExamples
LANGUAGES C CXX)

enable_testing()

include(CheckSymbolExists)
include(CheckSourceCompiles)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_C_COMPILER_ID MATCHES "(GNU|Clang)")
  add_compile_options(-Wall
  -Werror=implicit-function-declaration
  -Wno-unknown-pragmas
  )
elseif(CMAKE_C_COMPILER_ID MATCHES "Intel")
  # error: '#pragma STDC FENV_ACCESS ON' is illegal when precise is disabled
  # https://www.intel.com/content/www/us/en/docs/cpp-compiler/developer-guide-reference/2021-10/fp-model-fp.html
  add_compile_options(-fp-model=precise)
endif()

# not all libc have the fenv.h exception functions.
# the prototype might be in the header, but not in the library.
# so we need to check if the function is available.
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
set(CMAKE_REQUIRED_LIBRARIES m)  # -lm required for Linux fe* functions
check_symbol_exists(feenableexcept "fenv.h" HAVE_FEENABLEEXCEPT)

if(NOT HAVE_FEENABLEEXCEPT)
  check_source_compiles(C "#include <fenv.h>
  int main(void) { fenv_t env; env.__fpcr; return 0; }" HAVE_FPCR)
  if(NOT HAVE_FPCR)
    message(WARNING "no FP exception handling available on this platform")
  endif()
endif()

add_executable(feenv_demo feenv_demo.c)
add_test(NAME feeenv COMMAND feenv_demo)
target_link_libraries(feenv_demo PRIVATE m)

add_executable(test_fpe fpeh.c)
target_link_libraries(test_fpe PRIVATE m)
target_compile_definitions(test_fpe PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
)

add_executable(test_fpe_cpp fpeh.cpp)
target_link_libraries(test_fpe_cpp PRIVATE m)
target_compile_definitions(test_fpe_cpp PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
)

add_executable(fetestexcept_demo fetestexcept_demo.cpp)
target_link_libraries(fetestexcept_demo PRIVATE m)
add_test(NAME fetestexcept_cpp COMMAND fetestexcept_demo)
set_property(TEST fetestexcept_cpp PROPERTY LABELS "Cpp")

foreach(fpe IN ITEMS 1 0)

add_test(NAME Invalid_FPE${fpe} COMMAND test_fpe 1 ${fpe})
add_test(NAME Overflow_FPE${fpe} COMMAND test_fpe 2 ${fpe})
add_test(NAME Underflow_FPE${fpe} COMMAND test_fpe 3 ${fpe})
add_test(NAME DivideByZero_FPE${fpe} COMMAND test_fpe 4 ${fpe})
add_test(NAME QuietNaN_FPE${fpe} COMMAND test_fpe 5 ${fpe})
add_test(NAME SignalNaN_FPE${fpe} COMMAND test_fpe 6 ${fpe})

endforeach()

# Overflow didn't fire on NVHPC 23.5
set(fire_tests Invalid_FPE1 Overflow_FPE1 DivideByZero_FPE1 SignalNaN_FPE1)

# Underflow may not fire on all platforms
if(NOT CMAKE_C_COMPILER_ID MATCHES "(GNU|NVHPC)")
  list(APPEND fire_tests Underflow_FPE1)
endif()

set_property(TEST ${fire_tests} PROPERTY
  PASS_REGULAR_EXPRESSION "Floating point exception encountered")

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  file(GENERATE OUTPUT .gitignore CONTENT "*")
endif()