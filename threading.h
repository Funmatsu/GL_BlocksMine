#pragma once
#include "Sky.h"
#include "Chunk.h"
#include "normals.h"

//vector<vec2> chunkCoords;

uint32_t to(ivec2 vec) {
    uint16_t x = (uint16_t)vec.x, y = (uint16_t)vec.y;
    return (x << 16) ^ y;
}

std::queue<uint> chunkRequestQueue;
std::queue<vec2> cloudRequestQueue;
std::mutex queueMutex;
std::mutex cloudqMutex;
std::condition_variable queueCV, cloudqCV;

std::queue<chPack> chunkResultQueue;
std::queue<pair<unique_ptr<CloudMesh>, ivec2>> cloudResultQueue;
std::mutex resultMutex;
std::mutex cloudrMutex;
std::mutex addChunkMutex;
std::mutex meshMutex;
//std::queue<vec2> chunkRequestQueue;
std::mutex chunkRequestMutex;

//std::queue<Chunk> chunkResultQueue;
std::mutex chunkResultMutex;

std::queue<vec2> chunkRequestQueue2;
std::mutex chunkRequestMutex2;

std::queue<Chunk> chunkResultQueue2;
std::mutex chunkResultMutex2;

std::queue<Block> breakResQueue;
std::mutex breakResMutex;

std::queue<vec3> breakReqQueue;
std::mutex breakReqMutex;

std::queue<vec3> placeReqQueue;
std::mutex placeReqMutex;

std::queue<Block> placeResQueue;
std::mutex placeResMutex;
std::mutex worldChunkDataMutex;

std::queue<chNeighPack*> chunkCleanupQueue;
std::queue<chNeighResult*> chunkMeshResult;

std::atomic<bool> chunkGenRunning = true;
std::atomic<bool> chunkGenRunning2 = true;
std::atomic<bool> chunkUpdateGenRunning = true;
std::atomic<bool> blockBreaking = true;
std::atomic<bool> blockPlacing = true;
std::atomic<bool> stopChunkUpdaters = false;
std::atomic<bool> stopChunkNeighCheck = false;
bool blockBreakingOut = false;
bool blockPlacingOut = false;

ivec3   normX0 = ivec3( 1, 0, 0),
        normX1 = ivec3(-1, 0, 0),
        normZ0 = ivec3( 0, 0, 1),
        normZ1 = ivec3( 0, 0,-1),
        normY0 = ivec3( 0, 1, 0),
        normY1 = ivec3( 0,-1, 0);

ivec3   nextNormX0 = normX0 * CHUNK_SIZE,
        nextNormX1 = normX1 * CHUNK_SIZE,
        nextNormZ0 = normZ0 * CHUNK_SIZE,
        nextNormZ1 = normZ1 * CHUNK_SIZE,
        nextNormY0 = normY0 * CHUNK_SIZE,
        nextNormY1 = normY1 * CHUNK_SIZE;

std::random_device rd;
std::mt19937 gen(rd()); // Mersenne Twister engine

int blockTypes1[] = {
    STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, IRON_ORE.id
};
int blockTypes2[] = {
    1, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, IRON_ORE.id, IRON_ORE.id
};
int blockTypes3[] = {
    DIRT_BLOCK.id, STONE_BLOCK.id
};

std::uniform_int_distribution<> dist(0, sizeof(blockTypes1) / sizeof(int) - 1);
std::uniform_int_distribution<> dist2(0, sizeof(blockTypes2) / sizeof(int) - 1);
std::uniform_int_distribution<> dist3(0, sizeof(blockTypes3) / sizeof(int) - 1);

// Base hills: smooth FBm Perlin
FastNoiseLite baseNoise;
// Ridged mountains: sharp features
FastNoiseLite ridgedNoise;
// Mask: controls where mountains vs plains appear
FastNoiseLite maskNoise;
FastNoiseLite caveNoise;
FastNoiseLite cloudNoise;

//cloudNoise.SetFrequency(0.045);  
void initChunksNoise() {
    baseNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    baseNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    baseNoise.SetFrequency(0.001f);      // low frequency = broad features
    baseNoise.SetFractalOctaves(3);
    baseNoise.SetFractalLacunarity(2.0f);
    baseNoise.SetFractalGain(0.5f);

    ridgedNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    ridgedNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    ridgedNoise.SetFrequency(0.001f);     // higher frequency = more detail
    ridgedNoise.SetFractalOctaves(4);
    ridgedNoise.SetFractalLacunarity(2.0f);
    ridgedNoise.SetFractalGain(0.5f);

    //maskNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    maskNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    maskNoise.SetFrequency(0.01f);      // very low frequency = large biome regions
    maskNoise.SetFractalOctaves(4);
    maskNoise.SetFractalGain(0.5f);

    //FastNoiseLite noise;   
    //noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    //noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    //noise.SetFractalOctaves(6);
    //noise.SetFractalLacunarity(2.0f);
    //noise.SetFractalGain(0.5f);

    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    caveNoise.SetFrequency(0.05f);
    cloudNoise.SetFrequency(0.045);
}

bool isAir(Item item);

void generateBlocks(vec2 xyChunk, Chunk* repChunk) {
    mutex genBlocksMutex;
    lock_guard<mutex> lock(genBlocksMutex);
    //auto start = std::chrono::high_resolution_clock::now();

    Item blockType = AIR;

    float cloudDensity = 0;
    float density = 0;
    int maxHeight = 0;
    bool jump = 0;
    Chunk& ch = *repChunk;
    auto norm = [](float n) { return (n + 1) * 0.5f; }; //defining a function inline

    int minX = (xyChunk.x) * CHUNK_SIZE, minZ = (xyChunk.y) * CHUNK_SIZE;
    for (int x = minX; x < minX + CHUNK_SIZE; x++) {
        for (int z = minZ; z < minZ + CHUNK_SIZE; z++) {
            uint skylight = 15;
            ch(x, 1, z) = BEDROCK;
            
            float fx = float(x), fz = float(z);

            float base = norm(baseNoise.GetNoise(fx, fz)) * 10.0f;
            float ridged = norm(ridgedNoise.GetNoise(fx, fz)) * 28.0f;
            float mask = norm(maskNoise.GetNoise(fx, fz));

            // Blend between base and ridged using the mask
            float blend = smoothstep(0.0f, 1.0f, mask);
            float height = mix(base, ridged, blend);
            float treeHeight = randomFloat(3.0, 5.0) + 2, treeDistrib = rand();
            float randomNumberForDeco = rand();

            //float scaledHeight = height;
            float scaledHeight = height * 4 + CHUNK_SIZE * 5;
            maxHeight = (scaledHeight > maxHeight) ? scaledHeight + treeHeight + 5 : maxHeight;

            for (int y = scaledHeight; y > 1; y--) {
                jump = 0;
                
                int radius = 3;  //adjust for size
                if (y >= scaledHeight - 1) {
                    blockType = GRASS_BLOCK;
                }
                else if (y > scaledHeight - 4 && y < scaledHeight - 1) {
                    blockType = DIRT_BLOCK;
                }
                else if (y <= scaledHeight - 4 && y > scaledHeight - 7) {
                    blockType = items[blockTypes3[dist3(gen)]];
                }
                else if (y <= scaledHeight - 7 && y > scaledHeight - 20) {
                    blockType = items[blockTypes1[dist(gen)]];
                }
                else if (y <= scaledHeight - 20) {
                    blockType = items[blockTypes2[dist2(gen)]];
                }
                //else continue;
                density = caveNoise.GetNoise(fx, (float)y, fz);
                //if (!(blockType == 0) && skylight > 1) { ch(x, y, z).blight = skylight--; }
                if (density < -0.25f) {
                    continue;
                }
                ch(x, y, z) = blockType;

                //Making the trees and leaves
                if (x >= (xyChunk.x + 1) * CHUNK_SIZE - radius || x <= (xyChunk.x) * CHUNK_SIZE + radius ||
                    z >= (xyChunk.y + 1) * CHUNK_SIZE - radius || z <= (xyChunk.y) * CHUNK_SIZE + radius) {
                    jump = 1;
                    //goto notree;
                }
                if (blockType == GRASS_BLOCK && !jump) {
                    if (treeDistrib > 0.0 && treeDistrib <= 75) {

                        for (int i = y + 1; i < y + treeHeight; i++) {
                            ch(x, i, z) = OAK_WOOD;
                        }

                        float floorHeight = y + treeHeight + 1;

                        for (int dx = -radius; dx <= radius; ++dx) {
                            for (int dy = -radius; dy <= radius; ++dy) {
                                for (int dz = -radius; dz <= radius; ++dz) {
                                    float dist = glm::length(vec3(dx, dy, dz));

                                    if (dist <= radius + randomFloat(-0.3f, 0.3f)) {
                                        BlockData& bdt   = ch(x + dx, floorHeight + dy, z + dz),
                                                   bdtup = ch(x + dx, floorHeight + 1, z + dz);

                                        if (bdt == AIR) {
                                            bdt = OAK_LEAVES;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            //notree:;  //No trees should be created if they are near the chunks border

                if (blockType == GRASS_BLOCK && blockType != OAK_WOOD) {
                    BlockData& blockData = ch(x, y + 1, z);
                    if (blockData == AIR) {
                        if (randomNumberForDeco < 2000) {
                            blockData = BlockData(GRASS);
                            //for (int i = 1; i <= randomFloat(1, 2); i++) {
                            //    BlockData& blockData = ch(x, y + i, z);
                            //    blockData = GRASS;
                            //}
                        }
                        else if (randomNumberForDeco > 2000 && randomNumberForDeco < 2500) {
                            blockData = POPPY;
                        }
                        else if (randomNumberForDeco > 2500 && randomNumberForDeco < 2800) {
                            blockData = BLUE_ORCHID;
                        }
                    }

                }
            }
        }
    }
    //repChunk->terrainHeight = maxHeight;
    //auto end = chrono::high_resolution_clock::now();
    //double timelapsed = chrono::duration<double>(end - start).count();
    //cout << chunkCount++ << "chunks, " << timelapsed << "s" << endl;
}

void generateClouds(CloudMesh& chm) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            chm(x, z) = 1;
        }
    }
}

bool isAir(Item item) { return item == AIR; }

bool shouldEmitFace(vec2 xyChunk, Chunk* cd, Item currentBlockType, int x, int y, int z, int dx, int dy, int dz) {
    if (currentBlockType.isFlat() || currentBlockType == OAK_PLANK) return 1;
    int nx = x + dx, ny = y + dy, nz = z + dz;
    //ivec3 checkPos = ivec3(nx, ny, nz);
    if (!cd->inBounds(nx, ny, nz) && ny >= -1 && ny < CHUNK_HEIGHT) {
        if (ny == -1) return 1;
        uint32_t pos = pack(ivec2(floorDiv(nx, CHUNK_SIZE), floorDiv(nz, CHUNK_SIZE)));
        if (!world.chunkData.count(pos)) return 0;
        else {
            BlockData& bData = (*world.chunkData[pos])(nx, ny, nz);
            return (isAir(bData.blockType)) || bData.blockType.isFlat();
        }
    }
    else if (ny < 0 || ny >= CHUNK_HEIGHT) { return 0; }
    BlockData& blockData = (*cd)(nx, ny, nz);
    Item& blockType = blockData.blockType;
    if (blockType == OAK_LEAVES) return 1;
    return isAir(blockType) ||
        blockType.isFlat() ||
        currentBlockType == TORCH;
}

void emitFace(Mesh& m, int face, Item blockType, float x, float y, float z, vec3 direction) {
    float tintr = 1.0f, tintg = 1.0f, tintb = 1.0f;
    if (blockType == GRASS_BLOCK && face != 4) {
        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
    }
    else if (blockType == GRASS) {
        tintr = 0.2f, tintg = 1.45f, tintb = 0.15f;
    }
    else if (blockType == OAK_LEAVES) {
        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
    }
    //vec3 rgb = { tintr, tintg, tintb };
    vec3 cMask = { tintr, tintg, tintb };

    // face: 0=-X,1=+X,2=-Z,3=+Z,4=+Z,5=-Z
    // Define 4 positions and normal per face
    static const glm::vec3 normals[6] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}, {0, 1, 0}, { 0, -1, 0 }
    };

    auto absl = [](int n) { return n >= 0 ? n : -n; };
    glm::vec3 n = normals[face];

    const int bottomDir = 4 - 4 * (-abs(direction.y) + 1) + (abs(direction.y)) * (0.5 * (-direction.y + 1)) + (-abs(direction.y) + 1) * (0.5 * (-direction.x + 1)) * (-direction.z * direction.z + 1) + (-abs(direction.y) + 1) * (-direction.x * direction.x + 1) * (2 + 0.5 * (-direction.z + 1));
    const int topDir = 5 - 4 * (-abs(direction.y) + 1) + (abs(direction.y)) * (0.5 * (direction.y + 1) - 1) + (-abs(direction.y) + 1) * (0.5 * (direction.x + 1) - 1) * (-direction.z * direction.z + 1) + (-abs(direction.y) + 1) * (-direction.x * direction.x + 1) * (1 + 0.5 * (direction.z + 1));

    glm::vec3 v[4];
    if (blockType == TORCH) {
        switch (face) {
        case 0: /* left face */   v[0] = { x + 0.4375, y + 0.0, z + 0.4375 };      v[1] = { x + 0.4375, y + 0.5625, z + 0.4375 };   v[2] = { x + 0.4375, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.4375, y + 0.0, z + 0.5625 };    break;
        case 1: /* right face */  v[0] = { x + 0.5625, y + 0.0, z + 0.4375 };      v[1] = { x + 0.5625, y + 0.0, z + 0.5625 };      v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.5625, y + 0.5625, z + 0.4375 }; break;
        case 2: /* front face */  v[0] = { x + 0.4375, y + 0.0, z + 0.4375 };      v[1] = { x + 0.5625, y + 0.0, z + 0.4375 };      v[2] = { x + 0.5625, y + 0.5625, z + 0.4375 };   v[3] = { x + 0.4375, y + 0.5625, z + 0.4375 }; break;
        case 3: /* back face */   v[0] = { x + 0.4375, y + 0.0, z + 0.5625 };      v[1] = { x + 0.4375, y + 0.5625, z + 0.5625 };   v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.5625, y + 0.0, z + 0.5625 };    break;
        case 4: /* bottom face */ v[0] = { x + 0.0   , y + 0.0, z + 0.0 };         v[1] = { x + 0.0, y + 0.0, z + 0.0 };            v[2] = { x + 0.0, y + 0.0, z + 0.0 };            v[3] = { x + 0.0, y + 0.0, z + 0.0 };          break;
        case 5: /* top face */    v[0] = { x + 0.4375, y + 0.5625, z + 0.4375 };   v[1] = { x + 0.5625, y + 0.5625, z + 0.4375 };   v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.4375, y + 0.5625, z + 0.5625 }; break;
        }
    }
    else if (!blockType.isFlat()) {
        switch (face) {
        case 0: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + -0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + -0.5f,y + -0.5f,z + 0.5f }; break; // -X
        case 1: v[0] = { x + 0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + 0.5f,y + -0.5f,z + 0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + 0.5f,y + 0.5f,z + -0.5f }; break; // +X
        case 2: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + 0.5f,y + -0.5f,z + -0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + -0.5f }; v[3] = { x + -0.5f,y + 0.5f,z + -0.5f }; break; // -Z
        case 3: v[0] = { x + -0.5f,y + -0.5f,z + 0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + 0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + 0.5f,y + -0.5f,z + 0.5f }; break; // +Z
        case 4: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + -0.5f,z + 0.5f }; v[2] = { x + 0.5f,y + -0.5f,z + 0.5f }; v[3] = { x + 0.5f,y + -0.5f,z + -0.5f }; break; // -Y
        case 5: v[0] = { x + -0.5f,y + 0.5f,z + -0.5f };   v[1] = { x + 0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + -0.5f,y + 0.5f,z + 0.5f }; break; // +Y
        }
    }
    else {
        switch (face) {
        case 0: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + 0.5f,y + -0.5f,z + 0.5f };  break; // -X disgonal
        case 1: v[0] = { x + -0.5f,y + -0.5f,z + 0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + 0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + -0.5f }; v[3] = { x + 0.5f,y + -0.5f,z + -0.5f };  break; // +X diagonal
        case 2: v[0] = { x + 0.5f,y + -0.5f,z + 0.5f };   v[1] = { x + 0.5f,y + 0.5f,z + 0.5f }; v[2] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[3] = { x + -0.5f,y + -0.5f,z + -0.5f };  break; // -X disgonal
        case 3: v[0] = { x + 0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + 0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + -0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + -0.5f,y + -0.5f,z + 0.5f };  break; // +X diagonal
        }
    }

    // Simple tile UV (replace with atlas lookup per block/face)
    vector<glm::vec3> uv;

    float UVs[7];
    getUVs(blockType, UVs);
    float xoffset = UVs[0],
        yoffset = UVs[1],
        xoffsetTop = UVs[2],
        yoffsetTop = UVs[3],
        xoffsetBottom = UVs[4],
        yoffsetBottom = UVs[5],
        transparency = UVs[6];

    float clipX = 0.0f, clipY = 1.0f;
    int offsetX = 0, offsetY = 0;

    if (blockType == TORCH) { clipX = 0.4375 - 0.5; clipY = 0.5625 - 0.5; }

    if (face == bottomDir) { offsetX = xoffsetBottom; offsetY = yoffsetBottom; }
    else if (face == topDir) { offsetX = xoffsetTop;  offsetY = yoffsetTop; }

    uint32_t uintUVs = ((uint8_t(y) << 24)) | ((xdimens << 16) & 0xFFFFFF) | ((uint8_t)(yoffset + offsetY) << 8) | ((uint8_t)(xoffset + offsetX)); // Packaging floats into one integer
    float startUvs;
    memcpy(&startUvs, &uintUVs, sizeof(float));

    auto feed = [](vector<vec3>& uv, float l, float h, float packed, int dir, float w = 0) { // l is for low and h is for high; Transmits texture coordinates to vector.
        if (!dir) { // cw feed
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(l, h, packed));
            uv.push_back(vec3(h, h, packed));
            uv.push_back(vec3(h, l, packed));
        }
        else if (dir == 1) { // ccw feed
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(h, l, packed));
            uv.push_back(vec3(h, h, packed));
            uv.push_back(vec3(l, h, packed));
        }
        else if (dir == 2) {
            uv.push_back(vec3(l, h, packed));
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(h, l, packed));
            uv.push_back(vec3(h, h, packed));
        }
        else if (dir == 3) {
            uv.push_back(vec3(h, l, packed));
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(l, h, packed));
            uv.push_back(vec3(h, h, packed));
        }
        else if (dir == 4) {
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(h, l, packed));
            uv.push_back(vec3(h, w, packed));
            uv.push_back(vec3(l, w, packed));
        }
        else if (dir == 5) {
            uv.push_back(vec3(l, l, packed));
            uv.push_back(vec3(l, w, packed));
            uv.push_back(vec3(h, w, packed));
            uv.push_back(vec3(h, l, packed));
        }
        };

    switch (face) {
    case 1:
    case 2:
    case 5: if (!blockType.isFlat() || blockType == TORCH) {
        if (abs(direction.x) && !blockType.isFlat() || abs(direction.z) && !blockType.isFlat()) { feed(uv, 0.0f, 1.0f, startUvs, 0); }
        else if (blockType == TORCH) { feed(uv, 0.0f, 1.0f, startUvs, 4, 0.625f); }
        else { feed(uv, 0.0f, 1.0f, startUvs, 1); }
    }
          else { // for grass and other flats
        feed(uv, 1.0f, 0.0f, startUvs, 2);
    }
          break;
    default:
        if (blockType == TORCH && face != 5 && blockType != GRASS) { feed(uv, 0.0f, 1.0f, startUvs, 5, 0.625f); }
        else {
            if (abs(direction.x) && !blockType.isFlat()) {
                if (face == 5) { feed(uv, 1.0f, 0.0f, startUvs, 0); }
                else { feed(uv, 1.0f, 0.0f, startUvs, 3); }
            }
            else if (abs(direction.z) && !blockType.isFlat()) {
                if (face == 5) { feed(uv, 0.0f, 1.0f, startUvs, 3); }
                else if (face == 4) { feed(uv, 0.0f, 1.0f, startUvs, 0); }
                else { feed(uv, 1.0f, 0.0f, startUvs, 3); }
            }
            else { feed(uv, 0.0f, 1.0f, startUvs, 0); }
        }
        break;
    }

    uint32_t norm_color = ((abyte(n.x < 0 ? 1 : 0) & 0x1) << 5) | ((abyte(absl(n.x)) & 0x1) << 4)
        | ((abyte(n.y < 0 ? 1 : 0) & 0x1) << 3) | ((abyte(absl(n.y)) & 0x1) << 2)
        | ((abyte(n.z < 0 ? 1 : 0) & 0x1) << 1) | ((abyte(absl(n.z)) & 0x1) << 0)
        | ((abyte(cMask.x * 100) & 0x7F) << 20)
        | ((abyte(cMask.y * 100) & 0x7F) << 13)
        | ((abyte(cMask.z * 100) & 0x7F) << 6);
    float normcolor;
    memcpy(&normcolor, &norm_color, sizeof(float));

    uint32_t base = (uint32_t)(m.vertices.size() / 7);
    for (int i = 0; i < 4; i++) {
        m.vertices.push_back(v[i].x);
        m.vertices.push_back(v[i].y);
        m.vertices.push_back(v[i].z);
        m.vertices.push_back(uv[i].x);
        m.vertices.push_back(uv[i].y);
        m.vertices.push_back(uv[i].z);
        m.vertices.push_back(normcolor);
    }
    // Two triangles (0,1,2) (2,3,0)
    m.indices.push_back(base + 1); m.indices.push_back(base + 2); m.indices.push_back(base + 3);
    m.indices.push_back(base + 3); m.indices.push_back(base + 0); m.indices.push_back(base + 1);
}

void meshChunk(vec2 xyChunk, Chunk* cd, Mesh& out, vec3 direction, ivec3 position) {
    //out.vertices.clear(); out.indices.clear();
    //out.vertices.reserve(CHUNK_SIZE * (CHUNK_SIZE * CHUNK_SIZE)); // heuristic
    //out.indices.reserve(CHUNK_SIZE * (CHUNK_SIZE * CHUNK_SIZE));

    for (int x = (xyChunk.x) * CHUNK_SIZE; x < (xyChunk.x + 1) * CHUNK_SIZE; ++x)
        for (int y = 0; y < (CHUNK_SIZE * CHUNK_SIZE); ++y) // + 2 * CHUNK_SIZE
            for (int z = (xyChunk.y) * CHUNK_SIZE; z < (xyChunk.y + 1) * CHUNK_SIZE; ++z) {
                BlockData& block_data = (*cd)(x, y, z);
                Item& blockType = block_data.blockType;
                if (isAir(block_data.blockType)) continue;
                auto absl = [](int n) { return n >= 0 ? n : -n; };
                auto sign_shift = [](uint8_t num, int shift) { return ((num << shift & 1) == 1 ? -1 : 1); };
                vec3 blockOrientation;
                //vec3 blockOrientation = vec3(sign_shift(block_data.orientation, 5) * (block_data.orientation >> 4) & 1,
                //                             sign_shift(block_data.orientation, 3) * (block_data.orientation >> 2) & 1,
                //                             sign_shift(block_data.orientation, 1) * (block_data.orientation >> 0) & 1);
                if (direction != vec3(0, 1, 0)){//&& position == blockPos) {
                    //block_data.orientation = ((direction.x < 0 ? 1 : 0) << 5) | ((absl(direction.x)) << 4) |
                    //                         ((direction.x < 0 ? 1 : 0) << 3) | ((absl(direction.y)) << 2) |
                    //                         ((direction.x < 0 ? 1 : 0) << 1) | ((absl(direction.z)) << 0);
                    blockOrientation = direction;
                }

                if (!blockType.isFlat() || blockType == TORCH)
                {
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, -1, 0, 0)) emitFace(out, 0, blockType, x, y, z, blockOrientation);
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, +1, 0, 0)) emitFace(out, 1, blockType, x, y, z, blockOrientation);
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, 0, 0, -1)) emitFace(out, 2, blockType, x, y, z, blockOrientation);
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, 0, 0, +1)) emitFace(out, 3, blockType, x, y, z, blockOrientation);
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, 0, -1, 0)) emitFace(out, 4, blockType, x, y, z, blockOrientation);
                    if (shouldEmitFace(xyChunk, cd, blockType, x, y, z, 0, +1, 0)) emitFace(out, 5, blockType, x, y, z, blockOrientation);
                }
                else {
                    for (int i = 0; i < 4; i++) {
                        emitFace(out, i, blockType, x, y, z, direction);
                    }
                }
            }
}

//void generateChunkAt(vec2 xyChunk, Chunk* repChunk) {
//    auto start = std::chrono::high_resolution_clock::now();
//
//    generateBlocks(xyChunk, repChunk);
//    meshChunk(xyChunk, repChunk, repChunk->mesh, vec3(0, 1, 0), vec3(0, -1, 0));
//
//    auto end = std::chrono::high_resolution_clock::now();
//    cout << repChunk->mesh.vertices.size() << " : Elapsed: " << std::chrono::duration<double>(end - start).count() << " s\n";
//}

/// <summary>
/// This will begin an attempt for a fast greedy meshing algorithm
/// </summary>
bool shouldEmitFace(vec2 xyChunk, Chunk* cd, Item currentBlockType, ivec3 checkPos) {
    if (currentBlockType.isFlat() || currentBlockType == OAK_LEAVES || currentBlockType == TORCH) return 1;
    if (!cd->inBounds(checkPos)) {
        if (checkPos.y == -1) return 1;
        else if (checkPos.y >= 0 && checkPos.y < CHUNK_HEIGHT) {
            uint32_t pos = pack(ivec2(floorDiv(checkPos.x, CHUNK_SIZE), floorDiv(checkPos.z, CHUNK_SIZE)));
            Item& bType = (*world.chunkData[pos])[checkPos].blockType;
            return (isAir(bType)) || bType.isFlat();
        }
        else return 0;
    }
    Item& blockType = (*cd)[checkPos].blockType;
    return isAir(blockType) ||
        blockType.isFlat();
}

bool shouldEmitFace(vec2 xyChunk, Chunk* cd, Chunk* neighChunk, Item currentBlockType, ivec3 checkPos) {
    //if (currentBlockType.isFlat() || (checkPos.y == -1) || currentBlockType == OAK_LEAVES || currentBlockType == TORCH) return 1;
    //if (cd->inBounds(checkPos)) {
    //    Item& bType = (*cd)[checkPos].blockType;
    //    return isAir(bType) || bType.isFlat();
    //}
    //if (neighChunk->inBounds(checkPos)) {
    if (checkPos.y >= 0 && checkPos.y < CHUNK_HEIGHT) {
        //uint32_t pos = pack(ivec2(floorDiv(checkPos.x, CHUNK_SIZE), floorDiv(checkPos.z, CHUNK_SIZE)));
        Item& bType = (*neighChunk)[checkPos].blockType;
        return (isAir(bType)) || bType.isFlat();
    }
    //}
    return 0;
}

ivec3 comb[]{
    {0,1,2},
    {2,1,0},
    {1,2,0},
};

vec3 getMaskCellPos(int planePos, int l, int b, int face) {
    int combs[3];
    combs[comb[face].x] = planePos;
    combs[comb[face].y] = l;
    combs[comb[face].z] = b;

    return vec3(combs[0], combs[1], combs[2]);
}

vec3 getMaskCellPos(int planePos, int l, int b, int minX, int minZ, int face) {
    int combs[3];
    combs[comb[face].x] = planePos;
    combs[comb[face].y] = l;
    combs[comb[face].z] = b;

    return vec3(combs[0] + minX, combs[1], combs[2] + minZ);
}

bool buildMask(Chunk* ch, BlockData* bData, int planeDirVal, int faceDir, int W, int H, uint8_t* mask, ivec3 normal) {
    bool allAir = true;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W; b++) {
            int idx = b + l * W;
            uint8_t& maskItem = mask[idx];
            ivec3 pos = getMaskCellPos(planeDirVal, l, b, faceDir / 2); //(dir == 0) ? ivec3(planeDirVal + minX, l, minZ + b) : (dir == 1) ? ivec3(minX + b, l, planeDirVal + minZ) : ivec3(minX + b, planeDirVal, minZ + l);            
            if (pos.y == -1) continue;
            ivec3 checkPos = pos + normal;
            if (checkPos.y == -1 || checkPos.y >= CHUNK_HEIGHT) continue;

            uint8_t neighType = bData[at(checkPos)].blockType.id;
            if (neighType == 0 || isFlat(neighType) || neighType == 8)
                maskItem = bData[at(pos)].blockType.id;
        }
    }
    return allAir;
}

bool buildMaskY(BlockData* bData, int planeDirVal, int faceDir, int W, int H, uint8_t* mask, ivec3 normal) {
    bool allAir = true;
    //ivec3 pos = ivec3(0, planeDirVal, 0);
    int posx, posy = planeDirVal, posz;
    int checkposy = posy + normal.y;

    if (posy < 0 || checkposy < 0 || checkposy >= CHUNK_HEIGHT) return allAir;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W; b++) {
            int idx = b + l * W;
            uint8_t& maskItem = mask[idx]; maskItem = 0;
            posx = b, posz = l; 

            uint8_t neighItem = bData[at(posx, checkposy, posz)].blockType.id;
            if (items[neighItem].isUncullable())
                maskItem = bData[at(posx, posy, posz)].blockType.id;
        }
    }
    return allAir;
}

void buildMask(Chunk* ch, BlockData* bData, BlockData* neighData, int planeDirVal, int faceDir, int W, int H, uint8_t* mask, ivec3 realnorm, ivec3 normal) {
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W; b++) {
            int idx = b + l * W;
            uint8_t& maskItem = mask[idx];
            ivec3 pos = getMaskCellPos(planeDirVal, l, b, faceDir / 2); //(dir == 0) ? ivec3(planeDirVal + minX, l, minZ + b) : (dir == 1) ? ivec3(minX + b, l, planeDirVal + minZ) : ivec3(minX + b, planeDirVal, minZ + l);            
            if (pos.y == -1) continue;
            ivec3 checkPos = pos + realnorm;
            if (checkPos.y == -1 || checkPos.y >= CHUNK_HEIGHT) continue;

            BlockData* neighbour = nullptr; int nextPos;
            if (ch->localInBounds(checkPos)) { neighbour = bData;  nextPos = at(checkPos); }
            else { neighbour = neighData; nextPos = at(checkPos + normal); }
            Item neighItem = neighbour[nextPos].blockType;
            uint8_t neighType = neighItem.id;
            if (neighType == 0 || neighItem.isFlat() || neighType == 8) {
                if (ch->localInBounds(pos)) maskItem = bData[at(pos)].blockType.id;
                else { 
                    int index = at(pos + normal);  
                    maskItem = neighData[index].blockType.id;
                }
            }
        }
    }
}

void buildMaskX(BlockData* bData, BlockData* neighData, int planeDirVal, int faceDir, int W, int H, uint8_t* mask, ivec3 realnorm, ivec3 normal) {
    int posz, posy, posx = planeDirVal;
    int checkposx = planeDirVal + realnorm.x;
    int xnorm = normal.x * CHUNK_SIZE; // indexing adds CHUNK_SIZE for each unit step in the x axis

    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W; b++) {
            int idx = b + l * W;
            uint8_t& maskItem = mask[idx]; maskItem = 0;
            posy = l, posz = b;

            BlockData* neighbour = nullptr; int nextPos = at(checkposx, posy, posz);
            if (localInBounds(checkposx, posy, posz)) { neighbour = bData; }
            else { neighbour = neighData; nextPos += xnorm; }
            uint8_t neighItem = neighbour[nextPos].blockType.id;
            if (items[neighItem].isUncullable()) {
                int index = at(posx, posy, posz);
                if (localInBounds(posx, posy, posz)) maskItem = bData[index].blockType.id;
                else { maskItem = neighData[index + xnorm].blockType.id; }
            }
        }
    }
}

void buildMaskZ(BlockData* bData, BlockData* neighData, int planeDirVal, int faceDir, int W, int H, uint8_t* mask, ivec3 realnorm, ivec3 normal) {
    int posx, posy, posz = planeDirVal;
    int checkposz = planeDirVal + realnorm.z;
	int znorm = normal.z;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W; b++) {
            int idx = b + l * W;
            uint8_t& maskItem = mask[idx]; maskItem = 0;
            posx = b, posy = l;

            BlockData* neighbour = nullptr; int nextPos = at(posx, posy, checkposz);
            if (localInBounds(posx, posy, checkposz)) { neighbour = bData; }
            else { neighbour = neighData; nextPos += znorm; }
            uint8_t neighItem = neighbour[nextPos].blockType.id;
            if (items[neighItem].isUncullable()) {
                int index = at(posx, posy, posz);
                if (localInBounds(posx, posy, posz)) maskItem = bData[index].blockType.id;
                else { maskItem = neighData[index + znorm].blockType.id; }
            }
        }
    }
}

#define absl(n) n >= 0 ? n : -n

void feed(float* uv, float w, float h, float packed, int direction) { // direction = 0 ? clockwise : counterclockwise
    if (!direction) { //feedCW
        uv[0] = 0, uv[ 1] = 0, uv[ 2] = packed;
        uv[3] = 0, uv[ 4] = h, uv[ 5] = packed;
        uv[6] = w, uv[ 7] = h, uv[ 8] = packed;
        uv[9] = w, uv[10] = 0, uv[11] = packed;
    }
    else { // feedCCW 
        uv[0] = 0, uv[ 1] = 0, uv[ 2] = packed;
        uv[3] = w, uv[ 4] = 0, uv[ 5] = packed;
        uv[6] = w, uv[ 7] = h, uv[ 8] = packed;
        uv[9] = 0, uv[10] = h, uv[11] = packed;
    }
    };

void emitFace(Mesh& m, int face, uint8_t blockType, ivec3 blockPos, ivec3 dims, ivec3 normals, int& base) {
    float v[12];
    float uv[12];
    vec3 cMask(1.0);
    const int bottomDir = 4;
    const int topDir = 5;
    float tintr, tintg, tintb;
    if ((blockType == GRASS.id || blockType == OAK_LEAVES.id) || (blockType == GRASS_BLOCK.id && face != 4)) {
        tintr = 0.1f, tintg = 0.75f, tintb = 0.1f;
        cMask = { tintr, tintg, tintb };
        if (isFlat(blockType)) { normals = vec3(0, -1, 0); }
    }
    bool flat = items[(blockType)].isFlat();

    float* UVs;
    UVs = getUVs(blockType);
    float xoffset = UVs[0],
        yoffset = UVs[1],
        xoffsetTop = UVs[2],
        yoffsetTop = UVs[3],
        xoffsetBottom = UVs[4],
        yoffsetBottom = UVs[5],
        transparency = UVs[6];

    int dir = face / 2;
    float clipX = 0.0f, clipY = 1.0f;
    float clipXX = 0.0f, clipXY = !flat ? ((dir == 0) ? dims.z : dims.x) : 1.0f,
        clipYX = 0.0f, clipYY = !flat ? ((dir == 2) ? dims.z : dims.y) : 1.0f;
    int   offsetX = 0, offsetY = 0;
    // Simple tile UV (replace with atlas lookup per block/face but now is length of tile based in greedy meshing)

    if (!flat) {
        switch (face) {
        case 0: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;             v[3] = blockPos.x + -0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + -0.5f;           v[6] = blockPos.x + -0.5f, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f + dims.z; v[9] = blockPos.x + -0.5f, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + -0.5f + dims.z; break; // -X
        case 1: v[0] = blockPos.x + -0.5f + dims.x, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;             v[3] = blockPos.x + -0.5f + dims.x, v[4] = blockPos.y + -0.5f, v[5] = blockPos.z + -0.5f + dims.z;  v[6] = blockPos.x + -0.5f + dims.x, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f + dims.z; v[9] = blockPos.x + -0.5f + dims.x, v[10] = blockPos.y + -0.5f + dims.y, v[11] = blockPos.z + -0.5f; break; // +X
        case 2: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;             v[3] = blockPos.x + -0.5f + dims.x, v[4] = blockPos.y + -0.5f, v[5] = blockPos.z + -0.5f;           v[6] = blockPos.x + -0.5f + dims.x, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f; v[9] = blockPos.x + -0.5f, v[10] = blockPos.y + -0.5f + dims.y, v[11] = blockPos.z + -0.5f; break; // -Z
        case 3: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f + dims.z;    v[3] = blockPos.x + -0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + -0.5f + dims.z;  v[6] = blockPos.x + -0.5f + dims.x, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f + dims.z; v[9] = blockPos.x + -0.5f + dims.x, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + -0.5f + dims.z; break; // +Z
        case 4: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;             v[3] = blockPos.x + -0.5f, v[4] = blockPos.y + -0.5f, v[5] = blockPos.z + -0.5f + dims.z;  v[6] = blockPos.x + -0.5f + dims.x, v[7] = blockPos.y + -0.5f, v[8] = blockPos.z + -0.5f + dims.z; v[9] = blockPos.x + -0.5f + dims.x, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + -0.5f; break; // -Y
        case 5: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f + dims.y, v[2] = blockPos.z + -0.5f;             v[3] = blockPos.x + -0.5f + dims.x, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + -0.5f;           v[6] = blockPos.x + -0.5f + dims.x, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f + dims.z; v[9] = blockPos.x + -0.5f, v[10] = blockPos.y + -0.5f + dims.y, v[11] = blockPos.z + -0.5f + dims.z; break; // +Y
        }
    }
    else {
        switch (face) {
        case 0: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;   v[3] = blockPos.x + -0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + -0.5f; v[6] = blockPos.x + 0.5f, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + 0.5f; v[9] = blockPos.x + 0.5f, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + 0.5f;  break; // -X disgonal
        case 1: v[0] = blockPos.x + -0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + 0.5f;   v[3] = blockPos.x + -0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + 0.5f; v[6] = blockPos.x + 0.5f, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f; v[9] = blockPos.x + 0.5f, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + -0.5f;  break; // +X diagonal
        case 2: v[0] = blockPos.x + 0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + 0.5f;   v[3] = blockPos.x + 0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + 0.5f; v[6] = blockPos.x + -0.5f, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + -0.5f; v[9] = blockPos.x + -0.5f, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + -0.5f;  break; // -X disgonal
        case 3: v[0] = blockPos.x + 0.5f, v[1] = blockPos.y + -0.5f, v[2] = blockPos.z + -0.5f;   v[3] = blockPos.x + 0.5f, v[4] = blockPos.y + -0.5f + dims.y, v[5] = blockPos.z + -0.5f; v[6] = blockPos.x + -0.5f, v[7] = blockPos.y + -0.5f + dims.y, v[8] = blockPos.z + 0.5f; v[9] = blockPos.x + -0.5f, v[10] = blockPos.y + -0.5f, v[11] = blockPos.z + 0.5f;  break; // +X diagonal
        }
    }

    if (face == bottomDir) { offsetX = xoffsetBottom; offsetY = yoffsetBottom; }
    else if (face == topDir) { offsetX = xoffsetTop;  offsetY = yoffsetTop; }

    uint32_t uintUVs = ((uint8_t)(blockPos.y/3) << 24) | ((uint8_t)(xdimens) << 16) | ((uint8_t)(yoffset + offsetY) << 8) | (uint8_t)(xoffset + offsetX); // Packaging floats into one integer
    float startUvs;
    memcpy(&startUvs, &uintUVs, sizeof(float));
    uint32_t norm_color = ((abyte(normals.x < 0 ? 1 : 0) & 0x1) << 5) | ((abyte(absl(normals.x)) & 0x1) << 4)
        | ((abyte(normals.y < 0 ? 1 : 0) & 0x1) << 3) | ((abyte(absl(normals.y)) & 0x1) << 2)
        | ((abyte(normals.z < 0 ? 1 : 0) & 0x1) << 1) | ((abyte(absl(normals.z)) & 0x1) << 0)
        | ((abyte(cMask.x * 100) & 0x7F) << 20)
        | ((abyte(cMask.y * 100) & 0x7F) << 13)
        | ((abyte(cMask.z * 100) & 0x7F) << 6);
    float normcolor;
    memcpy(&normcolor, &norm_color, sizeof(float));

    int direction = (face == 0 || face == 3 || face == 4) ? 0 : !flat;
    feed(uv, clipXY, clipYY, startUvs, direction);

    float finalVerts[28], finalInds[6];

    for (int i = 0; i < 4; i++) {
        int idx = 7 * i, uvidx = 3 * i;
        finalVerts[idx + 0] = v[uvidx + 0];
        finalVerts[idx + 1] = v[uvidx + 1];
        finalVerts[idx + 2] = v[uvidx + 2];
        finalVerts[idx + 3] = uv[uvidx + 0];
        finalVerts[idx + 4] = uv[uvidx + 1];
        finalVerts[idx + 5] = uv[uvidx + 2];
        finalVerts[idx + 6] = normcolor;
    }
    finalInds[0] = base + 0; finalInds[1] = base + 1; finalInds[2] = base + 2;
    finalInds[3] = base + 2; finalInds[4] = base + 3; finalInds[5] = base + 0;

    m.vertices.insert(m.vertices.end(), finalVerts, finalVerts + 28);
    m.indices.insert(m.indices.end(), finalInds, finalInds + 6);
    // Two triangles (0,1,2) (2,3,0)
    base += 4;
}

void emitFaceX(Mesh& m, int face, uint8_t blockType, int blockPosx, int blockPosy, int blockPosz, int dimsx, int dimsy, int dimsz, ivec3 normals, int& base) {
    auto feed = [&](vec3* uv, float w, float h, float packed) { // direction = 0 ? clockwise : counterclockwise
            uv[0] = vec3(0, 0, packed);
            uv[1] = vec3(0, h, packed);
            uv[2] = vec3(w, h, packed);
            uv[3] = vec3(w, 0, packed); };
    float tintr = 1.0f, tintg = 1.0f, tintb = 1.0f;
    if ((blockType == GRASS.id || blockType == OAK_LEAVES.id) || (blockType == GRASS_BLOCK.id && face != 4)) {
        tintr = 0.1f, tintg = 0.75f, tintb = 0.1f;
        if (isFlat(blockType)) { normals = vec3(0, -1, 0); } }
    bool flat = item(blockType).isFlat(); 
    float* UVs;
    UVs = getUVs(blockType);
    float xoffset = UVs[0],
          yoffset = UVs[1],
          xoffsetTop = UVs[2],
          yoffsetTop = UVs[3],
          xoffsetBottom = UVs[4],
          yoffsetBottom = UVs[5],
          transparency = UVs[6]; 
    int dir = face / 2;
    float clipX = 0.0f, clipY = 1.0f;
    float clipXX = 0.0f, clipXY = !flat ? (dimsz) : 1.0f,
        clipYX = 0.0f, clipYY = !flat ? (dimsy) : 1.0f;
    int   offsetX = 0, offsetY = 0;
    // Simple tile UV (replace with atlas lookup per block/face but now is length of tile based in greedy meshing)
    //if (blockType == TORCH) { clipX = 0.4375; clipY = 0.5625; }

    vec3 v[4];
    vec3 uv[4];
    vec3 cMask = { tintr, tintg, tintb };

    if (!flat) {
        switch (face) {
        case 0: v[0] = { blockPosx + -0.5f         , blockPosy + -0.5f         , blockPosz + -0.5f };    v[1] = { blockPosx + -0.5f         ,blockPosy + -0.5f + dimsy,blockPosz + -0.5f };  v[2] = { blockPosx + -0.5f         ,blockPosy + -0.5f + dimsy,blockPosz + -0.5f + dimsz };  v[3] = { blockPosx + -0.5f         ,blockPosy + -0.5f         ,blockPosz + -0.5f + dimsz }; break; // -X
        case 1: v[0] = { blockPosx + -0.5f + dimsx, blockPosy + -0.5f         , blockPosz + -0.5f };    v[1] = { blockPosx + -0.5f + dimsx,blockPosy + -0.5f         ,blockPosz + -0.5f + dimsz };  v[2] = { blockPosx + -0.5f + dimsx,blockPosy + -0.5f + dimsy,blockPosz + -0.5f + dimsz };  v[3] = { blockPosx + -0.5f + dimsx,blockPosy + -0.5f + dimsy,blockPosz + -0.5f }; break; // +X
        }
    }
    else {
        switch (face) {
        case 0: v[0] = { blockPosx + -0.5f,blockPosy + -0.5f,blockPosz + -0.5f };   v[1] = { blockPosx + -0.5f,blockPosy + -0.5f + dimsy,blockPosz + -0.5f }; v[2] = { blockPosx + 0.5f,blockPosy + -0.5f + dimsy,blockPosz + 0.5f }; v[3] = { blockPosx + 0.5f,blockPosy + -0.5f,blockPosz + 0.5f };  break; // -X disgonal
        case 1: v[0] = { blockPosx + -0.5f,blockPosy + -0.5f,blockPosz + 0.5f };   v[1] = { blockPosx + -0.5f,blockPosy + -0.5f + dimsy,blockPosz + 0.5f }; v[2] = { blockPosx + 0.5f,blockPosy + -0.5f + dimsy,blockPosz + -0.5f }; v[3] = { blockPosx + 0.5f,blockPosy + -0.5f,blockPosz + -0.5f };  break; // +X diagonal
        }
    }

    uint32_t uintUVs = ((uint8_t)(blockPosy + 100) << 24) | ((uint8_t)(xdimens) << 16) | ((uint8_t)(yoffset + offsetY) << 8) | (uint8_t)(xoffset + offsetX); // Packaging floats into one integer
    float startUvs;
    memcpy(&startUvs, &uintUVs, sizeof(float));
    uint32_t norm_color = ((abyte(normals.x < 0 ? 1 : 0) & 0x1) << 5) | ((abyte(absl(normals.x)) & 0x1) << 4)
         | ((0x0) << 3) | (0x0)
         | ((0x0) << 1) | (0x0)
         | ((abyte(cMask.x * 100) & 0x7F) << 20)
         | ((abyte(cMask.y * 100) & 0x7F) << 13)
         | ((abyte(cMask.z * 100) & 0x7F) << 6);
    float normcolor;
    memcpy(&normcolor, &norm_color, sizeof(float));

    feed(uv, clipXY, clipYY, startUvs);

    float finalVerts[28], finalInds[6];

    for (int i = 0; i < 4; i++) {
        int idx = 7 * i;
        finalVerts[idx + 0] = v[i].x;
        finalVerts[idx + 1] = v[i].y;
        finalVerts[idx + 2] = v[i].z;
        finalVerts[idx + 3] = uv[i].x;
        finalVerts[idx + 4] = uv[i].y;
        finalVerts[idx + 5] = uv[i].z;
        finalVerts[idx + 6] = normcolor;
    }
    finalInds[0] = base + 0; finalInds[1] = base + 1; finalInds[2] = base + 2;
    finalInds[3] = base + 2; finalInds[4] = base + 3; finalInds[5] = base + 0;

    m.vertices.insert(m.vertices.end(), finalVerts, finalVerts + 28);
    m.indices.insert(m.indices.end()  , finalInds , finalInds + 6);
    // Two triangles (0,1,2) (2,3,0)
    base += 4;
}

void greedyMerge(uint8_t* mask, Mesh& m, vec2& xyChunk, int planeDirVal, int W, int H, int faceDir, ivec3 normals, int& base) {
    int dir = faceDir / 2;
    int minX = xyChunk.x * CHUNK_SIZE, minZ = xyChunk.y * CHUNK_SIZE;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W;) {
            int idx = b + l * W;
            uint8_t type = mask[idx];
            if (type == 0) {
                b++; continue;
            }

            int w = 1, h = 1; int stop = 0;
            while (b + w < W && mask[idx + w] == type) { w++; }
            while (l + h < H) {
                int row = (l + h) * W + b;
                for (int d = 0; d < w; d++) {
                    if (mask[row + d] != type) { stop = 1; break; }
                }
				if (stop) break;
                h++;
            }

            ivec3 dims = (dir == 0) ? vec3(1, h, w) : (dir == 1) ? vec3(w, h, 1) : vec3(w, 1, h);
            ivec3 blockPos = (dir == 0) ? ivec3(planeDirVal, l, minZ + b) : (dir == 1) ? ivec3(minX + b, l, planeDirVal) : ivec3(minX + b, planeDirVal, minZ + l);
            emitFace(m, faceDir, type, blockPos, dims, normals, base);
            uint8_t* maskPtr = &mask[b + l * W];
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    *(maskPtr + j + i * W) = 0;
                }
            }
            b += w;
        }
    }
}

void greedyMergeX(uint8_t* mask, Mesh& m, vec2& xyChunk, int planeDirVal, int W, int H, int faceDir, ivec3 normals, int& base) {
    int minZ = xyChunk.y * CHUNK_SIZE;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W;) {
            int idx = b + l * W;
            uint8_t& type = mask[idx];
            if (type == 0) {
                b++; continue;
            }

            int w = 1, h = 1;
            while (b + w < W && mask[idx + w] == type) { w++; }
            while (l + h < H) {
                int row = (l + h) * W + b;
                for (int d = 0; d < w; d++) {
                    if (mask[row + d] != type) goto not_type;
                }
                h++;
            }

        not_type:
            ivec3 dims = vec3(1, h, w);
            ivec3 blockPos = ivec3(planeDirVal, l, minZ + b);
            emitFace(m, faceDir, type, blockPos, dims, normals, base);

            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    mask[b + j + (l + i) * W] = 0;
                }
            }
            b += w;
        }
    }
}

void greedyMergeZ(uint8_t* mask, Mesh& m, vec2& xyChunk, int planeDirVal, int W, int H, int faceDir, ivec3 normals, int& base) {
    int minX = xyChunk.x * CHUNK_SIZE;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W;) {
            int idx = b + l * W;
            uint8_t& type = mask[idx];
            if (type == 0) {
                b++; continue;
            }

            int w = 1, h = 1;
            while (b + w < W && mask[idx + w] == type) { w++; }
            while (l + h < H) {
                int row = (l + h) * W + b;
                for (int d = 0; d < w; d++) {
                    if (mask[row + d] != type) goto not_type;
                }
                h++;
            }

        not_type:
            ivec3 dims = vec3(w, h, 1);
            ivec3 blockPos = ivec3(minX + b, l, planeDirVal);
            emitFace(m, faceDir, type, blockPos, dims, normals, base);

            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    mask[b + j + (l + i) * W] = 0;
                }
            }
            b += w;
        }
    }
}

void greedyMergeY(uint8_t* mask, Mesh& m, vec2& xyChunk, int planeDirVal, int W, int H, int faceDir, ivec3 normals, int& base) {
    int minX = xyChunk.x * CHUNK_SIZE, minZ = xyChunk.y * CHUNK_SIZE;
    for (int l = 0; l < H; l++) {
        for (int b = 0; b < W;) {
            int idx = b + l * W;
            uint8_t& type = mask[idx];
            if (type == 0) {
                b++; continue;
            }

            int w = 1, h = 1;
            while (b + w < W && mask[idx + w] == type) { w++; }
            while (l + h < H) {
                int row = (l + h) * W + b;
                for (int d = 0; d < w; d++) {
                    if (mask[row + d] != type) goto not_type;
                }
                h++;
            }

        not_type:
            ivec3 dims = vec3(w, 1, h);
            ivec3 blockPos = ivec3(minX + b, planeDirVal, minZ + l);
            emitFace(m, faceDir, type, blockPos, dims, normals, base);

            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    mask[b + j + (l + i) * W] = 0;
                }
            }
            b += w;
        }
    }
}

void meshChunk(chNeighPack* chNeigh) {
	vec2 xyChunk = unpack(chNeigh->coords);
    chNeighResult* chNeighRes = new chNeighResult();
    unique_ptr<Mesh>& mesh = chNeighRes->mesh;
    chNeighRes->coords = toCoords(xyChunk);

    Mesh& m = *mesh;
    m.vertices.reserve(5000 * 8.5);
    m.indices.reserve(5000 * 2.5);
    int base = 0;

    uint8_t maskY[CHUNK_SIZE * CHUNK_SIZE];
    uint8_t maskX[CHUNK_SIZE * CHUNK_HEIGHT];
    uint8_t maskZ[CHUNK_SIZE * CHUNK_HEIGHT];

    int minX = xyChunk.x * CHUNK_SIZE, minY = 0, minZ = xyChunk.y * CHUNK_SIZE;
    
    for (int x = minX; x < minX + CHUNK_SIZE; ++x) {
        buildMaskX(chNeigh->block_data.data(), chNeigh->neighbour_data[0].data(), (x + 0) - minX, 0, CHUNK_SIZE, CHUNK_HEIGHT - 1, maskX, -normX0, nextNormX0);
        greedyMerge(maskX, m, xyChunk, x + 0, CHUNK_SIZE, CHUNK_HEIGHT - 1, 0, normX0, base);

        buildMaskX(chNeigh->block_data.data(), chNeigh->neighbour_data[1].data(), (x + 1) - minX, 1, CHUNK_SIZE, CHUNK_HEIGHT - 1, maskX, -normX1, nextNormX1);
        greedyMerge(maskX, m, xyChunk, x + 1, CHUNK_SIZE, CHUNK_HEIGHT - 1, 1, normX1, base);
    }
    for (int z = minZ; z < minZ + CHUNK_SIZE; ++z) {
        buildMaskZ(chNeigh->block_data.data(), chNeigh->neighbour_data[2].data(), (z + 0) - minZ, 2, CHUNK_SIZE, CHUNK_HEIGHT - 1, maskZ, -normZ0, nextNormZ0);
        greedyMerge(maskZ, m, xyChunk, z + 0, CHUNK_SIZE, CHUNK_HEIGHT - 1, 2, normZ0, base);
        
        buildMaskZ(chNeigh->block_data.data(), chNeigh->neighbour_data[3].data(), (z + 1) - minZ, 3, CHUNK_SIZE, CHUNK_HEIGHT - 1, maskZ, -normZ1, nextNormZ1);
        greedyMerge(maskZ, m, xyChunk, z + 1, CHUNK_SIZE, CHUNK_HEIGHT - 1, 3, normZ1, base);
    }
    for (int y = minY; y < minY + CHUNK_HEIGHT - 1; ++y) {
        buildMaskY(chNeigh->block_data.data(), y + 0, 4, CHUNK_SIZE, CHUNK_SIZE, maskY, -normY0);
        greedyMerge(maskY, m, xyChunk, y + 0, CHUNK_SIZE, CHUNK_SIZE, 4, normY0, base);

        buildMaskY(chNeigh->block_data.data(), y + 1, 5, CHUNK_SIZE, CHUNK_SIZE, maskY, -normY1);
        greedyMerge(maskY, m, xyChunk, y + 1, CHUNK_SIZE, CHUNK_SIZE, 5, normY1, base);
    }
 
    std::lock_guard<std::mutex> lock(chunkMeshQueueMutex);
    chunkMeshResult.push(chNeighRes);
}

void breakThread() {
    while (blockBreaking) {
        if (blockBreakingOut) {
            std::lock_guard<std::mutex> lock(breakReqMutex);
            Block block = world.delBlocklook_at();
            //this_thread::sleep_for(chrono::milliseconds(100));
            ivec2 chunkPos = ivec2(floorDiv(block.position.x, CHUNK_SIZE), floorDiv(block.position.z, CHUNK_SIZE));
            //world.updateChunk(chunkPos, vec3(0), vec3(0));
            blockBreakingOut = false;
            breakResQueue.push(block);
        }
    }
}

std::thread blockBreakThread1(breakThread);

std::thread blockPlaceThread([&]() {
    while (blockPlacing) {
        if (blockPlacingOut) {
            std::lock_guard<std::mutex> lock(placeReqMutex);
            Block block = world.addBlocklook_at(inventory.mainInventorySlots[3][slot].item);
            //this_thread::sleep_for(chrono::milliseconds(100));
            blockBreakingOut = false;
            blockPlacingOut = false;
            if (!placeResQueue.empty())
                placeResQueue.pop();
        }
    }
    });


void chunkWorker() {
    while (chunkGenRunning) {
        uint coord;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [] { return !chunkRequestQueue.empty(); });
            coord = chunkRequestQueue.front();
            chunkRequestQueue.pop();
        }

        Chunk* newChunk = new Chunk();
        newChunk->coord = (coord);

        generateBlocks(unpack(coord), newChunk);
        
        {
            lock_guard<mutex> lock(addChunkMutex);
            chunkResultQueue.push({ newChunk, coord });
        }
    }
}////////////

void cloudWorker() {
    while (chunkGenRunning) {
        ivec2 coord;
        {
            std::unique_lock<std::mutex> lock(cloudqMutex);
            cloudqCV.wait(lock, [] { return !cloudRequestQueue.empty(); });
            coord = cloudRequestQueue.front();
            cloudRequestQueue.pop();
        }

        {
            std::lock_guard<std::mutex> lock(cloudrMutex);
            unique_ptr<CloudMesh> chm = make_unique<CloudMesh>();
            float cloudDensity = cloudNoise.GetNoise(float(coord.x), float(coord.y));
            if (cloudDensity > 0.2f) {
                //generateClouds(*chm);
                //meshClouds(*chm, coord);
                //cloudResultQueue.push({ move(chm), coord });
                //sky.addCloud(chm, coord);
            }
        }
    }
}

void updateChunkJob() {
    while (!stopChunkUpdaters) {
		chNeighPack* chNeigh;
        {
            unique_lock<mutex> lock(chunkUpdateRequestMutex);
            chunkUpdateCV.wait(lock, [] { return !chunkCleanupQueue.empty() || stopChunkUpdaters; });
            chNeigh = chunkCleanupQueue.front();
            chunkCleanupQueue.pop();
        }

        updateChunk(chNeigh, vec3(0), vec3(0));
        delete chNeigh;
    }
}/////////////