#include "Camera.h"
#include <chrono>
#include <thread>

using namespace std;

Camera::Camera() {
	position = vec3(0.0f, -1.0f, 0.0f);
	worldUp = vec3(0.0f, 1.0f, 0.0f);;
	yaw = 0.1f;
	pitch = 0.1f;
	front = vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = 0.1f;
	turnSpeed = 0.1f;
	gravity = 10.0f;

	update();
}
Camera::Camera(vec3 startPosition, vec3 startUp, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = vec3(0.0f, 0.0f, 0.0f);
	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	initial_velocity = vec3(0);
	velocity = vec3(0);

	update();
}

void Camera::update() {
	front = normalize
	(vec3(
		cos(radians(yaw)) * cos(radians(pitch)),
		sin(radians(pitch)), 
		sin(radians(yaw)) * cos(radians(pitch))
		)
	);
	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
}

//void Camera::keyControl(bool* keys, float deltaT, float dt) {
//	float deltaTime = deltaT;
//	if (keys[GLFW_KEY_CAPS_LOCK]) {
//		deltaTime = deltaT * 2;
//	}
//	if (keys[GLFW_KEY_W] && !GLFW_RELEASE) {
//		position += vec3(front.x * movementSpeed * deltaTime, 0, front.z * movementSpeed * deltaTime);
////		initial_velocity.x =  100 * front.x * movementSpeed * deltaTime * velocity_factor.x;
////		initial_velocity.z =  100 * front.z * movementSpeed * deltaTime * velocity_factor.z;
//	}
//	if (keys[GLFW_KEY_S]) {
//		position -= vec3(front.x * movementSpeed * deltaTime, 0, front.z * movementSpeed * deltaTime);
////		initial_velocity.x = -100 * front.x * movementSpeed * deltaTime * velocity_factor.x;
////		initial_velocity.z = -100 * front.z * movementSpeed * deltaTime * velocity_factor.z;
//	}
//	if (keys[GLFW_KEY_A]) {
//		position -= right * movementSpeed * deltaTime;
////		initial_velocity.x = -100 * right.x * movementSpeed * deltaTime * velocity_factor.x;
////		initial_velocity.z = -100 * right.z * movementSpeed * deltaTime * velocity_factor.z;
//	}
//	if (keys[GLFW_KEY_D]) {
//		position += right * movementSpeed * deltaTime;
////		initial_velocity.x =  100 * right.x * movementSpeed * deltaTime * velocity_factor.x;
////		initial_velocity.z =  100 * right.z * movementSpeed * deltaTime * velocity_factor.z;
//	} 
//	if (keys[GLFW_KEY_SPACE]) {
//		//position.y += 5 * movementSpeed * deltaTime;
//		initial_velocity.y = 100 * 3 * movementSpeed;
//	}
//	if (keys[GLFW_KEY_LEFT_SHIFT]) {
//		position.y -= movementSpeed * deltaTime;
//		//position.x /= 1.05f;
//		//position.z /= 1.05f;
//		//initial_velocity.y = -100 * movementSpeed * deltaTime * velocity_factor.y;
//	}
//	if (keys[GLFW_KEY_LEFT_CONTROL]) {
//		movementSpeed *= 1.4;
//		keys[GLFW_KEY_LEFT_CONTROL] = false;
//	}
//	if (keys[GLFW_KEY_RIGHT_CONTROL]) {
//		movementSpeed /= 1.2;
//		keys[GLFW_KEY_LEFT_CONTROL] = false;
//	}
//	calculateCamPos(dt);
//
//	if(keys[GLFW_KEY_L]) {
//		position = vec3(0.0f, 1.0f, 0.0f);
//	}
//}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch -= yChange;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update();
}

mat4 Camera::calcViewMatrix() {
	return lookAt(
		position,		   // Camera position
		position + front,  // Look at origin
		up 				   // Up vector
	);
}

void Camera::calculateCamPos(float dt) {
	if (initial_velocity != vec3(0)) {
		velocity += acceleration * dt * vec3(2.5);
		position += (initial_velocity + velocity) * dt;
	}
}

Camera::~Camera() {}