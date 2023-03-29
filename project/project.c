// Project

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
#include "helicopter.h"
#include "terrain.h"


// beginning Light
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
	// {0.0f, 0.0f, 1.0f}, // Blue light
	{1.0f, 1.0f, 1.0f},  // White light
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
// end Light


// Camera
vec3 p = {0, 10, 10};	// Camera position
vec3 l = {0, 10, 0};		// Position to look at
vec3 v = {0, 1, 0};		// Determines which axis is up
mat4 cameraMatrix;


// Octagon
vec3 octagon_pos;
vec3 octagon_pos_start; 
vec3 octagon_pos_end;
int octagon_dir = 1;

mat4 projectionMatrix;


// vertex array object
Model *m, *m2, *octagon;
GLuint texUnit;

// Reference to shader program
GLuint program;
GLuint tex1, tex2;

void init(void)
{
	// GL inits
	glClearColor(0.4,0.6,1.0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);

	// Load and compile shader
	program = loadShaders("project.vert", "project.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("green_grass.tga", &tex1);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 1
	LoadTGATextureSimple("stones.tga", &tex2);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);

	octagon = LoadModel("octagon.obj");
	helicopter_init();
	terrain_init(program, tex1, tex2);
}


float octagon_speed = 0.1;
void drawOctagon() {
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), false);
	
	if (octagon_pos.x < 0) {
		octagon_dir = 1;
	} else if (octagon_pos.x > (texture_data_height() - 1)) {
		octagon_dir = -1;
	}

	octagon_pos.x = octagon_pos.x + octagon_dir * octagon_speed;
	octagon_pos.z = octagon_pos.z + octagon_dir * octagon_speed;
	octagon_pos.y = find_height(octagon_pos.x, octagon_pos.z);

	mat4 trans = T(octagon_pos.x, octagon_pos.y, octagon_pos.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, trans.m);
	
	mat4 total = Mult(cameraMatrix, trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);	

	DrawModel(octagon, program, "inPosition", "inNormal", "inTexCoord");
}


float MOUSE_MOVE_SPEED = 400;
float init_x = 0;
float init_z = 0;
float angle_z = 0;
float angle_x = 0;
bool first = true;
bool leftMouseDown = false;

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

float MOVEMENT_SPEED = 0.8;
float pos_x = 0;
float pos_z = 0;
float const_ang = M_PI/4;
void keyboardMovement()
{
	float pos_x = 0;
	float pos_z = 0;
	float pos_y = 0;
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
	} if (glutKeyIsDown('q')) {
		pos_y -= MOVEMENT_SPEED;
	} if (glutKeyIsDown('e')) {
		pos_y += MOVEMENT_SPEED;
	}
	p.x += pos_x; 
	l.x += pos_x;
	p.z += pos_z; 
	l.z += pos_z;
	p.y += pos_y; 
	l.y += pos_y;

	cameraMatrix = Mult(Rx(angle_z), Mult(Ry(angle_x), lookAtv(p, l, v)));
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
}







void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // -
	glEnable(GL_BLEND);	// -

	keyboardMovement();

	printError("pre display");
	
	glUseProgram(program);

	drawSkybox(program, texUnit, angle_z, angle_x);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);	

	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), p.x, p.y, p.z);

	draw_terrain(program, cameraMatrix, p);

	drawOctagon();
	drawHelicopter(program, cameraMatrix);

	printError("display 2");
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1000, 1000);
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);

	glutPassiveMotionFunc(mouseMovement);

	glutMainLoop();
	exit(0);
}
