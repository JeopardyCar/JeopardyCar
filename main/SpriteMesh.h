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
#include "obj_loader/OBJMeshConvert.h"

class SpriteMesh{
public:
    SpriteMesh(){
        velocity = glm::vec3(0);
        baseTrans= glm::mat4(1);
        acc = glm::vec3(0);
    }
    
    SpriteMesh(char * meshfile, GLuint shaderProg){
        velocity = glm::vec3(0);
        baseTrans= glm::mat4(1);
        acc = glm::vec3(0);
        
        FILE *file = NULL;
		file = fopen(meshfile, "rb");
		
		if(file)
		{
			fclose(file);
			OBJMeshConvert objConvert;
			me = objConvert.loadOBJ(meshfile);
            this->mesh = &me;
        }else{
            printf("cant read file\n");
        }
        
        this->shaderProg = shaderProg;
        
        positionSlot = glGetAttribLocation(shaderProg, "pos");
        matSlot = glGetUniformLocation(shaderProg, "M");
        
    }
    
    
    
    void init(Mesh * mesh, GLuint shaderProg){
        this->mesh=mesh;
        this->shaderProg = shaderProg;
        //setup slot
        positionSlot = glGetAttribLocation(shaderProg, "pos");
        matSlot = glGetUniformLocation(shaderProg, "M");
    }
    
    void show(glm::mat4 T){
        velocity+=acc;
        baseTrans*=glm::translate(glm::mat4(1), velocity);
        T*=baseTrans;
        glUseProgram(shaderProg);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        mesh->draw();
        glUseProgram(0);
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
    Mesh * mesh;
    Mesh me;
    
    
    glm::vec3 velocity;
    glm::vec3 acc;
    glm::mat4 baseTrans;
};

#endif
