#pragma once

vector<GLfloat> long_normals = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,
        0.0f, 0.0f,-1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f,
        0.0f,-1.0f, 0.0f
};

vector<GLfloat> normalsLong[6] = {
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    },
    {
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f
    },
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    },
    {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    },
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    },
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    }
};

vector<GLfloat> normalsShort[6] = {
    {
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f
    },
    {
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f
    },
};

void calcAverageNormals(vector<GLfloat>& vertices, vector<unsigned int> indices, int vLength, int normalOffset, int indOffset) {
    for (int i = 0; i < indices.size(); i += 3) {
        unsigned int ind0 = (indices[i + 0] - indOffset) * vLength;
        unsigned int ind1 = (indices[i + 1] - indOffset) * vLength;
        unsigned int ind2 = (indices[i + 2] - indOffset) * vLength;
        vec3 v1(vertices[ind1] - vertices[ind0], vertices[ind1 + 1] - vertices[ind0 + 1], vertices[ind1 + 2] - vertices[ind0 + 2]);
        vec3 v2(vertices[ind2] - vertices[ind0], vertices[ind2 + 1] - vertices[ind0 + 1], vertices[ind2 + 2] - vertices[ind0 + 2]);
        vec3 normal = cross(v1, v2);
        normal = normalize(normal);

        ind0 += normalOffset; ind1 += normalOffset; ind2 += normalOffset;


        vertices[ind0] = normal.x; vertices[ind0 + 1] = normal.y; vertices[ind0 + 2] = normal.z;
        vertices[ind1] = normal.x; vertices[ind1 + 1] = normal.y; vertices[ind1 + 2] = normal.z;
        vertices[ind2] = normal.x; vertices[ind2 + 1] = normal.y; vertices[ind2 + 2] = normal.z;

    }

    //for (int i = 0; i < vertices.size() / vLength; i++) {
    //    unsigned int nOffset = i * (vLength)+normalOffset;
    //    vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
    //    vec = normalize(vec);
    //    vertices[nOffset] = vec.x, vertices[nOffset + 1] = vec.y, vertices[nOffset + 2] = vec.z;
    //}
}