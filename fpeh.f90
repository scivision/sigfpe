program fpeh

use, intrinsic :: ieee_exceptions
use, intrinsic :: ieee_arithmetic
use, intrinsic :: iso_fortran_env

implicit none

integer :: i
logical :: dummy

character :: buf
character(13) :: name
type(ieee_flag_type) :: flag

!> IEEE_MODE_TYPE is Fortran 2018; only oneAPI supports at this time.
! TYPE(IEEE_MODE_TYPE) :: modes
! CALL IEEE_GET_MODES(modes)
!print *, "IEEE modes: ", modes

print '(a)', compiler_version()
print '(a)', "Boolean values: Support_exception, support_halt, halt_mode, status_before, status_after, hex_value"


call get_command_argument(1, buf, status=i)
if (i == 0) then
  read(buf, '(i1)') i
  call enum_flag(i, name, flag)
  if(test_exception(name, flag)) error stop
else
  do i = 1, 6
    call enum_flag(i, name, flag)
    dummy = test_exception(name, flag)
    call ieee_set_flag(flag, .false.)
  end do
endif

contains

elemental subroutine enum_flag(i, name, flag)

integer, intent(in) :: i
character(*), intent(out) :: name
type(ieee_flag_type), intent(out) :: flag

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
  error stop "mode: 1: invalid  2: overflow 3: underflow 4: divzero 5: quiet NaN 6: signaling NaN"
end select

end subroutine enum_flag


logical function test_exception(name, flag)

character(*), intent(in) :: name
type(ieee_flag_type), intent(in) :: flag

type(ieee_flag_type) :: flags(5) = [ieee_invalid, ieee_overflow, ieee_underflow, ieee_divide_by_zero, ieee_inexact]
logical :: flags_status(5)

test_exception = check_exception(flag, name)

if (test_exception) then
  write(error_unit, '(a)') name // " Floating point exception encountered"
  return
endif

call ieee_get_flag(flags, flags_status)
print '(a, 5L2)', name // " no FPE encountered: ", flags_status

end function test_exception


logical function check_exception(flag, name)

type(ieee_flag_type), intent(in) :: flag
character(*), intent(in) :: name

logical :: support_exception, support_halting, halting_mode, before
real, volatile :: r

support_exception = ieee_support_flag(flag)
support_halting = ieee_support_halting(flag)
call ieee_get_halting_mode(flag, halting_mode)
call ieee_get_flag(flag, before)

select case (i)
case (1)
  r = 2-3
  r = sqrt(r)
case (2)
  r = 3000.
  r = exp(r)
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
print '(a14, 5L2, 2x, z8)', name, support_exception, support_halting, halting_mode, before, check_exception, r


end function check_exception

end program fpeh
