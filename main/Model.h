//
//  Model.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_Model_h
#define program5_Model_h


#include "glew/glew.h"
#include <vector>
#include "glm/glm.hpp"

using namespace std;

class Model{
public:
    
    virtual GLfloat const * getPosition() const{printf("qwer\n");}
    virtual GLuint const * getElements() const{}
    virtual size_t getVertexCount() const{}
    virtual size_t getPositionBytes() const{}
    virtual size_t getElementBytes() const {}
    virtual size_t getElementCount() const {}
    
    
};

#endif
