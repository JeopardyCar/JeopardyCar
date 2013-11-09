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
#include "SpriteMesh.h"
#include "CarSprite.h"
#include <fstream>
#include <string>


#define PI 3.1415926


struct Keyset{
    bool W;
    bool S;
    bool A;
    bool D;
    bool Q;
    bool E;
    bool R;
    bool F;
    
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
    
};



class RenderEngine
{
public:
	RenderEngine()
	{
        this->P = glm::mat4(1);
	}
    
	~RenderEngine()
	{
		
	}
    
	void init()
	{
		gamestate = 0; //main menu
		first = true;
		loadHighScores();

		setupGlew();
		setupShader();
		generateObjs();
	}
    
	void display(Keyset keys ,bool pickingEnabled=false)
	{
        if(keys.W){
            setCameraTransform("up", 1);
        }
        if(keys.S){
            setCameraTransform("down", 1);
        }
        if(keys.A){
            setCameraTransform("left", 1);
        }
        if(keys.D){
            setCameraTransform("right",1);
        }
        if(keys.Q){
            setCameraTransform("turn_left",3);
        }
        if(keys.E){
            setCameraTransform("turn_right", 3);
        }
        if(keys.R){
            setCameraTransform("turn_up", 3);
        }
        if(keys.F){
            setCameraTransform("turn_down", 3);
        }
        if(keys.UP){
            carGo("up");
        }
        if(keys.DOWN){
            carGo("down");
        }
        if(keys.LEFT){
            carGo("left");
        }
        if(keys.RIGHT){
            carGo("right");
        }
        
        
        
        
        
        GLfloat currentTime;
        currentTime = clk.GetElapsedTime();
        
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glm::mat4 T =  P*C*M;
        //show objects;
        car.show(P,C,M);
        c = car.getPos();
        
        e=c;
        e.z+=3;
        e.y-=7*car.getDirection().x;
        e.x-=7*car.getDirection().y;
        
        
        this->C = glm::lookAt(e, c, u);
        box2.show(T);
        maze.show(T);


		if(gamestate == 0){//main menu
			if(first){
				first = false;
				printf("Press the button to continue\n");
			}
			//display buttons and title

			//
			int somePickingValue = 1;//whatever button the user clicks on
			
			if(somePickingValue == 1){ //start button
				gamestate = 1;
				score = 0;
			}
			if(somePickingValue == 2){ //highscores button
				gamestate = 2;
			}
			if(somePickingValue == 3){ //exit button
				exit(EXIT_SUCCESS);
			}
			
		}
		if(gamestate == 1){//game screen
			score+=1; 
			if(checkCollision())
			{
				//printf("crashed!");
				gamestate = 0;
				saveScore();
			}
			car.show(T);
			box2.show(T);
			maze.show(T);
		}
		if(gamestate == 2){ //highscores screen
			//display highscores
			for(int x=0; x<3; x++)
			{
				printf("HIGHSCORE #%i : %i\n", x+1, highscores[x]);
			}

			int somePickingValue = 1;//whatever button the user clicks on
			if(somePickingValue == 1){ //main menu button
				gamestate = 0;
			}
		}
	}
    
	bool checkCollision(){

	}

		return true;//false;
	}
	void saveScore()
	{
		if(score>highscores[0])
		{
			highscores[2]= highscores[1];
			highscores[1]= highscores[0];
			highscores[0]= score;
		}
		else if(score>highscores[1])
		{
			highscores[2]= highscores[1];
			highscores[1]= score;
		}
		else if(score>highscores[2])
		{
			highscores[2]= score;
		}
		ofstream fout;
		fout.open ("highscores.txt");
		for(int x=0; x<3; x++)
		{
			fout << highscores[x];
			fout << "\n";
		}
		fout.close();
	}
	void loadHighScores(){
		string line ;
		int num = 0;
		ifstream fin ("highscores.txt");
		if (fin.is_open())
		{
			while ( getline (fin,line) )
			{
				highscores[num] = atoi(line.c_str());
				num++;
				printf("HIGHSCORE - %i\n",atoi(line.c_str()));
			}
			fin.close();
		}
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
    
    
    
    
    
    
    
    void carGo(string key){
        if(key=="up"){
            car.up();
        }else if(key == "down"){
            car.down();
        }else if(key == "left"){
            car.left();
        }else if(key == "right"){
            car.right();
        }
        
    }
    
    
    
    
    
    
    void setCameraTransform(string key, float update){
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
            //printf("turn left\n");
            
            glm::vec3 axis = glm::cross(e-c, glm::cross(e-c, u));
            glm::mat4 r = glm::rotate(glm::mat4(1), angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
        }
        
        else if(key == "turn_right"){
            const float angle = update;
            //printf("turn right\n");
            
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
           // printf("turn down\n");
            
            glm::vec3 axis = glm::cross(e-c, u);
            glm::mat4 r = glm::rotate(glm::mat4(1), -angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
            
        }
        
        this->C = glm::lookAt(e, c, u);
        //printf("changed camera view!! ============\n");
    }
    
    
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
	
	void generateObjs(unsigned int const & seed = 1)
	{
        car = CarSprite(boxShader);
        car.setPosM(glm::vec3(0,0,1));
        //car.setV(glm::vec3(0,0,.01f));
        //car.setAccelerate(glm::vec3(0,0,0.001f));
        
        
        
        box2 = BoxSprite2();
        box2.init(shaderProg);
        
        
        
        maze= MazeSprite();
        maze.init(shaderProg, 10, 10, 1);
        
		e = glm::vec3(0,0,.5);
		c = glm::vec3(.2,0,.5);
		u = glm::vec3(0,0,1);
		this->C = glm::lookAt(e, c, u);
        
        setCameraTransform("down", 10);
        setCameraTransform("left", 10);
        setCameraTransform("turn_right", 90);
	}
    
    
    
    
private:
    int gamestate;
	bool first;
	int score;
	int highscores[3];

    CarSprite car;
    BoxSprite2 box2;
    MazeSprite maze;
    
    sf::Clock clk;
    
	GLuint shaderProg;
    GLuint boxShader;
	
    glm::mat4 P;
    glm::mat4 C;
    glm::mat4 M;
    glm::vec3 e, c, u;
    
    Keyset keys;
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
	}
    
    
	
};

#endif