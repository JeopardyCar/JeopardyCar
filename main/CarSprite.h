//
//  CarSprite.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_CarSprite_h
#define program5_CarSprite_h


#include "SpriteMesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Utilities.h"
using namespace std;
class CarSprite: public SpriteMesh{
public:
    CarSprite()
    {
        direction = glm::vec3(0,-1,0);
        velocity = glm::vec3(0,-.001,0);
        topspeed = 2;
        lowspeed = 0.003;
    }
    
    CarSprite(char* modelpath ,GLuint shaderProg, char* texpath, GLuint TexID):
        SpriteMesh(modelpath, shaderProg,texpath,TexID){
        direction = glm::vec3(0,-1,0);
        velocity = glm::vec3(0,-.001,0);
        topspeed = 2;
        lowspeed = 0.003;
    }
    
    //Displays the car after updating the velocity
    void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
        glm::mat4 T= P*C*M;
        
        velocity += acc;
        glm::vec3 target= normalize((velocity + direction))*getLen(velocity);
        glm::vec3 change =target - velocity;
        change *= .3;
        velocity += change;
        
        baseTrans*=glm::translate(glm::mat4(1), velocity);
        T*=baseTrans;
        P*=baseTrans;
        C*=baseTrans;
        
        T*=baseRot;
        P*=baseRot;
        C*=baseRot;
        
        
        pos.x = (M*baseTrans)[3][0];
        pos.y = (M*baseTrans)[3][1];
        pos.z = (M*baseTrans)[3][2];
        
        glUseProgram(shaderProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
        glUniform1i(SamplerSlot, 0);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glUniformMatrix4fv(pSlot, 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(cSlot, 1, GL_FALSE, &C[0][0]);
        
        draw();
        glUseProgram(0);
    }
    void up(float v=.01f){
        if(getLen(velocity)>topspeed){
            return ;
        }
        velocity+= direction*v;
    }
    void down(float v=.002f){
        glm::vec3 vel = velocity;
        vel.z= 0;
        if(getLen(vel)<lowspeed){
            return ;
        }
        velocity-= direction*v;
    }

    
    
    void left(float v=2.){
        float angle = v;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 0);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
        
    }
    void right(float v=2.){
        float angle = -v;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 0);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
    }
    
    void turnUp(float v=5.){
        float angle = -v;
        glm::vec3 axis= glm::vec3(1,0,0);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        glm::vec4 dir = glm::vec4(velocity.x,velocity.y, velocity.z, 1.);
        dir= rot*dir;
        velocity = glm::vec3(dir.x,dir.y,dir.z);
    }
    void turnDown(float v=5.){
        float angle = v;
        glm::vec3 axis= glm::vec3(1,0,0);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        glm::vec4 dir = glm::vec4(velocity.x,velocity.y, velocity.z, 1.);
        dir= rot*dir;
        velocity = glm::vec3(dir.x,dir.y,dir.z);
    }
    
    
    glm::vec3 getDirection(){
        return direction;
    }
    

    glm::vec3 getGlobalV(){
        return glm::vec3(-getV().y*velocity.x,getV().y*-velocity.y,0);
    }
    
    
    void hitAndTurn(glm::vec3 norm){
        glm::vec3 v =getV();
        glm::vec3 newv;
        newv =2*glm::dot(v, norm)/getLen(norm)*norm/getLen(norm);
        newv*=.7;
        newv = v- newv;
        setV(newv);
        
        
        norm = normalize(norm);
        norm *= .01;
        while(glm::dot(direction, norm)<0){
            direction+=norm;
            normalize(direction);
        }
        
        glm::vec3 move = norm;
        move= normalize(move);
        move*=.3;
        while(glm::dot(newv,norm)<0){
            newv+= move;
            baseTrans *= glm::translate(glm::mat4(1), move);
            pos.x = (baseTrans)[3][0];
            pos.y = (baseTrans)[3][1];
            pos.z = (baseTrans)[3][2];
        }
        
        glm::vec3 newdir = direction;
        newdir.z=0;
        glm::mat4 rot= glm::rotate(glm::mat4(1), -glm::atan(newdir.x, newdir.y), glm::vec3(0,0,1));
    }
    
    void setDir(glm::vec3 dir){
        direction = dir;
    }
    
    void rotCar(float angle,glm::vec3 axis){
        glm::mat4 rot =glm::rotate(glm::mat4(1),-angle,axis);
        baseRot=rot*baseRot;
    }
    
    void rotCar(glm::mat4 rotMat){
        baseRot = rotMat;
    }
    
    
    bool testCol(SpriteMesh sm, float dis =1 ){
        vector<VectorV> vertices = sm.getVertices();
        vector<Triangle> triangles = sm.getTriangles();
        vector<VectorV> normals = sm.getNormals();
        for(int i = 0;i< triangles.size();i++){
            Triangle tri = triangles[i];
            glm::vec3 center= glm::vec3(0);
            
            center.x =vertices[tri.vertexIndex[0]].c[0]*.33+vertices[tri.vertexIndex[1]].c[0]*.33+vertices[tri.vertexIndex[2]].c[0]*.33;
            center.y =vertices[tri.vertexIndex[0]].c[1]*.33+vertices[tri.vertexIndex[1]].c[1]*.33+vertices[tri.vertexIndex[2]].c[1]*.33;
            center.z =vertices[tri.vertexIndex[0]].c[2]*.33+vertices[tri.vertexIndex[1]].c[2]*.33+vertices[tri.vertexIndex[2]].c[2]*.33;
            center+=sm.getPos();
            float distance =getDis(getPos(), center);
            if(distance<dis){
                return true;
            }
        }
        return false;
    }
    
    glm::vec3 testCollision(SpriteMesh sm, float dis = 1, bool pushback = true){
        vector<VectorV> vertices = sm.getVertices();
        vector<Triangle> triangles = sm.getTriangles();
        vector<VectorV> normals = sm.getNormals();
        for(int i = 0;i< triangles.size();i++){
            Triangle tri = triangles[i];
            glm::vec3 center= glm::vec3(0);
            
            center.x =vertices[tri.vertexIndex[0]].c[0]*.33+vertices[tri.vertexIndex[1]].c[0]*.33+vertices[tri.vertexIndex[2]].c[0]*.33;
            center.y =vertices[tri.vertexIndex[0]].c[1]*.33+vertices[tri.vertexIndex[1]].c[1]*.33+vertices[tri.vertexIndex[2]].c[1]*.33;
            center.z =vertices[tri.vertexIndex[0]].c[2]*.33+vertices[tri.vertexIndex[1]].c[2]*.33+vertices[tri.vertexIndex[2]].c[2]*.33;
            center+=sm.getPos();
            float distance =getDis(getPos(), center);
            if(distance<dis){
                glm::vec3 norm= glm::vec3(normals[tri.normalIndex[0]].c[0],normals[tri.normalIndex[0]].c[1],normals[tri.normalIndex[0]].c[2]);
                return norm;
            }
        }
        return glm::vec3(0);
    }
    float topspeed;
    float lowspeed;
private:
    glm::vec3 direction;
};


#endif
