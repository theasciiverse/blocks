# makefile for block - by default we try to use ncurses,
# but if you specify cblock or cbed it will use curses.
#
# If you are confident you should be able to do a 
# <make install> immediately. 

#the various flags and options
INCLUDE= -I/usr/include/ncurses/
CC= gcc
CFLAGS= -$(INCLUDE) -Wall
LL= -lncurses -ltermcap
CLL= -lcurses -ltermcap

#makes the game - this is default (has colors and keyboard flushing) 
block  : block.c
	$(CC) $(INCLUDE) -DUCOL -DNEW -s -o block block.c $(LL)

#make the editor
bed    : bed.c
	$(CC) $(INCLUDE) -s -o bed bed.c $(LL)

#makes the game, leaving out the newer ncurses options (keyboard) and colors
old    : block.c
	$(CC) $(INCLUDE) -s -o block block.c $(LL)

#makes the game, with no color  
nocolor: block.c
	$(CC) $(INCLUDE) -DNEW -s -o block block.c $(LL)

#makes the game, using curses instead of ncurses
cblock : block.c
	$(CC)  -DCURSES -s -o block block.c $(CLL)

#makes the editor, using curses instead of ncurses 
cbed   : bed.c
	$(CC) -DCURSES -s -o bed bed.c $(CLL)

#makes the game and editor
all    : block bed

#installs the entire game on your system -- you have to be root
install: block bed
	install -d /usr/games     #if you do not have <install> comment out this
	mkdir      /usr/games/lib
	mkdir	   /usr/games/lib/save
	chmod 777  /usr/games/lib/save
	cp block   /usr/games
	cp bed     /usr/games
	chmod 755  /usr/games/block
	chmod 755  /usr/games/bed
	cp 1.block /usr/games/lib/1.block
	cp 2.block /usr/games/lib/2.block
	cp 3.block /usr/games/lib/3.block
	cp 4.block /usr/games/lib/4.block
	cp 5.block /usr/games/lib/5.block
	cp 6.block /usr/games/lib/6.block
	chmod 644  /usr/games/lib/1.block
	chmod 644  /usr/games/lib/2.block
	chmod 644  /usr/games/lib/3.block
	chmod 644  /usr/games/lib/4.block
	chmod 644  /usr/games/lib/5.block
	chmod 644  /usr/games/lib/6.block
	install -d /usr/man/cat6  # and this line  
	cp block.6 /usr/man/cat6/block.6
	chmod 644  /usr/man/cat6/block.6 
	cp bed.6   /usr/man/cat6/bed.6
	chmod 644  /usr/man/cat6/bed.6 
