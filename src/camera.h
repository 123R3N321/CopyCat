#ifndef CAMERA_H
#define CAMERA_H

//#include <GLFW/glfw3.h>
#include "../library/include/GLFW/glfw3.h"



//#include <glm/glm.hpp>
#include "../library/include/glm/glm.hpp"


//#include <glm/gtc/matrix_transform.hpp>
#include "../library/include/glm/gtc/matrix_transform.hpp"


using namespace glm;

// this is the hard part
const float YAW = -90.0f;			//
const float PITCH = 0.0f;			//
const float SPEED = 30.0f;
const float HEIGHT = 10.0f;			//
const float SENSITIVITY = 0.05f;     //
const float ZOOM = 45.0f;			//
const float JUMPTIME = 0.1f;		//
const float GRAVITY = 9.8f;			//
const float JUMPSTRENGTH = 60.0f;	//

class Camera {
private:
	GLFWwindow* window;

	vec3 position;				// ����ͷλ��
	vec3 front;					// ָ������ͷǰ���ĵ�λ����
	vec3 right;					// ָ������ͷ�ҷ��ĵ�λ����
	vec3 up;					// ָ������ͷ�Ϸ��ĵ�λ����
	vec3 worldUp;				// ָ��������Ϸ��ĵ�λ����������ı�

	// ��Ծ
	float jumpTimer;			// ��Ծ����ʱ��
	float gravity;				// �������ٶ�
	bool isJump;				// �����жϵ�ǰ�ǲ��Ǵ�����Ծ״̬

	// ŷ����
	float yaw;					// ƫ����
	float pitch;				// ������

	// ����ͷѡ��
	float movementSpeed;		// ����ͷ�ƶ��ٶ�
	float mouseSensitivity;		// �ӽ��ƶ��ٶ�
	float zoom;					// �ӽǵĴ�С��һ��45.0f�Ƚ���ʵ

	// ���λ��
	double mouseX;
	double mouseY;
	bool firstMouse;			// ����Ƿ��һ�ν��봰��
public:
	Camera(GLFWwindow* window) {
		this->window = window;

		movementSpeed = SPEED;
		mouseSensitivity = SENSITIVITY;
		zoom = ZOOM;
		firstMouse = true;

		jumpTimer = 0;
		isJump = false;
		gravity = -GRAVITY;

		position = vec3(0.0f, HEIGHT, 70.0f);
		worldUp = vec3(0.0f, 1.0f, 0.0f);
		yaw = YAW;
		pitch = PITCH;

		UpdateCamera();
	}	
	// ��������ͷ�����ݺͿ��Ƽ��̡��������
	void Update(float deltaTime) {
		MouseMovement();
		KeyboardInput(deltaTime);
	}

	mat4 GetViewMatrix() {
		return lookAt(position, position + front, up);
	}

	vec3 GetPosition() {
		return position;
	}

	vec3 GetFront() {
		return front;
	}

	vec3 GetRight() {
		return right;
	}

	vec3 GetUp() {
		return up;
	}

	float GetZoom() {
		return zoom;
	}
private:
	// �������
	void MouseMovement() {
		double newMouseX, newMouseY;

		glfwGetCursorPos(window, &newMouseX, &newMouseY);

		if (firstMouse) {
			mouseX = newMouseX;
			mouseY = newMouseY;
			firstMouse = false;
		}

		yaw += ((newMouseX - mouseX) * mouseSensitivity);
		pitch += ((mouseY - newMouseY) * mouseSensitivity);

		mouseX = newMouseX;
		mouseY = newMouseY;

		// ��֤��������-90���90��֮��
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// ����Front��Right��Up
		UpdateCamera();
	}
	// ��������
	void KeyboardInput(float deltaTime) {
		float velocity = movementSpeed * deltaTime;
		vec3 forward = normalize(cross(worldUp, right));
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			position += forward * velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			position -= forward * velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			position -= right * velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			position += right * velocity;

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJump) {
			jumpTimer = JUMPTIME;
			isJump = true;
		}
		if (jumpTimer > 0) {
			gravity += (JUMPSTRENGTH * (jumpTimer / JUMPTIME)) * deltaTime;
			jumpTimer -= deltaTime;
		}
		gravity -= GRAVITY * deltaTime;
		position.y += gravity * deltaTime * 10;

		if (position.y < HEIGHT) {
			position.y = HEIGHT;
			gravity = 0;
			isJump = false;
		}

		CheckCollision();
	}
	// ��ײ���
	void CheckCollision() {
		if (position.x > 90.0f)
			position.x = 90.0f;
		if (position.x < -90.0f)
			position.x = -90.0f;
		if (position.z > 75.0f)
			position.z = 75.0f;
		if (position.z < -35.0f)
			position.z = -35.0f;
	}
	// ��������ͷ���������
	void UpdateCamera() {
		vec3 front;
		front.x = cos(radians(yaw)) * cos(radians(pitch));
		front.y = sin(radians(pitch));
		front.z = sin(radians(yaw)) * cos(radians(pitch));
		this->front = normalize(front);

		this->right = normalize(cross(this->front, this->worldUp));
		this->up = normalize(cross(this->right, this->front));
	}
};

#endif // !CAMERA_H
