#include "terrain.h"
#include "noise_wrapper.h"
#include "terrain_controller_shared.h"

#include "uthash.h"

// Model *tm, *skybox;
Model *skybox;
float terrainScale = 25;
TextureData ttex; // terrain
GLint isSkyLoc;
Model *tm[9];
int player_pos_x;
int player_pos_z;

typedef struct {
    int x_offset;
    int z_offset;
    Model *model;
	int key; // added member
    UT_hash_handle hh; // required for uthash
} TerrainSection;

TerrainSection *terrain_cache = NULL;


void terrainInit(GLuint *tex1, GLuint *tex2) {
    // Load terrain data
    LoadTGATextureSimple("green_grass.tga", tex1); // update tex1 and tex2 values
    LoadTGATextureSimple("stones.tga", tex2);
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 1

	ttex.width = 256;
	ttex.height = 256;
    printError("init terrain");

    skybox = LoadModel("labskybox.obj");
	isSkyLoc = glGetUniformLocation(program, "isSky");
}


float wrappedNoise2D(float x, float x_offset, float z, float z_offset, int terrainScale, TextureData *tex) {
	return (noise2D(x + x_offset * (tex->width - 1), z + z_offset * (tex->height - 1)) / terrainScale) * 1000;
}


Model* GenerateTerrain(TextureData *tex, int x_offset, int z_offset)
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
			vertexArray[(x + z * tex->width)].y = wrappedNoise2D(x, x_offset, z, z_offset, terrainScale, tex);
			vertexArray[(x + z * tex->width)].z = z / 1.0;

			// Normal vectors. You need to calculate these.
			if (x != 0 && x != (tex->width - 1) && z != 0 && z != (tex->height - 1)) 
			{
				float L = wrappedNoise2D(x - 1, x_offset, z, z_offset, terrainScale, tex);;
				float R = wrappedNoise2D(x + 1, x_offset, z, z_offset, terrainScale, tex);;
				float B = wrappedNoise2D(x, x_offset, z - 1, z_offset, terrainScale, tex);
				float T = wrappedNoise2D(x, x_offset, z + 1, z_offset, terrainScale, tex);
				vec3 map_vec = {2*(R-L), 1, 2*(B-T)};
				vec3 map_normal = normalize(map_vec);
				if (map_normal.y < 0) {
					map_normal = ScalarMult(map_normal , -1);
				}
				normalArray[(x + z * tex->width)].x = map_normal.x;
				normalArray[(x + z * tex->width)].y = map_normal.y;
				normalArray[(x + z * tex->width)].z = map_normal.z;
			}

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


Model *get_terrain_model(int x_pos, int z_pos) {
    TerrainSection *section;
    int x_offset = (int)ceil(x_pos / (ttex.width - 1));
    int z_offset = (int)ceil(z_pos / (ttex.height - 1));
    int key = x_offset * 100 + z_offset;

    // search for the key in terrain_cache
    HASH_FIND_INT(terrain_cache, &key, section);

    if (section == NULL) {
        // key not found, return NULL
        return NULL;
    } else {
        // key found, return the model
        return section->model;
    }
}


float find_height(float x, float z)
{
	int x_floor = (int) x;
	int z_floor = (int) z;
	Model *model = get_terrain_model(x_floor, z_floor);
	int width = ttex.width;

	// unique vertex (depending on which triangle the point is in)
	float xz_diff = (x - x_floor) + (z - z_floor); 	
	float x1, y1, z1;
	if (xz_diff < 1.0) {
		int idx = x_floor + z_floor * width;
		x1 = model->vertexArray[idx].x;
		y1 = model->vertexArray[idx].y;
		z1 = model->vertexArray[idx].z;
	} else {
		int idx = (x_floor + 1) + (z_floor + 1) * width;
		x1 = model->vertexArray[idx].x;
		y1 = model->vertexArray[idx].y;
		z1 = model->vertexArray[idx].z;
	}
	vec3 vertex1 = {x1, y1, z1};

	// shared vertex
	float x2 = model->vertexArray[(x_floor + 1) + z_floor * width].x;
	float y2 = model->vertexArray[(x_floor + 1) + z_floor * width].y;
	float z2 = model->vertexArray[(x_floor + 1) + z_floor * width].z;
	vec3 vertex2 = {x2, y2, z2};

	float x3 = model->vertexArray[x_floor + (z_floor + 1) * width].x;
	float y3 = model->vertexArray[x_floor + (z_floor + 1) * width].y;
	float z3 = model->vertexArray[x_floor + (z_floor + 1) * width].z;
	vec3 vertex3 = {x3, y3, z3};
	vec3 normal = CalcNormalVector(vertex1, vertex2, vertex3);

	float neg_d = normal.x * vertex1.x + normal.y * vertex1.y + normal.z * vertex1.z;
	float y = (neg_d - normal.x * x - normal.z * z) / normal.y;
	return y;
}



bool player_collides_with_terrain(float player_x, float player_z, float player_y) {
    float terrain_height = find_height(player_x, player_z);
	printf("\nterrain: %f", terrain_height);
	printf("\tplayer: %f", player_y);

    // if (player_y < terrain_height) {
    //     return true;
    // }
    // return false;
}

void detect_collision() {
	float x, y, z;
	get_player_pos(&x, &y, &z);
	// printf("Player position: (%f, %f, %f)\n\n", x, y, z);
	int collision_detected = player_collides_with_terrain(x, z, y);
	// printf("collision_detected: %d", collision_detected);
}



void draw_terrain_section(mat4 cameraMatrix, Model *terrainModel, float x, float z) 
{
    glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, true);
    glUniform1i(diffuseLightEnabledLoc, true);
    glUniform1i(textureEnabledLoc, true);

    mat4 modelView = T(x * (ttex.width - 1), 0, z * (ttex.height - 1));
    glUniformMatrix4fv(translationMatrixLoc, 1, GL_TRUE, modelView.m);
    mat4 total = Mult(cameraMatrix, modelView);
    glUniformMatrix4fv(mdlMatrixLoc, 1, GL_TRUE, total.m);
    DrawModel(terrainModel, program, "inPosition", "inNormal", "inTexCoord");
}



Model *find_or_generate_terrain(int x_offset, int z_offset) {
    TerrainSection *section;
    int key = x_offset * 100 + z_offset;
    int *pkey = &key;

    HASH_FIND_INT(terrain_cache, pkey, section);

    if (section == NULL) {
        section = (TerrainSection *)malloc(sizeof(TerrainSection));
        section->x_offset = x_offset;
        section->z_offset = z_offset;
        section->model = GenerateTerrain(&ttex, x_offset, z_offset);
        section->key = key; // set the key member
        HASH_ADD_INT(terrain_cache, key, section);
    }

    return section->model;
}


void draw_terrain(mat4 cameraMatrix, vec3 p)
{   
    player_pos_x = (int)ceil(p.x / (ttex.width - 1)) - 1;
    player_pos_z = (int)ceil(p.z / (ttex.height - 1)) - 1;

    for (int x_offset = player_pos_x - 1; x_offset <= player_pos_x + 1; x_offset++) {
        for (int z_offset = player_pos_z - 1; z_offset <= player_pos_z + 1; z_offset++) {
            Model *terrainModel = find_or_generate_terrain(x_offset, z_offset);
            draw_terrain_section(cameraMatrix, terrainModel, x_offset, z_offset);
        }
    }
}


float texture_data_height()  
{
    return ttex.height;
}


void drawSkybox(GLuint texUnit, float cameraAngleY, float cameraAngleX) 
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	glUniform1i(specularLightEnabledLoc, false);
    glUniform1i(ambientLightEnabledLoc, false);
    glUniform1i(diffuseLightEnabledLoc, false);
	glUniform1i(textureEnabledLoc, true);
	glUniform1i(isSkyLoc, true);

	mat4 translationMatrixSkybox = Mult(Rx(cameraAngleY), Ry(cameraAngleX));
	LoadTGATextureSimple("labskybox512.tga", &texUnit);			// Create texture object
	glBindTexture(GL_TEXTURE_2D, texUnit);						// Activate a texture object
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); 	// Texture unit 0
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, translationMatrixSkybox.m);
	DrawModel(skybox, program, "inPosition", "inNormal", "inTexCoord");

    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(isSkyLoc, false);
} 