# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
SOURCES     = ./src
INCFLAGS	:= -I${INCLUDES}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99
SRC = ./src

# CCVER := $(shell gcc --version | grep -e '^gcc' | sed -r -e 's/^.*([[:digit:]])\..*$/\1/p')
# VFLAGS :=
# ifneq "${CCVER}" "4"
# 	  VFLAGS := -no-pie
# endif


# The list of objects to include in the library

LIBEIOBJS	:= ${OBJDIR}/ei_application.o \
		   ${OBJDIR}/ei_frameclass.o \
		   ${OBJDIR}/ei_buttonclass.o\
		   ${OBJDIR}/ei_geometrymanager.o \
		   ${OBJDIR}/ei_widget.o \
		   ${OBJDIR}/ei_widgetclass.o \
		   ${OBJDIR}/ei_geometrymanager.o \
		   ${OBJDIR}/ei_placer.o \
		   ${OBJDIR}/ei_draw.o \
		   ${OBJDIR}/ei_draw_utils.o \
		   ${OBJDIR}/ei_event.o \
		   ${OBJDIR}/ei_event_utils.o \
		   ${OBJDIR}/ei_toplevelclass.o \
		   ${OBJDIR}/ei_application_utils.o \





# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lfreeimage -lSDL -lSDL_ttf -lSDL_gfx -framework AppKit

else
	ifeq (${PLATFORM},Linux)

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL

	LINK		= ${CC} #${VFLAGS}
	LIBEI		= ${OBJDIR}/libei.a
#	ARCH	        = 32
	ARCH	        = 64
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -L${PLATDIR} -lSDL -lSDL_ttf -lSDL_gfx${ARCH} -lfreeimage${ARCH} -lm -lstdc++

	else

	# Building for Windows

	PLATDIR		= _win
	INCFLAGS	:= ${INCFLAGS} -I/usr/include -I/usr/include/SDL -I/usr/local/include/SDL
	LINK		= ${CC} -mwindows -mno-cygwin
	LIBEI		= ${OBJDIR}/libei.lib
	LIBEIBASE	= ${PLATDIR}/libeibase.lib
	LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a -L/usr/lib -lSDL -lFreeImage

	endif
endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal frame button hello_world \
			puzzle minesweeper two048 toplevel_sibling \
			toplevel_fils_de_toplevel \
			button_in_frame_in_toplevel #parsing
all : ${TARGETS}



########## Test-programs

# Compilation des sources

$(OBJDIR)/%.o: ${SOURCES}/%.c
	${CC} ${CCFLAGS} ${INCFLAGS} -c -o $@ $<

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS}


${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${OBJDIR}/hello_world.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

# parsing
#
# parsing : ${OBJDIR}/parsing.o ${LIBEIBASE} ${LIBEI}
# 	${LINK} -o parsing ${LDFLAGS} ${OBJDIR}/parsing.o ${LIBEI} ${LIBS}
#
# ${OBJDIR}/parsing.o : ${TESTS}/parsing.c
# 	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/parsing.c -o ${OBJDIR}/parsing.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o

# two048

two048 : ${OBJDIR}/two048.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${OBJDIR}/two048.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o

# minesweeper

minesweeper : ${OBJDIR}/minesweeper.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o minesweeper ${OBJDIR}/minesweeper.o ${LIBEI} ${LIBS}

${OBJDIR}/minesweeper.o : ${TESTS}/minesweeper.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minesweeper.c -o ${OBJDIR}/minesweeper.o

#Une frame dans une frame

frame_dans_frame : ${OBJDIR}/frame_dans_frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame_dans_frame ${OBJDIR}/frame_dans_frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame_dans_frame.o : ${TESTS}/frame_dans_frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame_dans_frame.c -o ${OBJDIR}/frame_dans_frame.o

#Toplevel dans toplevel

toplevel_fils_de_toplevel : ${OBJDIR}/toplevel_fils_de_toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel_fils_de_toplevel ${OBJDIR}/toplevel_fils_de_toplevel.o ${LIBEI} ${LIBS}

${OBJDIR}/toplevel_fils_de_toplevel.o : ${TESTS}/toplevel_fils_de_toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel_fils_de_toplevel.c -o ${OBJDIR}/toplevel_fils_de_toplevel.o

#Toplevel avec un frere toplevel

toplevel_sibling : ${OBJDIR}/toplevel_sibling.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel_sibling ${OBJDIR}/toplevel_sibling.o ${LIBEI} ${LIBS}

${OBJDIR}/toplevel_sibling.o : ${TESTS}/toplevel_sibling.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel_sibling.c -o ${OBJDIR}/toplevel_sibling.o

#Toplevel contenant un frame contenant un bouton

button_in_frame_in_toplevel : ${OBJDIR}/button_in_frame_in_toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button_in_frame_in_toplevel ${OBJDIR}/button_in_frame_in_toplevel.o ${LIBEI} ${LIBS}

${OBJDIR}/button_in_frame_in_toplevel.o : ${TESTS}/button_in_frame_in_toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button_in_frame_in_toplevel.c -o ${OBJDIR}/button_in_frame_in_toplevel.o



# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}

${OBJDIR}/ei_application.o: ${SRC}/ei_application.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_application.c -o ${OBJDIR}/ei_application.o

${OBJDIR}/ei_frameclass.o: ${SRC}/ei_frameclass.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_frameclass.c -o ${OBJDIR}/ei_frameclass.o

${OBJDIR}/ei_geometrymanager.o: ${SRC}/ei_geometrymanager.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_geometrymanager.c -o ${OBJDIR}/ei_geometrymanager.o

${OBJDIR}/ei_widget.o: ${SRC}/ei_widget.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widget.c -o ${OBJDIR}/ei_widget.o

${OBJDIR}/ei_widgetclass.o: ${SRC}/ei_widgetclass.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_widgetclass.c -o ${OBJDIR}/ei_widgetclass.o

${OBJDIR}/ei_placer.o: ${SRC}/ei_placer.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_placer.c -o ${OBJDIR}/ei_placer.o

${OBJDIR}/ei_draw.o: ${SRC}/ei_draw.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw.c -o ${OBJDIR}/ei_draw.o

${OBJDIR}/ei_draw_utils.o: ${SRC}/ei_draw_utils.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_draw_utils.c -o ${OBJDIR}/ei_draw_utils.o

${OBJDIR}/ei_buttonclass.o: ${SRC}/ei_buttonclass.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_buttonclass.c -o ${OBJDIR}/ei_buttonclass.o

${OBJDIR}/ei_event.o: ${SRC}/ei_event.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_event.c -o ${OBJDIR}/ei_event.o

${OBJDIR}/ei_event_utils.o: ${SRC}/ei_event_utils.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_event_utils.c -o ${OBJDIR}/ei_event_utils.o

${OBJDIR}/ei_toplevelclass.o: ${SRC}/ei_toplevelclass.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_toplevelclass.c -o ${OBJDIR}/ei_toplevelclass.o

${OBJDIR}/ei_application_utils.o: ${SRC}/ei_application_utils.c ${INCLUDES}/*
	${CC} ${CCFLAGS} ${INCFLAGS} ${SRC}/ei_application_utils.c -o ${OBJDIR}/ei_application_utils.o



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
