#pragma once

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc/type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cmath>
#include <vector>

using namespace glm;

class Camera
{
public:
	Camera();
	Camera(vec3 startPosition, vec3 startUp, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed);
	~Camera();
	void operator=(Camera other) {
		position = other.position;
		worldUp = other.up;
		yaw = other.yaw;
		pitch = other.pitch;
		front = other.front;
		movementSpeed = other.movementSpeed;
		turnSpeed = other.turnSpeed;
		initial_velocity = vec3(0);
		gravity = other.gravity;
		update();
	}
	void keyControl(bool* keys, float deltaTime, float dt);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	vec3 getFront() { return front; }
	vec3 getPosition() { return position; }
	vec3& getPosition(bool) { return position; }
	vec3 getRight() { return right; }
	vec3 getInitVelocity() { return initial_velocity; }
	float getTurnSpeed() { return turnSpeed; }
	float getYaw() { return yaw; }
	float getPitch() { return pitch; }
	void setPosition(vec3 pos) { position = pos; }
	void setFront(vec3 other_front) { front = other_front; }
	float getGravity() { return gravity; }
	void setGravity(float g) { gravity = g; }
	mat4 calcViewMatrix();
	vec3 velocity = vec3(0), initial_velocity, acceleration = vec3(0,-gravity, 0), velocity_factor = vec3(1);
	void calculateCamPos(float dt);
private:
	vec3 position, front, up, right, worldUp;
	float yaw, pitch, movementSpeed, turnSpeed, gravity;
	void update();
};

