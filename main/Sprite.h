//
//  Sprite.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_Sprite_h
#define program5_Sprite_h



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"
class Sprite{
public:
    Sprite(){
        velocity = glm::vec3(0);
        baseTrans= glm::mat4(1);
        acc = glm::vec3(0);
        initialized = false;
        posM = glm::mat4(1);
        pos = glm::vec3(0);
    }
    
    
    Sprite(Model * model, GLuint shaderProg){
        velocity = glm::vec3(0);
        baseTrans= glm::mat4(1);
        acc = glm::vec3(0);
        this->model=model;
        this->shaderProg = shaderProg;
        //setup slot
        positionSlot = glGetAttribLocation(shaderProg, "pos");
        matSlot = glGetUniformLocation(shaderProg, "M");
        setupBuffer();
        initialized = true;
    }
    
    
    ~Sprite()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &elementBuffer);
		}
	}
    
    void init(Model* model, GLuint shaderProg){
        this->model=model;
        this->shaderProg = shaderProg;
        //setup slot
        positionSlot = glGetAttribLocation(shaderProg, "pos");
        matSlot = glGetUniformLocation(shaderProg, "M");
        setupBuffer();
        initialized = true;
    }
    
    
    void show(glm::mat4 P,glm::mat4 C, glm::mat4 M){
        glm::mat4 T = P*C*M*posM;
        if(initialized){
            velocity+=acc;
            baseTrans*=glm::translate(glm::mat4(1), velocity);
            T*=baseTrans;
            P*=baseTrans;
            C*=baseTrans;
            M*=posM;
            M*=baseTrans;
            pos.x = M[3][0];
            pos.y = M[3][1];
            pos.z = M[3][2];
        
            glUseProgram(shaderProg);
            glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
            glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
            glEnableVertexAttribArray(positionSlot);
            glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
            glDrawElements(GL_TRIANGLES, (*model).getElementCount(), GL_UNSIGNED_INT, 0);
            glUseProgram(0);
        }
    }
    void setPosM(glm::vec3 m){
        posM = glm::translate(glm::mat4(1), m);
    }
    
    glm::vec3 getPos(){
        return pos;
    }
    
    void setV(glm::vec3 v){
        velocity = v;
    }
    glm::vec3 getV(){
        return velocity;
    }
    
    void setAccelerate(glm::vec3 v){
        acc =v;
    }
    
    glm::vec3 getAccelerate(){
        return acc;
    }
    
    
protected:
    GLuint shaderProg;
    GLuint positionBuffer;
    GLuint elementBuffer;
    GLint positionSlot;
    GLint matSlot;
    bool initialized;
    Model* model;
    
    glm::vec3 velocity;
    glm::vec3 acc;
    glm::mat4 baseTrans;
    
    glm::mat4 posM;
    glm::vec3 pos;
    
    
    void setupBuffer(){
        //setup position buffer for cube
        glGenBuffers(1,&positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, (*model).getPositionBytes(),(*model).getPosition(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // elements
        glGenBuffers(1,&elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*model).getElementBytes(), (*model).getElements(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        this->initialized = true;
    }
};

#endif
