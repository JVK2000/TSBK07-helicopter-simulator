#include "terrain.h"
#include "noise_wrapper.h"
#include "terrain_controller_shared.h"

#include "uthash.h"


const int COLLISION_MARGINAL = 10;

float terrainScale = 25;
TextureData ttex; // terrain
Model *tm[9];
int playerPosX;
int playerPosZ;

typedef struct {
    int offsetX;
    int offsetZ;
    Model *model;
	int key;
    UT_hash_handle hh; // required for uthash
} TerrainSection;

TerrainSection *terrainCache = NULL;


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


float wrappedNoise2D(float x, float offsetX, float z, float offsetZ, int terrainScale, TextureData *tex) 
{
	return (noise2D(x + offsetX * (tex->width - 1), z + offsetZ * (tex->height - 1)) / terrainScale) * 1000;
}


Model* GenerateTerrain(TextureData *tex, int offsetX, int offsetZ)
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
			vertexArray[(x + z * tex->width)].y = wrappedNoise2D(x, offsetX, z, offsetZ, terrainScale, tex);
			vertexArray[(x + z * tex->width)].z = z / 1.0;

			if (x != 0 && x != (tex->width - 1) && z != 0 && z != (tex->height - 1)) 
			{
				float L = wrappedNoise2D(x - 1, offsetX, z, offsetZ, terrainScale, tex);;
				float R = wrappedNoise2D(x + 1, offsetX, z, offsetZ, terrainScale, tex);;
				float B = wrappedNoise2D(x, offsetX, z - 1, offsetZ, terrainScale, tex);
				float T = wrappedNoise2D(x, offsetX, z + 1, offsetZ, terrainScale, tex);
				vec3 mapVec = {2*(R-L), 1, 2*(B-T)};
				vec3 mapNormal = normalize(mapVec);
				if (mapNormal.y < 0) {
					mapNormal = ScalarMult(mapNormal , -1);
				}
				normalArray[(x + z * tex->width)].x = mapNormal.x;
				normalArray[(x + z * tex->width)].y = mapNormal.y;
				normalArray[(x + z * tex->width)].z = mapNormal.z;
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
		
		int maxWidth = tex->width - 1;
		int accessIndex = (maxWidth - 1) + z * tex->width;
		int updateIndex = maxWidth + z * tex->width;
		normalArray[updateIndex].x = normalArray[accessIndex].x;
		normalArray[updateIndex].y = normalArray[accessIndex].y;
		normalArray[updateIndex].z = normalArray[accessIndex].z;
	}
	for (x = 0; x < tex->width; x++)
	{
		normalArray[x].x = normalArray[(x + tex->width)].x;
		normalArray[x].y = normalArray[(x + tex->width)].y;
		normalArray[x].z = normalArray[(x + tex->width)].z;

		int maxHeight = tex->height - 1;
		int accessIndex = x + (maxHeight - 1) * tex->width;
		int updateIndex = x + maxHeight * tex->width;
		normalArray[updateIndex].x = normalArray[accessIndex].x;
		normalArray[updateIndex].y = normalArray[accessIndex].y;
		normalArray[updateIndex].z = normalArray[accessIndex].z;
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


Model *findOrGenerateTerrain(int offsetX, int offsetZ) 
{
    TerrainSection *section;
    int key = offsetX * 100 + offsetZ;
    int *pkey = &key;

    HASH_FIND_INT(terrainCache, pkey, section);

    if (section == NULL) {
        section = (TerrainSection *)malloc(sizeof(TerrainSection));
        section->offsetX = offsetX;
        section->offsetZ = offsetZ;
        section->model = GenerateTerrain(&ttex, offsetX, offsetZ);
        section->key = key;
        HASH_ADD_INT(terrainCache, key, section);
    }

    return section->model;
}


float findHeight(float x, float z)
{
    playerPosX = (int)ceil(x / (ttex.width - 1)) - 1;
    playerPosZ = (int)ceil(z / (ttex.height - 1)) - 1;

    Model *terrainModel = findOrGenerateTerrain(playerPosX, playerPosZ);

    int width = ttex.width;

	playerPosX = abs(playerPosX);
	playerPosZ = abs(playerPosZ);

    // Calculate the relative position within the section
    float relX = x - playerPosX * (ttex.width - 1);
    float relZ = z - playerPosZ * (ttex.height - 1);

	if (relZ < 0.0) {
		relZ += 2*width - 1;
	}

    int lowerX = (int) floor(relX);
    int upperX = lowerX + 1;
    int lowerZ = (int) floor(relZ);
    int upperZ = lowerZ + 1;

    // Get the heights of the four surrounding vertices
    float heightLL = terrainModel->vertexArray[(lowerX + lowerZ * width)].y;
    float heightLU = terrainModel->vertexArray[(lowerX + upperZ * width)].y;
    float heightUL = terrainModel->vertexArray[(upperX + lowerZ * width)].y;
    float heightUU = terrainModel->vertexArray[(upperX + upperZ * width)].y;

    // Bilinear interpolation
    float ratioX = relX - lowerX;
    float ratioZ = relZ - lowerZ;
    float y = heightLL * (1 - ratioX) * (1 - ratioZ) + heightUL * ratioX * (1 - ratioZ) +
                  heightLU * (1 - ratioX) * ratioZ + heightUU * ratioX * ratioZ; 

    return y;
}


int playerCollidesWithTerrain(float playerX, float playerZ, float playerY) 
{
	for (int offsetX = (int) (playerX - COLLISION_MARGINAL); offsetX <= (int) (playerX + COLLISION_MARGINAL); offsetX+=COLLISION_MARGINAL*2) {
        for (int offsetZ = (int) (playerZ - COLLISION_MARGINAL); offsetZ <= (int) (playerZ + COLLISION_MARGINAL); offsetZ+=COLLISION_MARGINAL*2) {
    		float terrainHeight = findHeight(offsetX, offsetZ);
			if (terrainHeight + COLLISION_MARGINAL > playerY) {
				setTerrainHeight(findHeight(playerX, playerZ));
				return 1;
			}
        }
    }
	return 0;
}


void detectCollision() 
{
	float x, y, z;
	getPlayerPos(&x, &y, &z);
	int collision = playerCollidesWithTerrain(x, z, y);
	setCollision(collision);
}


void drawTerrainSection(mat4 cameraMatrix, Model *terrainModel, float x, float z) 
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


void drawTerrain(mat4 cameraMatrix, vec3 p)
{   
    playerPosX = (int)ceil(p.x / (ttex.width - 1)) - 1;
    playerPosZ = (int)ceil(p.z / (ttex.height - 1)) - 1;

    for (int offsetX = playerPosX - 1; offsetX <= playerPosX + 1; offsetX++) {
        for (int offsetZ = playerPosZ - 1; offsetZ <= playerPosZ + 1; offsetZ++) {
            Model *terrainModel = findOrGenerateTerrain(offsetX, offsetZ);
            drawTerrainSection(cameraMatrix, terrainModel, offsetX, offsetZ);
        }
    }
}
