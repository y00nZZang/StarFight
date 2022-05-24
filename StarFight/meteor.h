#ifndef METEOR_H
#define METEOR_H

#include "shader.h"
#include <Model.h>
#include <iostream>

class StarShip {
public:
    Model* obj;
    Shader* shader;
    glm::vec3 modelPan;
    int maxX, maxY;

    StarShip() {
        this->obj = new Model((GLchar*)"res/models/starship/StarSparrow.obj");
        this->shader = new Shader("starShip.vs", "starShip.fs");
        this->setMaterial();
        this->setLight();
        this->modelPan = glm::vec3(0.0f, -3.0f, 0.0f);
    }

    StarShip(glm::mat4 projection, glm::mat4 view, glm::vec3 modelPos, glm::vec3 cameraPos, int maxX, int maxY) {
        this->obj = new Model((GLchar*)"res/models/starship/StarSparrow.obj");
        shader = new Shader("starShip.vs", "starShip.fs");
        this->setMaterial();
        this->setLight();
        this->setView(projection, view, cameraPos);
        this->maxX = maxX;
        this->maxY = maxY;
        this->modelPan = modelPos; // starship 초기 위치
    }

    void setView(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos) {
        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setVec3("viewPos", cameraPos);
    }

    void setMaterial() {
        shader->use();
        shader->setInt("material.diffuse", 0);
        shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shader->setFloat("material.shininess", 64.0f);
    }

    void setLight() {
        // dirLight-back
        shader->use();
        shader->setVec3("dirLight[0].direction", 0.0f, 0.0f, 1.0f);
        shader->setVec3("dirLight[0].ambient", 0.5f, 0.5f, 0.5f);
        shader->setVec3("dirLight[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader->setVec3("dirLight[0].specular", 1.0f, 1.0f, 1.0f);

        // dirLight-center
        shader->setVec3("dirLight[1].ambient", 0.1f, 0.1f, 0.1f);
        shader->setVec3("dirLight[1].diffuse", 0.6f, 0.6f, 0.6f);
        shader->setVec3("dirLight[1].specular", 1.0f, 1.0f, 1.0f);
    }

    void draw() {
        shader->use();

        // Draw the loaded model
        glm::mat4 model;
        model = glm::mat4(1.0f);
        model = glm::translate(model, modelPan);
        model = glm::scale(model, glm::vec3(0.4f));
        shader->setMat4("model", model);

        shader->setVec3("dirLight[1].direction", -modelPan);

        obj->Draw(this->shader);
    };

    void movement(int dir) {
        switch (dir) {
        case 0:
            if (this->modelPan[0] > -this->maxX) {
                this->modelPan[0] -= 0.5;
            }
            break;
        case 1:
            if (this->modelPan[0] < this->maxX) {
                this->modelPan[0] += 0.5;
            }
            break;
        case 2:
            if (this->modelPan[1] > -this->maxY) {
                this->modelPan[1] -= 0.5;
            }
            break;
        case 3:
            if (this->modelPan[1] < this->maxY) {
                this->modelPan[1] += 0.5;
            }
            break;
        }
        cout << modelPan[0] << ' ' << modelPan[1] << endl;
    }

};

#endif
