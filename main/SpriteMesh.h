//
//  Sprite.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_SpriteMesh_h
#define program5_SpriteMesh_h



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"


class SpriteMesh{
public:
    SpriteMesh(){
        velocity = glm::vec3(0);
        baseTrans= glm::mat4(1);
    }
    
    void init(Mesh * mesh, GLuint shaderProg){
        this->mesh=mesh;
        this->shaderProg = shaderProg;
        //setup slot
        positionSlot = glGetAttribLocation(shaderProg, "pos");
        matSlot = glGetUniformLocation(shaderProg, "M");
        //setupBuffer();
    }
    
    void show(glm::mat4 T){
        T*=baseTrans;
        baseTrans*=glm::translate(glm::mat4(1), velocity);
        glUseProgram(shaderProg);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        mesh->draw();
        /*
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, (*model).getElementCount(), GL_UNSIGNED_INT, 0);
        */
        glUseProgram(0);
    }
    
    
    void setV(glm::vec3 v){
        velocity = v;
    }
    glm::vec3 getV(){
        return velocity;
    }
    
    
protected:
    GLuint shaderProg;
    GLuint positionBuffer;
    GLuint elementBuffer;
    GLint positionSlot;
    GLint matSlot;
    Mesh * mesh;
    
    glm::vec3 velocity;
    glm::mat4 baseTrans;
};

#endif
