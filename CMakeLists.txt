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
target_link_libraries(feenv_demo PRIVATE ${libm})
target_compile_definitions(feenv_demo PRIVATE
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)
add_test(NAME feeenv COMMAND feenv_demo)
set_property(TEST feeenv PROPERTY LABELS "C")

add_executable(test_fpe_C fpeh.c)
target_link_libraries(test_fpe_C PRIVATE ${libm})
target_compile_definitions(test_fpe_C PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
$<$<BOOL:${HAVE_FPCR}>:HAVE_FPCR>
$<$<BOOL:${HAVE_SNAN}>:HAVE_SNAN>
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)

add_executable(test_fpe_CXX fpeh.cpp)
target_link_libraries(test_fpe_CXX PRIVATE ${libm})
target_compile_definitions(test_fpe_CXX PRIVATE
$<$<BOOL:${HAVE_FEENABLEEXCEPT}>:HAVE_FEENABLEEXCEPT>
$<$<BOOL:${HAVE_FPCR}>:HAVE_FPCR>
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)

if(fortran)
  add_executable(test_fpe_Fortran fpeh.f90)
endif()

add_executable(fetestexcept_demo_CXX fetestexcept_demo.cpp)
target_link_libraries(fetestexcept_demo_CXX PRIVATE ${libm})
target_compile_definitions(fetestexcept_demo_CXX PRIVATE
$<$<BOOL:${HAVE_PRAGMA_STDC_FENV}>:HAVE_PRAGMA_STDC_FENV>
)
add_test(NAME fetestexcept_CXX COMMAND fetestexcept_demo_CXX)
set_property(TEST fetestexcept_CXX PROPERTY LABELS "CXX")

set(names Invalid Overflow DivideByZero QuietNaN SignalNaN)
# Underflow omitted on purpose, it may trip inexact which is not useful.
set(index 1 2 4 5 6)

get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)

foreach(i name IN ZIP_LISTS index names)
  foreach(L IN LISTS languages)

      add_test(NAME ${name}_FPE_${L} COMMAND test_fpe_${L} ${i})
      set_property(TEST ${name}_FPE_${L} PROPERTY LABELS ${L})

  endforeach()
endforeach()

# Overflow didn't fire on NVHPC 23.5
set(fire_tests)
foreach(name IN ITEMS Invalid Overflow DivideByZero SignalNaN)
  foreach(L IN LISTS languages)
    list(APPEND fire_tests ${name}_FPE_${L})
  endforeach()
endforeach()

set_property(TEST ${fire_tests} PROPERTY
  PASS_REGULAR_EXPRESSION "Floating point exception encountered")

set_property(TEST SignalNaN_FPE_C SignalNaN_FPE_CXX
  PROPERTY DISABLED $<NOT:$<BOOL:${SUPPORT_SNAN}>>
)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  file(GENERATE OUTPUT .gitignore CONTENT "*")
endif()
