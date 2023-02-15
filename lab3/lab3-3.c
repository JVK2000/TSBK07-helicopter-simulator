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
#define far 200.0
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

GLfloat translationMatrixGround[] = {	
	1.0f, 0.0f, 0.0f, 50.0f,
	0.0f, 1.0f, 0.0f, -10.0f,
	0.0f, 0.0f, 1.0f, 50.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};
GLfloat translationMatrixTeapot[] = {	
	1.0f, 0.0f, 0.0f, 15.0f,
	0.0f, 1.0f, 0.0f, -10.0f,
	0.0f, 0.0f, 1.0f, 15.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};

GLfloat translationMatrixSkybox[] = {	
	1.0f, 0.0f, 0.0f, 2.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
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


vec3 p = {0, 0, 10};	// Camera position
vec3 l = {0, 0, 0};		// Position to look at
vec3 v = {0, 1, 0};		// Determines which axis is up


#define kGroundSize 100.0f
vec3 vertices[] =
{
 { -kGroundSize,0.0f,-kGroundSize},
 {-kGroundSize,0.0f,kGroundSize},
 {kGroundSize,-0.0f,-kGroundSize},
 {kGroundSize,-0.0f,kGroundSize}
};

vec3 vertex_normals[] =
{
  {0.0f,1.0f,0.0f},
  {0.0f,1.0f,0.0f},
  {0.0f,1.0f,0.0f},
  {0.0f,1.0f,0.0f}
};

vec2 tex_coords[] =
{
  {0.0f,0.0f},
  {0.0f,20.0f},
  {20.0f,0.0f}, 
  {20.0f,20.0f}
};
GLuint indices[] =
{
  0, 1, 2, 1, 3, 2
};
vec3 colors[] =
{
	{0.0f,1.0f,0.0f},
	{1.0f,1.0f,0.0f},
	{1.0f,0.0f,1.0f}
};

// References
GLuint program;
GLuint program_no_shader;
GLuint program_no_texture;
GLuint texUnit;
Model *windmill_wall;
Model *windmill_roof;
Model *windmill_balcony;
Model *windmill_blade;
Model *ground_model;
Model *teapot;
Model *skybox;

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
	teapot = LoadModel("models/various/teapot.obj");
	skybox = LoadModel("skybox/skyboxfull.obj");

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	//glEnable(GL_CCW);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	program_no_shader = loadShaders("lab3-3.vert", "lab3-3-no-shader.frag");
	program_no_texture = loadShaders("lab3-3.vert", "lab3-3-no-texture.frag");
	glUseProgram(program);

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

	printf("size: %lu", sizeof(vertices[0]));
	ground_model = LoadDataToModel(vertices, vertex_normals, tex_coords, colors, indices, sizeof(vertices[0]), sizeof(indices[0]));


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
	} if (glutKeyIsDown('a')) {
		pos_x -= MOVEMENT_SPEED * cos(angle_x); 
		pos_z -= MOVEMENT_SPEED * sin(angle_x); 
	} if (glutKeyIsDown('w')) {
		pos_x += MOVEMENT_SPEED * sin(angle_x); 
		pos_z -= MOVEMENT_SPEED * cos(angle_x);
	} if (glutKeyIsDown('s')) {
		pos_x -= MOVEMENT_SPEED * sin(angle_x); 
		pos_z += MOVEMENT_SPEED * cos(angle_x); 
	}
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
	
	glUseProgram(program);

	// Ground
	// glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixGround);
	// DrawModel(ground_model, program, "inPosition", "inNormal", "inTexCoord");
	
	// Teapot
	// glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixTeapot);
	// DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");

	// Walls, Roof, Balcony
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixStaticObj);
	DrawModel(windmill_wall, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_roof, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_balcony, program, "inPosition", "inNormal", "inTexCoord");

	mat4 rotationMatrixBlade = Rx(0 + t/1000);
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

	// Remove X-rotation used by the blades
	rotationMatrixBlade = Rx(0);
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrixBlade.m);


	// Skybox
	glUseProgram(program);
	LoadTGATextureSimple("labskybox512.tga", &texUnit);			// Create texture object
	glBindTexture(GL_TEXTURE_2D, texUnit);						// Activate a texture object
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 	// Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixSkybox);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");


	printError("display");
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("lab3-3");
	glutDisplayFunc(display); 
	glutRepeatingTimerFunc(20); // timer that will cause a redisplay every d milliseconds
	init ();
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
	return 0;
}
