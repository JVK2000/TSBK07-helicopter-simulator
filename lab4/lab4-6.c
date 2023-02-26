// Lab 4, terrain generation

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

// Octagon
vec3 octagon_pos;
vec3 octagon_pos_start; 
vec3 octagon_pos_end;
int octagon_dir = 1;

mat4 projectionMatrix;


float terrainScale = 25;
Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	vec3 *vertexArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec3 *normalArray = (vec3 *)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vec2 *texCoordArray = (vec2 *)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / terrainScale;
			vertexArray[(x + z * tex->width)].z = z / 1.0;

			// Normal vectors. You need to calculate these.
			if (x != 0 && x != (tex->width - 1) && z != 0 && z != (tex->height - 1)) 
			{
				float L = tex->imageData[((x - 1) + z * tex->width) * (tex->bpp/8)] / terrainScale;
				float R = tex->imageData[((x + 1) + z * tex->width) * (tex->bpp/8)] / terrainScale;
				float B = tex->imageData[(x + (z - 1) * tex->width) * (tex->bpp/8)] / terrainScale;
				float T = tex->imageData[(x + (z + 1) * tex->width) * (tex->bpp/8)] / terrainScale;
				vec3 map_vec = {2*(R-L), 1, 2*(B-T)};	// y värdet påverkar hur känslig normalen är?? små färden ger tydligare skillnad
				vec3 map_normal = normalize(map_vec);
				if (map_normal.y < 0) {
					map_normal = ScalarMult(map_normal , -1);
				}
				// if (map_normal.y == 1) {
				// 	printf("vector: %f,%f,%f\n", map_normal.x, map_normal.y, map_normal.z);
				// 	map_normal.x = -1.0;
				// 	map_normal.y = -0.5;
				// 	map_normal.z = -1.0;
				// }
				// printf("bpp %f, %f, %f\n", map_normal.x, map_normal.y, map_normal.z);
				normalArray[(x + z * tex->width)].x = map_normal.x;
				normalArray[(x + z * tex->width)].y = map_normal.y;
				normalArray[(x + z * tex->width)].z = map_normal.z;
			}
			// else {
			// 	normalArray[(x + z * tex->width)].x = 0;
			// 	normalArray[(x + z * tex->width)].y = 1.0;
			// 	normalArray[(x + z * tex->width)].z = 0;
			// }

			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)].x = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)].y = z; // (float)z / tex->height;
		}

	// Gives the edges of the map the normals of its neighbour
	for (z = 0; z < tex->height; z++)
	{
		// vec3 map_normal = normalArray[(x + z * tex->width)];
		normalArray[(z * tex->width)].x = normalArray[(1 + z * tex->width)].x;
		normalArray[(z * tex->width)].y = normalArray[(1 + z * tex->width)].y;
		normalArray[(z * tex->width)].z = normalArray[(1 + z * tex->width)].z;
		
		int max_width = tex->width - 1;
		int access_idx = (max_width - 1) + z * tex->width;
		int update_idx = max_width + z * tex->width;
		normalArray[update_idx].x = normalArray[access_idx].x;
		normalArray[update_idx].y = normalArray[access_idx].y;
		normalArray[update_idx].z = normalArray[access_idx].z;
	}
	for (x = 0; x < tex->width; x++)
	{
		normalArray[x].x = normalArray[(x + tex->width)].x;
		normalArray[x].y = normalArray[(x + tex->width)].y;
		normalArray[x].z = normalArray[(x + tex->width)].z;

		int max_height = tex->height - 1;
		int access_idx = x + (max_height - 1) * tex->width;
		int update_idx = x + max_height * tex->width;
		normalArray[update_idx].x = normalArray[access_idx].x;
		normalArray[update_idx].y = normalArray[access_idx].y;
		normalArray[update_idx].z = normalArray[access_idx].z;
	}

	
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	
	// End of terrain generation
	
	// Create Model and upload to GPU:
	
	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}



// vertex array object
Model *m, *m2, *tm, *octagon;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.4,0.6,1.0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 250.0);

	// Load and compile shader
	program = loadShaders("lab4-6.vert", "lab4-6.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("green_grass.tga", &tex1);
	glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 0
	LoadTGATextureSimple("stones.tga", &tex2);
	// LoadTGATextureSimple("maskros512.tga", &tex1);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[1]);

	octagon = LoadModel("octagon.obj");

	// Load terrain data
	LoadTGATextureData("fft-terrain.tga", &ttex);
	// LoadTGATextureData("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

}


vec3 get_normal_point_in_plane(float x, float z) {
	int x_floor = (int) x;
	int z_floor = (int) z;
	int width = ttex.width;

	// unique vertex (depending on which triangle the point is in)
	float xz_diff = (x - x_floor) + (z - z_floor); 	
	float x1, y1, z1;
	if (xz_diff < 1.0) {
		int idx = x_floor + z_floor * width;
		x1 = tm->vertexArray[idx].x;
		y1 = tm->vertexArray[idx].y;
		z1 = tm->vertexArray[idx].z;
	} else {
		int idx = (x_floor + 1) + (z_floor + 1) * width;
		x1 = tm->vertexArray[idx].x;
		y1 = tm->vertexArray[idx].y;
		z1 = tm->vertexArray[idx].z;
	}
	vec3 vertex1 = {x1, y1, z1};

	// shared vertex
	float x2 = tm->vertexArray[(x_floor + 1) + z_floor * width].x;
	float y2 = tm->vertexArray[(x_floor + 1) + z_floor * width].y;
	float z2 = tm->vertexArray[(x_floor + 1) + z_floor * width].z;
	vec3 vertex2 = {x2, y2, z2};

	float x3 = tm->vertexArray[x_floor + (z_floor + 1) * width].x;
	float y3 = tm->vertexArray[x_floor + (z_floor + 1) * width].y;
	float z3 = tm->vertexArray[x_floor + (z_floor + 1) * width].z;
	vec3 vertex3 = {x3, y3, z3};

	// https://math.stackexchange.com/questions/1154340/how-to-find-the-height-of-a-2d-coordinate-on-a-3d-triangle
	vec3 normal = CalcNormalVector(vertex1, vertex2, vertex3);
	return normal;
}


float find_height(float x, float z)
{
	int x_floor = (int) x;
	int z_floor = (int) z;
	int width = ttex.width;

	// unique vertex (depending on which triangle the point is in)
	float xz_diff = (x - x_floor) + (z - z_floor); 	
	float x1, y1, z1;
	if (xz_diff < 1.0) {
		int idx = x_floor + z_floor * width;
		x1 = tm->vertexArray[idx].x;
		y1 = tm->vertexArray[idx].y;
		z1 = tm->vertexArray[idx].z;
	} else {
		int idx = (x_floor + 1) + (z_floor + 1) * width;
		x1 = tm->vertexArray[idx].x;
		y1 = tm->vertexArray[idx].y;
		z1 = tm->vertexArray[idx].z;
	}
	vec3 vertex1 = {x1, y1, z1};

	// shared vertex
	float x2 = tm->vertexArray[(x_floor + 1) + z_floor * width].x;
	float y2 = tm->vertexArray[(x_floor + 1) + z_floor * width].y;
	float z2 = tm->vertexArray[(x_floor + 1) + z_floor * width].z;
	vec3 vertex2 = {x2, y2, z2};

	float x3 = tm->vertexArray[x_floor + (z_floor + 1) * width].x;
	float y3 = tm->vertexArray[x_floor + (z_floor + 1) * width].y;
	float z3 = tm->vertexArray[x_floor + (z_floor + 1) * width].z;
	vec3 vertex3 = {x3, y3, z3};

	// https://math.stackexchange.com/questions/1154340/how-to-find-the-height-of-a-2d-coordinate-on-a-3d-triangle
	vec3 normal = CalcNormalVector(vertex1, vertex2, vertex3);

	// ---------------------------
	// normal = (a, b, c)
	// (a * x) + (b * y) + (c * z) + d = 0					(d - distance to origin / offset)
	// y = (- d - (a * point_x) - (c * point_z)) / b
	// ---------------------------
	float neg_d = normal.x * vertex1.x + normal.y * vertex1.y + normal.z * vertex1.z;
	float y = (neg_d - normal.x * x - normal.z * z) / normal.y;
	return y;
	
	// printf("\n\nvertex1: (%f, %f, %f)", vertex1.x, vertex1.y, vertex1.z);
	// printf("\nvertex2: (%f, %f, %f)", vertex2.x, vertex2.y, vertex2.z);
	// printf("\nvertex3: (%f, %f, %f)", vertex3.x, vertex3.y, vertex3.z);
	// printf("\nPoint: (%f, %f, %f)", x, 0.0, z);
	// printf("\nHeight: %f", y);
}
float r2d(float rad){
    return rad * (180.0 / M_PI);
}



float m_dotProduct(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Calculate the magnitude of a vector
float m_magnitude(vec3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Calculate the angle between two vectors in radians
float angleBetweenVectors(vec3 a, vec3 b) {
    float dot = m_dotProduct(a, b);
    float magA = m_magnitude(a);
    float magB = m_magnitude(b);
    float cosTheta = dot / (magA * magB);
    float theta = acos(cosTheta);
    return theta;
}


float octagon_speed = 0.1;
void drawOctagon() {
	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), false);

	
	if (octagon_pos.x < 0) {
		octagon_dir = 1;
	} else if (octagon_pos.x > (ttex.height - 1)) {
		octagon_dir = -1;
	}
	// if (glutKeyIsDown('f')) {
	// 	octagon_pos.x += octagon_speed;
	// 	octagon_pos.z += octagon_speed;
	// 	octagon_pos.y = find_height(octagon_pos.x, octagon_pos.z);
	// } if (glutKeyIsDown('g')) {
	// 	octagon_pos.x -= octagon_speed;
	// 	octagon_pos.z -= octagon_speed;
	// 	octagon_pos.y = find_height(octagon_pos.x, octagon_pos.z);

	// }

	octagon_pos.x = octagon_pos.x + octagon_dir * octagon_speed;
	octagon_pos.z = octagon_pos.z + octagon_dir * octagon_speed;
	octagon_pos.y = find_height(octagon_pos.x, octagon_pos.z);
	// float x = 0.4;
	// float z = 0.4;
	// float y = find_height(x, z);
	int width = ttex.width;
	int x_ground = (int) octagon_pos.x;
	int y_ground = (int) octagon_pos.y;
	int z_ground = (int) octagon_pos.z;
	vec3 ground_normal = tm->normalArray[(x_ground + z_ground * width)];
	// vec3 ground_normal = get_normal_point_in_plane(octagon_pos.x, octagon_pos.z);
	// if (ground_normal.x < 0 && ground_normal.y < 0 && ground_normal.z < 0) {

	// }
	// else if (ground_normal.x < 0 || ground_normal.y < 0 || ground_normal.z < 0) {
	// 	printf("\n\n\n ------------------ ABS");
	// 	ground_normal.x = fabs(ground_normal.x);
	// 	ground_normal.y = fabs(ground_normal.y);
	// 	ground_normal.z = fabs(ground_normal.z);
	// }

	float xAngle = acos(ground_normal.x) - M_PI/2;
	float yAngle = acos(ground_normal.y) - M_PI/2;
	float zAngle = acos(ground_normal.z);
	xAngle = - sin(ground_normal.x);
	zAngle = - sin(ground_normal.x);

    vec3 xAxis = { 1, 0, 0 };
    vec3 yAxis = { 0, 1, 0 };
    vec3 zAxis = { 0, 0, 1 };
    float angleX = angleBetweenVectors(ground_normal, xAxis);
    float angleY = angleBetweenVectors(ground_normal, yAxis);
    float angleZ = angleBetweenVectors(ground_normal, zAxis);
	// printf("\n\nangles: (%f, %f, %f)", r2d(angleX), r2d(angleY), r2d(angleZ));

	// printf("\n\pos: (%d, %d, %d)", x_ground, y_ground, z_ground);
 	// printf("\n\nangles: (%f, %f, %f)", r2d(xAngle), r2d(yAngle), r2d(zAngle));
 	// printf("\nnormal: (%f, %f, %f)", ground_normal.x, ground_normal.y, ground_normal.z);
	mat4 octagonMat = IdentityMatrix();
	mat4 trans = T(octagon_pos.x, octagon_pos.y, octagon_pos.z);
	// mat4 rotx = Ry(1.5);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	mat4 rotx = Rx(xAngle);
	mat4 rotz = Rz(zAngle);
	rotx = Rx(angleX + M_PI/2);
	rotz = Rz(angleZ);

	// mat4 tot = Mult(rotx, Mult(octagonMat, trans));
	mat4 tot = Mult(trans, Mult(rotx, rotz));
	// mat4 tot = Mult(trans, rotz);
	// mat4 tot = trans;

	glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, trans.m);	// not used

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
	// if(leftMouseDown) {
	float curr_x = x - init_x;
	float curr_y = y - init_z;
	angle_x = (curr_x/MOUSE_MOVE_SPEED)*M_PI;
	angle_z = (curr_y/MOUSE_MOVE_SPEED)*M_PI;
	// }
}


float MOVEMENT_SPEED = 0.8;
// float MOVEMENT_SPEED = 2.0;
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

	mat4 cameraMatrix = Mult(Rx(angle_z), Mult(Ry(angle_x), lookAtv(p, l, v)));
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
}


void draw_terrain()
{

	float x_shift = round(p.x/ttex.width);
	float rel_x_scale = p.x/ttex.width - x_shift;
	float z_shift = round(p.z/ttex.height);
	float rel_z_scale = p.z/ttex.height - z_shift;

	mat4 modelView = T(x_shift * (ttex.width - 1), 0, z_shift * (ttex.height - 1));
	glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	if (0.5 < rel_x_scale) {
		modelView = T((x_shift + 1) * (ttex.width - 1), 0, z_shift * (ttex.height - 1));
		glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	} else if (rel_x_scale < 0.5) {
		modelView = T((x_shift - 1) * (ttex.width - 1), 0, z_shift * (ttex.height - 1));
		glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	}

	if (0.5 < rel_z_scale) {
		modelView = T(x_shift * (ttex.width - 1), 0, (z_shift + 1) * (ttex.height - 1));
		glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	} else if (rel_z_scale < 0.5) {
		modelView = T(x_shift * (ttex.width - 1), 0, (z_shift - 1) * (ttex.height - 1));
		glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	}

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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);	

	glUniform1i(glGetUniformLocation(program, "shadingEnabled"), true);
	glUniform1i(glGetUniformLocation(program, "textureEnabled"), true);
    glUniform3f(glGetUniformLocation(program, "cameraPos"), p.x, p.y, p.z);

	draw_terrain();

	// mat4 modelView = IdentityMatrix();
	// glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	// DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	// modelView = T(0, 0, ttex.height - 1);
	// glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	// DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	
	// // modelView = Mult(Ry(M_PI/2), modelView);
	// // modelView = Mult(S(-1, 1, 1), modelView);
	// modelView = T(ttex.width - 1, 0, 0);
	// glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	// DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	// modelView = T(ttex.width - 1, 0, ttex.height - 1);
	// glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	// DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");


	drawOctagon();

	printError("display 2");
	
	glutSwapBuffers();
}

// void mousePress(int button, int state, int x , int y) {
// 	if(button == 0) {
// 		leftMouseDown = !state;
// 	}
// }

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1000, 1000);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutRepeatingTimer(20);

	glutPassiveMotionFunc(mouseMovement);
	// glutMouseFunc(mousePress);

	glutMainLoop();
	exit(0);
}
