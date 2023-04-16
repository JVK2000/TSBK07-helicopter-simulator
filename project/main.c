#include "main.h"
#include "helicopter.h"
#include "terrain.h"
#include "controller.h"
#include "light.h"


// Octagon
Model *octagon;
vec3 octagon_pos;
vec3 octagon_pos_start; 
vec3 octagon_pos_end;
int octagon_dir = 1;
const float octagon_speed = 0.1;
float vertical_rotation = 0; // Vertical rotation modified by controller.c, used by helicopter.c to compensate for the vertical rotation, so the helicopter dont change its vertical rotation.   

// Reference to shader program
GLuint program;
GLuint texUnit;
GLuint tex1, tex2;

GLint specularLightEnabledLoc;
GLint ambientLightEnabledLoc;
GLint diffuseLightEnabledLoc;
GLint textureEnabledLoc;
GLint translationMatrixLoc;
GLint mdlMatrixLoc;
GLint normalMatrixLoc;


void init(void)
{
    // GL inits
    glClearColor(0.4,0.6,1.0,0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("project.vert", "project.frag");
    glUseProgram(program);
    printError("init shader");
    
    mat4 projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	lightInit();
	controllerInit();
	helicopterInit();
    terrainInit(&tex1, &tex2); 
    
    octagon = LoadModel("octagon.obj");

	specularLightEnabledLoc = glGetUniformLocation(program, "specularLightEnabled");
    ambientLightEnabledLoc = glGetUniformLocation(program, "ambientEnabled");
    diffuseLightEnabledLoc = glGetUniformLocation(program, "diffuseEnabled");
    textureEnabledLoc = glGetUniformLocation(program, "textureEnabled");
    translationMatrixLoc = glGetUniformLocation(program, "translationMatrix");
    mdlMatrixLoc = glGetUniformLocation(program, "mdlMatrix");
	normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");
}


void drawOctagon() {
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
    // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	

	keyboardMovement(program);

	printError("pre display");
	
	// Use the shader program
	glUseProgram(program);

	drawSkybox(texUnit, cameraAngleY, cameraAngleX);

    // Bind textures and set shader uniforms
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);	

	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

	draw_terrain(cameraMatrix, cameraPosition);

	drawOctagon();
	drawHelicopter(cameraMatrix, cameraAngleY);

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
	glutHideCursor();
	// glutSpecialFunc(keyboard);
	glutMainLoop();
	exit(0);
}
