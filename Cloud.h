#pragma once
#include <vector>
using a_byte = uint8_t;
using a_word = uint32_t;
inline int chunkSize = 16;

inline ivec2 unpack(uint32_t packedxz) {
    int32_t x = int16_t(packedxz >> 16), z = int16_t(packedxz);
    return ivec2(x, z);
}

inline uint32_t pack(ivec2 xz) {
    uint32_t x = uint16_t(xz.x) << 16, z = uint16_t(xz.y);
    return x | z;
}

class CloudMesh {
public:
    unique_ptr<Mesh> mesh;
    vector<a_byte> cloud_data;
    a_byte& operator()(int x, int z) {
        if (mesh) return cloud_data[x * chunkSize + z];
    }
    CloudMesh() {
        mesh = make_unique<Mesh>();
        cloud_data.assign(chunkSize * chunkSize, 0);
    }
};