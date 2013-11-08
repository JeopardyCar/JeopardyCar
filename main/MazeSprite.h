//
//  MazeSprite.h
//  program5
//
//  Created by Storm Max on 13-11-7.
//
//

#ifndef program5_MazeSprite_h
#define program5_MazeSprite_h

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "MazeModel.h"
#include "Maze.h"

using namespace std;

class MazeSprite{
public:
    MazeSprite(){
        
    }
    
    void show(glm::mat4 T){
        T=T*translateToOrigin;
        glUseProgram(shaderProg);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, model3D.get3DElementCount(), GL_UNSIGNED_INT, 0);
        glUseProgram(0);
    }
    
    void init(GLuint shaderProg, int w, int h, unsigned int const & seed = 1){
        this->shaderProg=shaderProg;
        Maze mazeLayout(w, h, seed);
		model = MazeModel(mazeLayout);
        model3D = MazeModel3D(model);
        translateToOrigin = glm::translate(glm::mat4(1), -model3D.getCentroid());
        setupBuffer();
    }
    
    
private:
    GLuint shaderProg;
    GLuint positionBuffer;
    GLuint elementBuffer;
    GLuint positionSlot;
    GLuint matSlot;
    
    MazeModel model;
    MazeModel3D model3D;
    MazeModel maze;
    
    glm::mat4 translateToOrigin;
    
    void setupBuffer(){
        //setup position buffer for maze
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model3D.get3DPositionBytes(), model3D.get3DPosition(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model3D.get3DElementBytes(), model3D.get3DElements(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        positionSlot = glGetAttribLocation(shaderProg, "pos");
		matSlot =      glGetUniformLocation(shaderProg, "M");
    }
};



#endif
