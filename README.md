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

Results:

* [Fortran](./results_Fortran.md)

Ideas for code:

* https://developer.apple.com/forums/thread/689159
* https://stackoverflow.com/questions/69059981/how-to-trap-floating-point-exceptions-on-m1-macs
