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
	0.5,-0.5,0.5,		// front rigth
	0.5,-0.5,-0.5,		// back rigth

	-0.5,-0.5,-0.5,		// back left
	-0.5,-0.5,0.5,		// front left	
	0.5,-0.5,0.5,		// front rigth

	// triangle rigth
	0.5,-0.5,-0.5,      // back rigth
	0.5,-0.5,0.5,       // front rigth
	0, 0.5, 0, 			// top

	// triangle left
	-0.5,-0.5,-0.5,		// back left
	0, 0.5, 0, 			// top
	-0.5,-0.5,0.5,      // front left

	// triangle back
	-0.5,-0.5,-0.5,		// back left
	0.5,-0.5,-0.5,      // back rigth
	0, 0.5, 0, 			// top

	// triangle front
	-0.5,-0.5,0.5,      // front left
	0, 0.5, 0, 			// top
	0.5,-0.5,0.5,       // front rigth

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
GLfloat rotationMatrixZ[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};
GLfloat rotationMatrixX[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
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
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

	//glCullFace(GL_BACK);
    glFrontFace(GL_CW);

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
	glBufferData(GL_ARRAY_BUFFER, 18*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for color data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 18*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
	
	// End of upload of geometry
	
	// Upload the rotation matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rotationMatrixZ);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rotationMatrixX);


	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME); // t in milisecunds

	mat4 rot, rot2;
	rot = Rz(t/1000);		// z rotate
	rot2 = Rx(t/1000);		// x rotate
	//glCullFace(GL_FRONT);

	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixZ"), 1, GL_TRUE, rot.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixX"), 1, GL_TRUE, rot2.m);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 18*3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	glutRepeatingTimerFunc(20); // timer that will cause a redisplay every d milliseconds
	init ();
	glutMainLoop();
	return 0;
}
