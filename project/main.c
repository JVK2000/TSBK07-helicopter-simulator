#include "helicopter.h"
#include "terrain.h"
#include "skybox.h"
#include "controller.h"
#include "light.h"


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
    program = loadShaders("shader.vert", "shader.frag");
    glUseProgram(program);
    printError("init shader");
    
    mat4 projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	lightInit();
	controllerInit();
	helicopterInit();
    terrainInit(&tex1, &tex2); 
    skyboxInit(); 
    
	specularLightEnabledLoc = glGetUniformLocation(program, "specularLightEnabled");
    ambientLightEnabledLoc = glGetUniformLocation(program, "ambientLightEnabled");
    diffuseLightEnabledLoc = glGetUniformLocation(program, "diffuseLightEnabled");
    textureEnabledLoc = glGetUniformLocation(program, "textureEnabled");
    translationMatrixLoc = glGetUniformLocation(program, "translationMatrix");
    mdlMatrixLoc = glGetUniformLocation(program, "mdlMatrix");
	normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");
}


void display(void)
{
    // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	

	controllerHandler();

	printError("pre display");
	
	glUseProgram(program);

	drawSkybox(cameraAngleY, cameraAngleX);

    // Bind textures and set shader uniforms
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);	

	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), cameraPosition.x, cameraPosition.y, cameraPosition.z);

	drawTerrain(cameraMatrix, cameraPosition);
	detectCollision();

	drawHelicopter(cameraMatrix, cameraAngleY);

	printError("display 2");
	
	glutSwapBuffers();
}


void reshape(int x, int y)
{
	mat4 projectionMatrix;
	float ratio = (float)x/y;
	if (ratio > 1)
		projectionMatrix = frustum(-0.5*ratio, 0.5*ratio, -0.5, 0.5, 1.0, 1000.0);
	else
		projectionMatrix = frustum(-0.5, 0.5, -0.5/ratio, 0.5/ratio, 1.0, 1000.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (2000, 2000);
	glutCreateWindow ("TSBK07 Project");
	glutDisplayFunc(display);
	init ();
	glutReshapeFunc(reshape);
	glutRepeatingTimer(20);
	glutHideCursor();
	glutMainLoop();
	exit(0);
}
