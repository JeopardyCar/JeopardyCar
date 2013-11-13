#ifndef Score_h
#define Score_h

#include <vector>
#include "SpriteMesh.h"
using namespace std; //makes using vectors easy

class Score
{
public:
	Score()
	{
	}
	~Score()
	{
        
	}
	void init(GLuint shaderProg,char * filename ="Model/num1.obj")
	{
        digit1 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID);  
        digit2 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID);     
        digit3 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID);      
        digit4 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID);      

	}
    
	void update(int currScore){
		
        //digit1.setPosM(glm::vec3(carPos.x,carPos.y,carPos.z));
       //
		
	}
	void show(){
        digit1.showStatic(glm::vec3(.9,0,-.8));
		digit2.showStatic(glm::vec3(.7,0,-.8));
		digit3.showStatic(glm::vec3(.5,0,-.8));
		digit4.showStatic(glm::vec3(.3,0,-.8));
	}
    
    
    
private:
    
    SpriteMesh digit1, digit2, digit3, digit4;
    GLuint TexID;
};
#endif