program fpeh

use, intrinsic :: ieee_exceptions
use, intrinsic :: ieee_arithmetic
use, intrinsic :: iso_fortran_env

implicit none

integer :: i, istat

type(ieee_flag_type) :: flag
character :: buf
character(13) :: name
character(*), parameter :: help = "mode: 1: invalid  2: overflow 3: underflow 4: divzero 5: quiet NaN 6: signaling NaN"

!> IEEE_MODE_TYPE is Fortran 2018; only oneAPI supports at this time.
! TYPE(IEEE_MODE_TYPE) :: modes
! CALL IEEE_GET_MODES(modes)
!print *, "IEEE modes: ", modes

print '(a)', compiler_version()

call get_command_argument(1, buf, status=istat)
if (istat /= 0) error stop help

read(buf, '(i1)') i

select case (i)
case (1)
  name = "invalid"
  flag = ieee_invalid
case (2)
  name = "overflow"
  flag = ieee_overflow
case (3)
  name = "underflow"
  flag = ieee_underflow
case (4)
  name = "divzero"
  flag = ieee_divide_by_zero
case (5)
  name = "quiet NaN"
  flag = ieee_invalid
case (6)
  name = "signaling NaN"
  flag = ieee_invalid
case default
  error stop help
end select

if (check_exception(flag, name)) error stop name // " Floating point exception encountered"

print '(a)', name // " no FPE encountered"

contains


logical function check_exception(flag, name)

type(ieee_flag_type), intent(in) :: flag
character(*), intent(in) :: name

logical :: support_exception, support_halting, halting_mode
real, volatile :: r

support_exception = ieee_support_flag(flag)
support_halting = ieee_support_halting(flag)
call ieee_get_halting_mode(flag, halting_mode)
call ieee_get_flag(flag, check_exception)
print '(a, 4L2)', name // ' Support_exception, support_halting, halting_mode, status_before: ',&
   support_exception, support_halting, halting_mode, check_exception

select case (i)
case (1)
  r = 2-3
  r = sqrt(r)
case (2)
  call random_number(r)
  r = huge(0.) + r
case (3)
  r = tiny(0.) * 0.5
case (4)
  r = 0
  r = 1 / r
case (5)
  r = ieee_value(0., ieee_quiet_nan)
case (6)
  r = ieee_value(0., ieee_signaling_nan) * 0.
end select

call ieee_get_flag(flag, check_exception)
print '(a13,a,z8,L2)', name, " value, status after: ", r, check_exception


end function check_exception

end program fpeh
