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
    }
    
    CarSprite(GLuint shaderProg):SpriteMesh("Model/car.obj", shaderProg){
        direction = glm::vec3(0,-1,0);
        
        
        
        vector<VectorV> vertices = getVertices();
        vector<Triangle> triangles = getTriangles();
        Triangle tri = triangles[1];
        //printf("first triangle:\n");
        //printf("(%f,%f,%f)\n", vertices[tri.vertexIndex[0]].c[0],vertices[tri.vertexIndex[0]].c[1],vertices[tri.vertexIndex[0]].c[2]);
        //printf("(%f,%f,%f)\n", vertices[tri.vertexIndex[1]].c[0],vertices[tri.vertexIndex[1]].c[1],vertices[tri.vertexIndex[1]].c[2]);
        //printf("(%f,%f,%f)\n", vertices[tri.vertexIndex[2]].c[0],vertices[tri.vertexIndex[2]].c[1],vertices[tri.vertexIndex[2]].c[2]);

    }
    
    
    
    void show(glm::mat4 P, glm::mat4 C, glm::mat4 M1){
//        if(getDis(targetDir, direction)<.01){
//            glm::vec3 dis =targetDir-direction;
//            dis*=.3;
//            direction+=dis;
//        }
        SpriteMesh::show(P,C,M1);
    }
    void up(float v=.01f){
        if(velocity>3){
            
        }else{
            velocity+=v;
        }
    }
    void down(float v=.01f){
        if(velocity>3){
            
        }else{
            velocity-=v;
        }
    }
    
    
    
    void left(float v=5.){
        float angle = v;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        printf("rot:%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n",rot[0][0],rot[0][1],rot[0][2],rot[0][3],rot[1][0],rot[1][1],rot[1][2],rot[1][3],rot[2][0],rot[2][1],rot[2][2],rot[2][3],rot[3][0],rot[3][1],rot[3][2],rot[3][3]);
        
        
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 0);
        //printf("axis: %f,%f,%f\n",axis.x,axis.y,axis.z);
        //rot =glm::rotate(glm::mat4(1),angle,axis);
        printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
//        targetDir = direction;
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        //velocity.x+=v;
    }
    void right(float v=5.){
        float angle = -v;
        printf("angle:%f\n",angle);
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        
        glm::vec4 dir = glm::vec4(direction, 1.);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
        
        printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        //velocity.x-=v;
//        targetDir = direction;
    }
    
    void turnUp(float v=5.){
        float angle = -v;
        glm::vec3 axis= glm::vec3(1,0,0);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        //printf("rot:%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n",rot[0][0],rot[0][1],rot[0][2],rot[0][3],rot[1][0],rot[1][1],rot[1][2],rot[1][3],rot[2][0],rot[2][1],rot[2][2],rot[2][3],rot[3][0],rot[3][1],rot[3][2],rot[3][3]);
        
        
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 1.);
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
//        targetDir = direction;
    }
    void turnDown(float v=5.){
        float angle = v;
        glm::vec3 axis= glm::vec3(1,0,0);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseRot=rot*baseRot;
        //printf("rot:%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n",rot[0][0],rot[0][1],rot[0][2],rot[0][3],rot[1][0],rot[1][1],rot[1][2],rot[1][3],rot[2][0],rot[2][1],rot[2][2],rot[2][3],rot[3][0],rot[3][1],rot[3][2],rot[3][3]);
        
        
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 1.);
        
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        dir= rot*dir;
        direction = glm::vec3(dir.x,dir.y,dir.z);
//        targetDir = direction;
    }
    
    
    glm::vec3 getDirection(){
        return direction;
    }
    
    glm::vec3 toDirection(glm::vec3 tDir){
        targetDir = tDir;
    }
    
    glm::vec3 getGlobalV(){
        return glm::vec3(-getV().y*direction.x,getV().y*-direction.y,0);
    }
    
    
    void hitAndTurn(glm::vec3 norm){
        
    }
    
    
    void rotCar(float angle,glm::vec3 axis){
        glm::mat4 rot =glm::rotate(glm::mat4(1),-angle,axis);
        baseRot=rot*baseRot;
        
    }
    
    glm::vec3 testCollision(SpriteMesh sm){
        vector<VectorV> vertices = sm.getVertices();
        vector<Triangle> triangles = sm.getTriangles();
        vector<VectorV> normals = sm.getNormals();
        for(int i = 0;i< triangles.size();i++){
            Triangle tri = triangles[i];
            glm::vec3 center= glm::vec3(0);
            center.x =vertices[tri.vertexIndex[0]].c[0]*.33+vertices[tri.vertexIndex[1]].c[0]*.33+vertices[tri.vertexIndex[2]].c[0]*.33;
            center.y =vertices[tri.vertexIndex[0]].c[1]*.33+vertices[tri.vertexIndex[1]].c[1]*.33+vertices[tri.vertexIndex[2]].c[1]*.33;
            center.z =vertices[tri.vertexIndex[0]].c[2]*.33+vertices[tri.vertexIndex[1]].c[2]*.33+vertices[tri.vertexIndex[2]].c[2]*.33;
            
            if(getDis(getPos(), center)<.2){
                //printf("collision center:%f,%f,%f\n",center.x,center.y,center.z);
                
                //setV(glm::vec3(0));
                return glm::vec3(normals[tri.normalIndex[0]].c[0],normals[tri.normalIndex[0]].c[1],normals[tri.normalIndex[0]].c[2]);
            }

        }
        
        return glm::vec3(0);
    }
private:
    glm::vec3 targetDir;
};


#endif
