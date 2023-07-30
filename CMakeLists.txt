cmake_minimum_required(VERSION 3.19)

project(fpeExamples
LANGUAGES C CXX)

enable_testing()

option(fortran "build Fortran examples")

if(fortran)
  enable_language(Fortran)
endif()

message(STATUS "CMake ${CMAKE_VERSION}   ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}
 CPU arch: ${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include(cmake/compilers.cmake)

add_executable(feenv_demo feenv_demo.c)
target_compile_definitions(feenv_demo PRIVATE
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)
add_test(NAME feeenv COMMAND feenv_demo)
target_link_libraries(feenv_demo PRIVATE ${libm})

add_executable(test_fpe_c fpeh.c)
target_link_libraries(test_fpe_c PRIVATE ${libm})
target_compile_definitions(test_fpe_c PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
$<$<BOOL:${HAVE_FPCR}>:HAVE_FPCR>
$<$<BOOL:${HAVE_SNAN}>:HAVE_SNAN>
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)

add_executable(test_fpe_cpp fpeh.cpp)
target_link_libraries(test_fpe_cpp PRIVATE ${libm})
target_compile_definitions(test_fpe_cpp PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
$<$<BOOL:${HAVE_FPCR}>:HAVE_FPCR>
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)

add_executable(fetestexcept_demo_cpp fetestexcept_demo.cpp)
target_link_libraries(fetestexcept_demo_cpp PRIVATE ${libm})
target_compile_definitions(fetestexcept_demo_cpp PRIVATE
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)
add_test(NAME fetestexcept_cpp COMMAND fetestexcept_demo_cpp)
set_property(TEST fetestexcept_cpp PROPERTY LABELS "Cpp")

foreach(fpe IN ITEMS 1 0)
  foreach(L IN ITEMS c cpp)

    add_test(NAME Invalid_FPE${fpe}_${L}
      COMMAND test_fpe_${L} 1 ${fpe})
    add_test(NAME Overflow_FPE${fpe}_${L}
      COMMAND test_fpe_${L} 2 ${fpe})
    add_test(NAME DivideByZero_FPE${fpe}_${L}
      COMMAND test_fpe_${L} 4 ${fpe})
    add_test(NAME QuietNaN_FPE${fpe}_${L}
      COMMAND test_fpe_${L} 5 ${fpe})
    add_test(NAME SignalNaN_FPE${fpe}_${L}
      COMMAND test_fpe_${L} 6 ${fpe})

    # This test may trip FP_INEXACT on some platforms e.g. macOS intel CPU
    # add_test(NAME Underflow_FPE${fpe}_${L} COMMAND test_fpe 3 ${fpe})
  endforeach()
endforeach()

# Overflow didn't fire on NVHPC 23.5
set(fire_tests
Invalid_FPE1_c Invalid_FPE1_cpp
Overflow_FPE1_c Overflow_FPE1_cpp
DivideByZero_FPE1_c DivideByZero_FPE1_cpp
SignalNaN_FPE1_c SignalNaN_FPE1_cpp
)

set_property(TEST ${fire_tests} PROPERTY
  PASS_REGULAR_EXPRESSION "Floating point exception encountered")

set_property(TEST
SignalNaN_FPE0_c SignalNaN_FPE1_c
SignalNaN_FPE0_cpp SignalNaN_FPE1_cpp
  PROPERTY DISABLED $<NOT:$<BOOL:${SUPPORT_SNAN}>>
)

if(fortran)
  add_executable(fpeh_fortran fpeh.f90)
  add_test(NAME FPEH_Fortran COMMAND fpeh_fortran)
endif()

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  file(GENERATE OUTPUT .gitignore CONTENT "*")
endif()
