#pragma once
#include "libraries.h"
class Hand {
public:
    float width = 5.0f, height = 10.0f;
    vector<float> handVerts = {
        0.0f,       0.0f,       0.0f,           0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       height,     0.0f,           1.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       height,     width,          1.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       0.0f,       width,          0.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,

        height,     0.0f,       0.0f,           0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     height,     0.0f,           1.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     height,     width,          1.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     0.0f,       width,          0.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,

        0.0f,       0.0f,    0.0f,              0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     0.0f,    0.0f,              1.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     0.0f,    width,             1.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       0.0f,    width,             0.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,

        0.0f,       height,  0.0f,              0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     height,  0.0f,              1.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     height,  width,             1.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       height,  width,             0.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,

        0.0f,       0.0f,    0.0f,              0.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     0.0f,    0.0f,              1.0, 0.0, 1.0, 0.0f, 0.0f, 0.0f,
        height,     height,  0.0f,              1.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
        0.0f,       height,  0.0f,              0.0, 1.0, 1.0, 0.0f, 0.0f, 0.0f,
    };

    vector<unsigned int> handInds = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,
    };
    //Mesh handMesh;

    //void createHand() {
    //    handMesh.createMesh(handVerts, handInds, handVerts.size(), handInds.size());
    //}
    //void drawHand(mat4 ortho) {
    //    glUniformMatrix4fv(glGetUniformLocation(shaders[0]->getShaderId(), "ortho"), 1, GL_FALSE, glm::value_ptr(ortho));
    //    handMesh.renderMesh();
    //}
};