#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include "MazeModel.h"
#include "MazeModel3D.h"
#include "Cube.h"
#include "BoxSprite.h"
#include "BoxSprite2.h"
#include "MazeSprite.h"
#include "Utilities.h"
#include "Geometry.h"

#define PI 3.1415926



class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
        this->P = glm::mat4(1);
	}
    
	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteVertexArrays(1, &vertexArray);
		}
	}
    
	void init()
	{
		this->w = 10;
		this->h = 10;
        
		setupGlew();
		setupShader();
		generateMaze();
		initialized = true;
	}
    
	void display(bool pickingEnabled=false)
	{
        
        
        GLfloat currentTime;
        currentTime = clk.GetElapsedTime();
        
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 T =  P*F*R*C*M*translateToOrigin;
        /*
        glUseProgram(shaderProg);
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, model3D.get3DElementCount(), GL_UNSIGNED_INT, 0);
        */
        /*
        //draw the cube
        glUseProgram(boxShader);
        glUniformMatrix4fv(boxMatSlot, 1, GL_FALSE, &T[0][0]);
        glBindBuffer(GL_ARRAY_BUFFER, boxPositionBuffer);
        glEnableVertexAttribArray(boxPositionSlot);
        glVertexAttribPointer(boxPositionSlot,3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawElements(GL_TRIANGLES, modelbox.getElementCount(), GL_UNSIGNED_INT, 0);
        */
        
        
        box123.show(T);
        glm::mat4 T2 =P*F*R*C*M;
        box2.show(T2);
        maze.show(T);
		//checkGLError("display");
	}
    
    
    
    
    void setProjectionTransform(glm::mat4 const & transform)
	{
		this->P = transform;
	}
	
    /*
    void setModelTransform(glm::mat4 const & transform)
	{
		this->M = transform;
	}
    */
    
    /*
    void setCameraTransform(glm::mat4 const & transform){
        this->C = transform;
    }
    */
    
    
    void setCameraTransform(string key, int update){
        float velocity = 5;
        
        if(key == "up") {
            glm::vec3 go = (e-c)*.2f*float(update)*velocity;
            c-=go;
            e-=go;
            
        }
        else if(key == "down") {
            glm::vec3 go = (e-c)*.2f*float(update)*velocity;
            c+=go;
            e+=go;
            
        }
        else if(key == "left")
        {
            glm::vec3 go = glm::cross(e-c, u)*.2f*float(update)*velocity;
            c+=go;
            e+=go;
            
        }
        else if (key == "right") {
            glm::vec3 go = glm::cross(e-c, u)*.2f*float(update)*velocity;
            c-=go;
            e-=go;
        }
        
        else if(key == "turn_left"){
            const float angle = update;
            printf("turn left\n");
            
            glm::vec3 axis = glm::cross(e-c, glm::cross(e-c, u));
            glm::mat4 r = glm::rotate(glm::mat4(1), angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
            
        }
        
        else if(key == "turn_right"){
            const float angle = update;
            printf("turn right\n");
            
            glm::vec3 axis = glm::cross(e-c, glm::cross(e-c, u));
            glm::mat4 r = glm::rotate(glm::mat4(1), -angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
        }
        
        else if(key == "turn_up"){
            const float angle = update;
            glm::vec3 axis;
            
            axis = glm::cross(e-c, u);
            glm::mat4 r = glm::rotate(glm::mat4(1), angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
            
        }
        
        else if(key == "turn_down"){
            const float angle = update;
            printf("turn down\n");
            
            glm::vec3 axis = glm::cross(e-c, u);
            glm::mat4 r = glm::rotate(glm::mat4(1), -angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
            
        }
        
        this->C = glm::lookAt(e, c, u);
        printf("changed camera view!! ============\n");
    }
    
    
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
	
	void generateMaze(unsigned int const & seed = 1)
	{
        box123 = Geometry();
        box123.init(boxShader);
        box123.setV(glm::vec3(0,0,.01f));
        box2 = BoxSprite2();
        box2.init(boxShader);
        maze= MazeSprite();
        maze.init(shaderProg, w, h,1);
        
        
        //mazeLayout.getLeftOpening(opening_x, opening_y);
        printf("opening xy: %d,%d\n",opening_x,opening_y);
        printf("w h : %d,%d\n", w, h);
        
        
		e = glm::vec3(0,0,.5);
		c = glm::vec3(.2,0,.5);
		u = glm::vec3(0,0,1);
		this->C = glm::lookAt(e, c, u);
        
        
        setCameraTransform("down", 50*float(w)/float(10)*(.5-float(opening_x)/float(w))+7);
        setCameraTransform("right", 50*float(h)/float(10)*(.5-float(opening_y)/float(h))-3);
        
        //mazeLayout.getRightOpening(exit_x, exit_y);
        
        
        this->C = glm::lookAt(e, c, u);
        translateToOrigin = glm::translate(glm::mat4(1), glm::vec3(0,2,0));
        //translateToOrigin = glm::translate(glm::mat4(1), -model3D.getCentroid());
        
        move= glm::translate(glm::mat4(1), glm::vec3(0,0,.00001f));
	}
    
    
    
    
private:
    
    Geometry box123;
    BoxSprite2 box2;
    
    MazeSprite maze;
    
    sf::Clock clk;
    
	bool initialized;
    
	GLuint shaderProg;
    GLuint boxShader;
    
	GLuint positionBuffer;
	GLuint elementBuffer;
	GLuint vertexArray;
    
    GLuint boxPositionBuffer;
	GLuint boxElementBuffer;
	GLuint boxVertexArray;
    
    
    
	GLint positionSlot;
	GLint matSlot;
	
    GLint boxPositionSlot;
    GLint boxMatSlot;
    
	unsigned int w;
	unsigned int h;
	
    glm::mat4 P;
    glm::mat4 C;
    glm::mat4 M;
    glm::mat4 F;
    glm::mat4 R;
    glm::vec3 e, c, u;
    
    glm::mat4 translateToOrigin;
    glm::mat4 move;
    float e_x = 2.0;
    float e_y = 2.0;
    float e_z = 10.0;
    
    float c_x = 5.0;
    float c_y = 5.0;
    float c_z = 0;
    
    float u_x = 0;
    float u_y = 0;
    float u_z = 1.0;
    
    unsigned int opening_x,opening_y;
    unsigned int exit_x,exit_y;
    
	void setupGlew()
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(1);
		}
		printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}
    
	void setupShader()
	{
		char const * vertPath = "Shaders/simple.vert";
		char const * fragPath = "Shaders/simple.frag";
        
		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);
        
        char const * vertBox = "Shaders/boxshader.vert";
		char const * fragBox = "Shaders/boxfrag.frag";
        
        boxShader = ShaderManager::shaderFromFile(&vertBox, &fragBox, 1, 1);
        
		// Find out where the shader expects the data
		positionSlot = glGetAttribLocation(shaderProg, "pos");
		matSlot =      glGetUniformLocation(shaderProg, "M");
        
        boxPositionSlot = glGetAttribLocation(boxShader, "pos");
        boxMatSlot = glGetUniformLocation(boxShader, "M");
        
        
		checkGLError("shader");
	}
    
    
	
};

#endif