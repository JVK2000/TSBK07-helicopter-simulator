# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab3-1 

lab3-1 : lab3-1.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
	gcc -Wall -o lab3-1 -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES lab3-1.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations

clean :
	rm lab3-1