//
//  Sprite.h
//  program5
//
//  Created by Storm Max on 13-11-7.
//
//

#ifndef program5_BoxSprite2_h
#define program5_BoxSprite2_h

#include <vector>
#include "Cube2.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Sprite.h"


using namespace std;


class BoxSprite2:public Sprite{
public:
    BoxSprite2()
    {
        modelbox = Cube2();
        
    }
    void init(GLuint shaderProg){
        Sprite::init(&modelbox, shaderProg);
    }
    
private:
    Cube2 modelbox;
    
};






#endif
