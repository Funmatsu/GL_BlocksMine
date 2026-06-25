#pragma once
#pragma once

#include "Block.h"
#include "Cloud.h"

using abyte = uint8;
constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_HEIGHT = CHUNK_SIZE * CHUNK_SIZE;  //(CHUNK_SIZE + 2) * (CHUNK_SIZE + 2);
constexpr int CHUNK_SIZE_2 = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;  //(CHUNK_SIZE + 2) * CHUNK_HEIGHT * (CHUNK_SIZE + 2); // Or just CHUNK_HEIGHT * CHUNK_HEIGHT

//int at(vec3 position) {
//    return position.x * CHUNK_SIZE + (position.y + 1) * CHUNK_HEIGHT + position.z;
//}

//struct ivec2_hash {
//    size_t operator()(const glm::ivec2& v) const noexcept {
//        // widen to unsigned to avoid UB on shifts of negatives
//        uint64_t x = static_cast<uint32_t>(v.x);
//        uint64_t y = static_cast<uint32_t>(v.y);
//        uint64_t h = x * 0x9E3779B185EBCA87ull ^ (y + 0x9E3779B185EBCA87ull + (x << 6) + (x >> 2));
//        // size_t may be 32-bit; fold if needed
//        if constexpr (sizeof(size_t) == 4) {
//            h ^= (h >> 32);
//        }
//        return static_cast<size_t>(h);
//    }
//};
//
//struct ivec2_eq {
//    bool operator()(glm::ivec2 const& a, glm::ivec2 const& b) const noexcept {
//        return a.x == b.x && a.y == b.y;
//    }
//};



//uint64_t pack(ivec2 xz) {
//    uint64_t x = uint32_t(xz.x), z = uint32_t(xz.y);
//    return (x << 32) | z;
//}

//struct ivec3_hash {
//    size_t operator()(glm::ivec3 const& v) const noexcept {
//        uint64_t ux = static_cast<uint32_t>(v.x);
//        uint64_t uy = static_cast<uint32_t>(v.y);
//        uint64_t uz = static_cast<uint32_t>(v.z);
//        uint64_t h = ux;
//        h ^= (uy + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));
//        h ^= (uz + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2));
//        return static_cast<size_t>(h);
//    }
//};
//
//struct ivec3_eq {
//    bool operator()(glm::ivec3 const& a, glm::ivec3 const& b) const noexcept {
//        return a.x == b.x && a.y == b.y && a.z == b.z;
//    }
//};

struct MeshData {
    Mesh mesh;
    ivec3 crd;
};

struct BlockData {
    Item blockType;
    //abyte blight = 0xF;
    //uint8_t orientation = (1 << 2); // 00 01 00 = 0,1,0  00 11 00 = 0,-1,0
    BlockData() { blockType = AIR; }
    BlockData(Item type) {
        blockType = type;
    }
    bool operator==(Item item) {
        return item == blockType;
    }
    //BlockData(const BlockData& data) {
    //    blockType = data.blockType;
    //}
};
// use it ? nope :) vectors allocate inside the heap (not the stack like some other :) which makes it better than using array or well a c array or a C++11 array<T, N> outside of scope. Hope I got that
// Maps are horrible and a despicable wastage of memory BUT provide O(1) acess which is all I need
// 
//std::unordered_map<glm::ivec3, Block, ivec3_hash, ivec3_eq> worldBlocks;
//std::unordered_map<glm::ivec3, blockData, ivec3_hash, ivec3_eq> worldBlocks;

class SubChunk {
public:
    unique_ptr<LightMesh> mesh;
    abyte needUpdate;
    SubChunk() {
        mesh = make_unique<LightMesh>();
        needUpdate = true;
    }
};

class Chunk {
public:
    array<BlockData, CHUNK_VOLUME> block_data;
	unique_ptr<LightMesh> mesh;
    
    uint32_t coord;
    abyte neighboursPresent : 5;
    void setAsDirty() { neighboursPresent &= ~(1 << 0); }
	void setAsClean() { neighboursPresent |=  (1 << 0); }
    bool getDirty() { return !((neighboursPresent >> 0) & 1); }
    ivec2 coords() {
        return ivec2(int16_t((coord >> 16)), int16_t(coord));
    }
    void toCoords(ivec2 chunkCoord) {
        coord = ((uint16_t(chunkCoord.x)) << 16) | (uint16_t(chunkCoord.y));
    }
    int at(vec3 position) {
        ivec2 thisCoord = coords();
        return ((position.y) * (CHUNK_HEIGHT) + 
                (position.x - thisCoord.x * (CHUNK_SIZE)) * (CHUNK_SIZE) + 
                (position.z - thisCoord.y * (CHUNK_SIZE)));
    }

    int at(int x, int y, int z) {
        ivec2 coord = coords();
        return ((y) * (CHUNK_HEIGHT)+
                (x - coord.x * CHUNK_SIZE) * (CHUNK_SIZE)+
                (z - coord.y * CHUNK_SIZE));
    }

    bool inBounds(vec3 position) {
        ivec2 thisCoord = coords();
        return position.x >= (thisCoord.x) * (CHUNK_SIZE) && position.x < (thisCoord.x + 1) * (CHUNK_SIZE) &&
               position.y >= 0                            && position.y < CHUNK_HEIGHT                     &&
               position.z >= (thisCoord.y) * (CHUNK_SIZE) && position.z < (thisCoord.y + 1) * (CHUNK_SIZE);
    }

    bool inBounds(int x, int y, int z) {
        ivec2 thisCoord = coords();
        float chunkx = (thisCoord.x) * (CHUNK_SIZE), chunkz = (thisCoord.y) * (CHUNK_SIZE);
        return x >= chunkx + 0 && x < chunkx + CHUNK_SIZE &&
               y >= 0 && y < CHUNK_HEIGHT &&
               z >= chunkz + 0 && z < chunkz + CHUNK_SIZE;
    }

    bool localInBounds(vec3 position) {
        return position.x >= 0 && position.x < CHUNK_SIZE   &&
               position.y >= 0 && position.y < CHUNK_HEIGHT &&
               position.z >= 0 && position.z < CHUNK_SIZE;
    }

    bool localInBounds(int x, int y, int z) {
        return x >= 0 && x < CHUNK_SIZE &&
               y >= 0 && y < CHUNK_HEIGHT &&
               z >= 0 && z < CHUNK_SIZE;
    }

    Chunk(){
        mesh = make_unique<LightMesh>();
        coord = uint32_t(-1);
    }

    BlockData& operator[](ivec3 pos) {
        int index = at(pos);
        return block_data[index];
    }

    BlockData& operator()(float x, float y, float z) {
        int index = at(x, y, z);
        return block_data[index];
    }
};

bool localInBounds(int x, int y, int z) {
    return  x >= 0 && x < CHUNK_SIZE   &&
            y >= 0 && y < CHUNK_HEIGHT &&
            z >= 0 && z < CHUNK_SIZE;
}

bool localInBounds(vec3 xyz) {
    return  xyz.x >= 0 && xyz.x < CHUNK_SIZE   &&
            xyz.y >= 0 && xyz.y < CHUNK_HEIGHT &&
            xyz.z >= 0 && xyz.z < CHUNK_SIZE;
}

int at(vec3 position, vec2 xychunk) {
    ivec2 thisCoord = xychunk;
    return ((position.y) * (CHUNK_HEIGHT)+
        (position.x - thisCoord.x * (CHUNK_SIZE)) * (CHUNK_SIZE)+
        (position.z - thisCoord.y * (CHUNK_SIZE)));
}

int at(vec3 position) {
    return ((position.y) * (CHUNK_HEIGHT)+
            (position.x) * (CHUNK_SIZE)+
            (position.z));
}

int at(int x, int y, int z) {
    return ((y) * (CHUNK_HEIGHT)+
            (x) * (CHUNK_SIZE)+
            (z));
}

uint toCoords(ivec2 crd) {
    return ((uint16_t(crd.x) & 0xFFFF) << 16) | (uint16_t(crd.y) & 0xFFFF);
}

uint16_t chunkCount = 0;

struct chPack {
	Chunk* chPtr;
    uint coords;
};

struct chNeighPack {
    uint coords;
    array<BlockData, CHUNK_VOLUME> block_data;
    array<BlockData, CHUNK_VOLUME> neighbour_data[4];
};

struct chNeighResult {
	unique_ptr<Mesh> mesh;
    uint coords;
    chNeighResult() {
        mesh = make_unique<Mesh>();
    }
};