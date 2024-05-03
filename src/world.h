#ifndef WORLD_H
#define WORLD_H

//#include <GLFW/glfw3.h>
#include "../library/include/GLFW/glfw3.h"
#include "place.h"
#include "player.h"
#include "camera.h"
#include "ballmanager.h"

class World {
private:
	GLFWwindow* window;
	vec2 windowSize;

    BallManager* ball;			//all 4 will be on the heap and need deletion
    Camera* camera;				// note that camera must be initialized first for other objects
    Place* place;				//
	Player* player;				//

    Shader* depthShader;  //this one also on heap
    GLuint depthMap;
	GLuint depthMapFBO;
	mat4 lightSpaceMatrix;

public:
	World(GLFWwindow* window, vec2 windowSize, int ballcount, float r, float b, float g) {
		this->window = window;
		this->windowSize = windowSize;

        depthShader = new Shader("../res/shader/shadow.vert", "../res/shader/shadow.frag");

		vec3 lightPos(-0.0, 400.0, 150.0);      //this part of the code is copied as is
		mat4 lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 500.0f);
		mat4 lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

        camera = new Camera(window);    //must be initialized first for the rest
        ball = new BallManager(windowSize, camera, ballcount, r, b, g);
		place = new Place(windowSize, camera);
		player = new Player(windowSize, camera);

		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    /*
     * safe deletion for memory management
     */
    ~World(){
        delete ball;
        delete camera;
        delete place;
        delete player;
        delete depthShader;
    }
	//
	void Update(float deltaTime) {
		camera->Update(deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			ball->Update(camera->GetPosition(), camera->GetFront(), true);
			player->Update(deltaTime, true);
		}
		else {
			ball->Update(camera->GetPosition(), camera->GetFront(), false);
			player->Update(deltaTime, false);
		}
		place->Update();
	}
	// This might be causing trouble
	void Render() {
		RenderDepth();
		player->Render();
		place->RoomRender(NULL, depthMap);
		place->SunRender();
		ball->Render(NULL, depthMap);
	}

	GLuint GetScore() {
		return ball->GetScore();
	}
	// delegated method, from main to world to ballmanager
	bool askBallManagerIfGameOver() {
		return ball->checkGameOverCondition();
	}
	// same as above
	void askBallManagerToSetGameMode(GLuint num) {
        ball->setGameMode(num);
	}
private:
	// ��Ⱦ���ͼ
	void RenderDepth() {
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		depthShader->Bind();
		depthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, 1024, 1024);
		glClear(GL_DEPTH_BUFFER_BIT);
		place->RoomRender(depthShader);
		ball->Render(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, windowSize.x, windowSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

#endif
