##################################
# Makefile for the rhythm sample
#
# using emx 0.8h, mm4emx 1.0
#
#
# Marc E.E. van Woerkom, 2/94
#

CC=gcc
CFLAGS=-Wall -Zomf
LFLAGS=-Zomf -Zsys -los2me -liostream -s


all: rhythm.exe

.cc.obj:
    $(CC) $(CFLAGS) -c $*.cc

.obj.exe:
    $(CC) -o $*.exe $*.obj $*.def $(LFLAGS)


# clean up
clean:
    del *.obj

