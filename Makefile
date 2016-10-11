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

HFILES  = ImageIO/ImageIO.${H} ImageIO/Image.${H}
OBJS    = ImageIO/ImageIO.o ImageIO/Image.o 
PROJECT = warp

${PROJECT}:	${OBJS} ${PROJECT}.o
	${CC} ${LFLAGS} -o ${PROJECT} ${OBJS} ${PROJECT}.o ${LDFLAGS}

${PROJECT}.o: ${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

ImageIO.o: ImageIO/ImageIO.${C} ${HFILES}
	${CC} ${CFLAGS} -c ImageIO.${C} 

Image.o: ImageIO/Image.${C} ${HFILES}
	${CC} ${CFLAGS} -c Image.${C} 

clean:
	rm -f core.* *.o *~ ${PROJECT} ${OBJS}
