//Replace with your usernames!
#define printusers() printf("Program by Yiren_Lu+Zizhao_Wang+Logan_Hallowell+Heather_Finnell\n");

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
#include "TrackBall.h"


#define RESOLUTION 512
#define TARGET_FPS 30                // controls spin update rate
#define TIME_WINDOW 3                // number of frames motion is valid after release


class Program4
{
public:
	Program4()
	{
        App = new sf::Window(sf::VideoMode(RESOLUTION*2, RESOLUTION, 32), "program5");
        
		render.init();
		step = 2;
        
        
        previousPos = glm::vec2(0);
		buttonDown[0]=false; buttonDown[1]=false; buttonDown[2]=false;
		trackball.setSize(RESOLUTION, RESOLUTION);
        
        glm::mat4 P = glm::perspective(60.0f, (float)RESOLUTION*2/(float)RESOLUTION, 0.1f, 100.0f);
		render.setProjectionTransform(P);
		
		while (App->IsOpened())
		{
			App->SetActive();
			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0){
				sf::Sleep(sleepTime);
            }
			render.display(keys);
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
    
    glm::ivec2 previousPos;
    bool buttonDown[3];
	glm::mat4 translateFromInput;
	glm::mat4 rotationFromInput;
	glm::mat4 rotationSpinStep;
    glm::mat4 ratioRescale;
    
    glm::vec3 ax;
    float ph;
    
    
    TrackBall trackball;
    
    Keyset keys;
    
    
    void updateRotate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
		float phi;
		glm::vec3 axis;
        
        glm::ivec2 cubePos;
        
        trackball.getRotation(phi, axis, oldPos,newPos);
        ax = axis;
        ph = phi;
        
        glm::mat4 r = glm::rotate(glm::mat4(1), phi, axis);
        rotationFromInput = r*rotationFromInput;
        if(abs(newPos.y-oldPos.y)>abs(newPos.x-oldPos.x)){
            
            if(newPos.y<oldPos.y){
                render.setCameraTransform("turn_up", 3);
            }else{
                render.setCameraTransform("turn_down", 3);
            }
        }else{
            if(newPos.x>oldPos.x){
                render.setCameraTransform("turn_right", 3);
            }else{
                render.setCameraTransform("turn_left", 3);
            }
        }
	}
	
	void updateXYTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
        #define XY_SENSITIVITY 0.005f   //may be helpful to reduce transform amount
        
        
        glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3((newPos.x-oldPos.x)*XY_SENSITIVITY, (oldPos.y-newPos.y)*XY_SENSITIVITY, 0));
        translateFromInput = t*translateFromInput;
        
	}
	
	void updateZTranslate(glm::ivec2 & oldPos, glm::ivec2 & newPos)
	{
#define Z_SENSITIVITY 0.005f //may be helpful to reduce transform amount
		glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, (oldPos.y-newPos.y)*XY_SENSITIVITY));
        translateFromInput = t*translateFromInput;
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
            
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::Up)){
                keys.UP =true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::Up)){
                keys.UP= false;
            }
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::Down)){
                keys.DOWN = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::Down)){
                keys.DOWN = false;
            }
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::Left)){
                keys.LEFT = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::Left)){
                keys.LEFT= false;
            }
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::Right)){
                keys.RIGHT = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::Right)){
                keys.RIGHT = false;
            }
            
            
            
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::W)){
                keys.W = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::W)){
                keys.W = false;
            }
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::S)){
                keys.S = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::S)){
                keys.S = false;
            }
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::A)){
                keys.A = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::A)){
                keys.A = false;
            }
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::D)){
                keys.D = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::D)){
                keys.D = false;
            }
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::Q)){
                keys.Q = true;
            }
            
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::Q)){
                keys.Q = false;
            }
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::E)){
                keys.E = true;
            }
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::E)){
                keys.E = false;
            }
            
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::R)){
                keys.R = true;
            }
            
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::R)){
                keys.R = false;
            }
            
            
            if(Event.Type == sf::Event::KeyPressed && (Event.Key.Code == sf::Key::F)){
                keys.F = true;
            }
            
            if(Event.Type == sf::Event::KeyReleased && (Event.Key.Code == sf::Key::F)){
                keys.F = false;
            }
           

		




            if (Event.Type == sf::Event::MouseButtonPressed)
			{
				previousPos = glm::vec2(Event.MouseButton.X, Event.MouseButton.Y);
				
				if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
				{
					buttonDown[0] = 1;
				}
				if(Event.MouseButton.Button == sf::Mouse::Right)
					buttonDown[1] = true;
				if(Event.MouseButton.Button == sf::Mouse::Middle)
					buttonDown[2] = true;
				if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
					buttonDown[2] = true;
			}
			
			if (Event.Type == sf::Event::MouseButtonReleased)
			{
				if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
					buttonDown[0] = false;
				if(Event.MouseButton.Button == sf::Mouse::Right)
					buttonDown[1] = false;
				if(Event.MouseButton.Button == sf::Mouse::Middle)
					buttonDown[2] = false;
				if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
					buttonDown[2] = false;
				float maxTime = 1.0f/(float)TARGET_FPS * TIME_WINDOW;
			}
			
			if (Event.Type == sf::Event::MouseMoved && (buttonDown[0] || buttonDown[1] || buttonDown[2]) )
			{
				glm::ivec2 newPos = glm::ivec2(Event.MouseMove.X, Event.MouseMove.Y);
				
				if(buttonDown[0])
					updateRotate(previousPos, newPos);
				if(buttonDown[1])
					updateXYTranslate(previousPos, newPos);
				if(buttonDown[2])
					updateZTranslate(previousPos, newPos);
				
				previousPos = newPos;
			}
            
            
		}
	}
};

int main(int argc, char ** argv)
{
	printusers();
	Program4 prog;
	
    return EXIT_SUCCESS;
}
