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
        
    }
    
    CarSprite(GLuint shaderProg):SpriteMesh("Model/0.obj", shaderProg){
        
    }
    void up(float v=.01f){
        if(velocity.y>=-.3){
            printf("%f\n",velocity.y);
            velocity.y-=v;
        }
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
        //velocity.x+=v;
    }
    void right(float v=.01f){
        float angle = -v*500.;
        glm::vec3 axis= glm::vec3(0,0,1);
        glm::mat4 rot =glm::rotate(glm::mat4(1),angle,axis);
        
        baseTrans*=rot;
        //velocity.x-=v;
    }
private:
    
};


#endif
