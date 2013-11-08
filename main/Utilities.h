//
//  Utilities.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_Utilities_h
#define program5_Utilities_h

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


float getLen(glm::vec3 vec){
    return sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}


#endif
