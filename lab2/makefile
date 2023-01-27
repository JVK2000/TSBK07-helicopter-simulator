# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab2-1

lab2-1 : lab2-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab2-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab2-1.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean :
	rm lab2-1