program fpeh

use, intrinsic :: ieee_exceptions
use, intrinsic :: ieee_arithmetic
use, intrinsic :: iso_fortran_env

implicit none

real, volatile :: sNaN, qNaN, r
integer :: i
logical :: L
logical, dimension(5) :: halt, status
type(ieee_flag_type), dimension(5), parameter :: flags = &
    [ieee_divide_by_zero, ieee_inexact, ieee_invalid, ieee_overflow, ieee_underflow]
character(9), parameter :: names(5) = [character(9) :: 'divzero ', 'inexact ', 'invalid ', 'overflow', 'underflow']

!> IEEE_MODE_TYPE is Fortran 2018; only oneAPI supports at this time.
! TYPE(IEEE_MODE_TYPE) :: modes
! CALL IEEE_GET_MODES(modes)
!print *, "IEEE modes: ", modes

print '(a)', compiler_version()

do i = 1,size(halt)
  print '(a,2L2)', names(i) // ' Support exception, halting ', ieee_support_flag(flags(i)), ieee_support_halting(flags(i))
  call ieee_get_halting_mode(flags(i), L)
  print '(a,L2)', names(i) // ' halting mode ', L
enddo


call print_status_flags(flags, status)
print '(a)',''

!> Quiet NaN
qNaN = ieee_value(0., ieee_quiet_nan)
print '(a,z32)', "set quiet NaN ", qNaN
call print_status_flags(flags, status)

!> Signaling NaN
sNan = ieee_value(0., ieee_signaling_nan) * 0.
print '(a,z32)', "set signaling NaN ", sNaN
call print_status_flags(flags, status)

!> Overflow
call random_number(r)
print '(a,z32)', "set overflow ", huge(0.) + r
call print_status_flags(flags, status)

!> Div by zero
print '(a,z32)', "set div by zero ", r / 0.
call print_status_flags(flags, status)


contains

subroutine print_status_flags(flags, status)

type(ieee_flag_type), dimension(5), intent(in) :: flags
logical, dimension(5), intent(out) :: status

call ieee_get_flag(flags, status)
print '(a,5L2)', 'Status: divzero, inexact, invalid, overflow, underflow ', status

end subroutine print_status_flags

end program fpeh
