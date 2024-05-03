#ifndef BALLMANAGER_H
#define BALLMANAGER_H

//#include <glm/glm.hpp>
#include "../library/include/glm/glm.hpp"



//#include <glm/gtc/random.hpp>
#include "../library/include/glm/gtc/random.hpp"


using namespace glm;
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
#include "model.h"
#include "shader.h"
#include "camera.h"

class BallManager {
private:
	vec2 windowSize;

	Model* ball;
	Shader* ballShader;
	GLuint number;						// ��ǰС����Ŀ
	GLuint maxNumber;					// С�������Ŀ
	vec3 basicPos;						// С���������
	vector<vec3> position;				// ���ϴ��ڵ�С������
	float moveSpeed;					// С���ƶ��ٶ�
	GLuint score;						// �÷�
	GLuint gameMode;					// ��Ϸģʽ
	vec3 lightPos;						// ��Դλ��
	mat4 lightSpaceMatrix;				// ��������������ת��Ϊ�Թ�ԴΪ���ĵ�����

	Camera* camera;
	// ģ�ͱ任����
	mat4 model;
	mat4 projection;
	mat4 view;
public:
	BallManager(vec2 windowSize, Camera* camera) {
		this->windowSize = windowSize;
		this->camera = camera;
		basicPos = vec3(0.0, 5.0, -30.0);
		number = 0;
		maxNumber = 3;
		moveSpeed = 0.1f;
		score = 0;
		this->lightPos = vec3(0.0, 400.0, 150.0);
		mat4 lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 500.0f);
		mat4 lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
		this->lightSpaceMatrix = lightProjection * lightView;
		AddBall();
		LoadModel();
	}
	// ������Ϸģʽ
	void setGameMode(GLuint num) {
        gameMode = num;
	}
	//
	void Update(vec3 pos, vec3 dir, bool isShoot) {
		this->view = camera->GetViewMatrix();
		this->projection = perspective(radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 500.0f);

		if (isShoot) {
			vector<vec3> temp;
			for (GLuint i = 0; i < position.size(); i++) {
				vec3 des = (pos.z - position[i].z) / (-dir.z) * dir + pos;
				if (pow(position[i].x - des.x, 2) + pow(position[i].y - des.y, 2) > 5) {
					temp.push_back(position[i]);
				}
				else {
					number--;
					score++;
				}
			}
			position = temp;
		}
		if (gameMode == 1) {
			AddBall();
			return;
		}
		for (GLuint i = 0; i < position.size(); i++)
			position[i].z += moveSpeed;

		if (number == 0) {
			maxNumber++;
			if (maxNumber == 10) {
				moveSpeed += 0.1f;
				maxNumber = 3;
			}
			AddBall();
		}
	}
	// �ж���Ϸ�Ƿ����
	bool IsOver() {
		if (position.size() > 0)
			if (position[0].z >= 70)
				return true;
		return false;
	}

	GLuint GetScore() {
		return score;
	}
	// ��ȾС��
	void Render(Shader* shader, GLuint depthMap = -1) {
		for (GLuint i = 0; i < position.size(); i++) {
			model = mat4(1.0);
			model[3] = vec4(position[i], 1.0);
			model = scale(model, vec3(5));
			if (shader == NULL) {
				shader = ballShader;
				shader->Bind();
				shader->SetMat4("projection", projection);
				shader->SetMat4("view", view);
			}
			else {
				shader->Bind();
			}
			shader->SetMat4("model", model);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glBindVertexArray(ball->GetVAO());
			glDrawElements(GL_TRIANGLES, static_cast<GLuint>(ball->GetIndices().size()), GL_UNSIGNED_INT, 0);
			
			shader->Unbind();
			glBindVertexArray(0);
			model = mat4(1.0);
		}
	}
private:
	void LoadModel() {
		ball = new Model("../res/model/dot.obj");
		ballShader = new Shader("../res/shader/ball.vert", "../res/shader/ball.frag");
		ballShader->Bind();
		ballShader->SetVec3("color", vec3(0.2, 0.5, 0.5f));
		ballShader->SetInt("shadowMap", 0);
		ballShader->SetVec3("lightPos", lightPos);
		ballShader->SetVec3("viewPos", camera->GetPosition());
		ballShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		ballShader->Unbind();
	}
	// ���С��
	void AddBall() {
		for (GLuint i = number; i < maxNumber; i++) {
			float judgeX = rand() % 2;
			float x = (judgeX >= 0.5) ? rand() % 30: -(rand() % 30);
			float y = rand() % 30;
			vec3 pos = vec3(basicPos.x + x, basicPos.y + y, basicPos.z);
			if (CheckPosition(pos)) {
				position.push_back(pos);
				number++;
			}
			else 
				i--;
		}
	}
	// of the ball itself
	bool CheckPosition(vec3 pos) {
		for (GLuint i = 0; i < position.size(); i++) {
			float away = pow(position[i].x - pos.x, 2) + pow(position[i].y - pos.y, 2);
			if (away < 100)
				return false;
		}
		return true;
	}
};

#endif // !BALLMANAGER_H
