// Lab 3-2.

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
#include "LittleOBJLoader.h"
#include "LoadTGA.h"

// Globals
// Data would normally be read from files
#define near 1.0
#define far 60.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat projectionMatrix[] = {    
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
	0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
	0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
	0.0f, 0.0f, -1.0f, 0.0f 
};

// To move the whole combinations of models
GLfloat globalTransform[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, -10.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};

// Used by static positioned windmill models (walls, balcony, roof) 
GLfloat translationMatrixStaticObj[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, -10.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};

// Model is pointing to the right on start, axis are tossed around  
GLfloat translationMatrixBlade[] = {	
	1.0f, 0.0f, 0.0f, 4.5f,		// Blade depth
	0.0f, 1.0f, 0.0f, -0.8f,	// Vertical
	0.0f, 0.0f, 1.0f, 0.0f,		// Horizon
	0.0f, 0.0f, 0.0f, 1.0f 
};

GLfloat cccc[] = {	
	1.0f, 0.0f, 0.0f, 0.0f,		// Blade depth
	0.0f, 1.0f, 0.0f, 0.0f,	// Vertical
	0.0f, 0.0f, 1.0f, 0.0f,		// Horizon
	0.0f, 0.0f, 0.0f, 1.0f 
};
vec3 p = {0, 0, 10};	// Camera position
vec3 l = {0, 0, 0};		// Position to look at
vec3 v = {0, 1, 0};		// Determines which axis is up


// References
GLuint program;
GLuint texUnit;
Model *windmill_wall;
Model *windmill_roof;
Model *windmill_balcony;
Model *windmill_blade;

// vertex array object
unsigned int bunnyVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	dumpInfo();

	// Load model 
	windmill_wall = LoadModel("windmill/windmill-walls2.obj");
	windmill_roof = LoadModel("windmill/windmill-roof.obj");
	windmill_balcony = LoadModel("windmill/windmill-balcony.obj");
	windmill_blade = LoadModel("windmill/blade.obj");

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	//glEnable(GL_CCW);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-2.vert", "lab3-2.frag");
	printError("init shader");
	

	// --- Upload geometry to the GPU ---

	// Allocate Vertex Array and Buffer object for model 
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);  

	glBindVertexArray(bunnyVertexArrayObjID);

    if (windmill_wall->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, windmill_wall->numVertices*2*sizeof(GLfloat), windmill_wall->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }

	// VBO for vertex data
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	// VBO for normal data
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, windmill_wall->numIndices*sizeof(GLuint), windmill_wall->indexArray, GL_STATIC_DRAW);

	// Upload the matrixes
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "globalTransform"), 1, GL_TRUE, globalTransform);

	// --- End of upload of geometry ---

	mat4 cameraMatrix = lookAtv(p, l, v);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrixRotation"), 1, GL_TRUE, cccc);
	mat4 rot_y = Ry(0);
	//rot_y = Ry(-M_PI/2);		// Non rotation of the model
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrixRotation"), 1, GL_TRUE, rot_y.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrixPos"), 1, GL_TRUE, rot_y.m);

	printError("init arrays");
}


float MOUSE_MOVE_SPEED = 400;
float init_x = 0;
float init_z = 0;
float angle_z = 0;
float angle_x = 0;
bool first = true;

void mouseMovement(int x, int y)
{
	if (first) {
		init_x = x;
		init_z = y;
		first = false;
	}
	float curr_x = x - init_x;
	float curr_y = y - init_z;
	angle_x = (curr_x/MOUSE_MOVE_SPEED)*M_PI;
	angle_z = (curr_y/MOUSE_MOVE_SPEED)*M_PI;
	// mat4 cccc = Mult(Rx(angle_z), Ry(angle_x));
	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrixRotation"), 1, GL_TRUE, cccc.m);

	printf(" %f,", angle_x);
	// l.x += (angle_x);
	// l.z = l.z * sin(angle_x);
	// mat4 cameraMatrix = lookAtv(p, l, v);
	
	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

}


float MOVEMENT_SPEED = 0.2;
float pos_x = 0;
float pos_z = 0;

void keyboardMovement()
{
	float pos_x = 0;
	float pos_z = 0;
	if (glutKeyIsDown('d')) {
		pos_x += MOVEMENT_SPEED * cos(angle_x); 
		pos_z += MOVEMENT_SPEED * sin(angle_x); 
	} else if (glutKeyIsDown('a')) {
		pos_x -= MOVEMENT_SPEED * cos(angle_x); 
		pos_z -= MOVEMENT_SPEED * sin(angle_x); 
	} 
	if (glutKeyIsDown('w')) {
		pos_x += MOVEMENT_SPEED * sin(angle_x); 
		pos_z -= MOVEMENT_SPEED * cos(angle_x);
	} else if (glutKeyIsDown('s')) {
		pos_x -= MOVEMENT_SPEED * sin(angle_x); 
		pos_z += MOVEMENT_SPEED * cos(angle_x); 
	}
	// mat4 cameraMatrixPos = T(pos_x, 0, pos_z);
	// glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrixPos"), 1, GL_TRUE, cameraMatrixPos.m);

	// vec3 p = {0, 0, 10};	// Camera position
	// vec3 l = {0, 0, 0};		// Position to look at
	// vec3 v = {0, 1, 0};		// Determines which axis is up
	p.x += pos_x; 
	l.x += pos_x;
	p.z += pos_z; 
	l.z += pos_z;
	mat4 cameraMatrix = Mult(Rx(angle_z), Mult(Ry(angle_x), lookAtv(p, l, v)));
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

}


void display(void)
{
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO

	keyboardMovement();

	// Upload rotation
	mat4 rot_y;
	rot_y = Ry(t/1000);
	//rot_y = Ry(-M_PI/2);		// Non rotation of the model
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixY"), 1, GL_TRUE, rot_y.m);

	// Remove X-rotation used by the blades
	mat4 rotationMatrixBlade = Rx(0);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);

	// Walls, Roof, Balcony
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixStaticObj);
	DrawModel(windmill_wall, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_roof, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_balcony, program, "inPosition", "inNormal", "inTexCoord");

	rotationMatrixBlade = Rx(0 + t/1000);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixBlade);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);
	DrawModel(windmill_blade, program, "inPosition", "inNormal", "inTexCoord");

	rotationMatrixBlade = Rx(M_PI/2 + t/1000);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);
	DrawModel(windmill_blade, program, "inPosition", "inNormal", "inTexCoord");
	
	rotationMatrixBlade = Rx(M_PI + t/1000);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);
	DrawModel(windmill_blade, program, "inPosition", "inNormal", "inTexCoord");
	
	rotationMatrixBlade = Rx(-M_PI/2 + t/1000);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);
	DrawModel(windmill_blade, program, "inPosition", "inNormal", "inTexCoord");

	printError("display");
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("lab3-2");
	glutDisplayFunc(display); 
	glutRepeatingTimerFunc(20); // timer that will cause a redisplay every d milliseconds
	init ();
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
	return 0;
}
