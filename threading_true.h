//#pragma once
//#include "Chunk.h"
//#include "normals.h"
//
////vector<vec2> chunkCoords;
//std::unordered_set<uint64_t> chunkCoords;
//
//uint64_t to(ivec2 vec) {
//    uint64_t x = (uint32_t)vec.x, y = (uint32_t)vec.y;
//    return x << 32 | y;
//}
//
//std::queue<vec2> chunkRequestQueue;
//std::mutex queueMutex;
//std::condition_variable queueCV;
//
//std::queue<unique_ptr<Chunk>> chunkResultQueue;
//std::mutex resultMutex;
//
////std::queue<vec2> chunkRequestQueue;
//std::mutex chunkRequestMutex;
//
////std::queue<Chunk> chunkResultQueue;
//std::mutex chunkResultMutex;
//
//std::queue<vec2> chunkRequestQueue2;
//std::mutex chunkRequestMutex2;
//
//std::queue<Chunk> chunkResultQueue2;
//std::mutex chunkResultMutex2;
//
//std::queue<Chunk> chunkUpdateRequestQueue;
//std::mutex chunkUpdateRequestMutex;
//
//std::queue<bool> chunkUpdateResultQueue;
//std::mutex chunkUpdateResultMutex;
//
//std::queue<Chunk> breakResQueue;
//std::mutex breakResMutex;
//
//std::queue<vec3> breakReqQueue;
//std::mutex breakReqMutex;
//
//std::queue<vec3> placeReqQueue;
//std::mutex placeReqMutex;
//
//std::queue<vec3> placeResQueue;
//std::mutex placeResMutex;
//
//std::atomic<bool> chunkGenRunning = true;
//std::atomic<bool> chunkGenRunning2 = true;
//std::atomic<bool> chunkUpdateGenRunning = true;
//std::atomic<bool> blockBreaking = true;
//std::atomic<bool> blockPlacing = true;
//bool blockBreakingOut = false;
//bool blockPlacingOut = false;
//
//// Base hills: smooth FBm Perlin
//FastNoiseLite baseNoise;
//// Ridged mountains: sharp features
//FastNoiseLite ridgedNoise;
//// Mask: controls where mountains vs plains appear
//FastNoiseLite maskNoise;
//FastNoiseLite caveNoise;
//FastNoiseLite cloudNoise;
//
////cloudNoise.SetFrequency(0.045);  
//void initChunksNoise() {
//    baseNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//    baseNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
//    baseNoise.SetFrequency(0.005f);      // low frequency = broad features
//    baseNoise.SetFractalOctaves(3);
//    baseNoise.SetFractalLacunarity(2.0f);
//    baseNoise.SetFractalGain(0.5f);
//
//    ridgedNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
//    ridgedNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
//    ridgedNoise.SetFrequency(0.005f);     // higher frequency = more detail
//    ridgedNoise.SetFractalOctaves(4);
//    ridgedNoise.SetFractalLacunarity(2.0f);
//    ridgedNoise.SetFractalGain(0.5f);
//
//    //maskNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//    maskNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
//    maskNoise.SetFrequency(0.02f);      // very low frequency = large biome regions
//    maskNoise.SetFractalOctaves(2);
//    maskNoise.SetFractalGain(0.5f);
//
//    //FastNoiseLite noise;   
//    //noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//    //noise.SetFractalType(FastNoiseLite::FractalType_FBm);
//    //noise.SetFractalOctaves(6);
//    //noise.SetFractalLacunarity(2.0f);
//    //noise.SetFractalGain(0.5f);
//
//    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
//
//    caveNoise.SetFrequency(0.05f);
//    cloudNoise.SetFrequency(0.045);
//
//}
//
//bool isAir(Item item);
//
//void generateBlocks(vec2 xyChunk, Chunk* repChunk) {
//    //auto start = std::chrono::high_resolution_clock::now();
//    Item blockType = AIR;
//    std::random_device rd;
//    std::mt19937 gen(rd()); // Mersenne Twister engine
//
//    int blockTypes1[] = {
//        STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, IRON_ORE.id
//    };
//    int blockTypes2[] = {
//        1, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, STONE_BLOCK.id, IRON_ORE.id, IRON_ORE.id
//    };
//    int blockTypes3[] = {
//        DIRT_BLOCK.id, STONE_BLOCK.id
//    };
//    std::uniform_int_distribution<> dist(0, sizeof(blockTypes1) / sizeof(int) - 1);
//    std::uniform_int_distribution<> dist2(0, sizeof(blockTypes2) / sizeof(int) - 1);
//    std::uniform_int_distribution<> dist3(0, sizeof(blockTypes3) / sizeof(int) - 1);
//
//    float cloudDensity = 0;
//    float density = 0;
//
//    for (int x = (xyChunk.x) * CHUNK_SIZE - 1; x < (xyChunk.x + 1) * CHUNK_SIZE + 1; x++) {
//        for (int z = (xyChunk.y) * CHUNK_SIZE - 1; z < (xyChunk.y + 1) * CHUNK_SIZE + 1; z++) {
//            if (x != (xyChunk.x) * CHUNK_SIZE - 1 && x != (xyChunk.x + 1) * CHUNK_SIZE + 1
//                && z != (xyChunk.y) * CHUNK_SIZE - 1 && z != (xyChunk.y + 1) * CHUNK_SIZE + 1)
//                repChunk->block_data[repChunk->at(ivec3(x, 0, z))] = BlockData(ivec3(x, 0, z), BEDROCK);
//            auto norm = [](float n) { return n * 0.5f + 0.5f; }; //defining a function inline
//            float base = norm(baseNoise.GetNoise((float)x, (float)z)) * 10.0f;
//            float ridged = norm(ridgedNoise.GetNoise((float)x, (float)z)) * 10.0f;
//            float mask = norm(maskNoise.GetNoise((float)x, (float)z));
//
//            // Blend between base and ridged using the mask
//            float blend = smoothstep(0.0f, 1.0f, mask);
//            float height = mix(base, ridged, blend);
//
//            //float scaledHeight = height;
//            float scaledHeight = height * 4 + CHUNK_SIZE * 5;
//            //float scaledHeight = ((height + 1.0f) * (CHUNK_SIZE * CHUNK_SIZE));
//            cloudDensity = cloudNoise.GetNoise((float)x, (float)z);
//            if (cloudDensity < -0.25f) {
//                repChunk->block_data[repChunk->at(ivec3(x, CHUNK_SIZE * CHUNK_SIZE + 2 * CHUNK_SIZE - 1, z))] = BlockData(ivec3(x, CHUNK_SIZE * CHUNK_SIZE + 2 * CHUNK_SIZE - 1, z), CLOUD);
//            }
//            float treeHeight = randomFloat(3.0, 5.0) + 2, treeDistrib = rand();
//            float randomNumberForDeco = rand();
//            for (int y = 1; y < scaledHeight; y++) {
//                if (y > scaledHeight - 2) {
//                    blockType = GRASS_BLOCK;
//                }
//                else if (y > scaledHeight - 4 && y <= scaledHeight - 2) {
//                    blockType = DIRT_BLOCK;
//                }
//                else if (y <= scaledHeight - 4 && y > scaledHeight - 7) {
//                    blockType = items[blockTypes3[dist3(gen)]];
//                }
//                else if (y <= scaledHeight - 7 && y > scaledHeight - 20) {
//                    blockType = items[blockTypes1[dist(gen)]];
//                }
//                else if (y <= scaledHeight - 20) {
//                    blockType = items[blockTypes2[dist2(gen)]];
//                }
//                else continue;
//                density = caveNoise.GetNoise((float)x, (float)y, (float)z);
//
//                if (density < -0.4f) {
//                    continue;
//                }
//                repChunk->block_data[repChunk->at(ivec3(x, y, z))] = BlockData(ivec3(x, y, z), blockType);
//
//                int radius = 3;  //adjust for size
//
//                //Making the trees and leaves
//                if (x >= (xyChunk.x + 1) * CHUNK_SIZE - radius || x <= (xyChunk.x) * CHUNK_SIZE + radius ||
//                    z >= (xyChunk.y + 1) * CHUNK_SIZE - radius || z <= (xyChunk.y) * CHUNK_SIZE + radius) {
//                    goto notree;
//                }
//                if (blockType == GRASS_BLOCK) {
//                    if (treeDistrib > 0.0 && treeDistrib <= 75) {
//
//                        for (int i = y + 1; i < y + treeHeight; i++) {
//                            repChunk->block_data[repChunk->at(ivec3(x, i, z))] = BlockData(ivec3(x, i, z), OAK_WOOD);
//                        }
//                        glm::ivec3 center(x, y + treeHeight + 1, z);
//
//                        for (int dx = -radius; dx <= radius; ++dx) {
//                            for (int dy = -radius; dy <= radius; ++dy) {
//                                for (int dz = -radius; dz <= radius; ++dz) {
//                                    glm::ivec3 pos = center + glm::ivec3(dx, dy, dz);
//                                    float dist = glm::length(glm::vec3(dx, dy, dz));
//
//                                    if (dist <= radius + randomFloat(-0.3f, 0.3f)) {
//                                        if (isAir(repChunk->block_data[repChunk->at(pos)].blockType)) {
//                                            repChunk->block_data[repChunk->at(ivec3(pos.x, pos.y, pos.z))] = BlockData(ivec3(pos.x, pos.y, pos.z), OAK_LEAVES);
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    //}
//                notree:  //No trees should be created if they are near the chunks border
//
//                    if (blockType == GRASS_BLOCK && blockType != OAK_WOOD) {
//                        ivec3 pos(x, y + 1, z);
//                        if (isAir(repChunk->block_data[repChunk->at(pos)].blockType)) {
//                            if (randomNumberForDeco < 2000) {
//                                repChunk->block_data[repChunk->at(pos)] = BlockData(pos, GRASS);
//                            }
//                            else if (randomNumberForDeco > 2000 && randomNumberForDeco < 2500) {
//                                repChunk->block_data[repChunk->at(pos)] = BlockData(pos, POPPY);
//                            }
//                            else if (randomNumberForDeco > 2500 && randomNumberForDeco < 2800) {
//                                repChunk->block_data[repChunk->at(pos)] = BlockData(pos, BLUE_ORCHID);
//                            }
//                        }
//
//                    }
//                }
//            }
//        }
//    }
//}
//
//bool isAir(Item item) { return item == AIR; }
//
//bool shouldEmitFace(vec2 xyChunk, Chunk* cd, int x, int y, int z, int dx, int dy, int dz) {
//    //return 0;
//    int nx = x + dx, ny = y + dy, nz = z + dz;
//    ivec3 checkPos = ivec3(nx, ny, nz), blockPos = ivec3(x, y, z);
//    Item& blockType = cd->block_data[cd->at(checkPos)].blockType;
//    if (((nx <= xyChunk.x * CHUNK_SIZE - 1 || nz <= xyChunk.y * CHUNK_SIZE - 1) ||
//        nx >= (xyChunk.x + 1) * CHUNK_SIZE + 1 || nz >= (xyChunk.y + 1) * CHUNK_SIZE + 1) && blockType != OAK_LEAVES) return false;
//    return       isAir(blockType) ||
//        blockType.isFlat ||
//        blockType == OAK_LEAVES;
//    //||       cd->block_data[cd->at(blockPos)].blockType == TORCH;
//}
//
//void emitFace(Mesh& m, int face, Item blockType, float x, float y, float z, vec3 direction, vec3 position) {
//
//    //if (isAir(blockType)) return;
//    float tintr = 1.0f, tintg = 1.0f, tintb = 1.0f;
//    if (blockType == GRASS_BLOCK && face != 4) {
//        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
//    }
//    else if (blockType == GRASS) {
//        tintr = 0.2f, tintg = 1.45f, tintb = 0.15f;
//    }
//    else if (blockType == OAK_LEAVES) {
//        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
//    }
//    //vec3 rgb = { tintr, tintg, tintb };
//    vec3 cMask[4] = { { tintr, tintg, tintb }, { tintr, tintg, tintb }, { tintr, tintg, tintb }, { tintr, tintg, tintb } };
//
//
//    // face: 0=-X,1=+X,2=-Z,3=+Z,4=+Z,5=-Z
//    // Define 4 positions and normal per face
//    static const glm::vec3 normals[6] = {
//        {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}, {0, 1, 0}, { 0, -1, 0 }
//    };
//    glm::vec3 n = normals[face];
//
//    const int bottomDir = 4 - 4 * (-abs(direction.y) + 1) + (abs(direction.y)) * (0.5 * (-direction.y + 1)) + (-abs(direction.y) + 1) * (0.5 * (-direction.x + 1)) * (-direction.z * direction.z + 1) + (-abs(direction.y) + 1) * (-direction.x * direction.x + 1) * (2 + 0.5 * (-direction.z + 1));
//    const int topDir = 5 - 4 * (-abs(direction.y) + 1) + (abs(direction.y)) * (0.5 * (direction.y + 1) - 1) + (-abs(direction.y) + 1) * (0.5 * (direction.x + 1) - 1) * (-direction.z * direction.z + 1) + (-abs(direction.y) + 1) * (-direction.x * direction.x + 1) * (1 + 0.5 * (direction.z + 1));
//
//    glm::vec3 v[4];
//    if (blockType == TORCH) {
//        switch (face) {
//        case 0: /* left face */   v[0] = { x + 0.4375, y + 0.0, z + 0.4375 };      v[1] = { x + 0.4375, y + 0.5625, z + 0.4375 };   v[2] = { x + 0.4375, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.4375, y + 0.0, z + 0.5625 };    break;
//        case 1: /* right face */  v[0] = { x + 0.5625, y + 0.0, z + 0.4375 };      v[1] = { x + 0.5625, y + 0.0, z + 0.5625 };      v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.5625, y + 0.5625, z + 0.4375 }; break;
//        case 2: /* front face */  v[0] = { x + 0.4375, y + 0.0, z + 0.4375 };      v[1] = { x + 0.5625, y + 0.0, z + 0.4375 };      v[2] = { x + 0.5625, y + 0.5625, z + 0.4375 };   v[3] = { x + 0.4375, y + 0.5625, z + 0.4375 }; break;
//        case 3: /* back face */   v[0] = { x + 0.4375, y + 0.0, z + 0.5625 };      v[1] = { x + 0.4375, y + 0.5625, z + 0.5625 };   v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.5625, y + 0.0, z + 0.5625 };    break;
//        case 4: /* bottom face */ v[0] = { x + 0.0   , y + 0.0, z + 0.0 };         v[1] = { x + 0.0, y + 0.0, z + 0.0 };            v[2] = { x + 0.0, y + 0.0, z + 0.0 };            v[3] = { x + 0.0, y + 0.0, z + 0.0 };          break;
//        case 5: /* top face */    v[0] = { x + 0.4375, y + 0.5625, z + 0.4375 };   v[1] = { x + 0.5625, y + 0.5625, z + 0.4375 };   v[2] = { x + 0.5625, y + 0.5625, z + 0.5625 };   v[3] = { x + 0.4375, y + 0.5625, z + 0.5625 }; break;
//        }
//    }
//    else if (!blockType.isFlat) {
//        switch (face) {
//        case 0: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + -0.5f,y + 0.5f,z + 0.5f }; v[3] = { x + -0.5f,y + -0.5f,z + 0.5f }; break; // -X
//        case 1: v[0] = { x + 0.5f,y + -0.5f,z + -0.5f };    v[1] = { x + 0.5f,y + -0.5f,z + 0.5f };  v[2] = { x + 0.5f,y + 0.5f,z + 0.5f };  v[3] = { x + 0.5f,y + 0.5f,z + -0.5f }; break; // +X
//        case 2: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + 0.5f,y + -0.5f,z + -0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + -0.5f }; v[3] = { x + -0.5f,y + 0.5f,z + -0.5f }; break; // -Z
//        case 3: v[0] = { x + -0.5f,y + -0.5f,z + 0.5f };    v[1] = { x + -0.5f,y + 0.5f,z + 0.5f };  v[2] = { x + 0.5f,y + 0.5f,z + 0.5f };  v[3] = { x + 0.5f,y + -0.5f,z + 0.5f }; break; // +Z
//        case 4: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + -0.5f,z + 0.5f }; v[2] = { x + 0.5f,y + -0.5f,z + 0.5f }; v[3] = { x + 0.5f,y + -0.5f,z + -0.5f }; break; // -Y
//        case 5: v[0] = { x + -0.5f,y + 0.5f,z + -0.5f };    v[1] = { x + 0.5f,y + 0.5f,z + -0.5f };  v[2] = { x + 0.5f,y + 0.5f,z + 0.5f };  v[3] = { x + -0.5f,y + 0.5f,z + 0.5f }; break; // +Y
//        }
//    }
//    else {
//        switch (face) {
//        case 0: v[0] = { x + -0.5f,y + -0.5f,z + -0.5f };   v[1] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[2] = { x + 0.5f,y + 0.5f,z + 0.5f };   v[3] = { x + 0.5f,y + -0.5f,z + 0.5f };  break; // -X disgonal
//        case 1: v[0] = { x + -0.5f,y + -0.5f,z + 0.5f };    v[1] = { x + -0.5f,y + 0.5f,z + 0.5f };  v[2] = { x + 0.5f,y + 0.5f,z + -0.5f };  v[3] = { x + 0.5f,y + -0.5f,z + -0.5f };  break; // +X diagonal
//        case 2: v[0] = { x + 0.5f,y + -0.5f,z + 0.5f };     v[1] = { x + 0.5f,y + 0.5f,z + 0.5f };   v[2] = { x + -0.5f,y + 0.5f,z + -0.5f }; v[3] = { x + -0.5f,y + -0.5f,z + -0.5f };  break; // -X disgonal
//        case 3: v[0] = { x + 0.5f,y + -0.5f,z + -0.5f };    v[1] = { x + 0.5f,y + 0.5f,z + -0.5f };  v[2] = { x + -0.5f,y + 0.5f,z + 0.5f };  v[3] = { x + -0.5f,y + -0.5f,z + 0.5f };  break; // +X diagonal
//        }
//    }
//
//    // Simple tile UV (replace with atlas lookup per block/face)
//    vector<glm::vec3> uv;
//
//    float UVs[7];
//    getUVs(blockType, UVs);
//    float xoffset = UVs[0],
//        yoffset = UVs[1],
//        xoffsetTop = UVs[2],
//        yoffsetTop = UVs[3],
//        xoffsetBottom = UVs[4],
//        yoffsetBottom = UVs[5],
//        transparency = UVs[6];
//
//    float clipX = 0.0f, clipY = 1.0f;
//    int offsetX = 0, offsetY = 0;
//
//    if (blockType == TORCH) { clipX = 0.4375; clipY = 0.5625; }
//
//    if (face == bottomDir) { offsetX = xoffsetBottom; offsetY = yoffsetBottom; }
//    else if (face == topDir) { offsetX = xoffsetTop;  offsetY = yoffsetTop; }
//
//    switch (face) {
//    case 1:
//    case 2: if (!blockType.isFlat || blockType == TORCH) {
//        if (abs(direction.x) && !blockType.isFlat) {
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//        }
//        else if (abs(direction.z) && !blockType.isFlat) {
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//        }
//        else if (blockType == TORCH) {
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (0.0f + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (0.0f + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (0.625 + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (0.625 + yoffset + offsetY) / ydimens, transparency));
//        }
//        else {
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//        }
//    }
//          else { // for grass and other flats
//        uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//        uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//        uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//        uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//    }
//          break;
//    default:
//        if (blockType == TORCH && face != 5 && blockType != GRASS) {
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (0.0f + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (0.625 + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (0.625 + yoffset + offsetY) / ydimens, transparency));
//            uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (0.0f + yoffset + offsetY) / ydimens, transparency));
//        }
//        else {
//            if (abs(direction.x) && !blockType.isFlat) {
//                if (face == 5) {
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                }
//                else {
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                }
//            }
//            else if (abs(direction.z) && !blockType.isFlat) {
//                if (face == 5) {
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                }
//                if (face == 4) {
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                }
//                else {
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                    uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                }
//            }
//            else {
//                uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//                uv.push_back(vec3((clipX + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipY + yoffset + offsetY) / ydimens, transparency));
//                uv.push_back(vec3((clipY + xoffset + offsetX) / xdimens, (clipX + yoffset + offsetY) / ydimens, transparency));
//            }
//        }
//        break;
//    }
//
//    uint32_t base = (uint32_t)(m.vertices.size() / 12);
//    for (int i = 0; i < 4; i++) {
//        m.vertices.push_back(v[i].x);
//        m.vertices.push_back(v[i].y);
//        m.vertices.push_back(v[i].z);
//        m.vertices.push_back(uv[i].x);
//        m.vertices.push_back(uv[i].y);
//        m.vertices.push_back(uv[i].z);
//        m.vertices.push_back(n.x);
//        m.vertices.push_back(n.y);
//        m.vertices.push_back(n.z);
//        m.vertices.push_back(cMask[i].x);
//        m.vertices.push_back(cMask[i].y);
//        m.vertices.push_back(cMask[i].z);
//    }
//    // Two triangles (0,1,2) (2,3,0)
//    m.indices.push_back(base + 1); m.indices.push_back(base + 2); m.indices.push_back(base + 3);
//    m.indices.push_back(base + 3); m.indices.push_back(base + 0); m.indices.push_back(base + 1);
//}
//
//void meshChunk(vec2 xyChunk, Chunk* cd, Mesh& out, vec3 direction, vec3 blockPosition) {
//    //out.vertices.clear(); out.indices.clear();
//    //out.vertices.reserve(CHUNK_SIZE * (CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE); // heuristic
//    //out.indices.reserve(CHUNK_SIZE * (CHUNK_SIZE * CHUNK_SIZE) * CHUNK_SIZE);
//
//    for (int x = (xyChunk.x) * CHUNK_SIZE - 1; x < (xyChunk.x + 1) * CHUNK_SIZE + 1; ++x)
//        for (int y = 0; y <= (CHUNK_SIZE * CHUNK_SIZE) + 2 * CHUNK_SIZE + 5; ++y) // + 2 * CHUNK_SIZE
//            for (int z = (xyChunk.y) * CHUNK_SIZE - 1; z < (xyChunk.y + 1) * CHUNK_SIZE + 1; ++z) {
//                BlockData& block_data = cd->block_data[cd->at(ivec3(x, y, z))];
//                Item& blockType = block_data.blockType;
//                if (isAir(block_data.blockType)) continue;
//                vec3 blockOrientation = block_data.orientation;
//                if (direction != vec3(0, 1, 0) && blockPosition == vec3(x, y, z)) {
//                    block_data.orientation = direction;
//                    blockOrientation = block_data.orientation;
//                }
//
//                if (!blockType.isFlat || blockType == TORCH)
//                {
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, -1, 0, 0)) emitFace(out, 0, blockType, x, y, z, blockOrientation, blockPosition);
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, +1, 0, 0)) emitFace(out, 1, blockType, x, y, z, blockOrientation, blockPosition);
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, 0, 0, -1)) emitFace(out, 2, blockType, x, y, z, blockOrientation, blockPosition);
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, 0, 0, +1)) emitFace(out, 3, blockType, x, y, z, blockOrientation, blockPosition);
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, 0, -1, 0)) emitFace(out, 4, blockType, x, y, z, blockOrientation, blockPosition);
//                    if (shouldEmitFace(xyChunk, cd, x, y, z, 0, +1, 0)) emitFace(out, 5, blockType, x, y, z, blockOrientation, blockPosition);
//                }
//                else {
//                    emitFace(out, 0, blockType, x, y, z, direction, blockPosition);
//                    emitFace(out, 1, blockType, x, y, z, direction, blockPosition);
//                    emitFace(out, 2, blockType, x, y, z, direction, blockPosition);
//                    emitFace(out, 3, blockType, x, y, z, direction, blockPosition);
//                }
//            }
//}
//
//void generateChunkAt(vec2 xyChunk, Chunk* repChunk) {
//    auto start = std::chrono::high_resolution_clock::now();
//    generateBlocks(xyChunk, repChunk);
//
//    Mesh& m = repChunk->mesh;
//    meshChunk(xyChunk, repChunk, m, vec3(0, 1, 0), vec3(-404, -404, -404));
//
//    auto end = std::chrono::high_resolution_clock::now();
//    std::cout << repChunk->mesh.vertices.size() << " : Elapsed: " << std::chrono::duration<double>(end - start).count() << " s\n";
//}
//
////std::thread chunkGenThread([&]() {
////    while (chunkGenRunning) {
////        vec2 coord;
////        {
////            std::lock_guard<std::mutex> lock(chunkRequestMutex);
////            if (!chunkRequestQueue.empty()) {
////                coord = chunkRequestQueue.front();
////                chunkRequestQueue.pop();
////            }
////            else {
////                continue;
////            }
////        }
////
////        Chunk newChunk;
////        {
////            //std::lock_guard<std::mutex> lock(chunkRequestMutex);
////            generateChunkAt(coord, newChunk);
////            newChunk.coords = coord;
////            newChunk.needUpdate = true;
////        }
////
////        //cout << newChunk.blockNum << endl;
////        {
////            std::lock_guard<std::mutex> lock(chunkResultMutex);
////            chunkResultQueue.push(std::move(newChunk));
////        }
////    }
////    });
//
////std::thread chunkGenThread2([&]() {
////    while (chunkGenRunning2) {
////        vec2 coord;
////        {
////            std::lock_guard<std::mutex> lock(chunkRequestMutex2);
////            if (!chunkRequestQueue2.empty()) {
////                coord = chunkRequestQueue2.front();
////                chunkRequestQueue2.pop();
////            }
////            else {
////                continue;
////            }
////        }
////
////        Chunk newChunk;
////        {
////            std::lock_guard<std::mutex> lock(chunkRequestMutex2);
////            generateChunkAt(coord, newChunk);
////        }
////
////        newChunk.coords = coord;
////        newChunk.needUpdate = true;
////        //cout << newChunk.blockNum << endl;
////        {
////            std::lock_guard<std::mutex> lock(chunkResultMutex2);
////            chunkResultQueue2.push(std::move(newChunk));
////        }
////    }
////    });
////
////std::thread chunkGenThread3([&]() {
////    while (chunkGenRunning3) {
////        vec2 coord;
////        {
////            std::lock_guard<std::mutex> lock(chunkRequestMutex3);
////            if (!chunkRequestQueue3.empty()) {
////                coord = chunkRequestQueue3.front();
////                chunkRequestQueue3.pop();
////            }
////            else {
////                continue;
////            }
////        }
////
////        Chunk newChunk;
////        generateChunkAt(coord, newChunk);
////
////        newChunk.coords = chunkCoords.back();
////        newChunk.needUpdate = true;
////        {
////            std::lock_guard<std::mutex> lock(chunkResultMutex3);
////            chunkResultQueue3.push(std::move(newChunk));
////        }
////    }
////    });
//
//std::thread blockBreakThread([&]() {
//    while (blockBreaking) {
//        {
//            std::lock_guard<std::mutex> lock(breakReqMutex);
//            if (!breakReqQueue.empty()) {
//                breakReqQueue.pop();
//            }
//            else {
//                continue;
//            }
//        }
//        if (blockBreakingOut) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(200));
//            std::lock_guard<std::mutex> lock(breakReqMutex);
//            world.delBlocklook_at();
//            blockBreakingOut = false;
//            if (!breakResQueue.empty())
//                breakResQueue.pop();
//        }
//    }
//    });
//
//std::thread blockPlaceThread([&]() {
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    while (blockPlacing) {
//        {
//            std::lock_guard<std::mutex> lock(placeReqMutex);
//            if (!placeReqQueue.empty()) {
//                placeReqQueue.pop();
//            }
//            else {
//                continue;
//            }
//        }
//        if (blockPlacingOut) {
//            std::lock_guard<std::mutex> lock(placeReqMutex);
//            world.addBlocklook_at(inventory.mainInventorySlots[3][slot].item);
//            blockPlacingOut = false;
//            if (!placeResQueue.empty())
//                placeResQueue.pop();
//        }
//    }
//    });
//
//
//void chunkWorker() {
//    while (chunkGenRunning) {
//        vec2 coord;
//        {
//            std::unique_lock<std::mutex> lock(queueMutex);
//            queueCV.wait(lock, [] { return !chunkRequestQueue.empty(); });
//            coord = chunkRequestQueue.front();
//            chunkRequestQueue.pop();
//        }
//
//        unique_ptr<Chunk> newChunk = make_unique<Chunk>();
//
//        //Chunk* newChunk = new Chunk();
//        {
//            std::lock_guard<std::mutex> lock(queueMutex);
//            newChunk->coords = coord;
//            generateChunkAt(coord, newChunk.get());
//            //cout << (newChunk.get())->mesh.vertices.size() << endl;
//            newChunk->needUpdate = true;
//        }
//        {
//            std::lock_guard<std::mutex> lock(queueMutex);
//            chunkResultQueue.push(move(newChunk));
//        }
//
//        //delete newChunk;
//    }
//}////////////
//
//std::thread chunkUpdateThread([&]() {
//    while (chunkUpdateGenRunning) {
//        Chunk* chunkToUpdate;
//        {
//            std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
//            if (!chunkUpdateRequestQueue.empty()) {
//                chunkToUpdate = &(chunkUpdateRequestQueue.front());
//                world.updateChunk(chunkToUpdate->coords, vec3(0, 1, 0), vec3(0, 0, 0));
//                chunkUpdateRequestQueue.pop();
//            }
//            else {
//                continue;
//            }
//        }
//
//        //world.updateChunk(chunkToUpdate.coords, vec3(0, 1, 0), vec3(0, 0, 0));
//        //chunkToUpdate.needUpdate = true;
//        //chunkToUpdate.unloaded = false;
//    }
//    });