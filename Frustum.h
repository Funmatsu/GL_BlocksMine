#pragma once
//#include "Chunk.h"

struct Plane {
    glm::vec3 n;
    float d; 
}; // plane: n.x * X + n.y * Y + n.z * Z + d = 0

Plane planesObj[6];
//vec4 planes[6];

void extractFrustumPlanes(const glm::mat4& m)
{
    // Left
    planesObj[0].n = glm::vec3(m[0][3] + m[0][0],
                            m[1][3] + m[1][0],
                            m[2][3] + m[2][0]);
    planesObj[0].d = m[3][3] + m[3][0];

    // Right
    planesObj[1].n = glm::vec3(m[0][3] - m[0][0],
                            m[1][3] - m[1][0],
                            m[2][3] - m[2][0]);
    planesObj[1].d = m[3][3] - m[3][0];

    // Bottom
    planesObj[2].n = glm::vec3(m[0][3] + m[0][1],
                            m[1][3] + m[1][1],
                            m[2][3] + m[2][1]);
    planesObj[2].d = m[3][3] + m[3][1];

    // Top
    planesObj[3].n = glm::vec3(m[0][3] - m[0][1],
                            m[1][3] - m[1][1],
                            m[2][3] - m[2][1]);
    planesObj[3].d = m[3][3] - m[3][1];

    // Near
    planesObj[4].n = glm::vec3(m[0][3] + m[0][2],
                            m[1][3] + m[1][2],
                            m[2][3] + m[2][2]);
    planesObj[4].d = m[3][3] + m[3][2];

    // Far
    planesObj[5].n = glm::vec3(m[0][3] - m[0][2],
                            m[1][3] - m[1][2],
                            m[2][3] - m[2][2]);
    planesObj[5].d = m[3][3] - m[3][2];

    // Normalize
    for (int i = 0; i < 6; i++) {
        float len = glm::length(planesObj[i].n);
        planesObj[i].n /= len;
        planesObj[i].d /= len;
    }
}

void extractFrustumPlanes(const glm::mat4& m, vec4* planes)
{
    // Left
    planes[0] = glm::vec4(m[0][3] + m[0][0],
                          m[1][3] + m[1][0],
                          m[2][3] + m[2][0],
                          m[3][3] + m[3][0]);

    // Right
    planes[1] = glm::vec4(m[0][3] - m[0][0],
                          m[1][3] - m[1][0],
                          m[2][3] - m[2][0],
                          m[3][3] - m[3][0]);

    // Bottom
    planes[2] = glm::vec4(m[0][3] + m[0][1],
                          m[1][3] + m[1][1],
                          m[2][3] + m[2][1],
                          m[3][3] + m[3][1]);

    // Top
    planes[3] = glm::vec4(m[0][3] - m[0][1],
                          m[1][3] - m[1][1],
                          m[2][3] - m[2][1],
                          m[3][3] - m[3][1]);

    // Near
    planes[4] = glm::vec4(m[0][3] + m[0][2],
                          m[1][3] + m[1][2],
                          m[2][3] + m[2][2],
                          m[3][3] + m[3][2]);

    // Far
    planes[5] = glm::vec4(m[0][3] - m[0][2],
                          m[1][3] - m[1][2],
                          m[2][3] - m[2][2],
                          m[3][3] - m[3][2]);

    // Normalize
    for (int i = 0; i < 6; i++) {
        float len = glm::length(vec3(planes[i]));
        planes[i] /= len;
    }
}


//void extractFrustumPlanes(const glm::mat4& VP) { // Left 
//    planes[0].n.x = VP[0][3] + VP[0][0]; 
//    planes[0].n.y = VP[1][3] + VP[1][0]; 
//    planes[0].n.z = VP[2][3] + VP[2][0]; 
//    planes[0].d = VP[3][3] + VP[3][0]; // Right 
//    planes[1].n.x = VP[0][3] - VP[0][0]; 
//    planes[1].n.y = VP[1][3] - VP[1][0]; 
//    planes[1].n.z = VP[2][3] - VP[2][0]; 
//    planes[1].d = VP[3][3] - VP[3][0]; // Bottom 
//    planes[2].n.x = VP[0][3] + VP[0][1]; 
//    planes[2].n.y = VP[1][3] + VP[1][1]; 
//    planes[2].n.z = VP[2][3] + VP[2][1]; 
//    planes[2].d = VP[3][3] + VP[3][1]; // Top 
//    planes[3].n.x = VP[0][3] - VP[0][1]; 
//    planes[3].n.y = VP[1][3] - VP[1][1]; 
//    planes[3].n.z = VP[2][3] - VP[2][1]; 
//    planes[3].d = VP[3][3] - VP[3][1]; // Near 
//    planes[4].n.x = VP[0][3] + VP[0][2];
//    planes[4].n.y = VP[1][3] + VP[1][2]; 
//    planes[4].n.z = VP[2][3] + VP[2][2]; 
//    planes[4].d = VP[3][3] + VP[3][2]; // Far 
//    planes[5].n.x = VP[0][3] - VP[0][2]; 
//    planes[5].n.y = VP[1][3] - VP[1][2]; 
//    planes[5].n.z = VP[2][3] - VP[2][2]; 
//    planes[5].d = VP[3][3] - VP[3][2]; // Normalize 
//    for (int i = 0; i < 6; i++) { 
//        float len = glm::length(planes[i].n);
//        planes[i].n /= len; planes[i].d /= len; 
//    }
//}

bool sphereInFrustum(const glm::vec3& center, float radius) {
    for (int i = 0; i < 6; i++) {
        float dist = glm::dot(planesObj[i].n, center) + planesObj[i].d;
        if (dist < -(radius + 1))
            return false; // completely outside
    }
    return true;
}

bool aabbIntersectsFrustum(glm::vec3& min, glm::vec3& max) {
    for (int p = 0; p < 6; ++p) {
        // compute positive vertex (vertex most in direction of plane normal)
        glm::vec3 positive;
        positive.x = (planesObj[p].n.x >= 0.0f) ? max.x : min.x;
        positive.y = (planesObj[p].n.y >= 0.0f) ? max.y : min.y;
        positive.z = (planesObj[p].n.z >= 0.0f) ? max.z : min.z;

        // if positive vertex is behind plane, entire box is behind that plane -> culled
        if (glm::dot(planesObj[p].n, positive) + planesObj[p].d < 0.0f) return false;
    }
    return true; // intersects or is fully inside
}

//struct Frustum {
//    glm::vec4 planes[6]; // Each plane: ax + by + cz + d = 0
//};
//
//Frustum extractFrustumPlanes(const glm::mat4& vp) {
//    Frustum f;
//
//    f.planes[0] = glm::vec4(vp[0][3] + vp[0][0], vp[1][3] + vp[1][0], vp[2][3] + vp[2][0], vp[3][3] + vp[3][0]); // Left
//    f.planes[1] = glm::vec4(vp[0][3] - vp[0][0], vp[1][3] - vp[1][0], vp[2][3] - vp[2][0], vp[3][3] - vp[3][0]); // Right
//    f.planes[2] = glm::vec4(vp[0][3] + vp[0][1], vp[1][3] + vp[1][1], vp[2][3] + vp[2][1], vp[3][3] + vp[3][1]); // Bottom
//    f.planes[3] = glm::vec4(vp[0][3] - vp[0][1], vp[1][3] - vp[1][1], vp[2][3] - vp[2][1], vp[3][3] - vp[3][1]); // Top
//    f.planes[4] = glm::vec4(vp[0][3] + vp[0][2], vp[1][3] + vp[1][2], vp[2][3] + vp[2][2], vp[3][3] + vp[3][2]); // Near
//    f.planes[5] = glm::vec4(vp[0][3] - vp[0][2], vp[1][3] - vp[1][2], vp[2][3] - vp[2][2], vp[3][3] - vp[3][2]); // Far
//
//    return f;
//}
//
//bool isChunkVisible(const Frustum& f, const glm::vec2& chunkPos) {
//    glm::vec3 min(chunkPos.x * CHUNK_SIZE, 0.0f, chunkPos.y * CHUNK_SIZE);
//    glm::vec3 max((chunkPos.x + 1) * CHUNK_SIZE, CHUNK_SIZE * CHUNK_SIZE, (chunkPos.y + 1) * CHUNK_SIZE);
//
//
//    for (int i = 0; i < 6; ++i) {
//        glm::vec4 plane = f.planes[i];
//        plane /= glm::length(glm::vec3(plane));
//
//        // Find the most positive vertex (farthest in direction of plane normal)
//        glm::vec3 p = glm::vec3(
//            (plane.x < 0) ? min.x : max.x,
//            (plane.y < 0) ? min.y : max.y,
//            (plane.z < 0) ? min.z : max.z
//        );
//
//        // If that vertex is outside the plane, chunk is outside
//        if (plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w < 0)
//            return false;
//    }
//
//    return true;
//}