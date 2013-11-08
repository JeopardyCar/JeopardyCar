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
    
    virtual GLfloat const * getPosition() const=0;
    virtual GLuint const * getElements() const=0;
    virtual size_t getVertexCount() const=0;
    virtual size_t getPositionBytes() const=0;
    virtual size_t getElementBytes() const=0;
    virtual size_t getElementCount() const=0;
    
    
};

#endif
