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
using namespace std;
class CarSprite: public SpriteMesh{
public:
    CarSprite()
    {
        direction = glm::vec3(-1,0,0);
    }
    
    CarSprite(GLuint shaderProg):SpriteMesh("Model/0.obj", shaderProg){
        direction = glm::vec3(-1,0,0);
    }
    void up(float v=.01f){
        if(velocity.y>=-.3){
            velocity.y-=v;
        }
        //printf("basemat:%f,%f,%f,%f\n",baseTrans[3][0],baseTrans[3][1],baseTrans[3][2],baseTrans[3][3]);
    }
    void down(float v=.01f){
        if(velocity.y<=.3){
            velocity.y+=v;
        }
    }
    void left(float v=.01f){
        float angle = v*500.;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseTrans*=rot;
        //printf("rot:%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f\n",rot[0][0],rot[0][1],rot[0][2],rot[0][3],rot[1][0],rot[1][1],rot[1][2],rot[1][3],rot[2][0],rot[2][1],rot[2][2],rot[2][3],rot[3][0],rot[3][1],rot[3][2],rot[3][3]);
        
        
        glm::vec4 dir = glm::vec4(direction.x,direction.y, direction.z, 1.);
        
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        dir= dir*rot;
        direction = glm::vec3(dir.x,dir.y,dir.z);
        
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        //velocity.x+=v;
    }
    void right(float v=.01f){
        float angle = -v*500.;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        baseTrans*=rot;
        
        glm::vec4 dir = glm::vec4(direction, 1.);
        dir= dir*rot;
        direction = glm::vec3(dir.x,dir.y,dir.z);
        
        //printf("dir: %f,%f,%f\n",dir.x,dir.y,dir.z);
        //velocity.x-=v;
    }
    
    glm::vec3 getDirection(){
        return direction;
    }
private:
    glm::vec3 direction;
};


#endif
