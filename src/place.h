#ifndef PLACE_H
#define PLACE_H

//#include <glad/glad.h>
#include "../library/include/glad/glad.h"


#include "model.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

class Place {
private:
	vec2 windowSize;
	// ����
	Model* room;
	Texture* roomTexture;
	Shader* roomShader;

	// ̫��
	Model* sun;
	vec3 lightPos;							// ��Դλ��
	mat4 lightSpaceMatrix;					// ��������������ת��Ϊ�Թ�ԴΪ���ĵ�����
	Shader* sunShader;

	Camera* camera;
	// ģ�ͱ任����
	mat4 model;
	mat4 projection;
	mat4 view;
public:
	Place(vec2 windowSize, Camera* camera) {
		this->windowSize = windowSize;
		this->camera = camera;
		this->lightPos = vec3(0.0, 400.0, 150.0);
		mat4 lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 500.0f);
		mat4 lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
		this->lightSpaceMatrix = lightProjection * lightView;
		LoadModel();
		LoadTexture();
		LoadShader();
	}
	// ���±任����
	void Update() {
		this->model = mat4(1.0);
		this->view = camera->GetViewMatrix();
		this->projection = perspective(radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 500.0f);
	}
	// ��Ⱦ����
	void RoomRender(Shader* shader, int depthMap = -1) {
		if (shader == NULL) {
			shader = roomShader;
			shader->Bind();
			shader->SetMat4("view", view);
			shader->SetMat4("projection", projection);
		}
		else {
			shader->Bind();
		}
		shader->SetMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, roomTexture->GetId());
		if (depthMap != -1) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
		}
		glBindVertexArray(room->GetVAO());
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(room->GetIndices().size()), GL_UNSIGNED_INT, 0);
		shader->Unbind();
		glBindVertexArray(0);
	}
	// ��Ⱦ̫��
	void SunRender() {
		Shader* shader = sunShader;
		shader->Bind();
		shader->SetMat4("projection", projection);
		shader->SetMat4("model", model);
		shader->SetMat4("view", view);
		glBindVertexArray(sun->GetVAO());
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(sun->GetIndices().size()), GL_UNSIGNED_INT, 0);
		shader->Unbind();
		glBindVertexArray(0);
	}
private:
	// ����ģ��
	void LoadModel() {
		room = new Model("../res/model/room.obj");
		sun = new Model("../res/model/sun.obj");
	}
	// ��������
	void LoadTexture() {
		roomTexture = new Texture("../res/texture/wall.jpg");
	}
	// ������ɫ��
	void LoadShader() {
		roomShader = new Shader("../res/shader/room.vert", "../res/shader/room.frag");
		roomShader->Bind();
		roomShader->SetInt("diffuse", 0);
		roomShader->SetInt("shadowMap", 1);
		roomShader->SetVec3("lightPos", lightPos);
		roomShader->SetVec3("viewPos", camera->GetPosition());
		roomShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		roomShader->Unbind();

		sunShader = new Shader("../res/shader/sun.vert", "../res/shader/sun.frag");
		sunShader->Bind();
		sunShader->Unbind();
	}
};

#endif // !PLACE_H
