#pragma once

//(DONE!!!) TO DO : Implement spectator mode kind of face culling;;;; Block right next to block of Air can be rendered. 

class Game;

vector<int> M(20);
std::vector<glm::ivec2> spiral, cloudSpir;

//void mainKeyCallback(int key, int code, int action, int mode);

class Game {
public:
    vector<vec4> chCenter_Radii;
    //vector<uint> chVis;
	vec4 planes[6];
    vector<LightMesh*> visChMeshes;
    vector<uint> visbuffer;
    GLsync sync = 0;

    bool handGesture = 0, 
         swing = 0,
         night = false,
         tp = 0,
         waitChDraw = false;

    GLfloat deltaTime = 2.0f,
            lastTime = 0.0f;

    float breakAngle = 0.0f,
          boundW = 0.5f, boundL = 0.5f, boundH = 2.0f,
          projAngle = 45.0,
          lastXChange = 0.0f, lastYChange = 0.0f,
          lastYaw = 0.0f, lastPitch = 0.0f,
          time = 300.0f, lowTime = 10.0f, maxTime = 1000.0,
          dt = 0.01;

    int renderDistance = 25,
        chunkRenderMonitor = 0,
        fpscount = 0,
        breaking = 0,
        spawn = 1,
        person_view = 0;
    int renderX = 0, renderY = 0,
        bufferWidth, bufferHeight;

    std::vector<std::thread> workers;

    LightMesh headMesh,
              lookingMesh,
              compassMesh;

    mat4 model, projection, view,
         ortho,
         itemProj, itemView, currentBlockView,
         itemModel,
         breakModel,
         VP;

    Projectile ball;

    abyte count = 0, count_time = 0;

    Crosshair crosshair;

    Block breakingBlock;
    
    Text position, craftedItemName, cursorPos;

    json Jitems, tools, player;

    chrono::steady_clock::time_point start;

    Block lookBlock;

	vec3 headPos, headFront;

    Game() {
        ifstream playerJSON("player.json");
        if (!playerJSON) {
            ofstream outJSON("player.json");
            json emptyPlayer;
            emptyPlayer["player"]["x"] = 0;
            emptyPlayer["player"]["y"] = 104;
            emptyPlayer["player"]["z"] = 0;
            outJSON << emptyPlayer.dump(4);
            outJSON.close();
            playerJSON.open("player.json");
        }
        playerJSON >> player;

        ifstream itemsJSON("items.json");
        if (!itemsJSON) {
            ofstream outJSON("items.json");
            outJSON.close();
            itemsJSON.open("items.json"); 
        }
        itemsJSON >> Jitems;

        ifstream toolsJSON("tools.json");
        if (!toolsJSON) {
            ofstream outJSON("tools.json");
            outJSON.close();
            toolsJSON.open("tools.json");
        }
        toolsJSON >> tools;
        firstCamera.setPosition(vec3(player["player"]["x"], player["player"]["y"], player["player"]["z"]));
        //mainWindow = Window(WIDTH, HEIGHT);
        //mainWindow.initialize();
        bufferWidth = mainWindow.getBufferWidth();
        bufferHeight = mainWindow.getBufferHeight();
        //mainWindow.setHandleKeyClicks(mainKeyCallback);

        mainWindow.disableMouse();
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_BLEND);

        //createShaders();
        //addTextures();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);

        json jsondata;
        ifstream ifs("config.json");
        ifs >> jsondata;
        renderDistance = jsondata["renderdistance"];

        crosshair.defineCrosshairGeometry();
        inventory.defineMainInventoryGeometry();
        inventory.defineHotbarGeometry();
        headMesh = world.createMeshCube(vec3(0), -2.5f, CLOUD);
        lookingMesh = world.createVertsOnlyMesh(vec3(0), 1.01f, CLOUD);
        compassMesh = createCompassVertsOnlyMesh(vec3(0));

        initChunksNoise();
        generateSpiral(spiral);
        dropped.push_back(Projectile());

        projection = perspective(radians(projAngle), (float(mainWindow.getBufferWidth()) / float(mainWindow.getBufferHeight())), 0.01f, float(renderDistance * CHUNK_SIZE));

        ortho = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT));

        itemProj = perspective(radians(1.0f), 1.0f, 0.01f, 1500.0f),
        itemView = lookAt(vec3(0, 0, 400), vec3(0), vec3(0, 1, 0)),
        currentBlockView = lookAt(vec3(0, 0, 1400), vec3(0), vec3(0, 1, 0));

        itemModel = scale(mat4(1.0f), vec3(0.08f, 0.1f, 0.08f)) *
                    rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1)) *
                    rotate(mat4(1.0f), radians( 30.0f), vec3(1, 0, 0)) *
                    rotate(mat4(1.0f), radians( 45.0f), vec3(0, 1, 0))
                    ;

        breakModel = mat4(1.0f);

        inventory.initInventorySlots();
        sky.buildSky();

        for (int i = 0; i < 3; ++i) {
            workers.push_back(thread(updateChunkJob));
            if (i < 2)
                workers.push_back(thread(chunkWorker)); // worker thread is somewhere in threading.h
        }

        //workers.push_back(thread(updateNeighbourJob));

        mainLight = DirectionalLight(mainWindow.getBufferWidth(), mainWindow.getBufferHeight(),
            1.0f, 1.0f, 1.0f,
            0.8f, 0.5f,
            0.0f, -CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, CHUNK_SIZE);//CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, CHUNK_SIZE

        auxLight = DirectionalLight(100 , 100 ,
                                    1.0f, 1.0f, 1.0f,
                                    0.7f, 0.5f,
                                   -1.0f, 1.5f, 0.0f);

        position.model = translate(mat4(1.0f), vec3(50, 1700, 0));

        mainLight.setShadowPos(firstCamera.getPosition());
		//computeShader->creatBuffers(CHUNK_HEIGHT * CHUNK_SIZE);

        glClearColor(1, 1, 1, 0);
    }

    void run() {
        while (!mainWindow.getShouldClose()) {
            //glBindVertexArray(item_vao);
            auto startframe = chrono::high_resolution_clock::now();
            static bool breakblockdb = 0, placeblockdb = 0, invtoggledb = 0; // db = debounce
            static int angletest = 0;
            static double frame_duration_calc;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();

            sky.applySky(view, projection);//
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            start = chrono::high_resolution_clock::now();
            if (inventory.mainInventoryOn || inventory.craftingTableInventoryOn) handleInvSlotClicks();
            cursor.x = mainWindow.getXPos();
            cursor.y = mainWindow.getYPos();

            lastYaw = firstCamera.getYaw(); lastPitch = firstCamera.getPitch();
            thirdCamera_back.setPosition(firstCamera.getPosition() + firstCamera.getFront() * vec3(-5));
            if ((int)abs(firstCamera.getPosition().x) > (int)abs(mainLight.getShadowPos().x) + 10 ||
                (int)abs(firstCamera.getPosition().z) > (int)abs(mainLight.getShadowPos().z) + 10) {
                mainLight.setShadowPos(firstCamera.getPosition());
            }
            if (person_view % 3 != 2)
                activeCamera.setFront(firstCamera.getFront());

            for (auto chunkOff : spiral) {
                ivec2 camChunkPos = ivec2(floorDiv(firstCamera.getPosition().x, CHUNK_SIZE), floorDiv(firstCamera.getPosition().z, CHUNK_SIZE));
                ivec2 chunkPos = camChunkPos + chunkOff;
                if (chunkCoords.count(to(chunkPos)) <= 0) {
                    chunkCoords.insert(to(chunkPos));
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        chunkRequestQueue.push(chunkPos);
                        queueCV.notify_one();
                    }
                    count++;
                    if (!(count % 8)) { break; }
                }
            }

            while (!chunkMeshResult.empty()) {
                chNeighResult chNeighRes;
                {
                    std::lock_guard<std::mutex> lock(chunkMeshQueueMutex);
                    chNeighRes = move(chunkMeshResult.front());
                    chunkMeshResult.pop();                    
                }
                if (!world.chunkData.count(chNeighRes.coords)) continue;
                world.chunkData[chNeighRes.coords]->mesh->createMesh(chNeighRes.mesh->vertices, chNeighRes.mesh->indices);

                if(!(count++ % 3)) break;
            }

            while (!chunkResultQueue.empty()) {
                //cout << " yayyyay!" << endl;
                pair<unique_ptr<Chunk>, ivec2> ch;
                {
                    lock_guard<mutex> lock(addChunkMutex);
                    auto& it = chunkResultQueue.front();
                    ch.first = move(it.first); ch.second = it.second;
                    chunkResultQueue.pop();
                }
                world.chunkData.emplace(toCoords(ch.second), move(ch.first));
                if (!(count++ % 9)) break;
            }

///*            while (!chunkMeshQueue.empty()) {
//
//                {
//                    std::lock_guard<std::mutex> lock(chunkMeshQueueMutex);
//                    auto& [m, chmesh/*, crd*/] = chunkMeshQueue.front();
//                    //ivec2 crd = ch->coords();
//
//                    chmesh->createMesh(m->vertices, m->indices);
//                    //auto& m = world.chunkData[pack(crd)]->mesh;
//
//                    //vec4 center_radii = vec4((crd.x + 0.5) * CHUNK_SIZE, CHUNK_HEIGHT * 0.5, CHUNK_SIZE * (crd.y + 0.5), CHUNK_SIZE * 2);
//                    //vec3 playerPos = firstCamera.getPosition();
//
//                    //if ((center_radii.x / CHUNK_SIZE >= playerPos.x / CHxUNK_SIZE - renderDistance * 0.75 && center_radii.x / CHUNK_SIZE <= playerPos.x / CHUNK_SIZE + renderDistance * 0.75) &&
//                    //    (center_radii.y / CHUNK_SIZE >= playerPos.z / CHUNK_SIZE - renderDistance * 0.75 && center_radii.y / CHUNK_SIZE <= playerPos.z / CHUNK_SIZE + renderDistance * 0.75)) {
//                    //chCenter_Radii.push_back(center_radii);
//                    //visChMeshes.push_back(chmesh);
//                    //}
//
//                    chunkMeshQueue.pop();
//                }
//                count++; // exception keeps happening here
//                if (!(count % 3)) break;
//            }*/

    //        while (!chunkMeshQueue.empty()) {
    //            auto& [m, crd] = chunkMeshQueue.front();
				////cout << "Mesh created for chunk " << crd.x << ", " << crd.y << ", " << crd.z << endl;
				//ivec2 crd2 = ivec2(crd.x, crd.z);
				//int y = crd.y;
    //            Chunk& ch = *world.chunkData[pack(crd2)];
    //            if (ch[y].needUpdate) {
    //                ch[y].mesh->createMesh(m->vertices, m->indices);
    //                ch[y].needUpdate = false;
    //            }
    //            chunkMeshQueue.pop();
    //            count++;
    //            if (count % 32) break;
    //        }

            VP = projection * firstCamera.calcViewMatrix();
            mat4 VP_t = VP;// transpose(VP);
            //extractFrustumPlanes(VP_t, planes);
            extractFrustumPlanes(VP_t);

            Textures[BLOCK_TEX]->useTexture();

            directionalShadowPass(&mainLight, model); //binds base shader shaders[0] as well
            glUniform3f(glGetUniformLocation(shaders[0]->getShaderId(), "camPos"), firstCamera.getPosition().x, firstCamera.getPosition().y, firstCamera.getPosition().z);
            glUniform1f(glGetUniformLocation(shaders[0]->getShaderId(), "fogStart"), 0.7 * CHUNK_SIZE * renderDistance);
            glUniform1f(glGetUniformLocation(shaders[0]->getShaderId(), "fogEnd"), 0.73 * CHUNK_SIZE * renderDistance);

            mainLight.getShadowMap()->read(GL_TEXTURE1);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "directionalShadowMap"), 1);

            Textures[BREAK_STAGE_TEX]->useTexture(GL_TEXTURE3);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "breakStageTexture"), 3);

            Textures[FOLL_TEX]->useTexture(GL_TEXTURE4);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "grassColorTexture"), 4);

			renderWorld();
            //renderWholeWorld();

            headPos = firstCamera.getPosition(), headFront = firstCamera.getFront();
            lookBlock = getBlockAt(lookingAtBlock());

            view = activeCamera.calcViewMatrix();
            //if (projAngle > 45.0f) {
            projection = perspective(radians(projAngle), (float(mainWindow.getBufferWidth()) / float(mainWindow.getBufferHeight())), 0.01f, float(renderDistance * CHUNK_SIZE));
            //}

            glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            glUniformMatrix4fv(shaders[0]->getViewLocation(), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(shaders[0]->getProjectionLocation(), 1, GL_FALSE, value_ptr(projection));
            glUniform3f(shaders[0]->getColorMaskLocation(), 1.0f, 1.0f, 1.0f);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreaking"), breaking);
            glUniform3f(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreakPos"), lookBlock.position.x, lookBlock.position.y, lookBlock.position.z);

            shaders[0]->setDirectionalLight(&mainLight); //replaced : mainLight.useLight(shaders[0]->getAmbientIntensityLocation(), shaders[0]->getAmbientColorLocation(), shaders[0]->getDiffuseIntensityLocation(), shaders[0]->getDirectionLocation());
            shaders[0]->setPointLights(pointLights, pointLightCount);

            keyControl(dt);

            if (inventory.mainInventoryOn || inventory.craftingTableInventoryOn) {
                glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                mainWindow.setMouseMoved();
            }
            else {
                glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstCamera.mouseControl(pow(1, person_view) * mainWindow.getXChange(), mainWindow.getYChange());
                thirdCamera_back.mouseControl(pow(-1, person_view) * mainWindow.getXChange(), mainWindow.getYChange());
            }

            if (mainWindow.getKeyPressed() >= GLFW_KEY_1 && mainWindow.getKeyPressed() <= GLFW_KEY_9) {
                slot = mainWindow.getKeyPressed() - GLFW_KEY_1; inventory.invDidChange(1);;
            }

            float lastPress = 0.0, maxDelay = 0.2 + glfwGetTime();
            if (!inventory.mainInventoryOn && !inventory.craftingTableInventoryOn) {
                if (mainWindow.getKeys()[GLFW_KEY_S]) {
                    ifstream itemsJSON("items.json");
                    if (!itemsJSON) {
                        ofstream outJSON("items.json");
                        outJSON.close();
                        itemsJSON.open("items.json");
                    }
                    itemsJSON >> Jitems;

                    ifstream toolsJSON("tools.json");
                    if (!toolsJSON) {
                        ofstream outJSON("tools.json");
                        outJSON.close();
                        toolsJSON.open("tools.json");
                    }
                    toolsJSON >> tools;
                }
                if (mainWindow.getKeys()[GLFW_KEY_P] && !placeblockdb) {
                    if (mainWindow.getKeys()[GLFW_KEY_I]) {
                        inventory.inf_blocks = true;
                    }
                    else if (mainWindow.getKeys()[GLFW_KEY_N]) {
                        inventory.inf_blocks = false;
                    }

                    if (mainWindow.getKeyPressed() > GLFW_KEY_1 && mainWindow.getKeyPressed() <= GLFW_KEY_9) world.addBlocklook_at(items[mainWindow.getKeyPressed() - GLFW_KEY_1]);
                    else world.addBlocklook_at(inventory.mainInventorySlots[3][slot].item);
                    if (mainWindow.getKeys()[GLFW_KEY_9]) world.addBlocklook_at(item(OAK_PLANK.id));
                    if (mainWindow.getKeys()[GLFW_KEY_1]) world.addBlocklook_at(item(TORCH.id));
                }

                if ((mainWindow.getKeys()[GLFW_KEY_T] || mainWindow.leftClickButtonPressed())) {
                    handGesture = (breakAngle <= -30.f || breakAngle >= 30.f);
                    if (handGesture) swing ^= 1;
                    Item currentTool = inventory.hotbarSlots[slot].item;
                    int itemSoftness = Jitems["items"][itemTypeString[breakingBlock.type.id]]["speed"];
                    int toolSpeed = 1;
                    if (currentTool.isTool()) { toolSpeed = (tools["tools"][itemTypeString[currentTool.id]]["speed"]); }
                    glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreaking"), 5 * (breaking) / itemSoftness);
                    breaking += toolSpeed;
                    if (breakingBlock == lookBlock && itemSoftness != -1) {
                        if (!(breaking % itemSoftness)) {
                            breakReqQueue.push(vec3(1.0f));
                            if (!blockBreakingOut) {
                                blockBreakingOut = true;
                            }
                            breaking = 0;
                        }
                    }
                    else {
                        breaking = 0;
                    }

                    breakingBlock = lookBlock;

                    breakAngle += swing ? 5 : -5;
                    breakModel = rotate(mat4(1.f), -radians(breakAngle), vec3(0, 0, 1))
                               * translate(mat4(1.f), vec3(breakAngle, 2 * breakAngle, 0))
                               ;
                }
                else {
                    glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreaking"), 0);
                }

                if (mainWindow.rightClickButtonPressed() && !placeblockdb) {
                    handGesture = (breakAngle <= -30.f || breakAngle >= 30.f);
                    if (!recipe.itemUsable(lookBlock.type)) {
                        if (mainWindow.getKeys()[GLFW_KEY_I]) {
                            inventory.inf_blocks = true;
                        }
                        else if (mainWindow.getKeys()[GLFW_KEY_N]) {
                            inventory.inf_blocks = false;
                        }
                        if (inventory.mainInventorySlots[3][slot].item != AIR && recipe.itemPlaceable(inventory.mainInventorySlots[3][slot].item)) {
                            {
                                placeReqQueue.push(vec3(1.0f));
                                blockPlacingOut = true;
                            }
                        }
                    }
                    else {
                        inventory.craftingTableInventoryOn = true;
                    }

                    breakAngle += swing ? 5 : -5;
                    breakModel = rotate(mat4(1.f), -radians(breakAngle), vec3(0, 0, 1))
                        * translate(mat4(1.f), vec3(breakAngle, 2 * breakAngle, 0))
                        ;
                }
                if (!breaking) { breakModel = mat4(1); breakAngle = breaking; }

                if (mainWindow.getKeys()[GLFW_KEY_CAPS_LOCK]) {
                    projAngle = (projAngle < 90.0) ? projAngle + 4 : 90.0;
                }
                else {
                    projAngle = (projAngle > 45.0) ? projAngle - 4 : 45.0;
                }
                if (mainWindow.getKeys()[GLFW_KEY_Q]) {
                    int xdrop, ydrop;
                    if (inventory.mainInventoryOn) {
                        xdrop = (int)slotX, ydrop = 3 - (int)slotY;
                    }
                    else {
                        xdrop = slot, ydrop = 3;
                    }
                    if (inventory.mainInventorySlots[ydrop][xdrop].item != AIR) {
                        dropped.push_back(Projectile());
                        dropped.back().shoot(firstCamera.getPosition() + normalize(firstCamera.getFront()), vec3(firstCamera.getFront().x, 0.25, firstCamera.getFront().z), inventory.mainInventorySlots[ydrop][xdrop].item);
                        dropped.back().mesh = world.createProjectileMesh(vec3(0), -4.0f, inventory.mainInventorySlots[ydrop][xdrop].item);
                        inventory.mainInventorySlots[ydrop][xdrop].count--;
                        inventory.invDidChange(1);
                    }
                }

                if (mainWindow.getKeys()[GLFW_KEY_U]) {
                    ball.shoot(firstCamera.getPosition(), firstCamera.getFront(), CLOUD, vec3(1.0f));
                    ball.mesh = world.createMeshCube(vec3(0), -4.0f, ball.item);
                }

                if (mainWindow.getKeys()[GLFW_KEY_F3]) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    person_view = (++person_view % 3);
                }

                if (mainWindow.getKeys()[GLFW_KEY_F5]) {
                    mat4 compassModel = translate(mat4(1.0f), headPos + headFront);
                    glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(compassModel));
                    compassMesh.renderMesh();
                    glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
                    cursorPos.replaceWord("cursor position: x = " + to_string(cursor.x) + ", y = " + to_string(cursor.y), vec3(0, 1, 0), vec2(50, 1500));
                    position.drawText(ortho), craftedItemName.drawText(ortho), cursorPos.drawText(ortho);

                    position.replaceWord("position  x: "
                        + to_string((int)headPos.x) + " y: "
                        + to_string((int)headPos.y) + " z: "
                        + to_string((int)headPos.z) + " cursor count -> "
                        + to_string(cursor.count) + " block : " + itemTypeString[cursor.item.id]
                        + "\n"
                        + "looking at "
                        + itemTypeString[lookBlock.type.id]
                        + "\n"
                        + " | Frame duration : " + to_string(frame_duration_calc)
                        + "FPS : " + to_string(fpscount)
                        , vec3(0.4, 1, 0.7));

                    craftedItemName.replaceWord("main craft slot 1 contains: " + itemTypeString[craftedItem.item.id]
                        + ", " + to_string(inventory.mainCraftingSlots[0][1].count) + (inventory.mainCraftingSlots[0][1].count <= 1 ? " item" : " items")
                        + (inventory.invChange() ? " inventory updating...." : " inventory up to date! "
                            + to_string(renderDistance) + " render distance"), normalize(vec3(1.3, 1, 0)), vec2(50, 1550));

                    if (!(count_time % 10)) {
                        auto end = chrono::high_resolution_clock::now();
                        double frame_duration(chrono::duration<double>(end - start).count());
                        fpscount = (int(1 / frame_duration));
                    }
                    count_time++;
                }

                if (mainWindow.getKeys()[GLFW_KEY_ENTER]) {
                    json jsondata;
                    ifstream ifs("renderdist.json");
                    ifs >> jsondata;
                    renderDistance = jsondata["renderdistance"];
                    generateSpiral(spiral);
                }

                if (!breakResQueue.empty()) {
                    Block droppedItem = breakResQueue.front();
                    dropped.back().shoot(droppedItem.position, vec3(-firstCamera.getFront().x, 0.5, -firstCamera.getFront().z), droppedItem.type);
                    dropped.back().mesh = world.createProjectileMesh(vec3(0), -4.0f, droppedItem.type);
                    dropped.push_back(Projectile());
                    breakResQueue.pop();
                }

                placeblockdb = (mainWindow.keyIsPressed(GLFW_KEY_P) || mainWindow.rightClickButtonPressed()) == 1;
                //breakblockdb = (mainWindow.keyIsPressed(GLFW_KEY_T) || mainWindow.leftClickButtonPressed())  == 1;
            }

            if (person_view == 0) {
                activeCamera = firstCamera;
            }
            else if (person_view == 1) {
                activeCamera = thirdCamera_back;
            }

            Textures[FACE_TEX]->useTexture();

            mat4 modelHead = translate(mat4(1.0f), headPos);
            mat4 rotation(1.0f);
            vec3 dir = normalize(firstCamera.getFront());
            vec3 right = cross(vec3(0, 1, 0), dir);
            vec3 up = cross(dir, right);
            rotation[0] = vec4(normalize(right), 0.0f); rotation[1] = vec4(normalize(up), 0.0f); rotation[2] = vec4(normalize(-dir), 0.0f);
            modelHead *= rotation;
            glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(modelHead));
            headMesh.renderMesh();
            Textures[SLOT_TEX]->useTexture();

            vec3 cameraPosition = firstCamera.getPosition();

            if (!blockExistsAt((vec3(ftoint(ball.position.x), ftoint(ball.position.y - 0.5), ftoint(ball.position.z))))) {
                tp = 1;
                if (ball.shot) {
                    ball.update();
                }
                ball.model = translate(mat4(1.0f), ball.position) * rotate(mat4(1.0f), radians(ball.angle++), vec3(0.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(ball.model));
                ball.draw();
                glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            }
            else {
                ball.shot = 0;
                if (tp == 1) {
                    firstCamera.setPosition(ball.position);
                    ball.velocity = vec3(0);
                    ball.initial_velocity = vec3(0);
                    tp = 0;
                }
            }

            //bool toolTexInd = 0; // indicator for binding tools texture or not
            Textures[BLOCK_TEX]->useTexture();
            Textures[TOP_TEX]->useNextTexture();
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "topTexture"), 2);
            for (auto it = dropped.begin(); it != dropped.end(); ) {
                Projectile& drop = *it;
                if (drop.item.isTool()){// && !toolTexInd) {
                    Textures[TOOLS_TEX]->useTexture();
                    //toolTexInd = 1;
                }
                else{// if(!drop.item.isTool() && toolTexInd){
                    Textures[BLOCK_TEX]->useTexture();
                    //toolTexInd = 0;
                }
                //Projectile& drop : dropped
                drop.shot = !blockExistsAt((vec3(ftoint(drop.position.x) + 0.5, drop.position.y, ftoint(drop.position.z) + 0.5))) && !drop.done;
                if (drop.shot) {
                    drop.update();
                }
                else {
                    drop.velocity *= vec3(0, 1, 0);
                    drop.initial_velocity = vec3(0);
                }

                drop.model = translate(mat4(1.0f), drop.position);
                drop.model *= translate(mat4(1.0f), vec3(0, 0.25 * sin(radians(float(drop.angle))), 0));
                drop.model *= rotate(mat4(1.0f), radians(drop.angle++), vec3(0, 1, 0));
                glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(drop.model));
                
                if (length(drop.position - firstCamera.getPosition()) <= 3 && drop.angle > 360) {
                    drop.done = 1;
                    drop.position += (firstCamera.getPosition() - drop.position) / 10.f;
                }
                if (length(drop.position - firstCamera.getPosition()) <= 1 && abs(drop.position.y - firstCamera.getPosition().y) <= 2 && drop.angle > 360) {
                    inventory.assignAvailableSlot(drop.item);
                    it = dropped.erase(it);
                    continue;
                }

                drop.draw();
                it++;
            }
            glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            shaders[5]->useShader();
            view = activeCamera.calcViewMatrix();
            //glUniformMatrix4fv(shaders[5]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            //glUniformMatrix4fv(shaders[5]->getViewLocation(), 1, GL_FALSE, value_ptr(view));
            //glUniformMatrix4fv(shaders[5]->getProjectionLocation(), 1, GL_FALSE, value_ptr(projection));

            //For block highlighting
            ivec3 lookPosition = lookingAtBlock();
            if (lookPosition.y >= 0) {
                mat4 modelLooking = translate(mat4(1.0f), vec3(lookPosition));
                glUniformMatrix4fv(shaders[5]->getModelLocation(), 1, GL_FALSE, value_ptr(modelLooking));
                lookingMesh.renderMesh();
                glUniformMatrix4fv(shaders[5]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            }

            glDisable(GL_DEPTH_TEST); // so crosshair draws on top
            shaders[2]->useShader();
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

            if (person_view == 0) {
                glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
                crosshair.drawCrosshair();
            }

            //glEnable(GL_DEPTH_TEST);

            shaders[2]->useShader();
            Textures[SLOT_TEX]->useTexture();
            inventory.defineHotbarSlotSelectorGeometry();
            inventory.drawHotbarSlotSelector();

            glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
            Textures[MAIN_INV_TEX]->useTexture();
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
            inventory.drawHotbar();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if(inventory.mainInventoryOn)
                Textures[LARGE_INV_TEX]->useTexture();
            if (mainWindow.getKeys()[GLFW_KEY_C]) {
                if (craftedItem.item != AIR) {
                    for (int i = (sizeof(inventory.mainInventorySlots) / sizeof(inventory.mainInventorySlots[3])) - 1; i >= 0; i--) {
                        for (int j = 0; j < (sizeof(inventory.mainInventorySlots[3]) / sizeof(InventorySlot)); j++) {
                            if (inventory.mainInventorySlots[i][j].item == AIR
                                || inventory.mainInventorySlots[i][j].item == craftedItem.item
                                ) {
                                inventory.mainInventorySlots[i][j].item = craftedItem.item;
                                inventory.mainInventorySlots[i][j].count += craftedItem.count;
                                if (inventory.mainInventoryOn)
                                    for (int k = 0; k < 2; k++) {
                                        for (int l = 0; l < 2; l++) {
                                            if (inventory.mainCraftingSlots[k][l].count)
                                                inventory.mainCraftingSlots[k][l].count--;
                                        }
                                    }
                                else if (inventory.craftingTableInventoryOn) {
                                    for (int k = 0; k < 3; k++) {
                                        for (int l = 0; l < 3; l++) {
                                            if (inventory.craftingTableSlots[k][l].count)
                                                inventory.craftingTableSlots[k][l].count--;
                                        }
                                    }
                                }
                                goto invcleared;
                            }
                        }
                    }
                invcleared:
                    craftedItem.count--;
                    inventory.invDidChange(1);
                }
            }

            if (mainWindow.getKeys()[GLFW_KEY_E] && !invtoggledb) { //DB = debouncing
                std::this_thread::sleep_for(chrono::milliseconds(100));
                inventory.mainInventoryOn = !inventory.mainInventoryOn;
                inventory.craftingTableInventoryOn = false;
                inventory.invDidChange(1);
            }

			invtoggledb = mainWindow.keyIsPressed(GLFW_KEY_E) == 1;

            if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT] && mainWindow.getKeys()[GLFW_KEY_E]) {
                for (int i = 0; i < (sizeof(inventory.mainInventorySlots) / sizeof(inventory.mainInventorySlots[3])); i++) {
                    for (int j = 0; j < (sizeof(inventory.mainInventorySlots[3]) / sizeof(InventorySlot)); j++) {
                        if (inventory.mainInventorySlots[i][j].item == AIR) {
                            for (int k = 0; k < sizeof(inventory.mainCraftingSlots) / sizeof(inventory.mainCraftingSlots[0]); k++) {
                                for (int l = 0; l < sizeof(inventory.mainCraftingSlots[0]) / sizeof(InventorySlot); l++) {
                                    if (inventory.mainCraftingSlots[k][l].count > 0) {
                                        inventory.mainInventorySlots[i][j] = inventory.mainCraftingSlots[k][l];
                                        inventory.mainCraftingSlots[k][l].count = 0;
                                        inventory.invDidChange(1);
                                        craftedItem.count = 0;
                                    }
                                }
                            }
                            goto done;
                        }
                    }
                }
            done:
                inventory.mainInventoryOn = false;
                inventory.craftingTableInventoryOn = false;
                //cursor = Cursor();
                firstCamera.mouseControl(lastXChange, lastYChange);
                inventory.invDidChange(1);
            }

            //int keyPress = mainWindow.getKeyPressed();
            //if (keyPress >= GLFW_KEY_RIGHT && keyPress <= GLFW_KEY_UP) {
            //    this_thread::sleep_for(chrono::milliseconds(20));
            //    if (keyPress >= GLFW_KEY_DOWN && keyPress <= GLFW_KEY_UP) {
            //        if (slotY > -(keyPress - GLFW_KEY_DOWN) && slotY < 4 - (keyPress - GLFW_KEY_DOWN)) {
            //            slotY += 2 * (keyPress - GLFW_KEY_DOWN) - 1; // down : slotY - 1, up : slotY + 1
            //        }
            //    }
            //    if (keyPress >= GLFW_KEY_RIGHT && keyPress <= GLFW_KEY_LEFT) {
            //        if (slotX > keyPress - GLFW_KEY_RIGHT - 1 && slotX < 8 + keyPress - GLFW_KEY_RIGHT) {
            //            slotX -= 2 * (keyPress - GLFW_KEY_RIGHT) - 1;
            //        }
            //    }
            //}

            if (inventory.mainInventoryOn) {
                Textures[LARGE_INV_TEX]->useTexture();
                glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

                inventory.drawMainInventory();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                Textures[SLOT_TEX]->useTexture();
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

                inventory.defineInvSlotSelectGeometry();
                inventory.drawInvSlotSelector();

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                Textures[BLOCK_TEX]->useTexture();

                inventory.drawMainInventorySlots(ortho, itemView, itemProj);

                if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT] && mainWindow.getKeys()[GLFW_KEY_ENTER]) {
                    if (mainWindow.getKeyPressed() >= GLFW_KEY_0 && mainWindow.getKeyPressed() <= GLFW_KEY_3) {
                        int x = (mainWindow.getKeyPressed() - GLFW_KEY_0) % 2, y = (mainWindow.getKeyPressed() - GLFW_KEY_0) / 2;
                        if (inventory.mainCraftingSlots[y][x].item == AIR || count == 0) {
                            inventory.mainCraftingSlots[y][x].item = inventory.mainInventorySlots[3 - (int)slotY][(int)slotX].item;
                            inventory.mainCraftingSlots[y][x].count += craftedItem.count;
                            inventory.mainInventorySlots[3 - (int)slotY][(int)slotX].count--;
                            inventory.invDidChange(1);
                        }
                    }
                }

                //Crafting inventory slots are being drawn here.
                for (int i = 0; i < (sizeof(inventory.mainCraftingSlots) / sizeof(inventory.mainCraftingSlots[0])); i++) {
                    mat4 mod = translate(mat4(1.0f), vec3(centerX, centerY, 0)), craftmod = translate(mat4(1.0f), vec3(centerX + 100, centerY, 0));;
                    for (int j = 0; j < (sizeof(inventory.mainCraftingSlots[0]) / sizeof(InventorySlot)); j++) {
                        render3Din2D(itemModel, inventory.mainCraftingSlots[i][j].mesh, inventory.mainCraftingSlots[i][j].model, inventory.mainCraftingSlots[i][j].quadMesh, ortho, itemView, itemProj, inventory.mainCraftingSlots[i][j].item);
                        inventory.mainCraftingSlots[i][j].textCount.drawText(ortho);
                    }
                }
                render3Din2D(itemModel, craftedItem.mesh, craftedItem.model, craftedItem.quadMesh, ortho, itemView, itemProj, craftedItem.item);
                craftedItem.textCount.drawText(ortho);

                render3Din2D(itemModel, cursor.mesh, translate(mat4(1.0f), vec3((float(cursor.x) / mainWindow.getBufferWidth()) * WIDTH, ((float)cursor.y / mainWindow.getBufferHeight()) * HEIGHT, 0)), cursor.quadMesh, ortho, itemView, itemProj, cursor.item);
                cursor.textCount.model = translate(mat4(1.0f), vec3((float(cursor.x) / mainWindow.getBufferWidth()) * WIDTH, ((float)cursor.y / mainWindow.getBufferHeight()) * HEIGHT, 0));
                if (cursor.count > 1) {
                    cursor.textCount.drawText(ortho);
                }
                //inventory.invDidChange(1);
            }

            // crafting inventory GUI appears here
            if (inventory.craftingTableInventoryOn) {
                if (mainWindow.getKeys()[GLFW_KEY_RIGHT_SHIFT] && mainWindow.getKeys()[GLFW_KEY_ENTER]) {
                    if (mainWindow.getKeyPressed() >= GLFW_KEY_0 && mainWindow.getKeyPressed() <= GLFW_KEY_8) {
                        int x = (mainWindow.getKeyPressed() - GLFW_KEY_0) % 3, y = (mainWindow.getKeyPressed() - GLFW_KEY_0) / 3;
                        if (inventory.craftingTableSlots[y][x].item == AIR) { // eg pressing 4 gives [5/3][5%3] = [1][2]
                            inventory.craftingTableSlots[y][x].item = inventory.mainInventorySlots[3 - (int)slotY][(int)slotX].item;
                            inventory.craftingTableSlots[y][x].count += craftedItem.count;
                            inventory.mainInventorySlots[3 - (int)slotY][(int)slotX].count--;
                        }
                    }
                    inventory.invDidChange(1);
                }

                Textures[CRAFT_GUI_TEX]->useTexture();
                glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                inventory.drawMainInventory();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                Textures[SLOT_TEX]->useTexture();
                glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                inventory.defineCrafingInvSlotSelectorGeometry();
                inventory.drawInvSlotSelector();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                Textures[BLOCK_TEX]->useTexture();

                inventory.drawMainInventorySlots(ortho, itemView, itemProj);

                for (int i = 0; i < (sizeof(inventory.craftingTableSlots) / sizeof(inventory.craftingTableSlots[0])); i++) {
                    for (int j = 0; j < (sizeof(inventory.craftingTableSlots[0]) / sizeof(InventorySlot)); j++) {
                        render3Din2D(itemModel, inventory.craftingTableSlots[i][j].mesh, inventory.craftingTableSlots[i][j].model, inventory.craftingTableSlots[i][j].quadMesh, ortho, itemView, itemProj, inventory.craftingTableSlots[i][j].item);
                        inventory.craftingTableSlots[i][j].textCount.drawText(ortho);
                    }
                }
                render3Din2D(itemModel, craftedItem.mesh, craftedItem.model, craftedItem.quadMesh, ortho, itemView, itemProj, craftedItem.item);
                craftedItem.textCount.drawText(ortho);

                render3Din2D(itemModel, cursor.mesh, translate(mat4(1.0f), vec3((float(cursor.x) / mainWindow.getBufferWidth()) * WIDTH, ((float)cursor.y / mainWindow.getBufferHeight()) * HEIGHT, 0)), cursor.quadMesh, ortho, itemView, itemProj, cursor.item);
                cursor.textCount.model = translate(mat4(1.0f), vec3((float(cursor.x) / mainWindow.getBufferWidth()) * WIDTH, ((float)cursor.y / mainWindow.getBufferHeight()) * HEIGHT, 0));;
                if (cursor.count > 1) {
                    cursor.textCount.drawText(ortho);
                }
                //inventory.invDidChange(1);
            }

            for (int i = 0; i < 9; i++) {
                mat4 itemModel = scale(mat4(1.0f), vec3(0.1f, 0.12f, 0.1f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1)) *
                    ((!inventory.hotbarSlots[i].item.isFlat()) ? rotate(mat4(1.0f), radians(30.0f), vec3(1, 0, 0)) * rotate(mat4(1.0f), radians(45.0f), vec3(0, 1, 0)) : mat4(1.0f)) *
                    rotate(mat4(1.0f), radians(inventory.hotbarSlots[i].angle), vec3(0, 1, 0));
                inventory.hotbarSlots[i].angle += 0.5f;
                render3Din2D(itemModel, inventory.hotbarSlots[i].mesh, inventory.hotbarSlots[i].model, inventory.hotbarSlots[i].quadMesh, ortho, itemView, itemProj, inventory.hotbarSlots[i].item);
                inventory.hotbarSlots[i].textCount.drawText(ortho);
            }
            Textures[BLOCK_TEX]->useTexture();
            inventory.updateCurrentBlock();
			
            render3Din2D(itemModel * breakModel
                * rotate(mat4(1.0f), radians((!currentBlock.item.isTool() ? -35.f : 0.f)), vec3(1, 1, 1))
                * rotate(mat4(1.0f), radians((!currentBlock.item.isTool() ? 45.f : 0.f)), vec3(0, 1, 0))
                * rotate(mat4(1.0f), radians((currentBlock.item.isTool() ? 75.f : 0.f)), vec3(0, 0, 1))
                //* rotate(mat4(1.0f), radians(( currentBlock.item.isTool() ? angletest :  0.f)), vec3(0, 1, 0))
                //* rotate(mat4(1.0f), radians(( currentBlock.item.isTool() ? 75.f :  0.f)), vec3(0, 0, 1))
                , currentBlock.mesh, translate(mat4(1.0f), vec3(centerX + 600 - 20 * (firstCamera.getYaw() - lastYaw), centerY - 650 - 20 * (firstCamera.getPitch() - lastPitch) - 2 * (firstCamera.initial_velocity.y + firstCamera.velocity.y), 0)), 
                currentBlock.quadMesh, ortho, currentBlockView, itemProj, currentBlock.item);
            angletest += 1;
            //activeCamera.setFront(vec3(activeCamera.getFront().x,
            //    activeCamera.getFront().y,
            //    activeCamera.getFront().z + sin(2.f * radians((float)inventory.hotbarSlots[0].angle))));
            //glEnable(GL_DEPTH_TEST);
            if (inventory.invChange()) { inventory.updateInventory(); }

            if (mainWindow.getShouldClose()) {
                json lastPlayer;
                lastPlayer["player"]["x"] = firstCamera.getPosition().x;
                lastPlayer["player"]["y"] = firstCamera.getPosition().y;
                lastPlayer["player"]["z"] = firstCamera.getPosition().z;
                ofstream playerJSON("player.json");
                playerJSON << lastPlayer.dump(4);

                chunkGenRunning = false;
                queueCV.notify_all(); // wake up sleeping threads

                for (auto& t : workers)
                    t.join();

                chunkGenRunning = false;
                chunkGenRunning2 = false;
                chunkUpdateGenRunning = false;
                stopChunkUpdaters = true;
                blockPlacing = false;
                blockBreaking = false;//chunkGenThread.join(); //chunkGenThread2.join(); //chunkGenThread3.join();
                blockBreakThread1.join();
                blockPlaceThread.join();

                //chunkUpdateThread.join();
                return;
            }

            if (spawn <= 511) {
                spawn++;
            }

            //blockExistsAt(vec3(ftoint(cameraPosition.x + 0.5), cameraPosition.y - 1.5, ftoint(cameraPosition.z + 0.5)))
            bool onGround = playerCollides();
            if (!onGround && spawn > 511) {
                firstCamera.initial_velocity.y -= 0.5;
                //firstCamera.velocity_factor = vec3(1, 1, 1);
                //firstCamera.calculateCamPos(0.01); // 0.01 instead of dt
                //cout << "\x1b[2J\x1b[H"; // clears console
            }
            else if (onGround) {
                firstCamera.velocity = vec3(0);
                firstCamera.initial_velocity *= vec3(0);
                firstCamera.velocity_factor = vec3(1);
                //firstCamera.initial_velocity.y += 0.5;
            }

            mainWindow.updateLastKeyPress();
            mainWindow.swapBuffers();

            auto endframe = chrono::high_resolution_clock::now();
            frame_duration_calc = (chrono::duration<double>(endframe - startframe).count());
            //cout << "FRAME LASTED ::: " << frame_duration_calc << endl;
        }

        json lastPlayer;
        lastPlayer["player"]["x"] = firstCamera.getPosition().x;
        lastPlayer["player"]["y"] = firstCamera.getPosition().y;
        lastPlayer["player"]["z"] = firstCamera.getPosition().z;
        ofstream outplayerJSON("player.json");
        outplayerJSON << lastPlayer.dump(4);

        
    }////

    template <typename T>
    void vec_erase(vector<T>& vec, int index) {
        //vec.swap(vec[index], vec.back());
        vec[index] = vec.back();
        vec.pop_back();
    }

    void renderInQueue(std::queue<pair<LightMesh*, vec2>>& chQueue) {
        static int v = 0;
        //visChunks.clear();
  //      chCenter_Radii.clear();

        //visChunks.reserve(world.chunkData.size());
  //      chCenter_Radii.reserve(world.chunkData.size());
        vec3 playerPos = firstCamera.getPosition();

        for (auto it = world.chunkData.begin(); it != world.chunkData.end(); ) {
            if (!it->second) {
                it = world.chunkData.erase(it);
                continue;
            }
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();

            if (!(chunk->neighboursPresent & 1)) {
                if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
                    int dirsX[] = { -1, 0, 0, 1 }, dirsY[] = { 0, -1, 1, 0 };
                    for (int i = 0; i < 4; i++) {
                        if (world.chunkData.count(pack(coords + ivec2(dirsX[i], dirsY[i]))) > 0) {
                            chunk->neighboursPresent |= (1 << (i + 1));
                        }
                    }
                } if (chunk->neighboursPresent == 0x1E) { // 1 1110
                    {
                        std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
                        chunkUpdateRequestQueue.push(coords);
                    }
                    chunkUpdateCV.notify_one();
                    chunk->neighboursPresent |= 1;

                } //if (!(chunk->neighboursPresent & 1)) continue;
            }

            if ((coords.x <= playerPos.x / CHUNK_SIZE - renderDistance * 1. || coords.x >= playerPos.x / CHUNK_SIZE + renderDistance * 1.) ||
                (coords.y <= playerPos.z / CHUNK_SIZE - renderDistance * 1. || coords.y >= playerPos.z / CHUNK_SIZE + renderDistance * 1.)) {
                //} //3 fev 11h30
    //            //else {
    //                //if (chunk->getDirty()) {
    //                //    chunk->mesh.reset();
    //                //    chunk->unloaded = true;
    //                //}
    //            //    else {
    ////                    auto itvisvec = find(visChunks.begin(), visChunks.end(), *chunk->mesh.get());//visChunks.erase(remove(visChunks.begin(), visChunks.end(), chunk.get()), visChunks.end());
    //                //auto itvis = find_if(visChunks.begin(), visChunks.end(), [coords](const pair<ivec2, unique_ptr<LightMesh>>& chVis) { return coords.x == chVis.first.x && coords.y == chVis.first.y; });
    //                    //if (itvis != visChunks.end()) {
    //                        //size_t chIdx = itvis - visChunks.begin();
    ////
    //                        //visChunks.erase(visChunks.begin() + chIdx);
    //                        //chCenter_Radii.erase(chCenter_Radii.begin() + chIdx);
    //                        //visbuffer.erase(visbuffer.begin() + chIdx);
    //                    //}
                chunkCoords.erase(to(coords));
                it = world.chunkData.erase(it);
                //cout << v++ << " deleted! current size : " << world.chunkData.size() << endl;
                continue;
                //}
            }
            else
            {
                vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerPos.y, (coords.y + 0.5) * CHUNK_SIZE);
                if (!sphereInFrustum(center, CHUNK_SIZE * 2)) {
                    it++;
                    continue;
                }

                ///Place holder for Occlusion Culling
                //visChunks.push_back(chunk.get());

                //if (chunk->needUpdate) {
                //    chunk->mesh->createMeshLocally();
                //    chunk->needUpdate = false;
                //}

                //n++;

                //if (mainWindow.getKeys()[GLFW_KEY_M])
                //    chunk->mesh->renderMeshAsLines();
                //else

                chQueue.push({ chunk->mesh.get(), vec2(coords) });
                //chunk->mesh->renderMesh();
            }

            it++;
        }
    }

    void renderWorld() {
        static int tps = 0; //tick per sec
        vec3 playerPos = firstCamera.getPosition()/vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
		vec2 _2dPlPosHi = vec2(playerPos.x, playerPos.z) + float(renderDistance),
             _2dPlPosLo = vec2(playerPos.x, playerPos.z) - float(renderDistance);

        for (auto it = world.chunkData.begin(); it != world.chunkData.end(); ) {
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();

            int dirs[] = { -1, 1, 0, 0,   0, 0, -1, 1 };

            if ((coords.x <= _2dPlPosLo.x || coords.x >= _2dPlPosHi.x) ||
                (coords.y <= _2dPlPosLo.y || coords.y >= _2dPlPosHi.y))
            {
                chunkCoords.erase((chunk->coord));
                it = world.chunkData.erase(it);
                continue;
            }
                
            if (!(chunk->neighboursPresent & 1)) {
                if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
                    for (int i = 0; i < 4; i++) {
                        ivec2 chcrds = coords + ivec2(dirs[i], dirs[i + 4]);
                        if (world.chunkData.count(pack(chcrds)) > 0) {
                            chunk->neighboursPresent |= (1 << (i + 1));
                        }
                    }
                } 
                if (chunk->neighboursPresent == 0x1E) { // 1 1110 
                    chNeighPack chunkochunks;
                    chunkochunks.coords = chunk->coord;
                    memcpy(chunkochunks.block_data.data(), chunk->block_data.data(), CHUNK_VOLUME);
                    for (int i = 0; i < 4; i++) {
                        ivec2 chcrds = coords + ivec2(dirs[i], dirs[i + 4]);
                        uint idxcrds = pack(chcrds);
                        if (world.chunkData.count(idxcrds)) {
                            unique_ptr<Chunk>& ch = world.chunkData[idxcrds] ;
                            memcpy(chunkochunks.neighbour_data[i].data(), ch->block_data.data(), CHUNK_VOLUME);
                        }
                    }
                    {
                        std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
                        chunkCleanupQueue.push(move(chunkochunks));
                    }
                    chunkUpdateCV.notify_one();
                    chunk->neighboursPresent |= 1;
                }
            }
            
            vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerPos.y, (coords.y + 0.5) * CHUNK_SIZE);
            if (sphereInFrustum(center, CHUNK_SIZE * 2)) {
                chunk->mesh->renderMesh();
            }

            it++;
        }
    }

    void renderShadowWorld() {
        vec3 playerpos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        vec2 _2dPlPosHi = vec2(playerpos.x, playerpos.z) + float(renderDistance) * 0.5f,
             _2dPlPosLo = vec2(playerpos.x, playerpos.z) - float(renderDistance) * 0.5f;

        for (auto& chunks : world.chunkData) {
            ivec2 coords = chunks.second->coords();
            unique_ptr<Chunk>& chunk = chunks.second;
           if (chunk) {
                if ((coords.x >= _2dPlPosLo.x && coords.x <= _2dPlPosHi.x) &&
                    (coords.y >= _2dPlPosLo.y && coords.y <= _2dPlPosHi.y)) {
                    vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerpos.y, (coords.y + 0.5) * CHUNK_SIZE);
                    if(sphereInFrustum(center, CHUNK_SIZE)) {
                        chunk->mesh->renderMesh();
                    }
                }
            }
        }
    }

//    void renderWorld() {
//        //visChunks.clear();
//        //int n = 0;
//        for (auto it = world.chunkData.begin(); it != world.chunkData.end(); ) {
//            if (!it->second) {
//                it = world.chunkData.erase(it);
//                continue;
//            }
//            auto& chunk = it->second;
//            ivec2 coords = chunk->coords();
//
//            if (!(chunk->neighboursPresent & 1)) {
//                if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
//                    int dirsX[] = { -1, 0, 0, 1 }, dirsY[] = { 0, -1, 1, 0 };
//                    for (int i = 0; i < 4; i++) {
//                        if (world.chunkData.count(pack(coords + ivec2(dirsX[i], dirsY[i]))) > 0) {
//                            chunk->neighboursPresent |= (1 << (i + 1));
//                        }
//                    }
//                }
//                if (chunk->neighboursPresent == 0x1E) { // 1 1110
//                    {
//                        std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
//                        chunkUpdateRequestQueue.push(coords);
//                    }
//                    chunkUpdateCV.notify_one();
//                    chunk->neighboursPresent |= 1;
//
//                } //if (!(chunk->neighboursPresent & 1)) continue;
//            }
//            if (chunk->mesh) {
//                if ((coords.x >= firstCamera.getPosition().x / CHUNK_SIZE - renderDistance * 1.5 && coords.x <= firstCamera.getPosition().x / CHUNK_SIZE + renderDistance * 1.5) &&
//                    (coords.y >= firstCamera.getPosition().z / CHUNK_SIZE - renderDistance * 1.5 && coords.y <= firstCamera.getPosition().z / CHUNK_SIZE + renderDistance * 1.5)) {
//
//                    if (chunk->unloaded) {
//                        { chunkUpdateRequestQueue.push(chunk->coords()); /*std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex); */ }
//                        chunk->needUpdate = true;
//                        chunk->unloaded = false;
//                    }
//                    //vec3 minAABB = vec3(coords.x * CHUNK_SIZE, 0, coords.y * CHUNK_SIZE);
//                    //vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
//					vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, CHUNK_HEIGHT * 0.5, (coords.y + 0.5) * CHUNK_SIZE);
//                    if (!sphereInFrustum(center, CHUNK_SIZE * 2)) {
//                        it++;
//                        continue;
//                    }
//
//					///Place holder for Occlusion Culling
//                    //visChunks.push_back(chunk.get());
//
//                    //if (chunk->needUpdate) {
//                    //    chunk->mesh->createMeshLocally();
//                    //    chunk->needUpdate = false;
//                    //}
//
//                    //n++;
//                    if (mainWindow.getKeys()[GLFW_KEY_M])
//                        chunk->mesh->renderMeshAsLines();
//                    else
//                        chunk->mesh->renderMesh();
//
//
//                } //3 fev 11h30
//                else {
//                    if (chunk->getDirty()) {
//                        if (chunk->mesh) {//&& chunk->mesh->vertices.size()
//                            //chunk->mesh->vertices.clear();
//                            //chunk->mesh->indices.clear();
//                            chunk->mesh.reset();
//                            chunk->unloaded = true;
//                        }
//                    }
//                    else {
//                        chunkCoords.erase(to(coords));
//                        it = world.chunkData.erase(it);
//                        continue;
//                    }
//                }
//            }
//
//            it++;
//        }
//		//cout << "rendered " << n << " chunks\n";
//    }
// 
//  void renderShadowWorld() {
        //{
        ////    //lock_guard<std::mutex> lock(worldChMutex);
        //    for (int i = 0; i < visbuffer.size(); i++) {
        ////        //if (visChMeshes[i]) {
        //            visChMeshes[i]->renderMesh();
        ////        //}
        ////        //else {
        ////        //    vec_erase(visChMeshes, i);
        ////        //    vec_erase(visbuffer, i);
        ////        //    vec_erase(chCenter_Radii, i);
        ////        //    continue;
        ////        //}
        ////        //i++;
        //    }
        //}
        //vec3 playerpos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        //vec2 _2dPlPosHi = vec2(playerpos.x, playerpos.z) + float(renderDistance) * 0.5f,
        //    _2dPlPosLo = vec2(playerpos.x, playerpos.z) - float(renderDistance) * 0.5f;

        //for (auto& chunks : world.chunkData) {
        //    //if (!chunks.second) continue;
        //    ivec2 coords = chunks.second->coords();
        //    unique_ptr<Chunk>& chunk = chunks.second;
        //    if (chunk) {
        //        if ((coords.x >= _2dPlPosLo.x && coords.x <= _2dPlPosHi.x) &&
        //            (coords.y >= _2dPlPosLo.y && coords.y <= _2dPlPosHi.y)) {
        //            //                    //vec3 minAABB = vec3(coords.x * CHUNK_SIZE, 0, coords.y * CHUNK_SIZE);
        //            //                    //vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
        //            vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerpos.y, (coords.y + 0.5) * CHUNK_SIZE);
        //            if (sphereInFrustum(center, CHUNK_SIZE)) {
        //                chunk->mesh->renderMesh();
        //            }
        //        }
        //    }
        //}
        //}
//
//    void renderShadowWorld() {
//        for (auto& chunks : world.chunkData) {
//            if (!chunks.second) continue;
//            ivec2 coords = chunks.second->coords();
//            unique_ptr<Chunk>& chunk = chunks.second;
//            if (chunk->mesh) {
//                //if ((coords.x >= firstCamera.getPosition().x / CHUNK_SIZE - renderDistance * 0.5 && coords.x <= firstCamera.getPosition().x / CHUNK_SIZE + renderDistance * 0.5) &&
//                //    (coords.y >= firstCamera.getPosition().z / CHUNK_SIZE - renderDistance * 0.5 && coords.y <= firstCamera.getPosition().z / CHUNK_SIZE + renderDistance * 0.5)) {
//
//                    //vec3 minAABB = vec3(coords.x * CHUNK_SIZE, 0, coords.y * CHUNK_SIZE);
//                    //vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
//                    vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, CHUNK_HEIGHT * 0.5, (coords.y + 0.5) * CHUNK_SIZE);
//                    if (!sphereInFrustum(center, CHUNK_SIZE)) {
//                        continue;
//                    }
//                    chunk->mesh->renderMesh();
//                //}
//            }
//        }
//    }

    //      int occWidth = 128, occHeight = 72;
//      sort(visChunks.begin(), visChunks.end(), [](Chunk* a, Chunk* b) {
//          vec3 aCenter = vec3((a->coords().x + 0.5) * CHUNK_SIZE, CHUNK_HEIGHT / 2, (a->coords().y + 0.5) * CHUNK_SIZE);
//          vec3 bCenter = vec3((b->coords().x + 0.5) * CHUNK_SIZE, CHUNK_HEIGHT / 2, (b->coords().y + 0.5) * CHUNK_SIZE);
//          float distA = length(firstCamera.getPosition() - aCenter);
//          float distB = length(firstCamera.getPosition() - bCenter);
//          return distA < distB; // sort in ascending order of distance
//          });

      //vector<float> occ_buffer; // occWidth * occHeight is the resolution of the occlusion buffer, which is a downscaled version of the screen for faster occlusion culling like instead of 16 : 9, its this!
      //occ_buffer.assign(occWidth * occHeight, FLT_MAX); // initialize with max depth

//      for (auto chunk : visChunks) {
//          float minX = FLT_MAX;
//          float minY = FLT_MAX;
//          float maxX = -FLT_MAX;
//          float maxY = -FLT_MAX;
      //	float minDepth = 1.f;

//          ivec2 coords = chunk->coords();
      //	vec3 minAABB = vec3(coords.x * CHUNK_SIZE, 0, coords.y * CHUNK_SIZE);
      //	vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
//          //vec3 screencorns[8];
//          //vec3 corners[8] = {
//          //    vec3(minAABB.x, minAABB.y, minAABB.z),
//          //    vec3(minAABB.x, maxAABB.y, minAABB.z),
//          //    vec3(minAABB.x, maxAABB.y, maxAABB.z),
//          //    vec3(minAABB.x, minAABB.y, maxAABB.z),
//          //    vec3(maxAABB.x, minAABB.y, minAABB.z),
//          //    vec3(maxAABB.x, maxAABB.y, minAABB.z),
//          //    vec3(maxAABB.x, maxAABB.y, maxAABB.z),
//          //    vec3(maxAABB.x, minAABB.y, maxAABB.z)
//          //};
//          vec3 corners[8] = {
//                  {minAABB.x, minAABB.y, minAABB.z},
//                  {maxAABB.x, minAABB.y, minAABB.z},
//                  {minAABB.x, maxAABB.y, minAABB.z},
//                  {maxAABB.x, maxAABB.y, minAABB.z},
//                  {minAABB.x, minAABB.y, maxAABB.z},
//                  {maxAABB.x, minAABB.y, maxAABB.z},
//                  {minAABB.x, maxAABB.y, maxAABB.z},
//                  {maxAABB.x, maxAABB.y, maxAABB.z}
//          };

//          for (int i = 0; i < 8; i++) {
//              vec4 ndccorner = VP * vec4(corners[i], 1.0f);
      //		float w = ndccorner.w;
      //		if (ndccorner.w <= 0) continue; // behind the camera, ignore
//              ndccorner /= w;

//              vec3 screencorner = vec3(((ndccorner.x + 1) / 2) * bufferWidth,
//                                   (1 - (ndccorner.y * .5 + 0.5) ) * bufferHeight,
//                                        (ndccorner.z + 1) / 2);
//              minX = std::min(minX, screencorner.x);
//              minY = std::min(minY, screencorner.y);
//              maxX = std::max(maxX, screencorner.x);
//              maxY = std::max(maxY, screencorner.y);
//              minDepth = std::min(minDepth, w);
//          }
//          if (minX == FLT_MAX)
//              continue; // whole AABB was behind the camera

//          minX = std::clamp(minX, 0.0f, bufferWidth - 1.0f);
//          maxX = std::clamp(maxX, 0.0f, bufferWidth - 1.0f);
//          minY = std::clamp(minY, 0.0f, bufferHeight - 1.0f);
//          maxY = std::clamp(maxY, 0.0f, bufferHeight - 1.0f);

//          float scaleX = (float)occWidth / (float)bufferWidth, scaleY = (float)occHeight / (float)bufferHeight;

      //	int bx0 = (int)(scaleX * minX);
      //	int bx1 = (int)(scaleX * maxX);
      //	int by0 = (int)(scaleY * minY);
      //	int by1 = (int)(scaleY * maxY);

//          bx0 = std::max(0, std::min(bx0, occWidth - 1));
//          bx1 = std::max(0, std::min(bx1, occWidth - 1));
//          by0 = std::max(0, std::min(by0,  occHeight - 1));   
//          by1 = std::max(0, std::min(by1,  occHeight - 1));

//          bx0 = std::max(0            , bx0 - 2);
//          bx1 = std::min(occWidth - 1 , bx1 + 2);
//          by0 = std::max(0            , by0 - 2);
//          by1 = std::min(occHeight - 1, by1 + 2);

//          bool visible = false;
//          float bias = 0.001f;
//          for (int y = by0; y <= by1 && !visible; y++) {
//              for (int x = bx0; x <= bx1; x++) {
//                  float storedDepth = occ_buffer[y * occWidth + x];

//                  if (storedDepth > minDepth + bias) {
//                      // This pixel is farther than the chunk → chunk is visible here
//                      visible = true;
//                      break;
//                  }
//              }
//          }

//          if (visible)
//          {
//              for (int y = by0; y <= by1; y++)
//              {
//                  for (int x = bx0; x <= bx1; x++)
//                  {
//                      occ_buffer[y * occWidth + x] = minDepth;
//                  }
//              }

//              chunk->mesh->renderMesh(); // draw it normally
//          }
//      }

   // void renderWholeWorld() {
   //     for (auto it = world.chunkData.begin(); it != world.chunkData.end(); ) {
   //         bool chunkErased = false;

   //         if (!it->second) {
   //             it = world.chunkData.erase(it);
   //             continue;
   //         }
   //         auto& chunk = it->second;
   //         ivec2 coords = chunk->coords();

   //         if (!(chunk->neighboursPresent & 1)) {
   //             if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
   //                 int dirsX[] = { -1, 0, 0, 1 }, dirsY[] = { 0, -1, 1, 0 };
   //                 for (int i = 0; i < 4; i++) {
   //                     if (world.chunkData.count(pack(coords + ivec2(dirsX[i], dirsY[i]))) > 0) {
   //                         chunk->neighboursPresent |= (1 << (i + 1));
   //                     }
   //                 }
   //             }
   //             if (chunk->neighboursPresent == 0x1E) { // 1 1110
   //                 {
   //                     std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
   //                     chunkUpdateRequestQueue.push(coords);
   //                 }
   //                 chunkUpdateCV.notify_one();
   //                 chunk->neighboursPresent |= 1;

   //             } //if (!(chunk->neighboursPresent & 1)) continue;
   //         }
   //         if ((coords.x >= firstCamera.getPosition().x / CHUNK_SIZE - renderDistance * 4 && coords.x <= firstCamera.getPosition().x / CHUNK_SIZE + renderDistance * 4) &&
   //             (coords.y >= firstCamera.getPosition().z / CHUNK_SIZE - renderDistance * 4 && coords.y <= firstCamera.getPosition().z / CHUNK_SIZE + renderDistance * 4)) {
   //             if (chunk->unloaded) {
   //                 { chunkUpdateRequestQueue.push(chunk->coords()); /*std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex); */ }
   //                 //subchunk.needUpdate = true;
   //                 chunk->unloaded = false;
   //             }
   //             for (int i = 0; i < chunk->subchunks.size(); i++) {
   //                 auto& subchunk = chunk->subchunks[i];
   //                 //cout << i << " " << subchunk.height() << endl;
   //                 //cout << "rendering chunk at " << subchunk.height() << endl;
   //                 if (subchunk.mesh) {
   //                     //cout << "rendering chunk at " << subchunk.height() << endl;
   //                     vec3 chCenter = vec3((coords.x + 0.5) * CHUNK_SIZE, /*firstCamera.getPosition().y*/(i + 0.5) * CHUNK_SIZE, (coords.y + 0.5) * CHUNK_SIZE);
			//			vec3 minAABB = vec3(coords.x * CHUNK_SIZE, i * CHUNK_SIZE, coords.y * CHUNK_SIZE);
			//			vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);
   //                     if (!aabbIntersectsFrustum(minAABB, maxAABB)) {
   //                         continue;
   //                     }

   //                     if (mainWindow.getKeys()[GLFW_KEY_M])
   //                         subchunk.mesh->renderMeshAsLines();
   //                     else
   //                         subchunk.mesh->renderMesh();
   //                 } //3 fev 11h30
   //                 else {
   //                     if (chunk->getDirty()) {
   //                         if (subchunk.mesh) {//&& chunk->mesh->vertices.size()
   //                             //chunk->mesh->vertices.clear();
   //                             //chunk->mesh->indices.clear();
   //                             subchunk.mesh.reset();
   //                             chunk->unloaded = true;
   //                         }
   //                     }
   //                     else {
   //                         chunkCoords.erase(to(coords));
   //                         it = world.chunkData.erase(it);
   //                         chunkErased = true;
   //                         break;
   //                         //continue;
   //                     }
   //                 }
   //             }
   //         }
			//if (chunkErased) continue;
   //         it++;
   //     }
   // }

   // void renderWholeShadowWorld() {
   //     for (auto& chunks : world.chunkData) {
   //         if (!chunks.second) continue;
   //         ivec2 coords = chunks.second->coords();
   //         unique_ptr<Chunk>& chunk = chunks.second;
   //         for (int i = 0; i < chunk->subchunks.size(); i++) {
   //             auto& subchunk = chunk->subchunks[i];
   //             if (subchunk.mesh) {
   //                 if ((coords.x >= firstCamera.getPosition().x / CHUNK_SIZE - renderDistance * 8 && coords.x <= firstCamera.getPosition().x / CHUNK_SIZE + renderDistance * 8) &&
   //                     (coords.y >= firstCamera.getPosition().z / CHUNK_SIZE - renderDistance * 8 && coords.y <= firstCamera.getPosition().z / CHUNK_SIZE + renderDistance * 8)) {

   //                     vec3 minAABB = vec3(coords.x * CHUNK_SIZE, i * CHUNK_SIZE, coords.y * CHUNK_SIZE);
   //                     vec3 maxAABB = minAABB + vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);
   //                     if (!aabbIntersectsFrustum(minAABB, maxAABB)) {
   //                         continue;
   //                     }
   //                     subchunk.mesh->renderMesh();
   //                 }
   //             }
   //         }
   //     }
   // }



    //void renderClouds() {
    //    for (auto& chunks : world.chunkData) {
    //        if (!chunks.second) continue;
    //        ivec2 coords = chunks.second->coords();
    //        unique_ptr<Chunk>& chunk = chunks.second;
    //        unique_ptr<Mesh>& chmesh = chunk->cloudmesh->mesh;
    //        if (chunk->mesh && chmesh) {
    //            if ((coords.x >= firstCamera.getPosition().x / CHUNK_SIZE - renderDistance * 2 && coords.x <= firstCamera.getPosition().x / CHUNK_SIZE + renderDistance * 2) &&
    //                (coords.y >= firstCamera.getPosition().z / CHUNK_SIZE - renderDistance * 2 && coords.y <= firstCamera.getPosition().z / CHUNK_SIZE + renderDistance * 2)) {
    //
    //                if (chunk->updateCloud()) {
    //                    chmesh->createMesh(chmesh->vertices, chmesh->indices, chmesh->vertices.size(), chmesh->indices.size());
    //                    chunk->stopCloud();
    //                }
    //                chmesh->renderMesh();
    //            }
    //        }
    //    }
    //}

    void directionalShadowPass(DirectionalLight* light, mat4 model) {
        directionalShadowShader->useShader();
        directionalShadowShader->setDirectionalLightTransform(light->calcLightTransform());
        glUniformMatrix4fv(directionalShadowShader->getModelLocation(), 1, GL_FALSE, value_ptr(model));
        light->shadow_map->write();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadowWorld();
        //renderWholeShadowWorld();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shaders[0]->useShader();
        shaders[0]->setDirectionalLightTransform(light->directionalLightTransform);
    }////

    int ftoint(float num) {
        return num >= 0 ? num : num - 1;
    }

    bool playerCollides() {
        return blockExistsAt(vec3(ftoint(firstCamera.getPosition().x - boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z - boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x - boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z + boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x + boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z + boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x + boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z - boundL / 2 + 0.5)), 1);
    }

    void keyControl(float dt) {
        bool press_repeat = mainWindow.keyWasRepeated(GLFW_KEY_W);
        if (mainWindow.getKeys()[GLFW_KEY_CAPS_LOCK] || press_repeat) {
            deltaTime = 4;
        }
        else {
            deltaTime = 2;
        }
        vec3& position = firstCamera.getPosition(1),
            right = firstCamera.getRight(),
            front = firstCamera.getFront(),
            & initial_velocity = firstCamera.initial_velocity;
        if (mainWindow.getKeys()[GLFW_KEY_W]) {
            vec3 checkPosX0 = firstCamera.getPosition() + vec3(front.x * movementSpeed * deltaTime, -1, 0),
                checkPosZ0 = firstCamera.getPosition() + vec3(0, -1, front.z * movementSpeed * deltaTime),
                checkPosX1 = firstCamera.getPosition() + vec3(front.x * movementSpeed * deltaTime, 0, 0),
                checkPosZ1 = firstCamera.getPosition() + vec3(0, 0, front.z * movementSpeed * deltaTime);
            if (!blockExistsAt(checkPosX0 + vec3(0.5, 0, 0), 1) && !blockExistsAt(checkPosX1 + vec3(0.5, 0, 0)), 1) position.x = checkPosX0.x;
            if (!blockExistsAt(checkPosZ0 + vec3(0, 0, 0.5), 1) && !blockExistsAt(checkPosZ1 + vec3(0, 0, 0.5)), 1) position.z = checkPosZ0.z;
        }
        if (mainWindow.getKeys()[GLFW_KEY_S]) {
            position -= vec3(front.x * movementSpeed * deltaTime, 0, front.z * movementSpeed * deltaTime);
        }
        if (mainWindow.getKeys()[GLFW_KEY_A]) {
            position -= right * movementSpeed * deltaTime;
        }
        if (mainWindow.getKeys()[GLFW_KEY_D]) {
            position += right * movementSpeed * deltaTime;
        }
        if (mainWindow.getKeys()[GLFW_KEY_SPACE]) {
            initial_velocity.y = 100 * 3 * movementSpeed;
        }
        if (mainWindow.getKeys()[GLFW_KEY_LEFT_SHIFT]) {
            vec3 checkPos = firstCamera.getPosition() + vec3(0, -movementSpeed * deltaTime, 0);
            if (!blockExistsAt(checkPos - vec3(0, 1, 0))) position.y = checkPos.y;
        }
        if (mainWindow.getKeys()[GLFW_KEY_LEFT_CONTROL]) {
            movementSpeed *= 1.4;
            mainWindow.getKeys()[GLFW_KEY_LEFT_CONTROL] = false;
        }
        if (mainWindow.getKeys()[GLFW_KEY_RIGHT_CONTROL]) {
            movementSpeed /= 1.2;
            mainWindow.getKeys()[GLFW_KEY_LEFT_CONTROL] = false;
        }
        firstCamera.calculateCamPos(dt);

        if (mainWindow.getKeys()[GLFW_KEY_L]) {
            position = vec3(1000.0f, 100.0f, 1000.0f);
        }
    }

    void generateSpiral(vector<ivec2>& spiral, int n = 1) {
        int radius = renderDistance * n;
        spiral.clear();
        int x = 0, y = 0;
        int dx = 0, dy = -1;

        int max = radius * radius * 4;
        for (int i = 0; i < max; i++) {
            if (abs(x) <= radius && abs(y) <= radius)
                spiral.emplace_back(x, y);

            if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y)) {
                int temp = dx;
                dx = -dy;
                dy = temp;
            }

            x += dx;
            y += dy;
        }
    }

//    static void mainGameKeyCallback(int key, int scancode, int action, int mods) {
//        bool& handGesture = game->handGesture,
//            swing = game->swing;
//
//        float& breakAngle = game->breakAngle,
//            projAngle = game->projAngle;
//
//        int& renderDistance = game->renderDistance,
//            fpscount = game->fpscount,
//            breaking = game->breaking;
//
//        LightMesh& headMesh = game->headMesh,
//            lookingMesh = game->lookingMesh,
//            compassMesh = game->compassMesh;
//
//        mat4& model = game->model, projection = game->projection, view = game->view,
//            ortho = game->ortho;
//
//        Block& breakingBlock = game->breakingBlock;
//
//
//        json Jitems = game->Jitems, tools = game->tools, player = game->player;
//
//        auto& start = game->start;
//
//        Block& lookBlock = game->lookBlock;
//		Projectile& ball = game->ball;
//		mat4& breakModel = game->breakModel;
//		int& person_view = game->person_view;
//
//        vec3& headPos = game->headPos, headFront = game->headPos;
//		abyte& count = game->count;
//		abyte& count_time = game->count_time;
//
//        Text& cursorPos = game->cursorPos;
//        Text& position = game->position;
//        Text& craftedItemName = game->craftedItemName;
//
//        if (!inventory.mainInventoryOn && !inventory.craftingTableInventoryOn) {
//            if (key == GLFW_KEY_S) {
//                ifstream itemsJSON("items.json");
//                if (!itemsJSON) {
//                    ofstream outJSON("items.json");
//                    outJSON.close();
//                    itemsJSON.open("items.json");
//                }
//                itemsJSON >> Jitems;
//
//                ifstream toolsJSON("tools.json");
//                if (!toolsJSON) {
//                    ofstream outJSON("tools.json");
//                    outJSON.close();
//                    toolsJSON.open("tools.json");
//                }
//                toolsJSON >> tools;
//            }
//            if (key == GLFW_KEY_P) {
//                if (mainWindow.getKeys()[GLFW_KEY_I]) {
//                    inventory.inf_blocks = true;
//                }
//                else if (mainWindow.getKeys()[GLFW_KEY_N]) {
//                    inventory.inf_blocks = false;
//                }
//
//                if (mainWindow.getKeyPressed() > GLFW_KEY_1 && mainWindow.getKeyPressed() <= GLFW_KEY_9) world.addBlocklook_at(items[mainWindow.getKeyPressed() - GLFW_KEY_1]);
//                else world.addBlocklook_at(inventory.mainInventorySlots[3][slot].item);
//                if (mainWindow.getKeys()[GLFW_KEY_9]) world.addBlocklook_at(item(OAK_PLANK.id));
//                if (mainWindow.getKeys()[GLFW_KEY_1]) world.addBlocklook_at(item(TORCH.id));
//            }
//            if (key == GLFW_KEY_T || mainWindow.leftClickButtonPressed()) {
//                handGesture = (breakAngle <= -30.f || breakAngle >= 30.f);
//                if (handGesture) swing ^= 1;
//                Item currentTool = inventory.hotbarSlots[slot].item;
//                int itemSoftness = Jitems["items"][itemTypeString[breakingBlock.type.id]]["speed"];
//                int toolSpeed = 1;
//                if (currentTool.isTool()) { toolSpeed = (tools["tools"][itemTypeString[currentTool.id]]["speed"]); }
//                glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreaking"), 5 * (breaking) / itemSoftness);
//                breaking += toolSpeed;
//                if (breakingBlock == lookBlock && itemSoftness != -1) {
//                    if (!(breaking % itemSoftness)) {
//                        breakReqQueue.push(vec3(1.0f));
//                        if (!blockBreakingOut) {
//                            blockBreakingOut = true;
//                        }
//                        breaking = 0;
//                    }
//                }
//                else {
//                    breaking = 0;
//                }
//
//                breakingBlock = lookBlock;
//
//                breakAngle += swing ? 5 : -5;
//                breakModel = rotate(mat4(1.f), -radians(breakAngle), vec3(0, 0, 1))
//                    * translate(mat4(1.f), vec3(breakAngle, 2 * breakAngle, 0))
//                    ;
//            }
//            else {
//                glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "uniformBreaking"), 0);
//            }
//
//            if (mainWindow.rightClickButtonPressed()) {
//                handGesture = (breakAngle <= -30.f || breakAngle >= 30.f);
//                if (!recipe.itemUsable(lookBlock.type)) {
//                    if (mainWindow.getKeys()[GLFW_KEY_I]) {
//                        inventory.inf_blocks = true;
//                    }
//                    else if (mainWindow.getKeys()[GLFW_KEY_N]) {
//                        inventory.inf_blocks = false;
//                    }
//                    if (inventory.mainInventorySlots[3][slot].item != AIR && recipe.itemPlaceable(inventory.mainInventorySlots[3][slot].item)) {
//                        {
//                            placeReqQueue.push(vec3(1.0f));
//                            blockPlacingOut = true;
//                        }
//                    }
//                }
//                else {
//                    inventory.craftingTableInventoryOn = true;
//                }
//
//                breakAngle += swing ? 5 : -5;
//                breakModel = rotate(mat4(1.f), -radians(breakAngle), vec3(0, 0, 1))
//                    * translate(mat4(1.f), vec3(breakAngle, 2 * breakAngle, 0))
//                    ;
//            }
//            if (!breaking) { breakModel = mat4(1); breakAngle = breaking; }
//
//            if (key == GLFW_KEY_CAPS_LOCK) {
//                projAngle = (projAngle < 90.0) ? projAngle + 4 : 90.0;
//            }
//            else {
//                projAngle = (projAngle > 45.0) ? projAngle - 4 : 45.0;
//            }
//            if (key == GLFW_KEY_Q) {
//                int xdrop, ydrop;
//                if (inventory.mainInventoryOn) {
//                    xdrop = (int)slotX, ydrop = 3 - (int)slotY;
//                }
//                else {
//                    xdrop = slot, ydrop = 3;
//                }
//                if (inventory.mainInventorySlots[ydrop][xdrop].item != AIR) {
//                    dropped.push_back(Projectile());
//                    dropped.back().shoot(firstCamera.getPosition() + normalize(firstCamera.getFront()), vec3(firstCamera.getFront().x, 0.25, firstCamera.getFront().z), inventory.mainInventorySlots[ydrop][xdrop].item);
//                    dropped.back().mesh = world.createProjectileMesh(vec3(0), -4.0f, inventory.mainInventorySlots[ydrop][xdrop].item);
//                    inventory.mainInventorySlots[ydrop][xdrop].count--;
//                    inventory.invDidChange(1);
//                }
//            }
//
//            if (key == GLFW_KEY_U) {
//                ball.shoot(firstCamera.getPosition(), firstCamera.getFront(), CLOUD, vec3(1.0f));
//                ball.mesh = world.createMeshCube(vec3(0), -4.0f, ball.item);
//            }
//
//            if (key == GLFW_KEY_F3) {
//                std::this_thread::sleep_for(std::chrono::milliseconds(200));
//                person_view = (++person_view % 3);
//            }
//
//            if (key == GLFW_KEY_F5) {
//                mat4 compassModel = translate(mat4(1.0f), headPos + headFront);
//                glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(compassModel));
//                compassMesh.renderMesh();
//                glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
//                cursorPos.replaceWord("cursor position: x = " + to_string(cursor.x) + ", y = " + to_string(cursor.y), vec3(0, 1, 0), vec2(50, 1500));
//                position.drawText(ortho), craftedItemName.drawText(ortho), cursorPos.drawText(ortho);
//
//                position.replaceWord("position  x: "
//                    + to_string((int)headPos.x) + " y: "
//                    + to_string((int)headPos.y) + " z: "
//                    + to_string((int)headPos.z) + " cursor count -> "
//                    + to_string(cursor.count) + " block : " + itemTypeString[cursor.item.id]
//                    + "\n"
//                    + "looking at "
//                    + itemTypeString[lookBlock.type.id]
//                    + "\n"
//                    + "FPS : " + to_string(fpscount)
//                    , vec3(0.4, 1, 0.7));
//
//                craftedItemName.replaceWord("main craft slot 1 contains: " + itemTypeString[craftedItem.item.id]
//                    + ", " + to_string(inventory.mainCraftingSlots[0][1].count) + (inventory.mainCraftingSlots[0][1].count <= 1 ? " item" : " items")
//                    + (inventory.invChange() ? " inventory updating...." : " inventory up to date! "
//                        + to_string(renderDistance) + " render distance"), normalize(vec3(1.3, 1, 0)), vec2(50, 1550));
//
//                if (!(count_time % 10)) {
//                    auto end = chrono::high_resolution_clock::now();
//                    double frame_duration(chrono::duration<double>(end - start).count());
//                    fpscount = (int(1 / frame_duration));
//                }
//                count_time++;
//            }
//
//            if (key == GLFW_KEY_ENTER) {
//                json jsondata;
//                ifstream ifs("renderdist.json");
//                ifs >> jsondata;
//                renderDistance = jsondata["renderdistance"];
//                game->generateSpiral(spiral);
//            }
//
//            if (!breakResQueue.empty()) {
//                Block droppedItem = breakResQueue.front();
//                dropped.back().shoot(droppedItem.position, vec3(-firstCamera.getFront().x, 0.5, -firstCamera.getFront().z), droppedItem.type);
//                dropped.back().mesh = world.createProjectileMesh(vec3(0), -4.0f, droppedItem.type);
//                dropped.push_back(Projectile());
//                breakResQueue.pop();
//            }
//        }
//	}
};

//void mainKeyCallback(int key, int code, int action, int mode) {
// //   Game* game = new Game();
//	//game->mainGameKeyCallback(key, code, action, mode);
//}