#include "helicopter.h"
#include "terrain.h"
#include "controller.h"


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


// Octagon
vec3 octagon_pos;
vec3 octagon_pos_start; 
vec3 octagon_pos_end;
int octagon_dir = 1;

mat4 projectionMatrix;


// vertex array object
Model *m, *m2, *octagon;

// Reference to shader program
GLuint program;
GLuint texUnit;
GLuint tex1, tex2;

void init(void)
{
    // GL inits
    glClearColor(0.4,0.6,1.0,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

	initController();
    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);

    // Load and compile shader
    program = loadShaders("project.vert", "project.frag");
    glUseProgram(program);
    printError("init shader");
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

    glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
    glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
    glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);

    octagon = LoadModel("octagon.obj");
    helicopter_init();

    // pass tex1 and tex2 as parameters to terrain_init
    terrain_init(program, &tex1, &tex2); 

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



void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // -
	glEnable(GL_BLEND);	// -

	keyboardMovement(program);

	printError("pre display");
	
	glUseProgram(program);

	drawSkybox(program, texUnit, cameraAngleZ, cameraAngleX);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);	

	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

	draw_terrain(program, cameraMatrix, cameraPosition);

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
