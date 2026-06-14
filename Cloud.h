#pragma once
#include <vector>
using abyte = uint8_t;
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
    vector<abyte> cloud_data;
    abyte& operator()(int x, int z) {
        if (mesh) return cloud_data[x * chunkSize + z];
    }
    CloudMesh() {
        mesh = make_unique<Mesh>();
        cloud_data.assign(chunkSize * chunkSize, 0);
    }
};