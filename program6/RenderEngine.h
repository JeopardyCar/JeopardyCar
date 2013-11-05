#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include "MazeModel.h"

class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		this->P = glm::mat4(1);
		this->e = glm::vec3(0,-4,4);
		this->c = glm::vec3(0,0,0);
		this->u = glm::vec3(0,1,0);
		this->C = glm::lookAt(e, c, u);
		this->M = glm::mat4(1);
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteBuffers(1, &colorBuffer);

			glDeleteVertexArrays(1, &vertexArray);
		}
	}
	
	void moveCameraView0(float xIn, float yIn)
	{
		this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
		this->c = glm::vec3(e.x , e.y+2, e.z-2);
		this->C = glm::lookAt(e, c, u);
	}
	
	void moveCameraView1(float xIn, float yIn, int d)
	{
		if(d==0){ //up
			this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
			this->c = glm::vec3(e.x, e.y+1, e.z-.001);
			this->u = glm::vec3(0,1,0);
		}
		else if(d==3){ //right
			this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
			this->c = glm::vec3(e.x+1, e.y, e.z-.001);
			this->u = glm::vec3(1,0,0);
		}
		else if(d==1){ //down
			this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
			this->c = glm::vec3(e.x, e.y-1, e.z-.001);
			this->u = glm::vec3(0,-1,0);
		}
		else if(d==2){ //left
			this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
			this->c = glm::vec3(e.x-1 , e.y, e.z-.001);
			this->u = glm::vec3(-1,0,0);
		}
		this->C = glm::lookAt(e, c, u);
	}
	/*
	void moveCameraView1(float xIn, float yIn, int d)
	{
		this->e = glm::vec3(e.x + xIn, e.y+yIn, e.z);
		this->c = glm::vec3(e.x , e.y+.4, e.z-.001);
		this->C = glm::lookAt(e, c, u);
	}*/
	void view0Camera(int xPos, int yPos)
	{
		this->e = glm::vec3(.5,-.5,2);
		this->c = glm::vec3(.5,1.5,0);
		this->u = glm::vec3(0,1,0);
		this->e = glm::vec3(e.x + xPos, e.y+yPos, e.z);
		this->c = glm::vec3(c.x + xPos, c.y+yPos, c.z);
		this->C = glm::lookAt(e, c, u);
	}
	void view1Camera(int xPos, int yPos)
	{
		this->e = glm::vec3(.5,.5,0);
		this->c = glm::vec3(.5,1.5,-.001);
		this->u = glm::vec3(0,1,0);

		this->e = glm::vec3(e.x + xPos, e.y+yPos, e.z);
		this->c = glm::vec3(c.x + xPos, c.y+yPos, c.z);
		this->C = glm::lookAt(e, c, u);
	}
	void init(unsigned int const & w, unsigned int const & h)
	{
		this->w = w;
		this->h = h;

		GLint vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);

		setupGlew();
		setupShader();
		generateMaze();
		initialized = true;
	}

	void display(bool pickingEnabled=false)
	{
		glEnable(GL_DEPTH_TEST);

		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//use shader
		
		glUseProgram(shaderProg);

		//glm::mat4 T = P*C*M;
		glm::mat4 T = P*C*M;
		//glm::mat4 T = P;
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);


		glUniformMatrix4fv(camSlot, 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(projSlot, 1, GL_FALSE, &P[0][0]);
		float t = clock.GetElapsedTime();
		glUniform1f(timeSlot, t);

		//draw
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, model.getElementCount(), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_LINE_LOOP, model.getElementCount(), GL_UNSIGNED_INT, 0);
		//cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("display");
	}
	
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
	
	void generateMaze(unsigned int const & seed = 1)
	{
		Maze mazeLayout(w, h, seed);
		model = MazeModel(mazeLayout);
		
		//this->P = glm::ortho(-model.getUnitSize(), (w+1)*model.getUnitSize(), -model.getUnitSize(), (h+1)*model.getUnitSize());
	
		if(initialized)
			rebuildBuffers();
		else
			setupBuffers();
	}
	void setModelTransform(glm::mat4 const & transform)
	{
		this->M = transform;
	}
	void setProjectionTransform(glm::mat4 const & transform)
	{
		this->P = transform;
	}
	void setMove(float x1, float y1, float z1)
	{
		for(int a=0; a<model.getMax(w,h)*3; a++){
			moveList[3*a] += x1;
			moveList[3*a+1] += y1;
			moveList[3*a+2] += z1;
		}
		glBindBuffer(GL_ARRAY_BUFFER, moveBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(moveList), moveList);
	}
	void resetMove()
	{
		for(int a=0; a<model.getMax(w,h)*3; a++){
			moveList[3*a] = 0;
			moveList[3*a+1] = 0;
			moveList[3*a+2] = 0;
		}
	}
private:
	MazeModel model;
	bool initialized;

	GLuint shaderProg;

	GLuint positionBuffer;
	GLuint elementBuffer;
	GLuint colorBuffer;
	GLuint moveBuffer;
	GLuint normalBuffer;
	GLuint vertexArray;
	

	GLint positionSlot;
	GLint matSlot;
	GLint colorSlot;
	GLint moveSlot;
	GLint normalSlot;
	GLint projSlot;
	GLint camSlot;
	GLint timeSlot; 
	sf::Clock clock;

	float moveList[3600];
	float colorList[3600];
	float normList[3600];
	unsigned int w;
	unsigned int h;

	glm::vec3 e, c, u;

	glm::mat4 P;
	glm::mat4 M;
	glm::mat4 C;

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

		// Find out where the shader expects the data
		positionSlot = glGetAttribLocation(shaderProg, "pos");
		matSlot =  glGetUniformLocation(shaderProg, "M");
		colorSlot =  glGetAttribLocation(shaderProg, "colorIn");
		moveSlot =  glGetAttribLocation(shaderProg, "move");
		normalSlot = glGetAttribLocation(shaderProg, "norm");
		timeSlot =     glGetUniformLocation(shaderProg, "time");


		projSlot =      glGetUniformLocation(shaderProg, "P");
		camSlot =      glGetUniformLocation(shaderProg, "C");
		checkGLError("shader");
	}

	void setupBuffers()
	{
		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, model.getPositionBytes(), model.getPosition(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getElementBytes(), model.getElements(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		int max = model.getMax(w,h);
		int unused = model.getUnused(w,h);
		
		for(int a=0; a<1200; a++){

			if(a%8<4){
				colorList[3*a] = .1;
				colorList[3*a+1] = .7;
				colorList[3*a+2] = .2;
			}
			else{
				/*
				colorList[3*a] = .1;
				colorList[3*a+1] = 0;
				colorList[3*a+2] = .2;
				*/
				
				colorList[3*a] = .1;
				colorList[3*a+1] = .7;
				colorList[3*a+2] = .2;
				
			}
		}
		for(int a=0; a<(max-unused)*3*3; a++){
			if(a%8<4){
				normList[3*a] = 0;
				normList[3*a+1] = 0;
				normList[3*a+2] = 1;
			}
			else{
				normList[3*a] = 0;
				normList[3*a+1] = 0;
				normList[3*a+2] = 0;
			}
		}


		
		for(int a=0; a<max*3; a++){
			//moveList[3*a] = .625;
			//moveList[3*a+1] = .625;
			moveList[3*a] = 0;
			moveList[3*a+1] = 0;
			moveList[3*a+2] = 0;
		}
		
		// now the colors
		/*
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getColorBytes(), model.getColors(), GL_STATIC_DRAW);
		*/
		
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorList), colorList, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &moveBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, moveBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(moveList), moveList, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normList), normList, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//setup the vertex array
		glGenVertexArrays(1, &vertexArray);
		
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, moveBuffer);
		glEnableVertexAttribArray(moveSlot);
		glVertexAttribPointer(moveSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glEnableVertexAttribArray(normalSlot);
		glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);
		
		checkGLError("setup");
	}
	
	void rebuildBuffers()
	{
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, model.getPositionBytes(), model.getPosition());

		int max = model.getMax(w,h);
		for(int a=0; a<max*3; a++){
			moveList[3*a] = 0;
			moveList[3*a+1] = 0;
			moveList[3*a+2] = 0;
		}
		glBindBuffer(GL_ARRAY_BUFFER, moveBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(moveList), moveList);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, model.getElementBytes(), model.getElements());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, colorBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colorList), colorList);

		glBindVertexArray(vertexArray);	
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, moveBuffer);
		glEnableVertexAttribArray(moveSlot);
		glVertexAttribPointer(moveSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBindVertexArray(0);
		checkGLError("rebuild");
	}
};

#endif