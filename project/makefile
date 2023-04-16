# determine the operating system
UNAME := $(shell uname)

# set the compiler and flags based on the operating system
ifeq ($(UNAME), Linux)
	CC = gcc
	CXX = g++
	CFLAGS = -Wall -I../common/Linux -I$(commondir) -DGL_GLEXT_PROTOTYPES
	LDFLAGS = -lXt -lX11 -lGL -lm -lnoise
	MICROGLUT = ../common/Linux/MicroGlut.c
else
	CC = clang
	CXX = clang++
	CFLAGS = -Wall -I../common/Mac -I$(commondir) -DGL_GLEXT_PROTOTYPES
	LDFLAGS = -framework OpenGL -framework Cocoa -lm -Wno-deprecated-declarations -lnoise
	MICROGLUT = ../common/Mac/MicroGlut.m
endif

# set the common directory
commondir = ../common/

# list all the source files
C_SRCS = main.c helicopter.c terrain.c controller.c light.c helicopter_controller_shared.c \
       $(commondir)GL_utilities.c \
       $(commondir)LittleOBJLoader.c \
       $(commondir)VectorUtils3.c \
       $(commondir)LoadTGA.c \
       $(MICROGLUT)

CPP_SRCS = noise_wrapper.cpp

# set the output filename
OUTFILE = project

# compile the project
$(OUTFILE): $(C_SRCS) $(CPP_SRCS)
	$(CC) -c $(CFLAGS) $(C_SRCS)
	$(CXX) -c $(CFLAGS) $(CPP_SRCS)
	$(CXX) -o $(OUTFILE) *.o $(LDFLAGS)

clean:
	rm -f $(OUTFILE) *.o