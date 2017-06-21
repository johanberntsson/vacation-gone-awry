! History:
! * 961105 First version created
! * 970402 New version to handle non-standard screen sizes
! * 970626 When only showing room name the string is now properly centered
!
! Created by Johan Berntsson, e-mail: goodday@ppp.bekkoame.ne.jp
! This routine can be used in any game for free, provided 
! that all references to me (Johan Berntsson) are kept.
!
! 
! Usage: See Status2.h

System_file;
#IFNDEF DEBUG;
Message "Thank you for using Statusline.h by Johan Berntsson";
#ENDIF;

Constant SL_COMPASS  12;
Global SL_TYPE=-1;
Replace DrawStatusLine;
