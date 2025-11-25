#
# Borland C++ IDE generated makefile
# Generated 11/2/25 at 11:22:14 AM 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCCDOS  = Bcc +BccDos.cfg 
TLINK   = C:\TC\BIN\TLink
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
LinkerLocalOptsAtDOS_typequesdexe =  -c -Tde /S:8192
ResLocalOptsAtDOS_typequesdexe = 
BLocalOptsAtDOS_typequesdexe = 
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

#
# Dependency List
#
Dep_Typeques = \
   typeques.exe

Typeques : BccDos.cfg $(Dep_Typeques)
  echo MakeNode

Dep_typequesdexe = \
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
-ml
-f287
| $@


