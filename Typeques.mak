#
# Borland C++ IDE generated makefile
# Generated 3/30/26 at 9:54:19 PM 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = TLink
TLIB    = TLib
TASM    = Tasm
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGSDOS =  -LC:\BC5\LIB
IDE_BFLAGS = 
CompLocalOptsAtDOS_typequesdexe =  -ml -f287 -dc- -Vf-
LinkerLocalOptsAtDOS_typequesdexe =  -c -Tde
ResLocalOptsAtDOS_typequesdexe = 
BLocalOptsAtDOS_typequesdexe = 
CompOptsAt_typequesdexe = $(CompLocalOptsAtDOS_typequesdexe)
CompInheritOptsAt_typequesdexe = -IC:\BC5\INCLUDE 
LinkerInheritOptsAt_typequesdexe = -m
LinkerOptsAt_typequesdexe = $(LinkerLocalOptsAtDOS_typequesdexe)
ResOptsAt_typequesdexe = $(ResLocalOptsAtDOS_typequesdexe)
BOptsAt_typequesdexe = $(BLocalOptsAtDOS_typequesdexe)
CompLocalOptsAtDOS_typequesdc = 
LinkerLocalOptsAtDOS_typequesdc = 
ResLocalOptsAtDOS_typequesdc = 
BLocalOptsAtDOS_typequesdc = 
CompOptsAt_typequesdc = $(CompOptsAt_typequesdexe) $(CompLocalOptsAtDOS_typequesdc)
CompInheritOptsAt_typequesdc = -IC:\BC5\INCLUDE 
LinkerInheritOptsAt_typequesdc = -m
LinkerOptsAt_typequesdc = $(LinkerOptsAt_typequesdexe) $(LinkerLocalOptsAtDOS_typequesdc)
ResOptsAt_typequesdc = $(ResOptsAt_typequesdexe) $(ResLocalOptsAtDOS_typequesdc)
BOptsAt_typequesdc = $(BOptsAt_typequesdexe) $(BLocalOptsAtDOS_typequesdc)
CompLocalOptsAtDOS_carddbdexe =  -ml -f
LinkerLocalOptsAtDOS_carddbdexe =  -c -Tde
ResLocalOptsAtDOS_carddbdexe = 
BLocalOptsAtDOS_carddbdexe = 
CompOptsAt_carddbdexe = $(CompLocalOptsAtDOS_carddbdexe)
CompInheritOptsAt_carddbdexe = -IC:\BC5\INCLUDE 
LinkerInheritOptsAt_carddbdexe = -x
LinkerOptsAt_carddbdexe = $(LinkerLocalOptsAtDOS_carddbdexe)
ResOptsAt_carddbdexe = $(ResLocalOptsAtDOS_carddbdexe)
BOptsAt_carddbdexe = $(BLocalOptsAtDOS_carddbdexe)
CompLocalOptsAtDOS_enemydbdexe =  -ml -f287
LinkerLocalOptsAtDOS_enemydbdexe =  -c -Tde
ResLocalOptsAtDOS_enemydbdexe = 
BLocalOptsAtDOS_enemydbdexe = 
CompOptsAt_enemydbdexe = $(CompLocalOptsAtDOS_enemydbdexe)
CompInheritOptsAt_enemydbdexe = -IC:\BC5\INCLUDE 
LinkerInheritOptsAt_enemydbdexe = -x
LinkerOptsAt_enemydbdexe = $(LinkerLocalOptsAtDOS_enemydbdexe)
ResOptsAt_enemydbdexe = $(ResLocalOptsAtDOS_enemydbdexe)
BOptsAt_enemydbdexe = $(BLocalOptsAtDOS_enemydbdexe)

#
# Dependency List
#
Dep_Typeques = \
   typeques.exe\
   carddb.exe\
   enemydb.exe

Typeques : BccDos.cfg $(Dep_Typeques)
  echo MakeNode

Dep_typequesdexe = \
   cards.h\
   cards.obj\
   enemies.h\
   enemies.obj\
   fonts.obj\
   fonts.h\
   battle.obj\
   battle.h\
   shared\enemy.h\
   shared\card.h\
   menu.h\
   menu.obj\
   timer.h\
   timer.obj\
   sound.h\
   sound.obj\
   ui.h\
   ui.obj\
   typedef.h\
   state.h\
   state.obj\
   keyboard.h\
   bmpfnt.h\
   keyboard.obj\
   bmpfnt.obj\
   bitmap.h\
   bitmap.obj\
   vga.h\
   vga.obj\
   draw.h\
   globals.h\
   draw.obj\
   globals.obj\
   typeques.obj

typeques.exe : $(Dep_typequesdexe)
  $(TLINK)   @&&|
 /v $(IDE_LinkFLAGSDOS) $(LinkerOptsAt_typequesdexe) $(LinkerInheritOptsAt_typequesdexe) +
C:\BC5\LIB\c0l.obj+
cards.obj+
enemies.obj+
fonts.obj+
battle.obj+
menu.obj+
timer.obj+
sound.obj+
ui.obj+
state.obj+
keyboard.obj+
bmpfnt.obj+
bitmap.obj+
vga.obj+
draw.obj+
globals.obj+
typeques.obj
$<,$*
C:\BC5\LIB\fp87.lib+
C:\BC5\LIB\mathl.lib+
C:\BC5\LIB\cl.lib



|
cards.obj :  cards.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ cards.c
|

enemies.obj :  enemies.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ enemies.c
|

fonts.obj :  fonts.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ fonts.c
|

battle.obj :  battle.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ battle.c
|

menu.obj :  menu.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ menu.c
|

timer.obj :  timer.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ timer.c
|

sound.obj :  sound.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ sound.c
|

ui.obj :  ui.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ ui.c
|

state.obj :  state.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ state.c
|

keyboard.obj :  keyboard.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ keyboard.c
|

bmpfnt.obj :  bmpfnt.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ bmpfnt.c
|

bitmap.obj :  bitmap.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ bitmap.c
|

vga.obj :  vga.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ vga.c
|

draw.obj :  draw.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ draw.c
|

globals.obj :  globals.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdexe) $(CompInheritOptsAt_typequesdexe) -o$@ globals.c
|

typeques.obj :  typeques.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_typequesdc) $(CompInheritOptsAt_typequesdc) -o$@ typeques.c
|

Dep_carddbdexe = \
   carddb.obj\
   shared\card.h

carddb.exe : $(Dep_carddbdexe)
  $(TLINK)   @&&|
 /v $(IDE_LinkFLAGSDOS) $(LinkerOptsAt_carddbdexe) $(LinkerInheritOptsAt_carddbdexe) +
C:\BC5\LIB\c0l.obj+
carddb.obj
$<,$*
C:\BC5\LIB\bidsl.lib+
C:\BC5\LIB\emu.lib+
C:\BC5\LIB\mathl.lib+
C:\BC5\LIB\cl.lib



|
carddb.obj :  carddb.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_carddbdexe) $(CompInheritOptsAt_carddbdexe) -o$@ carddb.c
|

Dep_enemydbdexe = \
   enemydb.obj\
   shared\enemy.h

enemydb.exe : $(Dep_enemydbdexe)
  $(TLINK)   @&&|
 /v $(IDE_LinkFLAGSDOS) $(LinkerOptsAt_enemydbdexe) $(LinkerInheritOptsAt_enemydbdexe) +
C:\BC5\LIB\c0l.obj+
enemydb.obj
$<,$*
C:\BC5\LIB\bidsl.lib+
C:\BC5\LIB\fp87.lib+
C:\BC5\LIB\mathl.lib+
C:\BC5\LIB\cl.lib



|
enemydb.obj :  enemydb.c
  $(BCCDOS) -P- -c @&&|
 $(CompOptsAt_enemydbdexe) $(CompInheritOptsAt_enemydbdexe) -o$@ enemydb.c
|

# Compiler configuration file
BccDos.cfg : 
   Copy &&|
-W-
-w
-R
-v
-vi
-H
-H=typeques.csm
| $@


