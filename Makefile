CC		= g++
C		= cpp
H       = h

CFLAGS	= -g 
LFLAGS	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

EXE			= warp tile
HFILES  = ImageIO/ImageIO.${H} ImageIO/Image.${H}
OBJS    = ImageIO/ImageIO.o ImageIO/Image.o 

all:	${EXE}

warp:	${OBJS} warp.o
	${CC} ${LFLAGS} -o warp ${OBJS} warp.o ${LDFLAGS}

warp.o: warp.${C} ${HFILES}
	${CC} ${CFLAGS} -c warp.${C}

tile:	${OBJS} tile.o
	${CC} ${LFLAGS} -o tile ${OBJS} tile.o ${LDFLAGS}

tile.o: tile.${C} ${HFILES}
	${CC} ${CFLAGS} -c tile.${C}

ImageIO.o: ImageIO/ImageIO.${C} ${HFILES}
	${CC} ${CFLAGS} -c ImageIO.${C} 

Image.o: ImageIO/Image.${C} ${HFILES}
	${CC} ${CFLAGS} -c Image.${C} 

clean:
	rm -f core.* *.o *~ ${EXE} ${OBJS}
