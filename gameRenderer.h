#pragma once

class Game;

vector<int> M(20);
std::vector<glm::ivec2> spiral, cloudSpir;

class Game {
public:
    vector<vec4> chCenter_Radii;
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

    uint count = 0, count_time = 0;

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
            ofstream outJSON("player.json", ios::app);
            json emptyPlayer;
            emptyPlayer["player"]["x"] = 0;
            emptyPlayer["player"]["y"] = 128;
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
        bufferWidth = mainWindow.getBufferWidth();
        bufferHeight = mainWindow.getBufferHeight();

        mainWindow.disableMouse();

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

        itemProj = perspective(radians(1.0f), 1.f/*(float)1920 / float(1080)*/, 0.01f, 1500.0f),
            itemView = lookAt(vec3(0, 0, 400), vec3(0), vec3(0, 1, 0)),
            currentBlockView = lookAt(vec3(0, 0, 1400), vec3(0), vec3(0, 1, 0));

        itemModel = scale(mat4(1.0f), vec3(0.08f, 0.1f, 0.08f)) *
            rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1)) *
            rotate(mat4(1.0f), radians(30.0f), vec3(1, 0, 0)) *
            rotate(mat4(1.0f), radians(45.0f), vec3(0, 1, 0))
            ;

        breakModel = mat4(1.0f);

        loadInventory();
        inventory.initInventorySlots();
        sky.buildSky();


        for (int i = 0; i < 1; ++i) {
            workers.push_back(thread(chunkWorker)); // worker thread is somewhere in threading.h
            workers.push_back(thread(updateChunkJob));
        }

        workers.push_back(thread(blockBreakThreadWorker));
        workers.push_back(thread(blockPlaceThreadWorker));

        //for (int i = 0; i < 1; ++i) {
        //    workers.push_back(thread(chunkMeshSchedWorker));
        //}

        mainLight = DirectionalLight(mainWindow.getBufferWidth(), mainWindow.getBufferHeight(),
            1.0f, 1.0f, 1.0f,
            0.8f, 0.5f,
            0.0f, -CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, CHUNK_SIZE);//CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, CHUNK_SIZE

        auxLight = DirectionalLight(100, 100,
            1.0f, 1.0f, 1.0f,
            0.7f, 0.5f,
            -1.0f, 1.5f, 0.0f);

        position.model = translate(mat4(1.0f), vec3(50, 1700, 0));

        mainLight.setShadowPos(firstCamera.getPosition());

        glClearColor(1, 1, 1, 0);
        meshSchedCV.notify_all();

        activeCamera = &firstCamera;
    }

    void run() {
        while (!mainWindow.getShouldClose()) {
            shaders[0]->useShader();
            auto startframe = chrono::high_resolution_clock::now();
            static bool breakblockdb = 0, placeblockdb = 0, invtoggledb = 0; // db = debounce
            static int angletest = 0;
            static double frame_duration_calc;
			static int spiralCount = 0;
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
            if (person_view % 4 < 2)
                activeCamera->setFront(firstCamera.getFront());

            for (; spiralCount <= spiral.size(); spiralCount++) {
                spiralCount = (spiralCount >= spiral.size()) ? 0 : spiralCount;
				ivec2 chunkOff = spiral[spiralCount];
                ivec2 camChunkPos = ivec2(floorDiv(firstCamera.getPosition().x, CHUNK_SIZE), floorDiv(firstCamera.getPosition().z, CHUNK_SIZE));
                ivec2 chunkPos = camChunkPos + chunkOff; // This is what triggers chuk
				uint  chCrds = pack(chunkPos);
                if (chunkCoords.count(chCrds) == 0) {
                    chunkCoords.insert(chCrds);
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        chunkRequestQueue.push(chCrds);
                    }
                    queueCV.notify_one();
                }
                if (!(++count % 16)) { break; }
            }

            while (!chunkMeshResult.empty()) {
                chNeighResult* chNeighRes;
                {
                    std::lock_guard<std::mutex> lock(chunkMeshQueueMutex);
                    chNeighRes = chunkMeshResult.front();
                    chunkMeshResult.pop();
                }
                auto it = world.chunkData.find(chNeighRes->coords); //if (it == world.chunkData.end()) { delete chNeighRes; continue; }
                auto& chunk = it->second;
                chunk->mesh->createMesh(chNeighRes->mesh->vertices, chNeighRes->mesh->indices);
                delete chNeighRes;

                if (!(++count % 16)) break; // To mesh as fast as ossible, donot cap n_o chunks meshed per frame.
            }

            while (!chunkResultQueue.empty()) {
                chPack ch;
                {
                    lock_guard<mutex> lock(addChunkMutex);
                    ch = chunkResultQueue.front();
                    chunkResultQueue.pop();
                }
                world.chunkData.try_emplace(ch.coords, move(ch.chPtr));
                if (!(++count % 16)) break;
            }

            VP = projection * firstCamera.calcViewMatrix();
            mat4 VP_t = VP;// transpose(VP);
            extractFrustumPlanes(VP_t);

            Textures[BLOCK_TEX]->useTexture();

            directionalShadowPass(&mainLight, model); //binds base shader shaders[0] as well
            glUniform3f(glGetUniformLocation(shaders[0]->getShaderId(), "camPos"), firstCamera.getPosition().x, firstCamera.getPosition().y, firstCamera.getPosition().z);
            glUniform1f(glGetUniformLocation(shaders[0]->getShaderId(), "fogStart"), 0.72 * CHUNK_SIZE * renderDistance);
            glUniform1f(glGetUniformLocation(shaders[0]->getShaderId(), "fogEnd"), 0.75 * CHUNK_SIZE * renderDistance);

            mainLight.getShadowMap()->read(GL_TEXTURE1);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "directionalShadowMap"), 1);

            Textures[BREAK_STAGE_TEX]->useTexture(GL_TEXTURE3);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "breakStageTexture"), 3);

            Textures[FOLL_TEX]->useTexture(GL_TEXTURE4);
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "grassColorTexture"), 4);

            //isolateWorld();
            //scheduleMeshWorld();
            //renderWorld();

            render();

            headPos = firstCamera.getPosition(), headFront = firstCamera.getFront();
            lookBlock = getBlockAt(lookingAtBlock());

            view = activeCamera->calcViewMatrix();
            projection = perspective(radians(projAngle), (float(mainWindow.getBufferWidth()) / float(mainWindow.getBufferHeight())), 0.01f, float(renderDistance * CHUNK_SIZE));

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
                if (person_view == 3 && !mainWindow.getKeys()[GLFW_KEY_1])
                    spectateCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
                else
                    firstCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
                thirdCamera_back.mouseControl(-mainWindow.getXChange(), mainWindow.getYChange());
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
                    person_view = (++person_view % 4);
					if (person_view == 3) {
						spectateCamera.setPosition(firstCamera.getPosition());
						spectateCamera.setFront(firstCamera.getFront());
						spectateCamera.setGravity(0.0f);
					}
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
                        + itemTypeString[lookBlock.type.id] + "Amount of dropped items : " + to_string(dropped.size())
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
            }

            if (person_view == 0) {
                activeCamera = &firstCamera;
            }
            else if (person_view == 1) {
                activeCamera = &thirdCamera_back;
            }
            else if (person_view == 3) {                
                activeCamera = &spectateCamera;
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


            Textures[BLOCK_TEX]->useTexture();
            Textures[TOP_TEX]->useNextTexture();
            glUniform1i(glGetUniformLocation(shaders[0]->getShaderId(), "topTexture"), 2);
            for (int dropIdx = 0; dropIdx < dropped.size(); dropIdx++) {
                Projectile& drop = dropped[dropIdx];
                if (drop.item.isTool()) {
                    Textures[TOOLS_TEX]->useTexture();
                }
                else {
                    Textures[BLOCK_TEX]->useTexture();
                }
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
                    vec_erase(dropped, dropIdx);
                    continue;
                }

                drop.draw();
            }
            glUniformMatrix4fv(shaders[0]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            shaders[5]->useShader();
            view = activeCamera->calcViewMatrix();

            //For block highlighting
            //ivec3 lookPosition = lookingAtBlock();
            //if (lookPosition.y >= 0) {
            //    mat4 modelLooking = translate(mat4(1.0f), vec3(lookPosition));
            //    glUniformMatrix4fv(shaders[5]->getModelLocation(), 1, GL_FALSE, value_ptr(modelLooking));
            //    lookingMesh.renderMesh();
            //    glUniformMatrix4fv(shaders[5]->getModelLocation(), 1, GL_FALSE, value_ptr(model));
            //}

            glDisable(GL_DEPTH_TEST); // so crosshair draws on top although I need the crosshair to give inverted colors
            shaders[2]->useShader();
            glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

            if (person_view == 0) {
                crosshair.drawCrosshair();
            }

            //shaders[2]->useShader(); // the day that I coded for this I was not quite sure if it was good enough. Why do I like to write "The day"?
            Textures[SLOT_TEX]->useTexture();
            inventory.defineHotbarSlotSelectorGeometry();
            inventory.drawHotbarSlotSelector();

            glUniformMatrix4fv(shaders[2]->getOrthoLocation(), 1, GL_FALSE, glm::value_ptr(ortho));
            Textures[MAIN_INV_TEX]->useTexture();
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
            inventory.drawHotbar();
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (inventory.mainInventoryOn)
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
                firstCamera.mouseControl(lastXChange, lastYChange);
                inventory.invDidChange(1);
            }

            if (inventory.mainInventoryOn) {
                Textures[LARGE_INV_TEX]->useTexture();
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
            }

            for (int i = 0; i < 9; i++) {
                mat4 itemModel = scale(mat4(1.0f), vec3(0.1f, 0.12f, 0.1f)) * rotate(mat4(1.0f), radians(-90.0f), vec3(0, 0, 1)) *
                    ((!inventory.hotbarSlots[i].item.isFlat()) ? rotate(mat4(1.0f), radians(30.0f), vec3(1, 0, 0)) * rotate(mat4(1.0f), radians(45.0f), vec3(0, 1, 0)) : mat4(1.0f)) *
                    rotate(mat4(1.0f), radians(inventory.hotbarSlots[i].angle), vec3(0, 1, 0));
                inventory.hotbarSlots[i].angle += 0.5f;
                render3Din2D(itemModel, inventory.hotbarSlots[i].mesh, inventory.hotbarSlots[i].model, inventory.hotbarSlots[i].quadMesh, ortho, itemView, itemProj, inventory.hotbarSlots[i].item);
                inventory.hotbarSlots[i].textCount.drawText(ortho);
            }

            render3Din2D(itemModel * breakModel
                * rotate(mat4(1.0f), radians((!currentBlock.item.isTool() ? -35.f : 0.f)), vec3(1, 1, 1))
                * rotate(mat4(1.0f), radians((!currentBlock.item.isTool() ? 45.f : 200.f)), vec3(0, 1, 0))
                * rotate(mat4(1.0f), radians((!currentBlock.item.isTool() ? 0.f : 30.f)), vec3(0, 0, 1)),
                currentBlock.mesh,
                translate(mat4(1.0f), vec3((float)centerX + 600.f - 20.f * (firstCamera.getYaw() - lastYaw),
                    (float)centerY - 650.f - 20.f * (firstCamera.getPitch() - lastPitch) - 2 * (firstCamera.initial_velocity.y + firstCamera.velocity.y),
                    0.f)),
                currentBlock.quadMesh, ortho, currentBlockView, itemProj, currentBlock.item);

            angletest += 1;

            if (inventory.invChange()) { inventory.updateInventory(); }

            if (mainWindow.getShouldClose()) {
                storeInventory();
                json lastPlayer;
                lastPlayer["player"]["x"] = firstCamera.getPosition().x;
                lastPlayer["player"]["y"] = firstCamera.getPosition().y;
                lastPlayer["player"]["z"] = firstCamera.getPosition().z;
                ofstream playerJSON("player.json");
                playerJSON << lastPlayer.dump(4);

                chunkGenRunning = false;
                chunkUpdateCV.notify_all();
                stopChunkUpdaters = true;
                stopChunkScheders = true;
                queueCV.notify_all();
                blockPlacing = false;
                blockBreaking = false;
                
                stopChunkUpdaters = true;
                chunkGenRunning2 = false;
                chunkUpdateGenRunning = false;
                stopMeshing = false;

                for (auto& t : workers) {
                    cout << "Joining thread " << workers.size() << endl;
                    t.join();
                }

                return;
            }

            if (spawn <= 511) {
                spawn++;
            }

            bool onGround = playerCollides();
            if (!onGround && spawn > 511) {
                firstCamera.initial_velocity.y -= 0.5;
            }
            else if (onGround) {
                firstCamera.velocity = vec3(0);
                firstCamera.initial_velocity *= vec3(0);
                firstCamera.velocity_factor = vec3(1);
            }

            mainWindow.updateLastKeyPress();
            mainWindow.swapBuffers();

            auto endframe = chrono::high_resolution_clock::now();
            frame_duration_calc = (chrono::duration<double>(endframe - startframe).count());
        }

    }////

    template <typename T>
    void vec_erase(vector<T>& vec, int index) {
        vec[index] = vec.back();
        vec.pop_back();
    }

    void renderWorld() {
        vec3 playerPos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        vec2 _2dPlPosHi = vec2(playerPos.x, playerPos.z) + float(renderDistance),
            _2dPlPosLo = vec2(playerPos.x, playerPos.z) - float(renderDistance);

        for (auto it = world.chunkData.begin(); it != world.chunkData.end();) {
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();

            if ((coords.x <= _2dPlPosLo.x || coords.x >= _2dPlPosHi.x) ||
                (coords.y <= _2dPlPosLo.y || coords.y >= _2dPlPosHi.y))
            {
                chunkCoords.erase((chunk->coord));
                it = world.chunkData.erase(it);
                continue;
            }

            vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerPos.y, (coords.y + 0.5) * CHUNK_SIZE);
            if (sphereInFrustum(center, CHUNK_SIZE * 2))
                chunk->mesh->renderMesh();

            it++;
        }
    }

    void render() {
        //auto start = chrono::high_resolution_clock::now();
        vec3 playerPos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        vec2 _2dPlPosHi = vec2(playerPos.x, playerPos.z) + float(renderDistance),
            _2dPlPosLo = vec2(playerPos.x, playerPos.z) - float(renderDistance);

        for (auto it = world.chunkData.begin(); it != world.chunkData.end(); ) {
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();
            bool chunkReady = false;

            bool inUse = false;
            if (((coords.x <= _2dPlPosLo.x || coords.x >= _2dPlPosHi.x) ||
                (coords.y <= _2dPlPosLo.y || coords.y >= _2dPlPosHi.y)))
            {
                if (!chunk->safe_unload && chunk->inUse.compare_exchange_strong(chunkReady, true)) {
                    chunkCoords.erase((chunk->coord));
                    it = world.chunkData.erase(it);
                }
                else it++;
                continue;
            }

            if (chunk->getDirty()) {
                if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
                    for (int i = 0; i < 4; i++) {
                        ivec2 chcrds = coords + ivec2(dirs[i], dirs[i + 4]);
                        if (world.chunkData.count(pack(chcrds)) > 0)
                            chunk->neighboursPresent |= (1 << (i + 1));
                    }
                }
                if (chunk->neighboursPresent == 0x1E) { // 1 1110 = 0x1E = 30
                    chNeighPackPtr* chunkochunks = new chNeighPackPtr();
                    chunkochunks->coords = chunk->coord;
                    chunkochunks->mainChunk = chunk.get();
                    {
                        std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
                        chunkCleanupQueue.push(chunkochunks);
                    }
                    chunkUpdateCV.notify_one();
                    chunk->setAsClean();
                }
            }

            vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerPos.y, (coords.y + 0.5) * CHUNK_SIZE);
            if (sphereInFrustum(center, playerPos.y / 2))
                chunk->mesh->renderMesh();

            it++;
        }
        //auto end = chrono::high_resolution_clock::now();
        //double duration = chrono::duration<double>(end - start).count();
        //cout << "Render duration: " << duration << " seconds" << endl;
    }

    void renderShadowWorld() {
        vec3 playerpos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        vec2 _2dPlPosHi = vec2(playerpos.x, playerpos.z) + float(renderDistance) * 1.f,
            _2dPlPosLo = vec2(playerpos.x, playerpos.z) - float(renderDistance) * 1.f;

        for (auto& chunks : world.chunkData) {
            auto& chunk = chunks.second;
            ivec2 coords = chunk->coords();

            if ((coords.x >= _2dPlPosLo.x && coords.x <= _2dPlPosHi.x) &&
                (coords.y >= _2dPlPosLo.y && coords.y <= _2dPlPosHi.y)) {
                vec3 center = vec3((coords.x + 0.5) * CHUNK_SIZE, playerpos.y, (coords.y + 0.5) * CHUNK_SIZE);
                if (sphereInFrustum(center, CHUNK_SIZE))
                    chunk->mesh->renderMesh();
            }
        }
    }

    void isolateWorld() {
        vec3 playerPos = firstCamera.getPosition() / vec3(CHUNK_SIZE, 1, CHUNK_SIZE);
        vec2 _2dPlPosHi = vec2(playerPos.x, playerPos.z) + float(renderDistance),
            _2dPlPosLo = vec2(playerPos.x, playerPos.z) - float(renderDistance);

        for (auto it = world.chunkData.begin(); it != world.chunkData.end() &&
            ((it->second->coords().x <= _2dPlPosLo.x || it->second->coords().x >= _2dPlPosHi.x) ||
                (it->second->coords().y <= _2dPlPosLo.y || it->second->coords().y >= _2dPlPosHi.y)); ) {
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();
            bool chunkReady = false;

            if (!chunk->safe_unload && chunk->inUse.compare_exchange_strong(chunkReady, true)) {
                chunkCoords.erase((chunk->coord));
                it = world.chunkData.erase(it);
            }
            else it++;
        }
    }

    void scheduleMeshWorld() {
        for (auto it = world.chunkData.begin(); it != world.chunkData.end() && it->second->getDirty(); it++) {
            auto& chunk = it->second;
            ivec2 coords = chunk->coords();

            if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
                for (int i = 0; i < 4; i++) {
                    ivec2 chcrds = coords + ivec2(dirs[i], dirs[i + 4]);
                    if (world.chunkData.count(pack(chcrds)) > 0)
                        chunk->neighboursPresent |= (1 << (i + 1));
                }
            }
            if (chunk->neighboursPresent == 0x1E) { // 1 1110 = 0x1E = 30
                chNeighPackPtr* chunkochunks = new chNeighPackPtr();
                chunkochunks->coords = chunk->coord;
                chunkochunks->mainChunk = chunk.get();
                {
                    std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
                    chunkCleanupQueue.push(chunkochunks);
                }
                chunkUpdateCV.notify_one();
                chunk->setAsClean();
            }
        }
    }

    static void chunkMeshSchedWorker() {
        int count = 0;
        while (!stopChunkScheders) {
            for (auto it = world.chunkData.begin(); it != world.chunkData.end(); it++) {
                if (it->second->getDirty() && !(count++ % 2)) {
                    auto& chunk = it->second;
                    ivec2 coords = chunk->coords();

                    if ((chunk->neighboursPresent & 0x1E) != 0x1E) {
                        for (int i = 0; i < 4; i++) {
                            ivec2 chcrds = coords + ivec2(dirs[i], dirs[i + 4]);
                            if (world.chunkData.count(pack(chcrds)) > 0)
                                chunk->neighboursPresent |= (1 << (i + 1));
                        }
                    }
                    if (chunk->neighboursPresent == 0x1E) { // 1 1110 = 0x1E = 30
                        chNeighPackPtr* chunkochunks = new chNeighPackPtr();
                        chunkochunks->coords = chunk->coord;
                        chunkochunks->mainChunk = chunk.get();
                        {
                            std::lock_guard<std::mutex> lock(chunkUpdateRequestMutex);
                            chunkCleanupQueue.push(chunkochunks);
                        }
                        chunkUpdateCV.notify_one();
                        chunk->setAsClean();
                    }
                }
            }
        }
    }

    void directionalShadowPass(DirectionalLight* light, mat4 model) {
        directionalShadowShader->useShader();
        directionalShadowShader->setDirectionalLightTransform(light->calcLightTransform());
        glUniformMatrix4fv(directionalShadowShader->getModelLocation(), 1, GL_FALSE, value_ptr(model));
        light->shadow_map->write();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadowWorld();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shaders[0]->useShader();
        shaders[0]->setDirectionalLightTransform(light->directionalLightTransform);
    }////

    int ftoint(float num) {
        return num >= 0 ? num : num - 1;
    }

    bool playerCollides() {
        return  blockExistsAt(vec3(ftoint(firstCamera.getPosition().x - boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z - boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x - boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z + boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x + boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z + boundL / 2 + 0.5)), 1) ||
            blockExistsAt(vec3(ftoint(firstCamera.getPosition().x + boundW / 2 + 0.5), firstCamera.getPosition().y - 1.5, ftoint(firstCamera.getPosition().z - boundL / 2 + 0.5)), 1);
    }

    void keyControl(float dt) {
		Camera* activeCamera = person_view == 3 ? &spectateCamera : &firstCamera;
        bool press_repeat = mainWindow.keyWasRepeated(GLFW_KEY_W);
        if (mainWindow.getKeys()[GLFW_KEY_CAPS_LOCK] || press_repeat) {
            deltaTime = 4;
        }
        else {
            deltaTime = 2;
        }
        vec3& position = activeCamera->getPosition(1),
            right = activeCamera->getRight(),
            front = activeCamera->getFront(),
            & initial_velocity = activeCamera->initial_velocity;
        if (mainWindow.getKeys()[GLFW_KEY_W]) {
            vec3 checkPosX0 = activeCamera->getPosition() + vec3(front.x * movementSpeed * deltaTime, -1, 0),
                checkPosZ0 = activeCamera->getPosition() + vec3(0, -1, front.z * movementSpeed * deltaTime),
                checkPosX1 = activeCamera->getPosition() + vec3(front.x * movementSpeed * deltaTime, 0, 0),
                checkPosZ1 = activeCamera->getPosition() + vec3(0, 0, front.z * movementSpeed * deltaTime);
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
			if (person_view == 3)
                position += vec3(0, movementSpeed * deltaTime, 0);
            else
                initial_velocity.y = 100 * 3 * movementSpeed;
        }
        if (mainWindow.getKeys()[GLFW_KEY_LEFT_SHIFT]) {
            vec3 checkPos = activeCamera->getPosition() + vec3(0, -movementSpeed * deltaTime, 0);
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
        activeCamera->calculateCamPos(dt);

        if (mainWindow.getKeys()[GLFW_KEY_L]) {
            position.y = 100;
            if (mainWindow.getKeys()[GLFW_KEY_B]) {
                position = vec3(1000.0f, 100.0f, 1000.0f);
            }
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

    void loadInventory() {
        json inventoryData;
        ifstream ifs("inventory.json");

        ifs >> inventoryData;
        for (int i = 0; i < 9; i++) {
            inventory.hotbarSlots[i].item.id = inventoryData["mainInventory"][3][i]["id"];
            inventory.hotbarSlots[i].count = inventoryData["mainInventory"][3][i]["count"];
        }
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 9; j++) {
                inventory.mainInventorySlots[i][j].item.id = inventoryData["mainInventory"][i][j]["id"];
                inventory.mainInventorySlots[i][j].count = inventoryData["mainInventory"][i][j]["count"];
            }

        inventory.invDidChange(1);
    }

    void storeInventory() {
        json inventoryData;
        for (int i = 0; i < 9; i++) {
            inventoryData["mainInventory"][3][i]["item"] = itemTypeString[inventory.hotbarSlots[i].item.id];
            inventoryData["mainInventory"][3][i]["count"] = inventory.hotbarSlots[i].count;
            inventoryData["mainInventory"][3][i]["id"] = (int)inventory.hotbarSlots[i].item.id;
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 9; j++) {
                inventoryData["mainInventory"][i][j]["item"] = itemTypeString[inventory.mainInventorySlots[i][j].item.id];
                inventoryData["mainInventory"][i][j]["count"] = inventory.mainInventorySlots[i][j].count;
                inventoryData["mainInventory"][i][j]["id"] = (int)inventory.mainInventorySlots[i][j].item.id;
            }
        }
        ofstream outFile("inventory.json");
        outFile << inventoryData.dump(4);
    }
};