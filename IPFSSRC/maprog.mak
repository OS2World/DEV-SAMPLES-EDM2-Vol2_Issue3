.SUFFIXES:
.SUFFIXES: .c .obj .dll .csc .sc .h .ih .ph .psc .rc .res

SOMTEMP = .\somtemp
SCPATH  = \toolkt20\sc
HPATH   = \toolkt20\c\os2h
LIBPATH = \toolkt20\os2lib

!if [set SMINCLUDE=.;$(SCPATH);] || \
    [set SMTMP=$(SOMTEMP)] || \
    [set SMEMIT=ih;h;ph;psc;sc;c;def]
!endif

!if [cd $(SOMTEMP)]
!  if [md $(SOMTEMP)]
!    error error creating $(SOMTEMP) directory
!  endif
!else
!  if [cd ..]
!    error - Couldn't cd .. from $(SOMTEMP) directory
!  endif
!endif

#
# Compiler/tools Macros
#

CC      = icc /c /Gd- /Se /Re /ss /Ms /Gm+ /Ge- /W3 /Wpro
LINK    = LINK386
LDFLAGS = /noi /map /nol /nod /exepack /packcode /packdata /align:16
DLLLIBS = DDE4MBS + OS2386 + som.lib

#****************************************************************************
# Set up Macros that will contain all the different dependencies for the
# executables and dlls etc. that are generated.
#****************************************************************************

OBJS = maprog.obj

#****************************************************************************
#   Setup the inference rules for compiling source code to
#   object code.
#****************************************************************************

.c.obj:
        $(CC) -I$(HPATH) -c $<

.csc.c:
        sc -v -r $*.csc

all: maprog.dll

#
# Specific Process Tag
#

maprog.c:   $$(@B).csc $(HPATH)\wpdataf.h

maprog.obj: $$(@B).c $$(@B).ih $$(@B).h  $$(@B).sc $(HPATH)\wpdataf.h

maprog.dll: $$(@B).def $(OBJS) maprog.res
         $(LINK) $(LDFLAGS) $(OBJS),$@,,$(DLLLIBS),$*;
         rc $*.res $*.dll
         mapsym maprog.map
         install

maprog.res: $$(@B).rc $$(@B).ih $$(@B).ico
         rc -r $*.rc $*.res

