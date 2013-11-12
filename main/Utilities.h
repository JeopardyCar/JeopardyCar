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

GLuint loadBMP(const char * imagepath, GLuint *textureID){
    
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){
        printf("Image could not be opened %s \n",imagepath); return 0;
    }
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
            return false;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)
        imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)
        dataPos=54; // The BMP header is done that way
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    int r = fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    
    // Create one OpenGL texture
    glGenTextures(1, textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, *textureID);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


}

#endif
