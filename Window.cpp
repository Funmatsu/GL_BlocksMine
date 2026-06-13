#include "Window.h"

using namespace std;


Window::Window() {
	width = 800;
	height = 600;

    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}
Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

int Window::initialize() {
    if (!glfwInit()) {
        cout << "GLFW initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    mainWindow = glfwCreateWindow(mode->width, mode->height, "GLBlocksMine", NULL, NULL); // passing monitor instead of NULL makes it full screen

    if (!mainWindow) {
        cout << "Default Window failed to open!" << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    createCalbacks();
    //glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        cout << "GLEW was not properly initialized!";
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(mainWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}
int Window::initializeFullScreen() {
    if (!glfwInit()) {
        cout << "GLFW initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    mainWindow = glfwCreateWindow(mode->width, mode->height, "GLBlocksMine", monitor, NULL); // passing monitor instead of NULL makes it full screen

    if (!mainWindow) {
        cout << "Default Window failed to open!" << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    createCalbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        cout << "GLEW was not properly initialized!";
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(mainWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

}
void Window::createCalbacks() {
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
        //theWindow->handleKeyClicks(key, code, action, mode);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key <= 1024) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        if (action == GLFW_RELEASE) {
            theWindow->keyspressed[key] = theWindow->keys[key] ? theWindow->keyspressed[key] | 1 : theWindow->keyspressed[key];
            theWindow->keys[key] = false;
        }
        if (action == GLFW_REPEAT) {
			theWindow->keyspressed[key] = (theWindow->keyspressed[key] & 1) ? theWindow->keyspressed[key] | (1 << 1) : 0;
            if(theWindow->keyspressed[key] & (1 << 1))cout << "rain" << endl;
        }

        theWindow->recentlyPressedKey = action == GLFW_PRESS ? key : 0;
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }
    
    theWindow->xChange = (xPos)-(theWindow->lastX);
    theWindow->yChange = (yPos)-(theWindow->lastY);
    

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;

    theWindow->mouseX = xPos;

    //theWindow->handleInventoryClicks();
}

int Window::getKeyPressed() {
    return recentlyPressedKey;
}

double Window::getXPos(){
    glfwGetCursorPos(mainWindow, &mouseX, &mouseY);
    return mouseX;
}
double Window::getYPos(){
    glfwGetCursorPos(mainWindow, &mouseX, &mouseY);
    return bufferHeight - mouseY;
}

void Window::initializeDepthBuffer() {
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // request 24-bit depth buffer

    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Border color for out-of-bounds samples
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Window::bindDepthBuffer() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Attach depth texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    // No color buffer needed if you only want depth
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Framebuffer not complete!\n";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::useDepthBuffer(int shaderId) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glUniform1i(glGetUniformLocation(shaderId, "depthTex"), 1);
}

void Window::updateDepthTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    // draw scene from light’s POV or whatever pass fills depthTex
}

Window::~Window() {
    //cout << "Default Window failed to open!" << endl;
    glfwTerminate();
}////////////