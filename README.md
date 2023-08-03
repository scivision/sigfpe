# Apple Silicon exception handling

[![ci](https://github.com/scivision/sigfpe/actions/workflows/ci.yml/badge.svg)](https://github.com/scivision/sigfpe/actions/workflows/ci.yml)

[Original idea](https://github.com/ForestClaw/forestclaw/pull/201/files)

This example implements DYI
[floating point](https://en.cppreference.com/w/c/numeric/fenv)
exception handling.

This approach is intended Unix-like operating systems only at this time (Linux, macOS, ...).

Windows (MinGW GCC, MSVC) does also work with the "check" function vs. signaling.
Windows signaling could be a
[future extension](https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2012/e9b52ceh(v=vs.110)).

It can be tested with

```sh
test_fpe <fpe_id>
```

where <fpe_id> is an FPE type to trap.

Valid types are indicated by integer 1-5 :

1 : INVALID, e.g. sqrt(-1), log(-1), acos(2) etc.

2 : OVERFLOW, e.g. 2^1024, exp(1000)

3 : UNDERFLOW, e.g. denormalized numbers such as 2^(-1074), exp(-710)

4 : DIVBYZERO, e.g. 1/0

5 : Quiet NAN : This is useful for detecting use of uninitialized data

6 : Signaling NAN : This NAN will get trapped.

Ideas for code:

* https://developer.apple.com/forums/thread/689159
* https://stackoverflow.com/questions/69059981/how-to-trap-floating-point-exceptions-on-m1-macs

## Fortran

### macOS

GNU Fortran 13.1.0, macOS Apple Silicon

```
GCC version 13.1.0
...
set quiet NaN                         7FC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set signaling NaN                         7FE00000
Status: divzero, inexact, invalid, overflow, underflow  F F T F F
set overflow                         7F7FFFFF
Status: divzero, inexact, invalid, overflow, underflow  F T T F F
set div by zero                         7F800000
Status: divzero, inexact, invalid, overflow, underflow  T T T F F
```

### Linux


GCC 11.4.1: Linux x86_64

```
GCC version 11.4.1 20230605 (Red Hat 11.4.1-2)
...
set quiet NaN                         FFC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set signaling NaN                         FFC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set overflow                         7F7FFFFF
Status: divzero, inexact, invalid, overflow, underflow  F T F F F
set div by zero                         7F800000
Status: divzero, inexact, invalid, overflow, underflow  T T F F F
```

GCC 12.2.1: Linux x86_64

```
GCC version 12.2.1 20221121 (Red Hat 12.2.1-7)
set quiet NaN                         FFC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set signaling NaN                         FFC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set overflow                         7F7FFFFF
Status: divzero, inexact, invalid, overflow, underflow  F T F F F
set div by zero                         7F800000
Status: divzero, inexact, invalid, overflow, underflow  T T F F F
```

Intel oneAPI: Linux x86_64

```
Intel(R) Fortran Compiler for applications running on Intel(R) 64, Version 2023.2.0 Build 20230622
...
set quiet NaN                         7FC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set signaling NaN                         7FE00000
Status: divzero, inexact, invalid, overflow, underflow  F F T F F
set overflow                         7F7FFFFF
Status: divzero, inexact, invalid, overflow, underflow  F T T F F
set div by zero                         7F800000
Status: divzero, inexact, invalid, overflow, underflow  T T T F F
```

Nvidia HPC SDK: Linux x86_64

```
nvfortran 23.5-0
...
set quiet NaN                         7FC00000
Status: divzero, inexact, invalid, overflow, underflow  F F F F F
set signaling NaN                         7FE00000
Status: divzero, inexact, invalid, overflow, underflow  F F T F F
set overflow                         7F7FFFFF
Status: divzero, inexact, invalid, overflow, underflow  F T T F F
set div by zero                         7F800000
Status: divzero, inexact, invalid, overflow, underflow  T T T F F
```


### Windows


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
