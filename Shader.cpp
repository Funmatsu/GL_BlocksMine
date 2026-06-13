#include "shader.h"

using namespace std;

glShader::glShader() {
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
    pointLightCount = 0;
}

void glShader::createShaderFromString(const char* vertexCode, const char* fragmentCode) {
    addShader(vertexCode, fragmentCode);
}

unsigned int glShader::compileShader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    const char* src = source;  // does same as &source[0] or points to beginning of the string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << (type == GL_VERTEX_SHADER ? "Vertex" : type == GL_FRAGMENT_SHADER ? "Fragment" : type == GL_COMPUTE_SHADER ? "Compute" : "Other") << " shader says: Failed to compile shader; \n" << message << endl;
        glDeleteShader(id);
    }

    return id;
}

void glShader::addShader(const char* computeCode) {
    unsigned int program = glCreateProgram();
    unsigned int compShad = compileShader(GL_COMPUTE_SHADER, computeCode);

    glAttachShader(program, compShad);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(compShad);

    shaderId = program;
    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");

    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
    for(int i = 0; i < MAX_POINT_LIGHTS; i++){
        char locBuff[200] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientintensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }

    uniformTexture = glGetUniformLocation(shaderId, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");
    uniformColorMask = glGetUniformLocation(shaderId, "colorMask");
    uniformOrtho = glGetUniformLocation(shaderId, "ortho");
    uniformUipos = glGetUniformLocation(shaderId, "uiPos");
}

void glShader::addShader(const char* vertexCode, const char* fragmentCode) {
    unsigned int program = glCreateProgram();
    unsigned int vertShad = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragShad = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    glAttachShader(program, vertShad);
    glAttachShader(program, fragShad);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertShad);
    glDeleteShader(fragShad);

    shaderId = program;
    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");

    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[200] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientintensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderId, locBuff);
    }

    uniformTexture = glGetUniformLocation(shaderId, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");
    uniformColorMask = glGetUniformLocation(shaderId, "colorMask");
    uniformOrtho = glGetUniformLocation(shaderId, "ortho");
    uniformUipos = glGetUniformLocation(shaderId, "uiPos");
}

string glShader::readShaderFiles(const char* fileLocation) {
    string shaderContent, line;
    ifstream ifs(fileLocation);
    if (!ifs.is_open()) {
        cout << "failed to open \"" << fileLocation << "\"" << endl;
    }
    stringstream ss;
    while (getline(ifs, line)) {
        ss << line << "\n";
    }
    shaderContent = ss.str();

    return shaderContent;
}

void glShader::createShaderFromFiles(const char* vertexFilePath, const char* fragmentFilePath) {
    string vertexCodeString, fragmentCodeString;
    vertexCodeString = readShaderFiles(vertexFilePath);
    fragmentCodeString = readShaderFiles(fragmentFilePath);
     addShader(vertexCodeString.c_str(), fragmentCodeString.c_str());
}
void glShader::createShaderFromFiles(const char* computeFilePath) {
    string computeCodeString;
    computeCodeString = readShaderFiles(computeFilePath);
    addShader(computeCodeString.c_str());
}

void glShader::useShader(){
    glUseProgram(shaderId);
}

void glShader::clearShader() {
    glDeleteProgram(shaderId);
    shaderId = 0;
}

unsigned int glShader::getModelLocation() {
    return uniformModel;
}
unsigned int glShader::getProjectionLocation() {
    return uniformProjection;
}
unsigned int glShader::getViewLocation() {
    return uniformView;
}
unsigned int glShader::getAmbientIntensityLocation() {
    return uniformDirectionalLight.uniformAmbientIntensity;
}
unsigned int glShader::getAmbientColorLocation() {
    return uniformDirectionalLight.uniformColor;
}
unsigned int glShader::getDiffuseIntensityLocation() {
    return uniformDirectionalLight.uniformDiffuseIntensity;
}
unsigned int glShader::getDirectionLocation() {
    return uniformDirectionalLight.uniformDirection;
}
unsigned int glShader::getColorMaskLocation() {
    return uniformColorMask;
}
unsigned int glShader::getOrthoLocation() {
    return uniformOrtho;
}
unsigned int glShader::getUiposLocation() {
    return uniformUipos;
}
void glShader::setDirectionalLight(DirectionalLight* dLight) {
    dLight->useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void glShader::setPointLights(PointLight* pLight, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) {
        lightCount = MAX_POINT_LIGHTS;
    }
    glUniform1i(uniformPointLightCount, lightCount);
    for (int i = 0; i < lightCount; i++) {
        pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity
            , uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

    }
}

void glShader::setTexture(GLenum texture_unit) {
    glUniform1i(uniformTexture, texture_unit);
}

void glShader::setDirectionalShadowMap(GLenum texture_unit) {
    glUniform1i(uniformDirectionalShadowMap, texture_unit);
}

void glShader::setDirectionalLightTransform(mat4 lTransform) {
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
    //cout << lTransform[0].x << " " << lTransform[0].y << " " << lTransform[0].z << endl;
    //cout << lTransform[1].x << " " << lTransform[1].y << " " << lTransform[1].z << endl;
    //cout << lTransform[2].x << " " << lTransform[2].y << " " << lTransform[2].z << endl;
    //cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

glShader::~glShader() {
    clearShader();
}


/////////////////////////////*COMPUTE SHADER SECTION*////////////////////////////////////


//glComputeShader::glComputeShader() {
//    id = 0;
//}

unsigned int glComputeShader::compileShader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    const char* src = source;  // does same as &source[0] or points to beginning of the string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << (type == GL_VERTEX_SHADER ? "Vertex" : type == GL_FRAGMENT_SHADER ? "Fragment" : type == GL_COMPUTE_SHADER ? "Compute" : "Other") << " shader says: Failed to compile shader; \n" << message << endl;
        glDeleteShader(id);
    }

    return id;
}

void glComputeShader::addShader(const char* computeCode) {
    unsigned int program = glCreateProgram();
    unsigned int compShad = compileShader(GL_COMPUTE_SHADER, computeCode);

    glAttachShader(program, compShad);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(compShad);

    id = program;
}

string glComputeShader::readShaderFiles(const char* fileLocation) {
    string shaderContent, line;
    ifstream ifs(fileLocation);
    if (!ifs.is_open()) {
        cout << "failed to open \"" << fileLocation << "\"" << endl;
    }
    stringstream ss;
    while (getline(ifs, line)) {
        ss << line << "\n";
    }
    shaderContent = ss.str();

    return shaderContent;
}

void glComputeShader::createShaderFromFiles(const char* computeFilePath) {
    string computeCodeString;
    computeCodeString = readShaderFiles(computeFilePath);
    addShader(computeCodeString.c_str());
}

void glComputeShader::useShader() {
    glUseProgram(id);
}

void glComputeShader::clearShader() {
    glDeleteProgram(id);
    id = 0;
}
glComputeShader::~glComputeShader() {
    clearShader();
}