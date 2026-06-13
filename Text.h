#pragma once
#include "libraries.h"

inline const char LETTERS[] = {
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7' , '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X' , 'Y', 'Z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' , 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x' , 'y', 'z', '{', '|', '}', '~', '\0'
};

inline int letter_seps[] = {
    3, 1, 3, 5, 5, 5, 5, 1, 3, 3, 3, 5, 1, 5, 1, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 4, 5, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    2, 5, 5, 5, 5, 5, 4, 5, 5, 1, 5, 5, 2, 5, 5, 5,
    5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 3, 1, 3, 6, 5,
};

inline vec2 getTextUvs(char c) {
    int index;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++)
            if (LETTERS[i * 16 + j] == c) {
                index = i * 16 + j;
                //cout << c << " char " << i << " i " << j << " j " << endl;
                return vec2(j, 13 - i);
            }
    }
    return vec2(-1);
}

inline LightMesh createTextQuad(vec2 pos, char c, float scale = 1) {
    float yexponent = 1.5f;
    vector<GLfloat> vertices;
    vector<GLfloat> globalUVs;
    vector<unsigned int> indices;
    vector<GLfloat> normals;
    vector<GLfloat> colorMask;

    vec2 quadUvs = getTextUvs(c);

    float clipX = quadUvs.x, clipY = quadUvs.y;
    float u0 = clipX / 16.f, v0 = clipY / 16.f;
    float u1 = u0 + 1.f / 16.f, v1 = v0 + 1.f / 16.f;

    indices = { 0, 1, 2,
                2, 3, 0 };

    globalUVs.push_back(u0); globalUVs.push_back(v0); globalUVs.push_back(1);
    globalUVs.push_back(u0); globalUVs.push_back(v1); globalUVs.push_back(1);
    globalUVs.push_back(u1); globalUVs.push_back(v1); globalUVs.push_back(1);
    globalUVs.push_back(u1); globalUVs.push_back(v0); globalUVs.push_back(1);

    vertices = {
        -0.5f * (scale)+pos.x,  -0.5f * scale * yexponent + pos.y,  -1.0f,
        -0.5f * (scale)+pos.x,   0.5f * scale * yexponent + pos.y,  -1.0f,
         0.5f * (scale)+pos.x,   0.5f * scale * yexponent + pos.y,  -1.0f,
         0.5f * (scale)+pos.x,  -0.5f * scale * yexponent + pos.y,  -1.0f,
    };

    normals.assign(vertices.size(), 0);
    colorMask.assign(vertices.size(), 1);

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

inline void createTextQuad(Mesh& m, string str, float height, vec3 color = vec3(1), float text_scale = 1) {
    float yexponent = 1.5f;
    vec2 pos = vec2(0);
    for (char c : str) {
        vec2 quadUvs = getTextUvs(c);
        int spaceIdx = (int)(13 - quadUvs.y) * 16 + (int)quadUvs.x;
        int spacing = letter_seps[spaceIdx] * 5;
        float clipX = quadUvs.x, clipY = quadUvs.y;
        float u0 = clipX / 16.f, v0 = clipY / 16.f;
        float u1 = u0 + 1.f / 16.f, v1 = v0 + 1.f / 16.f;

        vector<float> verts = {
            -0.5f * (text_scale)+pos.x,  -0.5f * text_scale * yexponent + pos.y + height,  -1.0f,  u0, v0, 1, 0, 0, 0, color.x, color.y, color.z,
            -0.5f * (text_scale)+pos.x,   0.5f * text_scale * yexponent + pos.y + height,  -1.0f,  u0, v1, 1, 0, 0, 0, color.x, color.y, color.z,
             0.5f * (text_scale)+pos.x,   0.5f * text_scale * yexponent + pos.y + height,  -1.0f,  u1, v1, 1, 0, 0, 0, color.x, color.y, color.z,
             0.5f * (text_scale)+pos.x,  -0.5f * text_scale * yexponent + pos.y + height,  -1.0f,  u1, v0, 1, 0, 0, 0, color.x, color.y, color.z,
        };

        m.vertices.insert(m.vertices.end(), verts.begin(), verts.end());

        pos += vec2(spacing*0.7, 0);
    }

    int quadCount = m.vertices.size() / 48;

    for (int i = 0; i < quadCount; i++) {
        int base = i * 4;
        m.indices.push_back(base + 0); m.indices.push_back(base + 1); m.indices.push_back(base + 2);
        m.indices.push_back(base + 2); m.indices.push_back(base + 3); m.indices.push_back(base + 0);
    }
}

struct Text
{
    LightMesh letters;
    mat4 model = mat4(1.0f);

    Text() {}

    Text(string word) {
        model = translate(mat4(1.0f), vec3(900, 900, 0));
        defineWord(word, vec3(1));
    }

    Text(int num) {
        string words = to_string(num);
        model = translate(mat4(1.0f), vec3(900, 900, 0));
        defineWord(words, vec3(1));
    }

    Text(string word, vec3 color) {
        model = translate(mat4(1.0f), vec3(900, 900, 0));
        defineWord(word, color);
    }

    Text(int num, vec3 color) {
        string words = to_string(num);
        model = translate(mat4(1.0f), vec3(900, 900, 0));
        defineWord(words, color);
    }

    Text(string word, vec3 color, vec2 position) {
        model = translate(mat4(1.0f), vec3(position, 0));
        defineWord(word, color);
    }

    Text(int num, vec3 color, vec2 position) {
        string words = to_string(num);
        model = translate(mat4(1.0f), vec3(position, 0));
        defineWord(words, color);
    }

    void deleteWord() {
        letters.clearMesh();
    }

    void replaceWord(string word, vec3 color, string delim = "\n", float size = 25) {
        //deleteWord();
        stringstream ss(word);
        string line;
        float height = 0;
        Mesh linemesh;
        while (getline(ss, line)) {
            createTextQuad(linemesh, line, height, color, size);
            height -= 50;
        }
        letters.createMesh(linemesh.vertices, linemesh.indices, linemesh.vertices.size(), linemesh.indices.size());
    }

    //void replaceWord(string word, vec3 color, float size = 25) {
    //    deleteWord();
    //    defineWord(word, color, size);
    //}

    void replaceWord(int num, vec3 color, float size = 25) {
        //deleteWord();
        defineWord(to_string(num), color, size);
    }

    void replaceWord(string word, vec3 color, vec2 position, float size = 25) {
        //deleteWord();
        defineWord(word, color, size);
        model = translate(mat4(1), vec3(position.x, position.y, 0));
    }

    void replaceWord(int num, vec3 color, vec2 position, float size = 25) {
        //deleteWord();
        defineWord(to_string(num), color, size);
        model = translate(mat4(1), vec3(position.x, position.y, 0));
    }

    void defineWord(string word, vec3 color, float size = 25) {
        Mesh m;
        createTextQuad(m, word, 0, color, size);
        letters.createMesh(m.vertices, m.indices, m.vertices.size(), m.indices.size());
    }

    void drawText(mat4 ortho) {
        bool depthEnabled = glIsEnabled(GL_DEPTH_TEST);
        glDisable(GL_DEPTH_TEST); // so crosshair draws on top
        shaders[4]->useShader();
        glUniformMatrix4fv(shaders[4]->getOrthoLocation(), 1, GL_FALSE, value_ptr(ortho));
        glUniformMatrix4fv(shaders[4]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
        Textures[TEXT_TEX]->useTexture();
        //glClearColor(0, 1, 0, 1);
        letters.renderMesh();
        //if (depthEnabled) glEnable(GL_DEPTH_TEST);  
    }

    void drawonly() {
        glDisable(GL_DEPTH_TEST);
        glUniformMatrix4fv(shaders[4]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
        Textures[TEXT_TEX]->useTexture();
        letters.renderMesh();
    }
};