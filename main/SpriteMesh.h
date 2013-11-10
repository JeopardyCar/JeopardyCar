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
        posM = glm::mat4(1);
        acc = glm::vec3(0);
        pos = glm::vec3(0);
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
        
        //positionSlot = glGetAttribLocation(shaderProg, "pos");
        normalSlot = glGetAttribLocation(shaderProg, "norm");
        
        matSlot = glGetUniformLocation(shaderProg, "M");
        pSlot = glGetUniformLocation(shaderProg, "P");
        cSlot = glGetUniformLocation(shaderProg, "C");
        
    }
    
    void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
        glm::mat4 T= P*C*M*posM;
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
        //printf("x:%f,y:%f,z:%f\n", M[3][0],M[3][1],M[3][2]);
        glUseProgram(shaderProg);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glUniformMatrix4fv(pSlot, 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(cSlot, 1, GL_FALSE, &C[0][0]);
        me.draw();
        glUseProgram(0);
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
    
    void setFriction(float f){
        if(f<0){
            f = 0.01;
        }
        if(f>1){
            f =.99;
        }
        friction = f;
    }
    
    void setAccelerate(glm::vec3 v){
        acc =v;
    }
    
    glm::vec3 getAccelerate(){
        return acc;
    }
    
    std::vector<VectorV> getNormals(){
        return me.normals;
    }
    
    vector<VectorV> getVertices(){
        return me.vertices;
    }
    vector<Triangle> getTriangles(){
        return me.triangles;
    }
    
    
    
protected:
    GLuint shaderProg;
    GLuint positionBuffer;
    GLuint elementBuffer;
    //GLint positionSlot;
    GLint normalSlot;
    GLint matSlot;
    GLint pSlot;
    GLint cSlot;
    Mesh * mesh;
    Mesh me;
    
    float friction;
    glm::mat4 posM;
    glm::vec3 pos;
    
    glm::vec3 velocity;
    glm::vec3 acc;
    glm::mat4 baseTrans;
};

#endif
