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
