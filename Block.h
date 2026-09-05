#pragma once

#include "libraries.h"

struct Face {
    ivec3 center = ivec3(0);
    vector<float> vertices;
    vector<unsigned int> indices;
    int faceId = 0;
    bool operator==(const Face& face) {
        return (vertices == face.vertices);
    }
};

class Block {
public:
    vec3 position = vec3(0.0f);
    Item type;
    int bLight = 0;

    Block();

    Block(vec3 pos, Item blockType, int blockLight = 0);

    Block(const Block& block);

    void operator=(Block block) {
        type = block.type;
        position = block.position;
		bLight = block.bLight;
    }

    bool operator==(Block block) {
        return (type == block.type &&
            position == block.position);
    }
};

Block::Block() {
    position = vec3();
    type = AIR;
}

Block::Block(vec3 pos, Item blockType, int blockLight) {
    position = pos;
    type = blockType;
    bLight = blockLight;
}

Block::Block(const Block& block) {
    position = block.position;
    type = block.type;
	bLight = block.bLight;
}

vector<GLfloat> blockVerts = {
    0.0f,       0.0f,      0.0f , // 0
    1.0f,       0.0f,      0.0f , // 1
    1.0f,       1.0f,      0.0f , // 2
    0.0f,       1.0f,      0.0f , // 3
    0.0f,       0.0f,      1.0f , // 4
    1.0f,       0.0f,      1.0f , // 5
    1.0f,       1.0f,      1.0f , // 6
    0.0f,       1.0f,      1.0f   // 7
};
