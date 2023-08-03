# Fortran results

Generate results by:

```sh
cmake -Bbuild
cmake --build build

build/test_fpe_Fortran
```

## macOS

GNU Fortran 13.1.0, macOS Apple Silicon

```
GCC version 13.1.0
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  7FC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F T
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```

## Linux


GCC 11.4.1: Linux x86_64

```
GCC version 11.4.1 20230605 (Red Hat 11.4.1-2)
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  FFC00000
quiet NaN     no FPE encountered:  F F F F F
 signaling NaN T T F F F  FFC00000
signaling NaN no FPE encountered:  F F F F F
```

GCC 12.2.1: Linux x86_64

```
GCC version 12.2.1 20221121 (Red Hat 12.2.1-7)
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  FFC00000
quiet NaN     no FPE encountered:  F F F F F
 signaling NaN T T F F F  FFC00000
signaling NaN no FPE encountered:  F F F F F
```

GCC 13.1.1: Linux x86_64

```
GCC version 13.1.1 20230614 (Red Hat 13.1.1-4)
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F T
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```

Intel oneAPI: Linux x86_64

```
Intel(R) Fortran Compiler for applications running on Intel(R) 64, Version 2023.2.0 Build 20230622
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F T
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```

Nvidia HPC SDK: Linux x86_64
NVHPC doesn't detect overflow with nvfortran 23.7.

```
nvfortran 23.7-0
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F F  7F800000
overflow      no FPE encountered:  F F F F F
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F F
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F F
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```


## Windows


GCC 13.1.0, Windows MSYS2

```
GCC version 13.1.0
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F F  7F800000
overflow      no FPE encountered:  F F F F F
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F F
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F F
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```

Intel oneAPI: Windows x86_64

```
Intel(R) Fortran Compiler for applications running on Intel(R) 64, Version 2023.1.0 Build 20230320
Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value
 invalid       T T F F T  FFC00000
invalid       Floating point exception encountered
 overflow      T T F F T  7F800000
overflow      Floating point exception encountered
 underflow     T T F F F    400000
underflow     no FPE encountered:  F F F F T
 divzero       T T F F T  7F800000
divzero       Floating point exception encountered
 quiet NaN     T T F F F  7FC00000
quiet NaN     no FPE encountered:  F F F F T
 signaling NaN T T F F T  7FE00000
signaling NaN Floating point exception encountered
```
