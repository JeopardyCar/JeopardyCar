//
//  Sprite.h
//  program5
//
//  Created by Storm Max on 13-11-7.
//
//

#ifndef program5_BoxSprite_h
#define program5_BoxSprite_h


#include "Cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Sprite.h"


using namespace std;


class BoxSprite: public Sprite{
public:
    BoxSprite()
    {
        modelbox = Cube();
    }
    
    void init(GLuint shaderProg){
        Sprite::init(&modelbox, shaderProg);
    }
    
private:
    Cube modelbox;
    
    
};






#endif
