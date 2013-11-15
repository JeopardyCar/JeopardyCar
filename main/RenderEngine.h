#ifndef __RENDERENGINE
#define __RENDERENGINE

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include "Cube.h"
#include "BoxSprite.h"
#include "BoxSprite2.h"
#include "Utilities.h"
#include "SpriteMesh.h"
#include "CarSprite.h"
#include "RoadGen.h"
#include "Score.h"
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
    
	//Initializes the game
	void init()
	{
		gamestate = 0; //main menu
		loadHighScores();
        totalFlyTime = 5;
        oldDirection = glm::vec3(1,1,1);
        targetDirection = glm::vec3(1,1,1);
		setupGlew();
		setupShader();
		generateObjs();
	}
    
	//Displays the game, called every frame
	void display(Keyset keys ,bool pickingEnabled=false)
	{
        numframes++;
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

        //If the car is in flying mode, then we update the time counter for seconds of flying left
        if(flying){
            GLfloat curTime = clk.GetElapsedTime();
            flyingleft = totalFlyTime-(curTime - flyingstart);
			keepScore.showFlyCounter((int)flyingleft+1,texShader);
			//If the car is out of time for flying mode, we revert to the standard model 
            if(flyingleft<0){
                flying = false;
                car.rebuildSprite("Model/blue.obj","Model/bluemap.bmp",TexID);
            }
        }

		bool onroad = true;

        if(onroad&&car.getPos().z<1){
            car.setPos(carpos.x, carpos.y, 1);
        }
        
        updateFlys();
        updateItems();
        
        bg.setPosM(c);
        
		//Initially, we planned to have a main menu function, but we were not able to complete this
		if(gamestate == 0){//main menu
			gamestate = 1;
			score = 0;
			subscore = 0;			
		}
		if(gamestate == 1){//game screen
			subscore++;
			if(subscore == 10) //the score is updated every 10 frames, based on the car's current velocity
			{
				subscore = 0;
				score+=abs(car.getV().y*10); 
			}
			keepScore.showFlyCounter(10-(subscore/30)%10,texShader);
			keepScore.update(score,texShader);
			keepScore.show();
			if(checkCollision())//If the car collides with an object, the game is over
			{
				counter = 0;
				gamestate = 2;
				saveScore();
			}
			car.show(P,C,M);
            bg.show(P, C, M);
			roads.update(car.getPos());
			roads.show(P,C,M);            
		}
		if(gamestate == 2){ //highscores screen
			//display highscores
			keepScore.update(score,texShader);
			keepScore.show();
            counter++;
			if(counter>20)
				keepScore.showHighScores(highscores[2],2,texShader);
			if(counter>40)
				keepScore.showHighScores(highscores[1],1,texShader);
			if(counter>60)
				keepScore.showHighScores(highscores[0],0,texShader);
            car.show(P,C,M);
            bg.setPosM(c);
            bg.show(P, C, M);
			roads.update(car.getPos());
			roads.show(P,C,M);
		}
	}
	
    void flyingmode(){
        score+=50;
        car.rebuildSprite("Model/car.obj","Model/texture.bmp",TexID);
        GLfloat currentTime;
        currentTime = clk.GetElapsedTime();
        flyingstart = currentTime;
        flying=true;
        flyingleft = 10.;
    }
	
    void updateItems(){
        glm::vec3 carpos = car.getPos();
        for(int i=0;i<items.size();i++)
        {
            glm::vec3 itempos = items[i].getPos();
            items[i].show(P,C,M);
            glm::vec3 norm = car.testCollision(items[i],1);
            if(getDis(carpos, items[i].getPos())<3){ //Flying mode
                flyingmode();
                int rand1 = (rand() % 1000000 + 1)/100000-5;
                int rand2 = (rand() % 1000000 + 1)/12500-40;
                items[i].setPosM(glm::vec3(carpos.x+rand1, carpos.y-rand2-90,1));
            }
            
            if(itempos.y<carpos.y+200){
                continue;
            }
            int rand1 = (rand() % 1000000 + 1)/100000-5;
            int rand2 = (rand() % 1000000 + 1)/12500-40;
            items[i].setPosM(glm::vec3(carpos.x+rand1, carpos.y-rand2-90,1));
            
            
            
        }
    }
    
    void updateFlys(){
        glm::vec3 carpos = car.getPos();
        for(int i=0;i<flys.size();i++){   
            glm::vec3 obspos=flys[i].getPos();
            flys[i].show(P, C, M);
            if(getDis(carpos, flys[i].getPos())<2){//If the car hits a black box
                glm::vec3 v = car.getV();
                v*=.85;
                car.setV(v);
            }  
            if(obspos.y<carpos.y+10){
                continue;
            }
            int rand1 = (rand() % 1000000 + 1)/100000-5;
            int rand2 = (rand() % 1000000 + 1)/12500-40;
            int rand3 = (rand() % 1000000 + 1)/100000-5;            
            flys[i].setPosM(glm::vec3(carpos.x+rand1*4, carpos.y-rand2-120,rand3*5));
            
        }
    } 
    
	bool checkCollision(){
        glm::vec3 carpos = car.getPos();
        for(int i=0;i<numframes/200+1;i++){
            if(i>obstacles.size()){
                return true;
            }
            glm::vec3 obspos=obstacles[i].getPos();
            obstacles[i].show(P, C, M);
            glm::vec3 norm = car.testCollision(obstacles[i],1);
            if(getLen(norm)>0){//The car has collided with something
                car.setAccelerate(glm::vec3(0,0,-.5));
                return true;
            }
            if(obspos.y<carpos.y+10){
                continue;
            }
            int rand1 = (rand() % 1000000 + 1)/100000-5;
            int rand2 = (rand() % 1000000 + 1)/12500-40;
            obstacles[i].setPosM(glm::vec3(carpos.x+rand1, carpos.y-rand2-90,0.5));
            
        }
        if(!flying){ //If the car is not flying, check if it is off the track
            if(car.getPos().x >5.5){
                car.setAccelerate(glm::vec3(0,0,-.5));
                return true;
            }else if(car.getPos().x<-5.5){
                car.setAccelerate(glm::vec3(0,0,-.5));
                return true;
            }
        }
        return false;
        
	}

	//Updates and saves the highscores to a text file
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

	//Retrieves the highscores from the text file
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
	
	//Generates the obstacles on the track
	void generateObjs(unsigned int const & seed = 1)
	{
        int numobs =10;
        for(int i=0;i<numobs;i++){
            SpriteMesh obs = SpriteMesh("Model/box.obj",texShader,"Model/red.bmp",TexID);
            obs.setPos(0, 0, 100);
            obstacles.push_back(obs);
        }
        
        int numflys =60;
        
        for(int i=0;i<numflys/2;i++){
            SpriteMesh obs = SpriteMesh("Model/box2.obj",texShader);
            obs.setPos(0, 0, 100);
            flys.push_back(obs);
        }
        for(int i=0;i<numflys/2;i++){
            SpriteMesh obs = SpriteMesh("Model/box3.obj",texShader);
            obs.setPos(0, 0, 100);
            flys.push_back(obs);
        }

        int numItem = 1;
        
        for (int i=0;i<numItem;i++){
            SpriteMesh item = SpriteMesh("Model/box2.obj",texShader, "Model/green.bmp",TexID);
            item.setPos(0,0,100);
            items.push_back(item);
        }

        car = CarSprite("Model/blue.obj",texShader,"Model/bluemap.bmp",TexID);
        car.setPosM(glm::vec3(0,0,1));
        
        bg = CarSprite("Model/bg.obj",boxShader,"Model/bg.bmp",TexID);

        box2 = BoxSprite2();
        box2.init(shaderProg);
        
        keepScore.init(texShader);
		roads.init(boxShader);

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
	int score,subscore,counter;
	int highscores[3];

    CarSprite car;
    BoxSprite2 box2;
    CarSprite bg;
    RoadGen roads;
    GLuint TexID;
	Score keepScore;
    
    int numframes;

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
    
    bool flying;
    GLfloat flyingleft;
    GLfloat flyingstart;
    GLfloat totalFlyTime;
    
    
    vector<SpriteMesh> obstacles;
    vector<SpriteMesh> flys;
    vector<SpriteMesh> items;
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