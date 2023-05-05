#include "terrain.h"
#include "noise_wrapper.h"
#include "terrain_controller_shared.h"

#include "uthash.h"


const int COLLISION_MARGINAL = 10;

float terrainScale = 25;
TextureData ttex; // terrain
Model *tm[9];
int player_pos_x;
int player_pos_z;

typedef struct {
    int x_offset;
    int z_offset;
    Model *model;
	int key;
    UT_hash_handle hh; // required for uthash
} TerrainSection;

TerrainSection *terrain_cache = NULL;


void terrainInit(GLuint *tex1, GLuint *tex2) 
{
    LoadTGATextureSimple("assets/terrain/green_grass.tga", tex1);
    LoadTGATextureSimple("assets/terrain/stones.tga", tex2);
    glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
    glUniform1i(glGetUniformLocation(program, "tex2"), 1); // Texture unit 1

	ttex.width = 256;
	ttex.height = 256;
    printError("init terrain");
}


float wrappedNoise2D(float x, float x_offset, float z, float z_offset, int terrainScale, TextureData *tex) 
{
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
			vertexArray[(x + z * tex->width)].x = x / 1.0;
			vertexArray[(x + z * tex->width)].y = wrappedNoise2D(x, x_offset, z, z_offset, terrainScale, tex);
			vertexArray[(x + z * tex->width)].z = z / 1.0;

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

			texCoordArray[(x + z * tex->width)].x = x;
			texCoordArray[(x + z * tex->width)].y = z;
		}

	// Gives the edges of the map the normals of its neighbour
	for (z = 0; z < tex->height; z++)
	{		
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


Model *find_or_generate_terrain(int x_offset, int z_offset) 
{
    TerrainSection *section;
    int key = x_offset * 100 + z_offset;
    int *pkey = &key;

    HASH_FIND_INT(terrain_cache, pkey, section);

    if (section == NULL) {
        section = (TerrainSection *)malloc(sizeof(TerrainSection));
        section->x_offset = x_offset;
        section->z_offset = z_offset;
        section->model = GenerateTerrain(&ttex, x_offset, z_offset);
        section->key = key;
        HASH_ADD_INT(terrain_cache, key, section);
    }

    return section->model;
}


float find_height(float x, float z)
{
    player_pos_x = (int)ceil(x / (ttex.width - 1)) - 1;
    player_pos_z = (int)ceil(z / (ttex.height - 1)) - 1;

    Model *terrainModel = find_or_generate_terrain(player_pos_x, player_pos_z);

    int width = ttex.width;

	player_pos_x = abs(player_pos_x);
	player_pos_z = abs(player_pos_z);

    // Calculate the relative position within the section
    float rel_x = x - player_pos_x * (ttex.width - 1);
    float rel_z = z - player_pos_z * (ttex.height - 1);

	if (rel_z < 0.0) {
		rel_z += 2*width - 1;
	}

    int x_lower = (int) floor(rel_x);
    int x_upper = x_lower + 1;
    int z_lower = (int) floor(rel_z);
    int z_upper = z_lower + 1;

    // Get the heights of the four surrounding vertices
    float h_ll = terrainModel->vertexArray[(x_lower + z_lower * width)].y;
    float h_lu = terrainModel->vertexArray[(x_lower + z_upper * width)].y;
    float h_ul = terrainModel->vertexArray[(x_upper + z_lower * width)].y;
    float h_uu = terrainModel->vertexArray[(x_upper + z_upper * width)].y;

    // Bilinear interpolation
    float x_ratio = rel_x - x_lower;
    float z_ratio = rel_z - z_lower;
    float y_res = h_ll * (1 - x_ratio) * (1 - z_ratio) + h_ul * x_ratio * (1 - z_ratio) +
                  h_lu * (1 - x_ratio) * z_ratio + h_uu * x_ratio * z_ratio; 

    return y_res;
}


int player_collides_with_terrain(float player_x, float player_z, float player_y) 
{
	for (int x_offset = (int) (player_x - COLLISION_MARGINAL); x_offset <= (int) (player_x + COLLISION_MARGINAL); x_offset+=COLLISION_MARGINAL*2) {
        for (int z_offset = (int) (player_z - COLLISION_MARGINAL); z_offset <= (int) (player_z + COLLISION_MARGINAL); z_offset+=COLLISION_MARGINAL*2) {
    		float terrain_height = find_height(x_offset, z_offset);
			if (terrain_height + COLLISION_MARGINAL > player_y) {
				setTerrainHeight(find_height(player_x, player_z));
				return 1;
			}
        }
    }
	return 0;
}


void detect_collision() 
{
	float x, y, z;
	get_player_pos(&x, &y, &z);
	int collision = player_collides_with_terrain(x, z, y);
	set_collision(collision);
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
