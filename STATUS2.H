! Created by Johan Berntsson, e-mail: goodday@ppp.bekkoame.ne.jp
! See Status.h for details on usage and distribution.
!
! Abstract: Enables the user to select which kind of statusline to
!           use.
! 
! Usage: Include "Status";    ! must be included before Parser.h
!        ...
!        Include "Parser.h";  ! standard library
!        ...
!        Include "Status2.h"; ! The main file
!
! Grammar:
!   statusline
!
! Constant(s) & Variable(s):
!   SL_COMPASS
!   SL_WIDTH_CALC
!
! Replaced routine(s):
!   DrawStatusLine
!
Verb meta "sl" "status" "statusline"
  *                 -> StatusHelp
  * "on"            -> StatusOn
  * "off"           -> StatusOff
  * "score"         -> StatusScoreOn
  * "score" "on"    -> StatusScoreOn
  * "score" "off"   -> StatusScoreOff
  * "time"          -> StatusTimeOn
  * "time" "on"     -> StatusTimeOn
  * "time" "off"    -> StatusTimeOff
  * "compass"       -> StatusCompassOn
  * "compass" "on"  -> StatusCompassOn
  * "compass" "off" -> StatusCompassOff
  * "exits"         -> StatusExitsOn
  * "exits" "on"    -> StatusExitsOn
  * "exits" "off"   -> StatusExitsOff;
[ StatusHelpSub;
  print "The status line layout can be modified using this syntax:^^";
  font off;
  print "statusline on          - show the status line^";
  print "statusline off         - hide the status line^";
  print "statusline score       - show score (if notify on) and turns^";
  print "statusline score off   - hide score and turns^";
  if(the_time~=NULL) print "statusline time        - show time^";
  if(the_time~=NULL) print "statusline time off    - hide time^";
  print "statusline compass     - show the compass rose^";
  print "statusline compass off - hide the compass rose^";
  print "statusline exits       - show the exits^";
  print "statusline exits off   - hide the exits^";
  font on;
  print "^Please note that if the screen is too narrow some elements will not be shown.^";
];
[ StatusOnSub;
  if(SL_TYPE~=0) "Already enabled.";
  SL_TYPE=1; DrawStatusLine(); "Ok.";
];
[ StatusOffSub;
  if(SL_TYPE==0) "Already disabled.";
  SL_TYPE=0; DrawStatusLine(); "Ok.";
];
[ StatusScoreOnSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  1,2: "Already switched on.";
  4,6: SL_TYPE=2;
  7,9: SL_TYPE=8;
  default: SL_TYPE=1;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusScoreOffSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  3,4,5,6: "Already switched off.";
  2: SL_TYPE=6;
  8: SL_TYPE=7;
  default: SL_TYPE=5;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusTimeOnSub;
  if(the_time==NULL) "This game doesn't use time.";
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  3,4: "Already switched on.";
  2,6: SL_TYPE=4;
  7,8: SL_TYPE=9;
  default: SL_TYPE=3;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusTimeOffSub;
  if(the_time==NULL) "This game doesn't use time.";
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  1,2,5,6: "Already switched off.";
  3: SL_TYPE=6;
  9: SL_TYPE=7;
  default: SL_TYPE=5;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusCompassOnSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  2,4: "Already switched on.";
  1,8: SL_TYPE=2;
  3,9: SL_TYPE=4;
  default: SL_TYPE=6;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusCompassOffSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  1,3,5: "Already switched off.";
  2,8: SL_TYPE=1;
  4,9: SL_TYPE=3;
  default: SL_TYPE=5;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusExitsOnSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  1,2: SL_TYPE=8;
  3,4: SL_TYPE=9;
  7,8,9: "Already switched on.";
  default: SL_TYPE=7;
  }
  DrawStatusLine(); "Ok.";
];
[ StatusExitsOffSub;
  switch(SL_TYPE) {
  0: "The statusline is disabled.";
  default: SL_TYPE=1;
  }
  DrawStatusLine(); "Ok.";
];


Array SL_WIDTH_CALC table 40;
[ DrawStatusLine type width i room_len;
   ! Indata: type: statusline to display (0 gives SL_TYPE)
   width=0->33;

   ! Calculate width of room name (had a constant value of 20 before)
   @output_stream 3 SL_WIDTH_CALC; PrintShortName(location); @output_stream -3;
   room_len=SL_WIDTH_CALC-->0;

   ! Set default for SL_TYPE
   if(SL_TYPE==-1) if(width>room_len+13) SL_TYPE=1; else SL_TYPE=5;

   ! if no type argument given (or ==0), use SL_TYPE
   if(type==0) type=SL_TYPE;

   switch(type) {
   0:@split_window 0; @set_window 0; rtrue;
   1:! location, score/turns
     @split_window 1; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     if(width>room_len+13) {
       if(width<room_len+26) {
         i=width-13; @set_cursor 1 i;
         if(notify_mode==1) print score, "/", turns; else print turns;
       } else {
         if(notify_mode==1) {
           i=width-26; @set_cursor 1 i; print "Score: ", score;
         }
         i=width-13; @set_cursor 1 i; print "Moves: ", turns;
       }
     }
   2:! location, compass, score/turns
     i=(width-SL_COMPASS)/2;
     if(i<room_len)
       i=room_len+(width-room_len-SL_COMPASS)/2;
     if(i<room_len) return DrawStatusLine(1);
     @split_window 3; @set_window 1; style reverse;
     @set_cursor 1 1; spaces(0->33);
     @set_cursor 2 1; spaces(0->33);
     @set_cursor 3 1; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     DrawCompassRose(i);
     if(width>room_len+SL_COMPASS+13) {
       if(notify_mode==1) {
         i=width-13; @set_cursor 1 i; print "Score: ", score;
         i=width-13; @set_cursor 2 i; print "Moves: ", turns;
       } else {
         i=width-13; @set_cursor 1 i; print "Moves: ", turns;
       }
     } else {
       @set_cursor 3 2;
       if(notify_mode==1) print score, "/", turns; else print turns;
     }
   3:! location, time
     @split_window 1; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     if(width>room_len+15) {
       i=width-15; @set_cursor 1 i; print "Time: ";
       i=(the_time/60)%12; if (i<10) print " ";
       if (i==0) i=12;
       print i, ":";
       if ((the_time%60)<10) print "0";
       print (the_time%60);
       if (((the_time/60)/12) > 0) print " pm"; else print " am";
     }
   4:! location, compass, time
     i=(width-SL_COMPASS)/2;
     if(i<room_len)
       i=room_len+(width-room_len-SL_COMPASS)/2;
     if(i<room_len) return DrawStatusLine(3);
     @split_window 3; @set_window 1; style reverse;
     @set_cursor 1 1; spaces(0->33);
     @set_cursor 2 1; spaces(0->33);
     @set_cursor 3 1; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     DrawCompassRose(i);
     if(width>room_len+SL_COMPASS+15) {
       i=width-15; @set_cursor 1 i; print "Time: ";
       i=(the_time/60)%12; if (i<10) print " ";
       if (i==0) i=12;
       print i, ":";
       if ((the_time%60)<10) print "0";
       print (the_time%60);
       if (((the_time/60)/12) > 0) print " pm"; else print " am";
     } else {
       @set_cursor 3 2; print "Time: ";
       i=(the_time/60)%12; if (i<10) print " ";
       if (i==0) i=12;
       print i, ":";
       if ((the_time%60)<10) print "0";
       print (the_time%60);
       if (((the_time/60)/12) > 0) print " pm"; else print " am";
     }
   5:! location
     @split_window 1; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     i=(width-room_len)/2; if(i<2) i=2;
     @set_cursor 1 i; PrintShortName(location);
   6:! location, compass
     i=(width-SL_COMPASS)/2;
     if(i<room_len)
       i=room_len+(width-room_len-SL_COMPASS)/2;
     if(i<room_len) return DrawStatusLine(5);
     @split_window 3; @set_window 1; style reverse;
     @set_cursor 1 1; spaces(0->33);
     @set_cursor 2 1; spaces(0->33);
     @set_cursor 3 1; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     DrawCompassRose(i);
   7:! location, exits
     @split_window 2; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     @set_cursor 2 1; style reverse; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     @set_cursor 2 2; PrintExits(true);
   8:! location, score, exits
     @split_window 2; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     @set_cursor 2 1; style reverse; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     if(width>room_len+13) {
       if(width<room_len+26) {
         i=width-13; @set_cursor 1 i;
         if(notify_mode==1) print score, "/", turns; else print turns;
       } else {
         if(notify_mode==1) {
           i=width-26; @set_cursor 1 i; print "Score: ", score;
         }
         i=width-13; @set_cursor 1 i; print "Moves: ", turns;
       }
     }
     @set_cursor 2 2; PrintExits(true);
   9:! location, time, exits
     @split_window 2; @set_window 1;
     @set_cursor 1 1; style reverse; spaces(0->33);
     @set_cursor 2 1; style reverse; spaces(0->33);
     @set_cursor 1 2; PrintShortName(location);
     if(width>room_len+15) {
       i=width-15; @set_cursor 1 i; print "Time: ";
       i=(the_time/60)%12; if (i<10) print " ";
       if (i==0) i=12;
       print i, ":";
       if ((the_time%60)<10) print "0";
       print (the_time%60);
       if (((the_time/60)/12) > 0) print " pm"; else print " am";
     }
     @set_cursor 2 2; PrintExits(true);
   };
   @set_cursor 1 1; style roman; @set_window 0; font on;
];

[ PrintExits x i;
  if(location==thedark) return 0;

  i=6;
  if(x) print "Exits:";

  if(location.n_to~=0 && metaclass(location.n_to)~=String) { i=i+2; if(x) print " N"; }
  if(location.s_to~=0 && metaclass(location.s_to)~=String) { i=i+2; if(x) print " S"; }
  if(location.e_to~=0 && metaclass(location.e_to)~=String) { i=i+2; if(x) print " E"; }
  if(location.w_to~=0 && metaclass(location.w_to)~=String) { i=i+2; if(x) print " W"; }
  if(location.nw_to~=0 && metaclass(location.nw_to)~=String) { i=i+3; if(x) print " NW"; }
  if(location.ne_to~=0 && metaclass(location.ne_to)~=String) { i=i+3; if(x) print " NE"; }
  if(location.sw_to~=0 && metaclass(location.sw_to)~=String) { i=i+3; if(x) print " SW"; }
  if(location.se_to~=0 && metaclass(location.se_to)~=String) { i=i+3; if(x) print " SE"; }
  if(location.u_to~=0 && metaclass(location.u_to)~=String) { i=i+2; if(x) print " U"; }
  if(location.d_to~=0 && metaclass(location.d_to)~=String) { i=i+2; if(x) print " D"; }
  if(location.in_to~=0 && metaclass(location.in_to)~=String) { i=i+3;if(x) print " In"; }
  if(location.out_to~=0 && metaclass(location.out_to)~=String){i=i+4;if(x) print " Out";}

  return i;
];
    
[ DrawCompassRose x i;
  if(location~=thedark) {
    i=x+6; @set_cursor 2 i; print "*";
    if(location.u_to~=0 && metaclass(location.u_to)~=String) {
      i=x+0; @set_cursor 1 i; print "Up";}
    if(location.nw_to~=0 && metaclass(location.nw_to)~=String) {
      i=x+5; @set_cursor 1 i; print "@@92";}
    if(location.n_to~=0 && metaclass(location.n_to)~=String) {
      i=x+6; @set_cursor 1 i; print "@@124";}
    if(location.ne_to~=0 && metaclass(location.ne_to)~=String) {
      i=x+7; @set_cursor 1 i; print "/";}
    if(location.in_to~=0 && metaclass(location.in_to)~=String) {
      i=x+9; @set_cursor 1 i; print "In";}
    if(location.w_to~=0 && metaclass(location.w_to)~=String){
      i=x+5; @set_cursor 2 i; print "-";}
    if(location.e_to~=0 && metaclass(location.e_to)~=String) {
      i=x+7; @set_cursor 2 i; print "-";}
    if(location.d_to~=0 && metaclass(location.d_to)~=String)  {
      i=x+0; @set_cursor 3 i; print "Down";}
    if(location.sw_to~=0 && metaclass(location.sw_to)~=String) {
      i=x+5; @set_cursor 3 i; print "/";}
    if(location.s_to~=0 && metaclass(location.s_to)~=String)  {
      i=x+6; @set_cursor 3 i; print "@@124";}
    if(location.se_to~=0 && metaclass(location.se_to)~=String) {
      i=x+7; @set_cursor 3 i; print "@@92";}
    if(location.out_to~=0 && metaclass(location.out_to)~=String) {
      i=x+9; @set_cursor 3 i; print "Out";}
  }
];
