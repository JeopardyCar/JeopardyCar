//Replace with your usernames!
#define printusers() printf("Program by USER1+USER2\n");

#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include "glew/glew.h"
#include <SFML/Window.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ShaderManager.h"
#include "GLHelper.h"
#include "RenderEngine.h"
#include "Maze.h"
#include "MazeModel.h"

#define RESOLUTION 512
#define TARGET_FPS 30                // controls spin update rate
#define TIME_WINDOW 3                // number of frames motion is valid after release
int currX, currY;
char facing;
Maze mazeLayout;
int mazeW,mazeH;
int viewMode;
class Program4
{
public:
	Program4(unsigned int const & w, unsigned int const & h)
	{
		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program5");
		glm::mat4 translateToOrigin = glm::translate(glm::mat4(1), glm::vec3(0,0,0));
		render.setModelTransform(translateToOrigin);
		
		
		glm::mat4 P = glm::perspective(60.0f, (float)RESOLUTION/(float)RESOLUTION, 0.1f, 100.0f);
		render.setProjectionTransform(P);
		viewMode=0;
		mazeW = w; mazeH = h;
		render.init(w, h);
		mazeLayout = Maze(w, h, 1);
		step = 2;
		currX = 0;
		currY = 0;
		facing = 'u';
		printStatus();
		while (App->IsOpened())
		{			
			App->SetActive();
			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0)
				sf::Sleep(sleepTime);
			render.display();
			App->Display();
			handleEvents();
		}
	}
	
	~Program4()
	{
	}
	
private:
	sf::Window *App;
	RenderEngine render;
	unsigned int step;
	void printStatus(){
		printf("You are at (%i , %i) facing ", currX, currY);
		switch(facing){
			case 'u':
				printf("Up\n");
				break;
			case 'r': 
				printf("Right\n");
				break;
			case 'd': 
				printf("Down\n");
				break;
			case 'l': 
				printf("Left\n");
				break;
		}
	}
	void handleEvents()
	{
		const sf::Input& Input = App->GetInput();
		bool shiftDown = Input.IsKeyDown(sf::Key::LShift) || Input.IsKeyDown(sf::Key::RShift);
		sf::Event Event;
		while (App->GetEvent(Event))
		{
			if (Event.Type == sf::Event::Closed)
				App->Close();
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App->Close();
			
			// This is for grading your code. DO NOT REMOVE
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Equal)
			{
				unsigned char *dest;
				unsigned int w = App->GetWidth();
				unsigned int h = App->GetHeight();
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
				glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
				
				FILE *file;
				file = fopen("_program1.ppm", "wb");
				fprintf(file, "P6\n%i %i\n%i\n", w, h, 255);
				for(int i=h-1; i>=0; i--)
					fwrite(&dest[i*w*3], sizeof(unsigned char), w*3, file);
				fclose(file);
				free(dest);
			}
			
			if (Event.Type == sf::Event::Resized)
			{ glViewport(0, 0, Event.Size.Width, Event.Size.Height); }
			
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
			{
				render.generateMaze(step);
				mazeLayout = Maze(mazeW, mazeH, step);
				currX = 0; currY=0;
				step++;
				printStatus();
				updateTrans(0,0,0);
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up))
			{
				if(viewMode==0){
					if(mazeLayout.topBlocked(currX, currY))
						printf("Path blocked!\n");
					else{
						currY++;
						printf("Moving to (%i , %i)\n", currX, currY);
						render.moveCameraView0(0,1.0f);
						facing = 'u';
					}
				}
				else{
					render.moveCameraView1(0,0,0);
					if(facing=='u'){
						if(mazeLayout.topBlocked(currX, currY))
							printf("Path blocked!\n");
						else{
							currY++;
							render.moveCameraView1(0,1.0f,0);
							facing = 'u';
						}
					}
					else if(facing=='r'){
						if(mazeLayout.rightBlocked(currX, currY))
							printf("Path blocked!\n");
						else {
							if(currX == mazeLayout.getWidth()-1){
									printf("Finish!\n");
									render.moveCameraView1(0,0,3);
									}
							else{
								currX++;
								render.moveCameraView1(1.0f,0,3);
								facing = 'r';
							}	
						}
					}
					else if(facing=='d'){
						if(mazeLayout.bottomBlocked(currX, currY))
							printf("Path blocked!\n");
						else{
							currY--;
							render.moveCameraView1(0,-1.0f,1);
							facing = 'd';
						}
					}
					else if(facing=='l'){
						
						if(mazeLayout.leftBlocked(currX, currY))
							printf("Path blocked!\n");
						else{ if(currX == 0)
							printf("Can't exit through the entrance!\n");
						else{
							currX--;
							render.moveCameraView1(-1.0f,0,2);
							facing = 'l';
						}}
					}
					printf("Moving to (%i , %i)\n", currX, currY);
				}

				
				printStatus();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down))
			{
				
					if(viewMode==0){
						if(mazeLayout.bottomBlocked(currX, currY))
							printf("Path blocked!\n");
						else{
							currY--;
							printf("Moving to (%i , %i)\n", currX, currY);
							render.moveCameraView0(0,-1.0f);
							facing = 'd';
						}
					}
					else{
							if(facing=='d'){
								facing='u';
								render.moveCameraView1(0,0,0);
							}
							else if(facing=='u'){
								facing='d';
								render.moveCameraView1(0,0,1);
							}
							else if(facing=='r'){
								facing='l';
								render.moveCameraView1(0,0,2);
							}
							else if(facing=='l'){
								facing='r';
								render.moveCameraView1(0,0,3);
							}
						/*
						if(facing=='d'){
							if(mazeLayout.topBlocked(currX, currY))
								printf("Path blocked!\n");
							else{
								currY++;
								render.moveCameraView1(0,1.0f,1);
							}
						}
						if(facing=='l'){
							if(mazeLayout.rightBlocked(currX, currY))
								printf("Path blocked!\n");
							else{
								currX++;
								render.moveCameraView1(1.0f,0,1);
							}
						}
						if(facing=='u'){
							if(mazeLayout.bottomBlocked(currX, currY))
								printf("Path blocked!\n");
							else{
								currY--;
								render.moveCameraView1(0,-1.0f,1);
							}
						}
						if(facing=='r'){
							if(mazeLayout.leftBlocked(currX, currY))
								printf("Path blocked!\n");
							else{
								currX--;
								render.moveCameraView1(-1.0f,0,1);
							}
						}
						printf("Moving to (%i , %i)\n", currX, currY);*/
					}
				
				printStatus();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Left))
			{

					if(viewMode==0){
						if(mazeLayout.leftBlocked(currX, currY))
							printf("Path blocked!\n");
						else {if(currX == 0)
							printf("Can't exit through the entrance!\n");
						else{
							currX--;
							printf("Moving to (%i , %i)\n", currX, currY);
							render.moveCameraView0(-1.0f,0);
							facing = 'l';
						}}
					}
						else{
							if(facing=='r'){
								facing='u';
								render.moveCameraView1(0,0,0);
							}
							else if(facing=='l'){
								facing='d';
								render.moveCameraView1(0,0,1);
							}
							else if(facing=='u'){
								facing='l';
								render.moveCameraView1(0,0,2);
							}
							else if(facing=='d'){
								facing='r';
								render.moveCameraView1(0,0,3);
							}
							/*
							if(facing=='l'){
								if(mazeLayout.bottomBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currY--;
									render.moveCameraView1(0,-1.0f,2);
								}
							}
							if(facing=='u'){
								if(mazeLayout.leftBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currX--;
									render.moveCameraView1(-1.0f,0,2);
								}
							}
							if(facing=='r'){
								if(mazeLayout.topBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currY++;
									render.moveCameraView1(0,1.0f,2);
								}
							}
							if(facing=='d'){
								if(mazeLayout.rightBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currX++;
									render.moveCameraView1(1.0f,0,2);
								}
							}
							printf("Moving to (%i , %i)\n", currX, currY);*/
						}
				
				printStatus();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Right))
			{
					
					if(viewMode==0){
						if(mazeLayout.rightBlocked(currX, currY))
							printf("Path blocked!\n");
						else{if(currX == mazeLayout.getWidth()-1){
							printf("Finish!\n");
							facing = 'r';}
						else{
							currX++;
							printf("Moving to (%i , %i)\n", currX, currY);
							render.moveCameraView0(1.0f,0);
							facing = 'r';
						}}
					}

						else{
							//render.moveCameraView1(0,0,3);
							if(facing=='l'){
								facing='u';
								render.moveCameraView1(0,0,0);
							}
							else if(facing=='r'){
								facing='d';
								render.moveCameraView1(0,0,1);
							}
							else if(facing=='d'){
								facing='l';
								render.moveCameraView1(0,0,2);
							}
							else if(facing=='u'){
								facing='r';
								render.moveCameraView1(0,0,3);
							}
							/*
							if(facing=='r'){
								if(mazeLayout.bottomBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currY--;
									render.moveCameraView1(0,-1.0f,3);
								}
							}
							if(facing=='d'){
								if(mazeLayout.leftBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currX--;
									render.moveCameraView1(-1.0f,0,3);
								}
							}
							if(facing=='l'){
								if(mazeLayout.topBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currY++;
									render.moveCameraView1(0,1.0f,3);
								}
							}
							if(facing=='u'){
								if(mazeLayout.rightBlocked(currX, currY))
									printf("Path blocked!\n");
								else{
									currX++;
									render.moveCameraView1(1.0f,0,3);
								}
							}
							printf("Moving to (%i , %i)\n", currX, currY);*/
						}
				//facing = 'r';
				printStatus();
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::X)){
				if(viewMode!=1){
					render.view1Camera(currX,currY);
						if(facing=='u'){
								render.moveCameraView1(0,0,0);
							}
							else if(facing=='d'){
								render.moveCameraView1(0,0,1);
							}
							else if(facing=='l'){
								render.moveCameraView1(0,0,2);
							}
							else if(facing=='r'){
								render.moveCameraView1(0,0,3);
							}
					printf("Switching to view mode 1\n");
				}
				//updateRotate(glm::ivec2(0,0), glm::ivec2(1,0));
				viewMode = 1;
			}
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Z)){
				if(viewMode!=0){
					render.view0Camera(currX,currY);
					printf("Switching to view mode 0\n");
				}
				//resetRotate(glm::ivec2(0,0), glm::ivec2(1,0));
				viewMode=0;
			}
		}
	}
	void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		
		float phi = -90.0;
		glm::vec3 axis=  glm::vec3(1,0,0);
		glm::mat4 rotationSpinStep= glm::rotate(glm::mat4(1.0f),phi,axis);
		render.setModelTransform(rotationSpinStep);
	}
	void updateTrans(float xchange, float ychange, float zchange)
	{
		render.setMove(xchange,ychange,zchange);
	}
	void resetRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{

		//computes the appropriate rotation data and stores in phi and axis
		float phi = 0.0;
		glm::vec3 axis=  glm::vec3(1,0,0);
		glm::mat4 rotationSpinStep= glm::rotate(glm::mat4(1.0f),phi,axis);
		render.setModelTransform(rotationSpinStep);
		//render.updateM(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
	}
};

int main(int argc, char ** argv)
{
	printusers();
	
	/* check that there are sufficient arguments */
	unsigned int w;
	unsigned int h;
	if (argc < 3) {
		w = 6;
		h = 6;
		fprintf(stderr, "The width and height can be specified as command line arguments. Defaulting to %i %i\n", w, h);
	}
	else {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}
	
	Program4 prog(w, h);
	
    return EXIT_SUCCESS;
}
