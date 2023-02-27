// Lab 3-4.

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

// Used by the ground model
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

// light sources
vec3 lightSourcesColorsArr[] = 
{ 
	{1.0f, 0.0f, 0.0f}, // Red light
	{0.0f, 1.0f, 0.0f}, // Green light
	{0.0f, 0.0f, 1.0f}, // Blue light
	{1.0f, 1.0f, 1.0f}  // White light
}; 
GLint isDirectional[] = 
{
	0,0,1,1
};
vec3 lightSourcesDirectionsPositions[] = 
{ 
	{10.0f, 5.0f, 0.0f}, // Red light, positional
	{0.0f, 5.0f, 10.0f}, // Green light, positional
	{-1.0f, 0.0f, 0.0f}, // Blue light along X
	{0.0f, 0.0f, -1.0f}  // White light along Z
}; 
GLfloat specularExponent[] = 
{
	100.0, 200.0, 60.0
};


// References
GLuint program;
GLuint texUnit;
Model *windmill_wall;
Model *windmill_roof;
Model *windmill_balcony;
Model *windmill_blade;
Model *ground_model;
Model *teapot;
Model *skybox;

// Camera
vec3 p = {0, 10, 0};		// Camera position
vec3 l = {0, 10, -1};	// Position to look at
vec3 v = {0, 1, 0};		// Determines which axis is up
mat4 cameraMatrix;


void init(void)
{
	dumpInfo();

	// Load model 
	windmill_wall = LoadModel("windmill/windmill-walls2.obj");
	windmill_roof = LoadModel("windmill/windmill-roof.obj");
	windmill_balcony = LoadModel("windmill/windmill-balcony.obj");
	windmill_blade = LoadModel("windmill/blade.obj");
	teapot = LoadModel("models/various/teapot.obj");
	skybox = LoadModel("labskybox.obj");

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	//glEnable(GL_CCW);

	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-4.vert", "lab3-4.frag");

	printError("init shader");
	
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

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
	cameraMatrix = Mult(Rx(angle_z), Mult(Ry(angle_x), lookAtv(p, l, v)));
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

}


void drawSkybox(void) {
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), false);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);

	mat4 translationMatrixSkybox = Mult(Rx(angle_z), Ry(angle_x));
	LoadTGATextureSimple("labskybox512.tga", &texUnit);			// Create texture object
	glBindTexture(GL_TEXTURE_2D, texUnit);						// Activate a texture object
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 	// Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}


void drawWindmillBlade(mat4 rotationMatrix) {
	// NOTE: Model is pointing to the right on start, axis are tossed around  
	mat4 translationMatrixBlade = T(4.5, 9.2, 0); // 4.5 : Blade depth. 	-0.8 : Vertical. 	0 : Horizon.
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, rotationMatrix.m);
	mat4 translationMatrix = Mult(translationMatrixBlade, rotationMatrix);
	mat4 total = Mult(cameraMatrix, translationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixBlade.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(windmill_blade, program, "inPosition", "inNormal", "inTexCoord");
}


void drawWindmill(void) {
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), false);

	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);

	// Walls, Roof, Balcony
	mat4 translationMatrixStaticObj = T(0, 0, 0);
	mat4 total = Mult(cameraMatrix, translationMatrixStaticObj);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixStaticObj.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(windmill_wall, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_roof, program, "inPosition", "inNormal", "inTexCoord");
	DrawModel(windmill_balcony, program, "inPosition", "inNormal", "inTexCoord");

	// Blades
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	drawWindmillBlade(Rx(0 + t/1000));
	drawWindmillBlade(Rx(M_PI/2 + t/1000));
	drawWindmillBlade(Rx(M_PI + t/1000));
	drawWindmillBlade(Rx(-M_PI/2 + t/1000));
	mat4 reset_extra_rot = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrixExtra"), 1, GL_TRUE, reset_extra_rot.m);

	
}


void drawGround(void) {
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), false);

	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);

	mat4 translationMatrixGround = T(0, 0, 0);
	mat4 total = Mult(cameraMatrix, translationMatrixGround);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixGround.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(ground_model, program, "inPosition", "inNormal", "inTexCoord");

	mat4 mirroredGroundMatrix = Mult(Ry(M_PI), translationMatrixGround);
	total = Mult(cameraMatrix, mirroredGroundMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, mirroredGroundMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(ground_model, program, "inPosition", "inNormal", "inTexCoord");
}


void drawTeapot(void) {
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), false);

	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[2]);

	mat4 translationMatrixTeapot = T( 20.0f, 0.0f, 20.0f);
	mat4 total = Mult(cameraMatrix, translationMatrixTeapot);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrixTeapot.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(teapot, program, "inPosition", "inNormal", "inTexCoord");
}


void display(void)
{
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

	keyboardMovement();

    glUniform3f(glGetUniformLocation(program, "cameraPos"), p.x, p.y, p.z);

    drawSkybox();
	drawGround();
	drawWindmill();
	drawTeapot();

	printError("display");
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 1200);
	glutCreateWindow ("lab3-4");
	glutDisplayFunc(display); 
	glutRepeatingTimerFunc(20); // timer that will cause a redisplay every d milliseconds
	init ();
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
	return 0;
}
