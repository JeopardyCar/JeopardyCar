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
#include "RoadGen.h"
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
        oldDirection = glm::vec3(1,1,1);
        targetDirection = glm::vec3(1,1,1);
		setupGlew();
		setupShader();
		generateObjs();
	}
    
	void display(Keyset keys ,bool pickingEnabled=false)
	{
        handleKeys(keys);
        
        GLfloat currentTime;
        currentTime = clk.GetElapsedTime();
        
		glEnable(GL_DEPTH_TEST);
		glClearColor(1.,  1., 1., 1.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        updateCamera();
        glm::vec3 carpos = car.getPos();
        vector<SpriteMesh*> roadsvec = roads.getRoads();
        glm::vec3 colnorm = glm::vec3(0,0,1);


		bool onroad = true;

        if(onroad&&car.getPos().z<1){
            car.setPos(carpos.x, carpos.y, 1);
        }
        
        
        
        /*
        glm::vec3 norm = car.testCollision(boxmesh,3);
        if(getLen(norm)>0){
            car.hitAndTurn(norm);
        }*/
        
        bg.setPosM(car.getPos());
        
        
        
        
        
        for(int i=0;i<roads.getEdges().size();i++){
            SpriteMesh * edge = roads.getEdges()[i];
            
            glm::vec3 norm =car.testCollision(*edge,3);
            //printf("edgexyz: %f,%f,%f\n", edge->getPos().x,edge->getPos().y,edge->getPos().z);
            //printf("carpos: %f,%f,%f\n", carpos.x,carpos.y,carpos.z);
            printf("normpos: %f,%f,%f\n", norm.x,norm.y,norm.z);
            if(getLen(norm)>0){
                printf("collision edge\n");
            }
        }
        
        
        
        
        
        for(int i=0;i<obstacles.size();i++){
            glm::vec3 obspos=obstacles[i].getPos();
            obstacles[i].show(P, C, M);
            
            glm::vec3 norm = car.testCollision(obstacles[i],1);
            if(getLen(norm)>0){
                //car.hitAndTurn(norm);
                //bg.hitAndTurn(norm);
                printf("carpos: %f,%f,%f\n", carpos.x,carpos.y,carpos.z);
                printf("obspos: %f,%f,%f\n", obspos.x,obspos.y,obspos.z);
                printf("hit!\n");
            }
            
            
            
            
            
            if(obspos.y<carpos.y+10){
                continue;
            }
            
            
            int rand1 = (rand() % 1000000 + 1)/100000-5;
            int rand2 = (rand() % 1000000 + 1)/25000-20;
            //printf("%f\n",obspos.y);
           
				obstacles[i].setPosM(glm::vec3(carpos.x+rand1, carpos.y-rand2-40,0.5));
            
        }
        
		
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
				printf("crashed!");
				gamestate = 2;
				saveScore();
			}
			car.show(P,C,M);
            bg.show(P, C, M);
            //boxmesh.show(P,C,M);
			roads.update(car.getPos());
			roads.show(P,C,M);
            //printf("pos: %f,%f,%f\n",car.getPos().x,car.getPos().y,car.getPos().z);
            
		}
		if(gamestate == 2){ //highscores screen
			//display highscores
			for(int x=0; x<3; x++)
			{
				printf("HIGHSCORE #%i : %i\n", x+1, highscores[x]);
				//scores.show()?
			}
            car.setAccelerate(glm::vec3(0,0,-.5));
            car.show(P,C,M);
            bg.setPosM(car.getPos());
            bg.show(P, C, M);
			roads.update(car.getPos());
			roads.show(P,C,M);

			gamestate = 2;
			int somePickingValue = 0;
			if(somePickingValue == 1){ //start button
				gamestate = 1;
				score = 0;
			}
			if(somePickingValue == 2){ //highscores button
				exit(EXIT_SUCCESS);
			}
		}
	}
    
	bool checkCollision(){
        if(car.getPos().x >5){
            return true;
        }else if(car.getPos().x<-5){
            return true;
        }
        return false;//false;
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
            //bg.up();
        }else if(key == "down"){
            car.down();
            //bg.down();
        }else if(key == "left"){
            car.left();
            //bg.left();
        }else if(key == "right"){
            car.right();
            //bg.right();
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
            
            glm::vec3 axis = glm::cross(e-c, glm::cross(e-c, u));
            glm::mat4 r = glm::rotate(glm::mat4(1), angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
        }
        
        else if(key == "turn_right"){
            const float angle = update;
            
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
            glm::vec3 axis = glm::cross(e-c, u);
            glm::mat4 r = glm::rotate(glm::mat4(1), -angle, axis);
            glm::vec4 c4= glm::vec4(e-c, 1);
            c4= c4*r;
            glm::vec3 c3 = glm::vec3(c4.x,c4.y,c4.z);
            e = c+c3;
        }
        
        this->C = glm::lookAt(e, c, u);
    }
    
    
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}
	
	void generateObjs(unsigned int const & seed = 1)
	{
        
        
        int numobs =1;
        
        
        
        for(int i=0;i<numobs;i++){
            SpriteMesh obs = SpriteMesh("Model/box.obj",shaderProg);
            obs.setPos(0, 0, 100);
            obstacles.push_back(obs);
        }
        
        
        
        car = CarSprite("Model/car.obj",texShader,"Model/texture.bmp",TexID);
        car.setPosM(glm::vec3(0,0,1));
        //car.setAccelerate(glm::vec3(0,0,-0.002));
        
        bg = CarSprite("Model/bg.obj",boxShader,"Model/whiteshadow.bmp",TexID);
        //bg.setAccelerate(glm::vec3(0,0,-0.002));
        
        box2 = BoxSprite2();
        box2.init(shaderProg);
        
        
		roads.init(boxShader);
        maze= MazeSprite();
        maze.init(shaderProg, 10, 10, 1);

        //boxmesh =SpriteMesh("",shaderProg);
        //boxmesh.setPosM(glm::vec3(0,0,10));
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
    //SpriteMesh boxmesh;
    CarSprite bg;
    RoadGen roads;
    GLuint TexID;


    sf::Clock clk;
    
	GLuint shaderProg;
    GLuint boxShader;
    GLuint texShader;
	
    glm::mat4 P;
    glm::mat4 C;
    glm::mat4 M;
    glm::vec3 e, c, u;
    
    glm::vec3 targetDirection;
    glm::vec3 oldDirection;
    Keyset keys;
    
    vector<SpriteMesh> obstacles;
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
    
    
    
    void handleKeys(Keyset keys){
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
    }
    
    
    void updateCamera(){
        c = car.getPos();
        e=c;
        
        if(getLen(targetDirection- normalize(car.getDirection()))>.03){
            targetDirection = normalize(car.getDirection());
        }
        
        
        if(getLen(oldDirection-targetDirection)>.03){
            glm::vec3 tmp =targetDirection-oldDirection;
            tmp*=.07;
            oldDirection += tmp;
            e.z+=3;//7*car.getDirection().z;
            e.y-=10*oldDirection.y;
            e.x-=10*oldDirection.x;
        }else{
            oldDirection = targetDirection;
            e.z+=3;//7*car.getDirection().z;
            e.y-=10*oldDirection.y;
            e.x-=10*oldDirection.x;
        }
        
        this->C = glm::lookAt(e, c, u);

    }
    
    
    
    
    
    
    
    
	void setupShader()
	{
		char const * vertPath = "Shaders/simple.vert";
		char const * fragPath = "Shaders/simple.frag";
		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);

        char const * vertBox = "Shaders/boxshader.vert";
		char const * fragBox = "Shaders/boxfrag.frag";
        boxShader = ShaderManager::shaderFromFile(&vertBox, &fragBox, 1, 1);
        
        char const * vertTex = "Shaders/tex.vert";
		char const * fragTex = "Shaders/tex.frag";
        texShader = ShaderManager::shaderFromFile(&vertTex, &fragTex, 1, 1);
	}
    
    
	
};

#endif