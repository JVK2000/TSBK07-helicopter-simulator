// Lab 1-5.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"


// Reference to shader program
GLuint program;


// Globals
// Data would normally be read from files

GLfloat vertices[18*3] =	// 18 straigth lines, 3 is the number of points per cordinat (x, y, z). three lines represent one triangle.
{
	// base plate
	-0.5,-0.5,-0.5,		// back left
	0.5,-0.5,-0.5,		// back rigth
	0.5,-0.5,0.5,		// front rigth

	-0.5,-0.5,-0.5,		// back left
	0.5,-0.5,0.5,		// front rigth
	-0.5,-0.5,0.5,		// front left	

	// triangle rigth
	0.5,-0.5,-0.5,      // back rigth
	0.5,-0.5,0.5,       // front rigth
	0, 0.5, 0, 			// top

	// triangle left
	-0.5,-0.5,-0.5,		// back left
	-0.5,-0.5,0.5,      // front left
	0, 0.5, 0, 			// top

	// triangle back
	-0.5,-0.5,-0.5,		// back left
	0.5,-0.5,-0.5,      // back rigth
	0, 0.5, 0, 			// top

	// triangle front
	-0.5,-0.5,0.5,      // front left
	0.5,-0.5,0.5,       // front rigth
	0, 0.5, 0, 			// top

};

GLfloat colors[18*3] = {
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red

	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red

	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red

	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red

	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
	1.0, 0.0, 0.0,	// Red
};

// The rotation matrices will be changed for animation
GLfloat rotationMatrix[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};
GLfloat rotationMatrix2[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};
GLfloat translationMatrix[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, -2.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};


// Perspective
#define near 1.0
#define far 30.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {	
	2.0f*near/(right-left), 0.0f, 					(right+left)/(right-left), 		0.0f,
	0.0f, 					2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 		0.0f,
	0.0f, 					0.0f, 					-(far + near)/(far - near), 	-2*far*near/(far - near),
	0.0f, 					0.0f, 					-1.0f, 							0.0f 
};



// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);	
	glBindVertexArray(vertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &colorBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
	
	// End of upload of geometry
	
	// Upload the rotation matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);


	printError("init arrays");
}

float a = 0.0;

void display(void)
{
	printError("pre display");
	a += 0.1;

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rotationMatrix2[0] = cos(a/5.0);
	rotationMatrix2[1] = -sin(a/5.0);
	rotationMatrix2[4] = sin(a/5.0);
	rotationMatrix2[5] = cos(a/5.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix2"), 1, GL_TRUE, rotationMatrix2);
	rotationMatrix[5] = cos(a);
	rotationMatrix[6] = -sin(a);
	rotationMatrix[9] = sin(a);
	rotationMatrix[10] = cos(a);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 36*3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	glutRepeatingTimerFunc(20); // timer that will cause a redisplay every d milliseconds
	init ();
	glutMainLoop();
	return 0;
}
