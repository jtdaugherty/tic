
APP=tic
CFLAGS=-I/opt/local/include
LDFLAGS=-L/opt/local/lib -framework OpenGL -framework GLUT -lGLEW

default: tic

tic: board.c geometry.c tic.c

clean:
	rm $(APP)