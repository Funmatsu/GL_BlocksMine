#include "LightMesh.h"

LightMesh::LightMesh() {
    meshAttrs = make_shared<MeshAttrContainer>();
    //refcnt = 0;
}

void LightMesh::createMesh(vector<float>& verts, vector<unsigned int>& inds) {
    unsigned int verticesCount = verts.size(), indicesCount = inds.size();
	if (verticesCount == 0 || indicesCount == 0) return;
    unsigned int& vao = meshAttrs->vao, &vbo = meshAttrs->vbo, &ibo = meshAttrs->ibo, &indexCount = meshAttrs->indexCount;
    indexCount = indicesCount;
    if (indexCount != 0) {
        if (vao == 0)
            glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        if (ibo == 0)
            glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * inds.size(), inds.data(), GL_STATIC_DRAW);

        if (vbo == 0)
            glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);                          //Position

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); //2D uv

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float))); //Packed texture offset

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    }
}

void LightMesh::createMesh(float* vertsdata, unsigned int* indsdata, size_t indsize, size_t vertsize) {
    if (!vertsdata || !indsdata) return;
    unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    unsigned int verticesCount = vertsize, indicesCount = indsize;
    indexCount = indicesCount;
    if (indexCount != 0) {
        if (vao == 0)
            glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        if (ibo == 0)
            glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, indsdata, GL_STATIC_DRAW);

        if (vbo == 0)
            glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesCount, vertsdata, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);                          //Position

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); //2D uv

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float))); //Packed texture offset

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    }
}

void LightMesh::createMesh(vector<float>& verts, vector<unsigned int>& inds, unsigned int verticesCount, unsigned int indicesCount) {
    unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    indexCount = indicesCount;
    if (indexCount != 0) {
        if (vao == 0)
            glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        if (ibo == 0)
            glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * inds.size(), inds.data(), GL_STATIC_DRAW);

        if (vbo == 0)
            glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(5 * sizeof(float)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    }
}

void LightMesh::renderMesh() {
    unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (indexCount != 0) {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void LightMesh::renderMeshAsLines() {
    unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    if (indexCount != 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(10.0f);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void LightMesh::clearMesh() {
	meshAttrs->clearMesh();
    //unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    //if (ibo != 0) {
    //    glDeleteBuffers(1, &ibo);
    //    ibo = 0;
    //}
    //if (vbo != 0) {
    //    glDeleteBuffers(1, &vbo);
    //    vbo = 0;
    //}
    //if (vao != 0) {
    //    glDeleteVertexArrays(1, &vao);
    //    vao = 0;
    //}
    //indexCount = 0;
}

void LightMesh::giveMesh() {
    unsigned int& vao = meshAttrs->vao, & vbo = meshAttrs->vbo, & ibo = meshAttrs->ibo, & indexCount = meshAttrs->indexCount;
    if (ibo != 0) {
        ibo = 0;
    }
    if (vbo != 0) {
        vbo = 0;
    }
    if (vao != 0) {
        vao = 0;
    }
    indexCount = 0;
}

///*void LightMesh::incRefcount() {
//    refcnt++;
//}
//
//void LightMesh::decRefcount() {
//    refcnt--;
//}*/

//LightMesh::~LightMesh() {
//    //decRefcount();
//	//cout << "deleting mesh with vao : " << vao << " vbo : " << vbo << " ibo : " << ibo << endl;
//    //if (refcnt <= 0) { clearMesh(); refcnt = 0; }
//    clearMesh();
//}