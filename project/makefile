# # set this variable to the director in which you saved the common files
# commondir = ../common/

# all : project

# SOURCES := project.c helicopter.c terrain.c 

# # --- Linux ---
# project: $(SOURCES)
# 	gcc -Wall -o project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(SOURCES) $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


# # --- Mac ---
# # project : project.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m
# # 	gcc -Wall -o project -I$(commondir) -I../common/Mac -DGL_GLEXT_PROTOTYPES project.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Mac/MicroGlut.m -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations


# # --- Linux ---
# # project : project.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
# # 	gcc -Wall -o project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES project.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

# # project: project.c helicopter.c terrain.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
# # 	gcc -Wall -o project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES project.c helicopter.c terrain.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


# clean :
# 	rm project


# determine the operating system
UNAME := $(shell uname)

# set the compiler and flags based on the operating system
ifeq ($(UNAME), Linux)
	CC = gcc
	CFLAGS = -Wall -I../common/Linux -I$(commondir) -DGL_GLEXT_PROTOTYPES
	LDFLAGS = -lXt -lX11 -lGL -lm
	MICROGLUT = ../common/Linux/MicroGlut.c
else
	CC = clang
	CFLAGS = -Wall -I../common/Mac -I$(commondir) -DGL_GLEXT_PROTOTYPES
	LDFLAGS = -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations
	MICROGLUT = ../common/Mac/MicroGlut.m
endif

# set the common directory
commondir = ../common/

# list all the source files
SRCS = project.c helicopter.c terrain.c \
       $(commondir)GL_utilities.c \
       $(commondir)LittleOBJLoader.c \
       $(commondir)VectorUtils3.c \
       $(commondir)LoadTGA.c \
       $(MICROGLUT)

# set the output filename
OUTFILE = project

# compile the project
$(OUTFILE): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUTFILE) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(OUTPUT)