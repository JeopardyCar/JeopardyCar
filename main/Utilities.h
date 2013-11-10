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


float getDis(glm::vec3 vec1, glm::vec3 vec2){
    return getLen(vec1-vec2);
}

glm::vec3 normalize(glm::vec3 vec){
    float overlen = 1/getLen(vec);
    return vec*=overlen;
}

#endif
