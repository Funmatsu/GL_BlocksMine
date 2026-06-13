//Textures[2]->loadTexture();
//Textures[3]->loadTexture();
//Textures[4]->loadTexture();
//Textures[5]->loadTexture();
//Textures[6]->loadTexture();

    //Textures.push_back(new Texture("textures\\grass_block_all.png"));
    //Textures.push_back(new Texture("textures\\dirt_face.jpg"));
    //Textures.push_back(new Texturegl("textures\\diamond_ore.jpg"));
    //Textures.push_back(new Texture("textures\\block_atlas.png"));
    //Textures.push_back(new Texture("textures\\grass_stone_atlas.png"));
    //Textures.push_back(new Texture("textures\\all_blocks_atlas.jpg"));

//void createCube(float xoffset, float yoffset, float zoffset, int blockType) {
//    float x = 0, y = 1, xoffsetTop = 0, yoffsetTop = 0, xoffsetBottom = 0, yoffsetBottom = 0, transparency = 1.0f;
//    int xdimens = 3, ydimens = 3;
//
//    int arbitraryoff = 0;
//    if (blockType == 2) {
//        x = 0, y = 2; xoffsetTop = 2; yoffsetTop = 0; xoffsetBottom = 2; yoffsetBottom = 1;
//    }
//
//    if (blockType == 3) {
//        x = 1; y = 1 + arbitraryoff;
//    }
//
//    if (blockType == 4) {
//        x = 1; y = 2;
//    }
//
//    if (blockType == 5) {
//        x = 2; y = 0;
//    }
//
//    if (blockType == 6) {
//        x = 2, y = 1;
//    }
//
//    vector<unsigned int> indices1 = {
//        0 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 1 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 2 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        3 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 4 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 5 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        6 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 7 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 8 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        9 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 10 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 11 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        12 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 13 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 14 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        15 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 16 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 17 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        18 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 19 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 20 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        21 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 22 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 23 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        24 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 25 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 26 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        27 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 28 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 29 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        30 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 31 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 32 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6),
//        33 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 34 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6), 35 + (world1.world.chunks[world1.world.chunks.size() - 1].blockNum * 6 * 6)
//    };
//
//    vector<GLfloat> globalUVs =
//    {
//        (0.05f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//
//        (0.05f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//
//        (0.05f + x + xoffsetBottom) / xdimens,   (0.05f + y + yoffsetBottom) / ydimens, transparency,
//        (0.95f + x + xoffsetBottom) / xdimens,   (0.05f + y + yoffsetBottom) / ydimens, transparency,
//        (0.05f + x + xoffsetBottom) / xdimens,   (0.95f + y + yoffsetBottom) / ydimens, transparency,
//
//        (0.05f + x + xoffsetBottom) / xdimens,   (0.95f + y + yoffsetBottom) / ydimens, transparency,
//        (0.95f + x + xoffsetBottom) / xdimens,   (0.05f + y + yoffsetBottom) / ydimens, transparency,
//        (0.95f + x + xoffsetBottom) / xdimens,   (0.95f + y + yoffsetBottom) / ydimens, transparency,
//
//        (0.05f + x + xoffsetTop) / xdimens,   (0.05f + y + yoffsetTop) / ydimens, transparency,
//        (0.05f + x + xoffsetTop) / xdimens,   (0.95f + y + yoffsetTop) / ydimens, transparency,
//        (0.95f + x + xoffsetTop) / xdimens,   (0.05f + y + yoffsetTop) / ydimens, transparency,
//        (0.05f + x + xoffsetTop) / xdimens,   (0.95f + y + yoffsetTop) / ydimens, transparency,
//        (0.95f + x + xoffsetTop) / xdimens,   (0.95f + y + yoffsetTop) / ydimens, transparency,
//        (0.95f + x + xoffsetTop) / xdimens,   (0.05f + y + yoffsetTop) / ydimens, transparency,
//
//        (0.05f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//
//        (0.05f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency,
//        (0.05f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.95f + y) / ydimens, transparency,
//        (0.95f + x) / xdimens,   (0.05f + y) / ydimens, transparency
//    };
//
//    vector<GLfloat> triangle = {
//        0.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        0.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//
//        1.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//
//        0.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//
//        1.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//
//        0.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//
//        0.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 0.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 0.0f + zoffset,
//
//        0.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//        0.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 1.0f + yoffset, 1.0f + zoffset,
//        1.0f + xoffset, 0.0f + yoffset, 1.0f + zoffset,
//    };
//
//    vector<GLfloat> finalVerts;
//    for (int i = 0; i < 36; i++) {
//        finalVerts.push_back(triangle[3 * i + 0]);
//        finalVerts.push_back(triangle[3 * i + 1]);
//        finalVerts.push_back(triangle[3 * i + 2]);
//
//        finalVerts.push_back(globalUVs[3 * i + 0]);
//        finalVerts.push_back(globalUVs[3 * i + 1]);
//        finalVerts.push_back(globalUVs[3 * i + 2]);
//    }
//
//    addMeshToWorld(vec3(xoffset, yoffset, zoffset), finalVerts, indices1, finalVerts.size(), indices1.size(), blockType);
//    world1.world.chunks[world1.world.chunks.size() - 1].blockNum++;
//}


//for (int i = 0; i >= -5; i--) {
//    for (int j = 0; j <= 5; j++) {
//        chunkCoords.push_back({ int(camera.getCameraPos().x / CHUNK_SIZE) + i, int(camera.getCameraPos().z / CHUNK_SIZE) + j });
//        //generateChunkAt(chunkCoords[chunkCoords.size() - 1]);
//        {
//            chunkGenRunning = true;
//            std::lock_guard<std::mutex> lock(chunkRequestMutex);
//            chunkRequestQueue.push(chunkCoords.back());
//        }
//        {
//            std::lock_guard<std::mutex> lock(chunkResultMutex);
//            //
// << chunkResultQueue.empty() << endl;
//            while (!chunkResultQueue.empty()) {
//                Chunk chunk = std::move(chunkResultQueue.front());
//                chunkResultQueue.pop();
//                world1.world.chunks.push_back(std::move(chunk));
//            }
//        }
//    }
//}
//
//for (int i = 0; i >= -5; i--) {
//    for (int j = 0; j >= -5; j--) {
//        chunkCoords.push_back({ int(camera.getCameraPos().x / CHUNK_SIZE) + i, int(camera.getCameraPos().z / CHUNK_SIZE) + j });
//        //generateChunkAt(chunkCoords[chunkCoords.size() - 1]);
//        {
//            chunkGenRunning = true;
//            std::lock_guard<std::mutex> lock(chunkRequestMutex);
//            chunkRequestQueue.push(chunkCoords.back());
//        }
//        {
//            std::lock_guard<std::mutex> lock(chunkResultMutex);
//            //cout << chunkResultQueue.empty() << endl;
//            while (!chunkResultQueue.empty()) {
//                Chunk chunk = std::move(chunkResultQueue.front());
//                chunkResultQueue.pop();
//                world1.world.chunks.push_back(std::move(chunk));
//            }
//        }
//    }
//}
//
//for (int i = 0; i >= -5; i--) {
//    for (int j = 0; j <= 5; j++) {
//        chunkCoords.push_back({ int(camera.getCameraPos().x / CHUNK_SIZE) + i, int(camera.getCameraPos().z / CHUNK_SIZE) + j });
//        //generateChunkAt(chunkCoords[chunkCoords.size() - 1]);
//        {
//            chunkGenRunning = true;
//            std::lock_guard<std::mutex> lock(chunkRequestMutex);
//            chunkRequestQueue.push(chunkCoords.back());
//        }
//        {
//            std::lock_guard<std::mutex> lock(chunkResultMutex);
//            //cout << chunkResultQueue.empty() << endl;
//            while (!chunkResultQueue.empty()) {
//                Chunk chunk = std::move(chunkResultQueue.front());
//                chunkResultQueue.pop();
//                world1.world.chunks.push_back(std::move(chunk));
//            }
//        }
//    }
//}



        //if ((camera.getCameraPos().x / CHUNK_SIZE) > (int)(chunkCoords.back().x - 20) ||
        //    (camera.getCameraPos().z / CHUNK_SIZE) > (int)(chunkCoords.back().y - 20)
        //    )
        //{
            //int sizeChunk = chunkCoords.size();
            //for (int i = 0; i <= 20; i++) {
            //    for (int j = 0; j <= 20; j++) {
            //        if (!contains(chunkCoords, { (int)(camera.getCameraPos().x / CHUNK_SIZE) + i, (int)(camera.getCameraPos().z / CHUNK_SIZE) + j })) {
            //            chunkCoords.push_back({ int(camera.getCameraPos().x / CHUNK_SIZE) + i, int(camera.getCameraPos().z / CHUNK_SIZE) + j });
            //            //generateChunkAt(chunkCoords[chunkCoords.size() - 1]);
            //            {
            //                chunkGenRunning = true;
            //                std::lock_guard<std::mutex> lock(chunkRequestMutex);
            //                chunkRequestQueue.push(chunkCoords.back());
            //            }
            //            {
            //                std::lock_guard<std::mutex> lock(chunkResultMutex);
            //                //cout << chunkResultQueue.empty() << endl;
            //                while (!chunkResultQueue.empty()) {
            //                    Chunk chunk = std::move(chunkResultQueue.front());
            //                    chunkResultQueue.pop();
            //                    world1.world.chunks.push_back(std::move(chunk));
            //                }
            //            }
            //        }
            //    }
            //}
        //}





//void createCubeInThread(float x, float y, float z, Chunk& repChunk, Item blockType) {
//    if (blockType == AIR) return;
//    float xoffset = 0, yoffset = 1, xoffsetTop = 0, yoffsetTop = 0, xoffsetBottom = 0, yoffsetBottom = 0, transparency = 1.0f;
//
//    int arbitraryoff = 0;
//    if (blockType == AIR) {
//        xoffset = 3; yoffset = 3;
//    }
//
//    else if (blockType == GRASS_BLOCK) {
//        xoffset = 0, yoffset = 2; xoffsetTop = 2; yoffsetTop = 0; xoffsetBottom = 2; yoffsetBottom = 2;
//    }
//
//    else if (blockType == IRON_ORE) {
//        xoffset = 1; yoffset = 1 + arbitraryoff;
//    }
//
//    else if (blockType == STONE_BLOCK) {
//        xoffset = 1; yoffset = 2;
//    }
//
//    else if (blockType == DIRT_BLOCK) {
//        xoffset = 2; yoffset = 0;
//    }
//
//    else if (blockType == OAK_WOOD) {
//        xoffset = 2, yoffset = 1; xoffsetTop = -2; yoffsetTop = 2; xoffsetBottom = -2; yoffsetBottom = 2;
//    }
//
//    else if (blockType == CLOUD) {
//        xoffset = 1, yoffset = 0, transparency = 0.25f;
//    }
//
//    else if (blockType == OAK_PLANK) {
//        xoffset = 3, yoffset = 0;
//    }
//
//    else if (blockType == OAK_LEAVES) {
//        xoffset = 0, yoffset = 0;
//    }
//
//    else if (blockType == GRASS) {
//        xoffset = 3, yoffset = 1; xoffsetTop = 0, xoffsetBottom = 0; yoffsetTop = 1, yoffsetBottom = 1;
//    }
//
//    else if (blockType == POPPY) {
//        xoffset = 3, yoffset = 2; xoffsetTop = 0, xoffsetBottom = 0; yoffsetTop = 1, yoffsetBottom = 1;
//    }
//
//    else if (blockType == BLUE_ORCHID) {
//        xoffset = 0, yoffset = 4;
//    }
//
//    else if (blockType == BEDROCK) {
//        xoffset = 4, yoffset = 0;
//    }
//
//    float clipX = 0.03f, clipY = 0.97f;
//    int num_of_faces = 6, final_face_num = 6;
//
//    unsigned int indexOffset = repChunk.indexOffset;
//    //cout << indexOffset << endl;
//    //for (int i = 0; i < repChunk.blocks.size(); i++) {
//        //cout << "conputer index offset!" << endl;
//    //}
//    vector<unsigned int> indices;
//    indices.reserve(36);
//
//    for (int i = 0; i < 36; i++) {
//        indices.push_back(i + indexOffset);
//    }
//
//    vector<unsigned int> blockIndices;
//    blockIndices.reserve(36);
//
//    for (int i = 0; i < 36; i++) {
//        blockIndices.push_back(i);
//    }
//
//    vector<GLfloat> globalUVs;
//    globalUVs.reserve(36);
//
//    for (int i = 0; i < 6; i++) {
//        int offsetX = 0, offsetY = 0;
//        if (i == 4) { offsetX = xoffsetTop; offsetY = yoffsetTop; }
//        else if (i == 5) { offsetX = xoffsetBottom;  offsetY = yoffsetTop; }
//        globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//    }
//
//    vector<GLfloat> triangle = {
//        0.0f + x, 0.0f + y, 0.0f + z,
//        0.0f + x, 1.0f + y, 0.0f + z,
//        0.0f + x, 0.0f + y, 1.0f + z,
//        0.0f + x, 1.0f + y, 0.0f + z,
//        0.0f + x, 1.0f + y, 1.0f + z,
//        0.0f + x, 0.0f + y, 1.0f + z,
//
//        1.0f + x, 0.0f + y, 0.0f + z,
//        1.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 0.0f + y, 1.0f + z,
//        1.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 1.0f + y, 1.0f + z,
//        1.0f + x, 0.0f + y, 1.0f + z,
//
//        0.0f + x, 0.0f + y, 0.0f + z,
//        0.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 0.0f + y, 0.0f + z,
//        0.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 0.0f + y, 0.0f + z,
//
//        0.0f + x, 0.0f + y, 1.0f + z,
//        0.0f + x, 1.0f + y, 1.0f + z,
//        1.0f + x, 0.0f + y, 1.0f + z,
//        0.0f + x, 1.0f + y, 1.0f + z,
//        1.0f + x, 1.0f + y, 1.0f + z,
//        1.0f + x, 0.0f + y, 1.0f + z,
//
//        0.0f + x, 0.0f + y, 0.0f + z,
//        1.0f + x, 0.0f + y, 0.0f + z,
//        0.0f + x, 0.0f + y, 1.0f + z,
//        1.0f + x, 0.0f + y, 0.0f + z,
//        1.0f + x, 0.0f + y, 1.0f + z,
//        0.0f + x, 0.0f + y, 1.0f + z,
//
//        0.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 1.0f + y, 0.0f + z,
//        0.0f + x, 1.0f + y, 1.0f + z,
//        1.0f + x, 1.0f + y, 0.0f + z,
//        1.0f + x, 1.0f + y, 1.0f + z,
//        0.0f + x, 1.0f + y, 1.0f + z,
//    };
//    vector<GLfloat> normals;
//
//    for (int i = 0; i < 36 * 3; i++) {
//        normals.push_back(0.0f);
//    }
//    if (blockType == GRASS || blockType == POPPY) {
//        indices.clear();
//        for (int i = 0; i < 12; i++) {
//            indices.push_back(i + indexOffset);
//        }
//
//        blockIndices.clear();
//        for (int i = 0; i < 12; i++) {
//            blockIndices.push_back(i);
//        }
//
//        triangle = {
//            0.0f + x, 0.0f + y, 0.0f + z,
//            0.0f + x, 1.0f + y, 0.0f + z,
//            1.0f + x, 0.0f + y, 1.0f + z,
//            0.0f + x, 1.0f + y, 0.0f + z,
//            1.0f + x, 1.0f + y, 1.0f + z,
//            1.0f + x, 0.0f + y, 1.0f + z,
//
//            0.0f + x, 0.0f + y, 1.0f + z,
//            0.0f + x, 1.0f + y, 1.0f + z,
//            1.0f + x, 0.0f + y, 0.0f + z,
//            0.0f + x, 1.0f + y, 1.0f + z,
//            1.0f + x, 1.0f + y, 0.0f + z,
//            1.0f + x, 0.0f + y, 0.0f + z,
//        };
//
//        globalUVs.clear();
//        for (int i = 0; i < 2; i++) {
//            int offsetX = 0, offsetY = 0;
//            globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//            globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//            globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//            globalUVs.push_back((clipX + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//            globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipY + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//            globalUVs.push_back((clipY + xoffset + offsetX) / xdimens); globalUVs.push_back((clipX + yoffset + offsetY) / ydimens); globalUVs.push_back(transparency);
//        }
//
//        for (int i = 0; i < 12 * 3; i++) {
//            normals.push_back(0.0f);
//        }
//    }
//
//    vector<GLfloat> finalVerts;
//    for (int i = 0; i < triangle.size() / 3; i++) {
//        finalVerts.push_back(triangle[3 * i + 0]);
//        finalVerts.push_back(triangle[3 * i + 1]);
//        finalVerts.push_back(triangle[3 * i + 2]);
//
//        finalVerts.push_back(globalUVs[3 * i + 0]);
//        finalVerts.push_back(globalUVs[3 * i + 1]);
//        finalVerts.push_back(globalUVs[3 * i + 2]);
//
//        finalVerts.push_back(normals[3 * i + 0]);
//        finalVerts.push_back(normals[3 * i + 1]);
//        finalVerts.push_back(normals[3 * i + 2]);
//    }
//    calcAverageNormals(finalVerts, indices, 9, 6, indexOffset);
//    repChunk.indexOffset += indices.size();
//    //cout << finalVerts[6] << endl;
//    repChunk.vertices.insert(repChunk.vertices.end(), finalVerts.begin(), finalVerts.end());
//    repChunk.indices.insert(repChunk.indices.end(), indices.begin(), indices.end());
//
//    repChunk.needUpdate = true;
//
//    Block newBlock(vec3(x, y, z), blockType, finalVerts, blockIndices);
//    //newBlock.blockMesh.createMesh(finalVerts, indices, finalVerts.size(), indices.size());
//    repChunk.addBlock(newBlock);
//    //cout << newBlock.position.x << " " << newBlock.position.y << " " << newBlock.position.z << endl;
//    //repChunk.blockNum++;
//}