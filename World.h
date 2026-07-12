#pragma once

#include "Chunk.h"
#include "normals.h"

std::unordered_set<uint32_t> chunkCoords;
std::queue<chPack> chunkMeshQueue;//pair<unique_ptr<Mesh>, Chunk*>

std::mutex chunkMeshQueueMutex;
std::mutex worldChMutex;

struct Projectile {
    vec3 position;
    vec3 velocity, initial_velocity = vec3(0);
    LightMesh mesh;
    Item item = OAK_PLANK;
    mat4 model;
    float angle = 0.0f;
    bool shot = 0, done = 0;
    void shoot(vec3 pos, vec3 direction) {
        shot = 1;
        initial_velocity = normalize(direction)*vec3(5, 20, 5);
        velocity = vec3(0);
        position = pos + normalize(direction);
    }
    void shoot(vec3 pos, vec3 direction, Item type) {
        if (type != AIR) {
            item = type;
            shot = 1;
            vec3 front = normalize(direction);
            position = pos + front;
            initial_velocity = front * vec3(5, 5, 5);
            velocity = vec3(0);
        }
    }
    void shoot(vec3 pos, vec3 direction, Item type, vec3 vel) {
        if (type != AIR) {
            item = type;
            shot = 1;
            vec3 front = normalize(direction);
            position = pos + front;
            initial_velocity = front * vec3(20, 20, 20) * vel;
            velocity = vec3(0);
        }
    }
    void update() {
        velocity += vec3(0, -10, 0) * vec3(0.01);
        position += (initial_velocity + velocity) * vec3(0.01);
    }
    void draw() {
        mesh.renderMesh();
    }
};

vector<Projectile> dropped;

float randomFloat(float min, float max) {
    static std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

bool contains(vector<vec2> vec, vec2 value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}
float movementSpeed = 0.05f;
Camera activeCamera = Camera(vec3(0.0f, CHUNK_SIZE / 2, 0.0f), vec3(0.0f,  0.5f, 0.0f), 0.0f, 0.0f, movementSpeed,  1.0f);
Camera firstCamera = Camera(vec3(0.0f, CHUNK_SIZE / 2, 0.0f), vec3(0.0f,  0.5f, 0.0f), 0.0f, 0.0f, movementSpeed, 1.0f);
Camera thirdCamera_back = Camera(vec3(0.0f, CHUNK_SIZE / 2, 0.0f), vec3(0.0f,  0.5f, 0.0f), 0.0f, 0.0f, movementSpeed,  1.0f);

const int MAX_CHUNK_NUM = 30;

//struct ChunkData {
//    vector<Chunk> chunks;
//    int chunkNum = 0;
//    ChunkData() {
//        chunks.resize(MAX_CHUNK_NUM * MAX_CHUNK_NUM);
//    }
//    Chunk& operator[](ivec2 chunkPos){
//        return chunks[((chunkPos.x + MAX_CHUNK_NUM) * MAX_CHUNK_NUM/2) + (chunkPos.y + MAX_CHUNK_NUM/2)];
//    }
//    Chunk& operator[](uint64_t chunkPos) {
//        return chunks[uint32_t(((chunkPos >> 32) + MAX_CHUNK_NUM/2) * MAX_CHUNK_NUM) + uint32_t((chunkPos & 0xffffffff) + MAX_CHUNK_NUM/2)];
//    }
//    bool empty() {
//        return chunkNum == 0;
//    }
//    void insert(ivec2 pos, Chunk* ch) {
//        Chunk& chunk = (*this)[pos];
//        chunk = *ch;
//        chunkNum++;
//    }
//};

class World {
    public:
    //vector<Chunk> chunks;
    //unordered_map<glm::ivec2, Chunk, ivec2_hash, ivec2_eq> chunkData;
    unordered_map<uint32_t, unique_ptr<Chunk>> chunkData;
    //vector<unique_ptr<Chunk>> chunkData;
    //ChunkData chunkData;
    World() {
        chunkData.reserve(4096);
    }

    BlockData getBlockAt(ivec3 blockPos);

    void addChunk(unique_ptr<Chunk>& newChunk, uint32_t xyChunk);

    void updateChunk(const ivec2& chunkCoord, vec3 direction, ivec3 position);

    void updateSubChunk(const ivec2& chunkCoord, int subCh, vec3 direction, ivec3 position);


    void updateWholeChunk(const ivec2& chunkCoord, vec3 direction, ivec3 position);


    void updateChunk(Chunk& chunk);

    LightMesh createVertsOnlyMesh(ivec3 xyz, float scale, Item blockType);

    LightMesh createMeshCube(vec3 blockPos, float scale, Item blockType);

    LightMesh createProjectileMesh(vec3 blockPos, float scale, Item blockType);

    void createItem(vec3 blockPos, Item blockType, vec3 direction);

    Block deleteBlockFromWorld(vec3 blockPos);

    Block delBlocklook_at();

    Block addBlocklook_at(Item blockType);

    //Chunk& operator[](ivec2& chunkPos) { return chunkData.at(pack(chunkPos)); }
};

World world;


//.cpp part----------------------------------------------------------------------------------------------------------------------

int floorDiv(float a, float b) {
    return (a >= 0) ? int(a / b) : int((a - b + 1) / b); // the + 1 is for negative multiples : -10/10 = -1, but (-10-10)/10 = -2 x wrong, (-10-10 + 1)/10 = -1.9 = -1 J correct
}

BlockData World::getBlockAt(ivec3 blockPos) {
    if (blockPos.y == -404) return BlockData(AIR);
    ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    auto& chunk = world.chunkData.at(pack(chunkPos));
    return BlockData(chunk->block_data[chunk->at(blockPos)].blockType);
}

Block getBlockAt(ivec3 blockPos) {
    if (blockPos.y == -404) return Block(blockPos, AIR);
    ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    auto& chunk = world.chunkData.at(pack(chunkPos));
    return Block(blockPos, chunk->block_data[chunk->at(blockPos)].blockType);
}

bool blockExistsAt(ivec3 blockPos) {
    if (world.chunkData.empty()) return false;
    ivec2 chunkCoord = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    uint64_t chunkIndex = pack(chunkCoord);
    auto chunkIt = world.chunkData.find(chunkIndex);
    if (chunkIt == world.chunkData.end()) return false;
    auto& chunk = chunkIt->second;
    //Chunk& chunk = world.chunkData[chunkCoord];
    //if (!chunk.exists) return false;
    int vec_blockPos = chunk->at(blockPos);
    if (vec_blockPos >= CHUNK_VOLUME || vec_blockPos < 0) {
        return false;
    }
    return (chunk->block_data[vec_blockPos].blockType != AIR);
}

bool blockExistsAt(ivec3 blockPos, bool block_excl_natural) {
    if (world.chunkData.empty()) return false;
    ivec2 chunkCoord = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    uint64_t chunkIndex = pack(chunkCoord);
    auto chunkIt = world.chunkData.find(chunkIndex);
    if (chunkIt == world.chunkData.end()) return false;
    auto& chunk = chunkIt->second;
    //Chunk& chunk = world.chunkData[chunkCoord];
    //if (!chunk.exists) return false;
    int vec_blockPos = chunk->at(blockPos);
    if (vec_blockPos >= CHUNK_VOLUME || vec_blockPos < 0) {
        return false;
    }
    Item excl_item = chunk->block_data[vec_blockPos].blockType;
    bool blockIsSolid = (block_excl_natural && !(excl_item.isUncullable() && excl_item != OAK_LEAVES));
    return (chunk->block_data[vec_blockPos].blockType != AIR) && blockIsSolid;
}

vec3 lookingAtBlock() {
    ivec3 blockPos = ivec3(0);
    glm::vec3 rayDir;
    glm::vec3 rayOrigin = firstCamera.getPosition() + 0.5f;
    rayDir = normalize(firstCamera.getFront());

    float maxDistance = 7.0f;
    float stepSize = 0.4f;
    for (float t = 0.0f; t < maxDistance; t += stepSize) {
        glm::vec3 point = rayOrigin + rayDir * t;
        blockPos = glm::floor(point);
        ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
        int chunkVal = pack(chunkPos);
        //if (!chunkCoords.count(chunkVal)) return vec3(0);
        if (rayOrigin.y > 0 && rayOrigin.y < CHUNK_SIZE * CHUNK_SIZE && world.chunkData.count(chunkVal)) {
            auto& chunk = world.chunkData.at(chunkVal);
            uint64 index = chunk->at(blockPos);
            if (chunk->block_data[index].blockType != AIR)
                return blockPos;
        }
    }
    return ivec3(-404);
}

void World::addChunk(unique_ptr<Chunk>& newChunk, uint32_t xyChunk) {
    //chunkData.insert(xyChunk, newChunk);
    chunkData.emplace(xyChunk, move(newChunk));
    //chunkCount++;
    //chunkData[pack(xyChunk)] = *newChunk;
}

LightMesh World::createProjectileMesh(vec3 blockPos, float scale, Item blockType) {
    if (blockType == AIR) return LightMesh();
    float UVs[7] = { 1, 0, 0, 0, 0, 0, 1 };
    getUVs(blockType, UVs);
    float   xoffset         = UVs[0],
            yoffset         = UVs[1],
            xoffsetTop      = UVs[2],
            yoffsetTop      = UVs[3],
            xoffsetBottom   = UVs[4],
            yoffsetBottom   = UVs[5];

    float clipX = 0.0f, clipY = 1.0f;

    auto absl = [](int n) { return n >= 0 ? n : -n; };

    vector<unsigned int> indices;
    for (int e = 0; e < 6; e++) {
        int base = e * 4;
        //indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
        //indices.push_back(base + 2); indices.push_back(base + 3); indices.push_back(base + 0);
        indices.push_back(base + 0); indices.push_back(base + 3); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 1); indices.push_back(base + 0);
    }

    float tintr = 1.0f, tintg = 1.0f, tintb = 1.0f;
    if (blockType == GRASS_BLOCK) {
        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
    }
    else if (blockType == GRASS) {
        tintr = 0.2f, tintg = 1.25f, tintb = 0.15f;
    }
    else if (blockType == OAK_LEAVES) {
        tintr = 0.2f, tintg = 1.0f, tintb = 0.2f;
    }
    //vec3 rgb = { tintr, tintg, tintb };

    vector<GLfloat> triangle;
    vector<GLfloat> globalUVs;
    vector<GLfloat> normals;
    vector<GLfloat> colorMask;

    if (!blockType.isFlat()) {
        for (int i = 0; i < 6; i++) {
            int offsetX = 0, offsetY = 0;
            if (i == 4) { offsetX = xoffsetBottom; offsetY = yoffsetBottom; }
            else if (i == 5) { offsetX = xoffsetTop;  offsetY = yoffsetTop; }

            uint32_t uintUVs = (((uint8_t)155 << 24)) | ((xdimens << 16) | (uint8_t)(yoffset + offsetY) << 8) | ((uint8_t)(xoffset + offsetX)); // Packaging floats into one integer
            float startUvs;
            memcpy(&startUvs, &uintUVs, sizeof(float));

            if (i % 3 == 0) {
                globalUVs.push_back(clipX); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipY); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipY); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipX); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
            }
            else {
                globalUVs.push_back(clipX); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipX); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipY); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
                globalUVs.push_back(clipY); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
            }
        }

        triangle = {
                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,
                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,
                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,
                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale,

                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,
                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,

                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,
                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale,       
                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,

                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,
                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,
                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,

                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,
                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,   -0.50f + blockPos.z + 0.1f * -scale,
                 0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,
                -0.50f + blockPos.x + 0.1f * -scale,  -0.50f + blockPos.y + 0.1f * -scale,    0.50f + blockPos.z + 0.1f * scale,

                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale,
                -0.50f + blockPos.x + 0.1f * -scale,   0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,     0.50f + blockPos.z + 0.1f * scale,
                 0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,    -0.50f + blockPos.z + 0.1f * -scale
        };

        normals = long_normals;
        for (int i = 0; i < triangle.size() / 3; i++) { colorMask.push_back(tintr); colorMask.push_back(tintg); colorMask.push_back(tintb); }
    }
    else {
        int tileSize = blockType.isTool() ? xdimensItem : xdimens;
        for (int e = 0; e < 2; e++) {
            int base = e * 4;
            indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
            indices.push_back(base + 2); indices.push_back(base + 3); indices.push_back(base + 0);
        }
        uint32_t uintUVs = (((uint8_t)155 << 24)) | (((uint8_t)tileSize << 16)) | ((uint8_t)(yoffset) << 8) | ((uint8_t)(xoffset)); // Packaging floats into one integer
        float startUvs;
        memcpy(&startUvs, &uintUVs, sizeof(float));
        //for (int i = 0; i < 6; i++) {
        globalUVs = {
            (clipX), (clipX), startUvs,
            (clipX), (clipY), startUvs,
            (clipY), (clipY), startUvs,     
            (clipY), (clipX), startUvs,

            (clipX), (clipX), startUvs,
            (clipY), (clipX), startUvs,
            (clipY), (clipY), startUvs,
            (clipX), (clipY), startUvs,
        };
        //}

        triangle = {
                -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.0f + blockPos.z,
                -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,    0.0f + blockPos.z,
                    0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,    0.0f + blockPos.z,
                    0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.0f + blockPos.z,

                -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.0f + blockPos.z,
                    0.50f + blockPos.x + 0.1f * scale,   -0.50f + blockPos.y + 0.1f * -scale,    0.0f + blockPos.z,
                    0.50f + blockPos.x + 0.1f * scale,    0.50f + blockPos.y + 0.1f * scale,    0.0f + blockPos.z,
                -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,    0.0f + blockPos.z,
        };

        normals = long_normals;
        for (int i = 0; i < triangle.size() / 3; i++) { colorMask.push_back(tintr); colorMask.push_back(tintg); colorMask.push_back(tintb); }
    }

    vector<GLfloat> finalvertices;
    for (int i = 0; i < triangle.size(); i += 3) {
        finalvertices.push_back(triangle[i + 0]);
        finalvertices.push_back(triangle[i + 1]);
        finalvertices.push_back(triangle[i + 2]);

        finalvertices.push_back(globalUVs[i + 0]);
        finalvertices.push_back(globalUVs[i + 1]);
        finalvertices.push_back(globalUVs[i + 2]);

        uint32_t norm_color = ((abyte(normals[i + 0] < 0 ? 1 : 0) & 0x1) << 5) | ((abyte(absl(normals[i + 0])) & 0x1) << 4)
            | ((abyte(normals[i + 1] < 0 ? 1 : 0) & 0x1) << 3) | ((abyte(absl(normals[i + 1])) & 0x1) << 2)
            | ((abyte(normals[i + 2] < 0 ? 1 : 0) & 0x1) << 1) | ((abyte(absl(normals[i + 2])) & 0x1) << 0)
            | ((abyte(tintr * 100) & 0x7F) << 20)
            | ((abyte(tintg * 100) & 0x7F) << 13)
            | ((abyte(tintb * 100) & 0x7F) << 6);
        float normcolor;
        memcpy(&normcolor, &norm_color, sizeof(float));

        finalvertices.push_back(normcolor);

        //finalvertices.push_back(normals[i + 0]);  
        finalvertices.push_back(normals[i + 1]);
        finalvertices.push_back(normals[i + 2]);

        finalvertices.push_back(colorMask[i + 0]);
        finalvertices.push_back(colorMask[i + 1]);
        finalvertices.push_back(colorMask[i + 2]);
    }

    LightMesh returnMesh;
    returnMesh.createMesh(finalvertices, indices, finalvertices.size(), indices.size());
    return returnMesh;
}

Block World::deleteBlockFromWorld(vec3 blockPos) {
    ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    auto& chunk = chunkData.at(pack(chunkPos));
    Block returnBlock;
    Item& blockType = chunk->block_data[chunk->at(blockPos)].blockType;
    if (blockType.isBreakable()) {
        //inventory.assignAvailableSlot(blockType);
        returnBlock = Block(blockPos, blockType);
        blockType.deassignLight(pointLights, blockPos);
        blockType = AIR;
        chunk->setAsDirty();
        chunk->safe_unload = 1;
    }
    return returnBlock;
}

vector<GLfloat> makeLine(vec3 begin, float lx, float wz, int face, float hy = 0) {
    vector<GLfloat> lineVerts;
    if ((face / 2) == 2) {
        lineVerts = {
            begin.x + 0.0f,  begin.y + 0.0f, begin.z + 0.0f,
            begin.x + lx,    begin.y + 0.0f, begin.z + 0.0f,
            begin.x + lx,    begin.y + 0.0f, begin.z + wz,
            begin.x + 0.0f,  begin.y + 0.0f, begin.z + wz,      //0->
        };
    }
    else if ((face / 2) == 1) {
        lineVerts = {
            begin.x + 0.0f,  begin.y + 0.0f, begin.z + 0.0f,
            begin.x + 0.0f,  begin.y + hy,   begin.z + 0.0f,
            begin.x + lx,    begin.y + hy,   begin.z + 0.0f,    //  ^
            begin.x + lx,    begin.y + 0.0f, begin.z + 0.0f,    // o||   =>
        };
    }
    else {
        lineVerts = {
            begin.x + 0.0f,  begin.y + 0.0f, begin.z + 0.0f,
            begin.x + 0.0f,  begin.y + hy, begin.z + 0.0f,
            begin.x + 0.0f,  begin.y + hy, begin.z + wz,    //<=o
            begin.x + 0.0f,  begin.y + 0.0f, begin.z + wz,
        };
    }
    return lineVerts;
}

vector<GLfloat> makeLine(vec3 begin, vec3 end, int face) {
    vector<GLfloat> lineVerts;
    float xb = begin.x, yb = begin.y, zb = begin.z, xe = xb + end.x, ye = yb + end.y, ze = zb + end.z;
    if ((face / 2) == 0) {
        lineVerts = {
            xb, yb, zb,
            xb, ye, zb,
            xb, ye, ze,
            xb, yb, ze,
        };
    }
    else if ((face / 2) == 1) {
        lineVerts = {
            xb, yb, zb,
            xb, ye, zb,
            xe, ye, zb,
            xe, yb, zb,
        };
    }
    else if ((face / 2) == 2) {
        lineVerts = {
            xb, yb, zb,
            xb, yb, ze,
            xe, yb, ze,
            xe, yb, zb,
        };
    }
    return lineVerts;
}

LightMesh World::createVertsOnlyMesh(ivec3 xyz, float scale, Item blockType) {
    if (blockType == AIR) {
        return LightMesh();
    }

    float t = 0.01f, T = 0.5f * scale, W = -0.5f * scale, H = 1.0f * scale;

    auto push = [](vector<float> line, vector<float>& verts) {
        for (int i = 0; i < line.size(); i++) {
            verts.push_back(line[i]);
        }
        };

    vector<GLfloat> vertices;

    vector<GLfloat> lineX01 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(W, t, H), 0); push(lineX01, vertices);
    vector<GLfloat> lineX02 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(W, H, t), 0); push(lineX02, vertices);
    vector<GLfloat> lineX03 = makeLine(vec3(xyz) + vec3(W, T, T), vec3(W, -H, -t), 0); push(lineX03, vertices);
    vector<GLfloat> lineX04 = makeLine(vec3(xyz) + vec3(W, T, T), vec3(W, -t, -H), 0); push(lineX04, vertices);

    vector<GLfloat> lineX11 = makeLine(vec3(xyz) + vec3(T, W, W), vec3(W, t, H), 1); push(lineX11, vertices);
    vector<GLfloat> lineX12 = makeLine(vec3(xyz) + vec3(T, W, W), vec3(W, H, t), 1); push(lineX12, vertices);
    vector<GLfloat> lineX13 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(W, -H, -t), 1); push(lineX13, vertices);
    vector<GLfloat> lineX14 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(W, -t, -H), 1); push(lineX14, vertices);

    vector<GLfloat> lineZ01 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(H, t, W), 2); push(lineZ01, vertices);
    vector<GLfloat> lineZ02 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(t, H, W), 2); push(lineZ02, vertices);
    vector<GLfloat> lineZ03 = makeLine(vec3(xyz) + vec3(T, T, W), vec3(-t, -H, W), 2); push(lineZ03, vertices);
    vector<GLfloat> lineZ04 = makeLine(vec3(xyz) + vec3(T, T, W), vec3(-H, -t, W), 2); push(lineZ04, vertices);

    vector<GLfloat> lineZ11 = makeLine(vec3(xyz) + vec3(W, W, T), vec3(H, t, W), 3); push(lineZ11, vertices);
    vector<GLfloat> lineZ12 = makeLine(vec3(xyz) + vec3(W, W, T), vec3(t, H, W), 3); push(lineZ12, vertices);
    vector<GLfloat> lineZ13 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(-t, -H, W), 3); push(lineZ13, vertices);
    vector<GLfloat> lineZ14 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(-H, -t, W), 3); push(lineZ14, vertices);

    vector<GLfloat> lineY01 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(H, W, t), 4); push(lineY01, vertices);
    vector<GLfloat> lineY02 = makeLine(vec3(xyz) + vec3(W, W, W), vec3(t, W, H), 4); push(lineY02, vertices);
    vector<GLfloat> lineY03 = makeLine(vec3(xyz) + vec3(T, W, T), vec3(-t, W, -H), 4); push(lineY03, vertices);
    vector<GLfloat> lineY04 = makeLine(vec3(xyz) + vec3(T, W, T), vec3(-H, W, -t), 4); push(lineY04, vertices);

    vector<GLfloat> lineY11 = makeLine(vec3(xyz) + vec3(W, T, W), vec3(H, W, t), 5); push(lineY11, vertices);
    vector<GLfloat> lineY12 = makeLine(vec3(xyz) + vec3(W, T, W), vec3(t, W, H), 5); push(lineY12, vertices);
    vector<GLfloat> lineY13 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(-t, W, -H), 5); push(lineY13, vertices);
    vector<GLfloat> lineY14 = makeLine(vec3(xyz) + vec3(T, T, T), vec3(-H, W, -t), 5); push(lineY14, vertices);

    vector<GLfloat> globalUVs; globalUVs.assign(vertices.size(), 0.0f);
    vector<GLfloat> colorMask; colorMask.assign(vertices.size(), 0.0f);
    vector<GLfloat> normals;   normals.assign(vertices.size(), 0.0f);

    vector<GLfloat> finalvertices;
    for (int i = 0; i < vertices.size(); i += 3) {
        finalvertices.push_back(vertices[i + 0]);
        finalvertices.push_back(vertices[i + 1]);
        finalvertices.push_back(vertices[i + 2]);

        finalvertices.push_back(globalUVs[i + 0]);
        finalvertices.push_back(globalUVs[i + 1]);
        finalvertices.push_back(globalUVs[i + 2]);

        finalvertices.push_back(normals[i + 0]);
        finalvertices.push_back(normals[i + 1]);
        finalvertices.push_back(normals[i + 2]);

        finalvertices.push_back(colorMask[i + 0]);
        finalvertices.push_back(colorMask[i + 1]);
        finalvertices.push_back(colorMask[i + 2]);
    }

    std::vector<unsigned int> indices;
    for (int e = 0; e < 12 * 4; e++) {
        int base = e * 4;
        indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 3); indices.push_back(base + 0);
        indices.push_back(base + 0); indices.push_back(base + 3); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 1); indices.push_back(base + 0);
    }

    LightMesh cubeMesh;
    cubeMesh.createMesh(finalvertices, indices, finalvertices.size(), indices.size());
    return cubeMesh;
}

LightMesh createCompassVertsOnlyMesh(ivec3 xyz) {
    std::vector<unsigned int> indices;
    for (int e = 0; e < 6 * 4; e++) {
        int base = e * 4;
        indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 3); indices.push_back(base + 0);
        indices.push_back(base + 0); indices.push_back(base + 3); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 1); indices.push_back(base + 0);
    }

    float t = 0.0075f;

    auto push = [](vector<float> line, vector<float>& verts) {
        for (int i = 0; i < line.size(); i++) {
            verts.push_back(line[i]);
        }
        };

    vector<GLfloat> vertices;

    vector<GLfloat> lineX = makeLine(xyz, 0.05f , 0.05f , 2, 0.005f);
    vector<GLfloat> lineY = makeLine(xyz, 0.005f, 0.005f, 2, 0.05f);
    vector<GLfloat> lineZ = makeLine(xyz, 0.005f, 0.05f , 0, 0.005f);

    push(lineX, vertices); push(lineY, vertices); push(lineZ, vertices);

    vector<GLfloat> globalUVs;
    for (int i = 0; i < vertices.size(); i++) { globalUVs.push_back(0.0f); }

    vector<GLfloat> colorMask;
    for (int i = 0; i < vertices.size(); i++) { colorMask.push_back(0.0f); }

    vector<GLfloat> normals;
    for (int i = 0; i < vertices.size(); i++) { normals.push_back(0.0f); }

    vector<GLfloat> finalvertices;
    for (int i = 0; i < vertices.size(); i += 3) {
        finalvertices.push_back(vertices[i + 0]);
        finalvertices.push_back(vertices[i + 1]);
        finalvertices.push_back(vertices[i + 2]);

        finalvertices.push_back(globalUVs[i + 0]);
        finalvertices.push_back(globalUVs[i + 1]);
        finalvertices.push_back(globalUVs[i + 2]);

        finalvertices.push_back(normals[i + 0]);
        finalvertices.push_back(normals[i + 1]);
        finalvertices.push_back(normals[i + 2]);

        finalvertices.push_back(colorMask[i + 0]);
        finalvertices.push_back(colorMask[i + 1]);
        finalvertices.push_back(colorMask[i + 2]);
    }

    LightMesh cubeMesh;
    cubeMesh.createMesh(finalvertices, indices, finalvertices.size(), indices.size());
    return cubeMesh;
}

LightMesh World::createMeshCube(vec3 blockPos, float scale, Item blockType) {
    float UVs[7] = { 1, 0, 0, 0, 0, 0, 1 };
    if (!blockType.isFlat())
        getUVs(blockType, UVs);
    float xoffset = UVs[0],
        yoffset = UVs[1],
        xoffsetTop = UVs[2],
        yoffsetTop = UVs[3],
        xoffsetBottom = UVs[4],
        yoffsetBottom = UVs[5],
        transparency = 1.0f;

    float clipX = 0.0f, clipY = 1.0f;

    auto absl = [](int n) { return n >= 0 ? n : -n; };

    vector<unsigned int> indices;
    for (int e = 0; e < 6; e++) {
        int base = e * 4;
        indices.push_back(base + 0); indices.push_back(base + 1); indices.push_back(base + 2);
        indices.push_back(base + 2); indices.push_back(base + 3); indices.push_back(base + 0);
    }

    int offsetX = 1, offsetY = 1;
    vector<GLfloat> globalUVs;
    uint32_t uintUVs = ((((uint8_t)155 << 24)) | ((uint8_t)1 << 16) | (uint8_t)(offsetY) << 8) | ((uint8_t)(offsetX)); // Packaging floats into one integer
    float startUvs;
    memcpy(&startUvs, &uintUVs, sizeof(float));

    for (int i = 0; i < 6; i++) {
        if (i % 3 != 0 && i != 5) {
            globalUVs.push_back(clipX); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipX); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipY); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipY); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
        }
        else {
            globalUVs.push_back(clipX); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipY); globalUVs.push_back(clipX); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipY); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
            globalUVs.push_back(clipX); globalUVs.push_back(clipY); globalUVs.push_back(startUvs);
        }
    }

    vector<GLfloat> triangle = {
            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,
            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,
            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,
            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale,

             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,
             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,

            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,
            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,

            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,
             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,
            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,

            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,     -0.50f + blockPos.z + 0.1f * -scale,
             0.50f + blockPos.x + 0.1f * scale,    -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,
            -0.50f + blockPos.x + 0.1f * -scale,   -0.50f + blockPos.y + 0.1f * -scale,      0.50f + blockPos.z + 0.1f * scale,

            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale,
            -0.50f + blockPos.x + 0.1f * -scale,    0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,       0.50f + blockPos.z + 0.1f * scale,
             0.50f + blockPos.x + 0.1f * scale,     0.50f + blockPos.y + 0.1f * scale,      -0.50f + blockPos.z + 0.1f * -scale
    };

    vector<GLfloat> normals = long_normals;
    vector<GLfloat> colorMask;
    for (int i = 0; i < triangle.size(); i++) { colorMask.push_back(1.0f); }

    vector<GLfloat> finalvertices;
    for (int i = 0; i < triangle.size(); i += 3) {
        finalvertices.push_back(triangle[i + 0]);
        finalvertices.push_back(triangle[i + 1]);
        finalvertices.push_back(triangle[i + 2]);

        finalvertices.push_back(globalUVs[i + 0]);
        finalvertices.push_back(globalUVs[i + 1]);
        finalvertices.push_back(globalUVs[i + 2]);

        uint32_t norm_color = ((abyte(normals[i + 0] < 0 ? 1 : 0) & 0x1) << 5) | ((abyte(absl(normals[i + 0])) & 0x1) << 4)
            | ((abyte(normals[i + 1] < 0 ? 1 : 0) & 0x1) << 3) | ((abyte(absl(normals[i + 1])) & 0x1) << 2)
            | ((abyte(normals[i + 2] < 0 ? 1 : 0) & 0x1) << 1) | ((abyte(absl(normals[i + 2])) & 0x1) << 0)
            | ((abyte(100) & 0x7F) << 20)
            | ((abyte(100) & 0x7F) << 13)
            | ((abyte(100) & 0x7F) << 6);
        float normcolor;
        memcpy(&normcolor, &norm_color, sizeof(float));

        finalvertices.push_back(normcolor);
        finalvertices.push_back(normals[i + 1]);
        finalvertices.push_back(normals[i + 2]);

        finalvertices.push_back(colorMask[i + 0]);
        finalvertices.push_back(colorMask[i + 1]);
        finalvertices.push_back(colorMask[i + 2]);
    }

    LightMesh returnMesh;
    returnMesh.createMesh(finalvertices, indices, finalvertices.size(), indices.size());
    return returnMesh;
}

void World::createItem(vec3 blockPos, Item blockType, vec3 direction) {
    ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    auto& chunk = chunkData.at(pack(chunkPos));
    BlockData& blockData = chunk->block_data[chunk->at(blockPos)];

    blockData = BlockData(blockType);
    ivec3 trueDir = floor(blockPos - direction) + vec3(1.0);
    chunk->setAsDirty();
    //updateChunk(chunkPos, trueDir, blockPos);
    inventory.deassignInvSlot(slot, 3);
    blockData.blockType.assignLight(pointLights, blockPos);
}

bool isAir(Item item);

bool shouldEmitFace(vec2 xyChunk, Chunk* cd, int x, int y, int z, int dx, int dy, int dz);

void emitFace(Mesh& m, int face, Item blockType, float x, float y, float z);

//void meshChunk(vec2 xyChunk, unique_ptr<Chunk>& ch);// , int subChunky);

void meshChunk(chNeighPack*);

//
//void meshSubChunks(vec2 xyChunk, Chunk* ch);// , Mesh& m, int subChunky);
//
//void meshSubChunk(vec2 xyChunk, Chunk* ch, int subChunky);
//
//
//void meshClouds(CloudMesh& cloudmesh, vec2 xyChunk);

void meshChunk(vec2 xyChunk, Chunk* cd, Mesh& out, vec3 direction, ivec3 position);

void generateChunkAt(vec2 xyChunk, Chunk* repChunk);

void updateChunk(chNeighPack* chNeigh, vec3 direction, ivec3 position) {
    //unique_ptr<Mesh> m = make_unique<Mesh>();
    // Rebuild mesh from current blockData
 //   static int vi = 0;
	//auto start = std::chrono::high_resolution_clock::now();
    meshChunk(chNeigh);
	//auto end = std::chrono::high_resolution_clock::now();
	//if (vi++ % 10 == 0)
 //     cout << "Chunk updating, Elapsed: " << std::chrono::duration<double>(end - start).count() << " s\n";
}

//void World::updateChunk(const ivec2& chunkCoord, vec3 direction, ivec3 position) {
//    //auto start = std::chrono::high_resolution_clock::now();
//
//    auto it = chunkData.find(pack(chunkCoord));
//    if (it == chunkData.end()) return; // Chunk doesn't exist, painful null ptr risk
//
//    auto& chunk = it->second;
//
//    //if (!chunk) return;
//	unique_ptr<Mesh> m = make_unique<Mesh>();
//    // Rebuild mesh from current blockData
//    meshChunk(chunkCoord, chunk//, i
//        //, direction, position
//    );
//    //auto end = std::chrono::high_resolution_clock::now();
//    //std::cout << chunk->mesh->vertices.size() << " verts. chunk n_" << chunkCount++ << " updating, Elapsed: " << std::chrono::duration<double>(end - start).count() << " s\n";
//}

Block World::delBlocklook_at() {
    ivec3 blockPos = lookingAtBlock();
    ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
    if (blockPos.y <= -404.0f) {
        return Block(vec3(0), AIR);
    }
    return deleteBlockFromWorld(blockPos);
}

Block World::addBlocklook_at(Item blockType) {
    if (recipe.isTool(blockType)) {
        return Block(vec3(-404.0f), AIR);
    }
    ivec3 blockPos = vec3(0.0f);
    glm::vec3 rayDir;
    glm::vec3 rayOrigin = firstCamera.getPosition() + 0.5f;
    rayDir = normalize(firstCamera.getFront());

    float maxDistance = 7.0f;
    float stepSize = 0.2f;
    for (float t = 0.0f; t < maxDistance; t += stepSize) {
        glm::vec3 point = rayOrigin + rayDir * t;
        blockPos = glm::floor(point);
        ivec2 chunkPos = ivec2(floorDiv(blockPos.x, CHUNK_SIZE), floorDiv(blockPos.z, CHUNK_SIZE));
        auto& chunk = world.chunkData.at(pack(chunkPos));
        int index = chunk->at(blockPos);
        if (blockExistsAt(blockPos) &&
            chunk->block_data[index].blockType != AIR &&
            blockType.isPlaceable()) {
            createItem(floor(point - rayDir * stepSize), blockType, point);
            chunk->setAsDirty();
            chunk->safe_unload = 1;
            return Block(floor(point - rayDir * stepSize), blockType);
        }
    }
}