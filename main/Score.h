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
    
	void update(int currScore,GLuint shaderProg,char * filename ="Model/num1.obj"){
		int a = currScore%10;
		switch (a){
		case 0: digit4 =SpriteMesh(filename,shaderProg,"Model/num0.bmp",TexID); break;
		case 1: digit4 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID); break;
		case 2: digit4 =SpriteMesh(filename,shaderProg,"Model/num2.bmp",TexID); break;
		case 3: digit4 =SpriteMesh(filename,shaderProg,"Model/num3.bmp",TexID); break;
		case 4: digit4 =SpriteMesh(filename,shaderProg,"Model/num4.bmp",TexID); break;
		case 5: digit4 =SpriteMesh(filename,shaderProg,"Model/num5.bmp",TexID); break;
		case 6: digit4 =SpriteMesh(filename,shaderProg,"Model/num6.bmp",TexID); break;
		case 7: digit4 =SpriteMesh(filename,shaderProg,"Model/num7.bmp",TexID); break;
		case 8: digit4 =SpriteMesh(filename,shaderProg,"Model/num8.bmp",TexID); break;
		case 9: digit4 =SpriteMesh(filename,shaderProg,"Model/num9.bmp",TexID); break;
		}
		a = (currScore%100-currScore%10)/ 10;
		switch (a){
		case 0: digit3 =SpriteMesh(filename,shaderProg,"Model/num0.bmp",TexID); break;
		case 1: digit3 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID); break;
		case 2: digit3 =SpriteMesh(filename,shaderProg,"Model/num2.bmp",TexID); break;
		case 3: digit3 =SpriteMesh(filename,shaderProg,"Model/num3.bmp",TexID); break;
		case 4: digit3 =SpriteMesh(filename,shaderProg,"Model/num4.bmp",TexID); break;
		case 5: digit3 =SpriteMesh(filename,shaderProg,"Model/num5.bmp",TexID); break;
		case 6: digit3 =SpriteMesh(filename,shaderProg,"Model/num6.bmp",TexID); break;
		case 7: digit3 =SpriteMesh(filename,shaderProg,"Model/num7.bmp",TexID); break;
		case 8: digit3 =SpriteMesh(filename,shaderProg,"Model/num8.bmp",TexID); break;
		case 9: digit3 =SpriteMesh(filename,shaderProg,"Model/num9.bmp",TexID); break;
		}
		a = (currScore%1000-currScore%100)/ 100;
		switch (a){
		case 0: digit2 =SpriteMesh(filename,shaderProg,"Model/num0.bmp",TexID); break;
		case 1: digit2 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID); break;
		case 2: digit2 =SpriteMesh(filename,shaderProg,"Model/num2.bmp",TexID); break;
		case 3: digit2 =SpriteMesh(filename,shaderProg,"Model/num3.bmp",TexID); break;
		case 4: digit2 =SpriteMesh(filename,shaderProg,"Model/num4.bmp",TexID); break;
		case 5: digit2 =SpriteMesh(filename,shaderProg,"Model/num5.bmp",TexID); break;
		case 6: digit2 =SpriteMesh(filename,shaderProg,"Model/num6.bmp",TexID); break;
		case 7: digit2 =SpriteMesh(filename,shaderProg,"Model/num7.bmp",TexID); break;
		case 8: digit2 =SpriteMesh(filename,shaderProg,"Model/num8.bmp",TexID); break;
		case 9: digit2 =SpriteMesh(filename,shaderProg,"Model/num9.bmp",TexID); break;
		}
        a = (currScore%10000-currScore%1000)/ 1000;
		switch (a){
		case 0: digit1 =SpriteMesh(filename,shaderProg,"Model/num0.bmp",TexID); break;
		case 1: digit1 =SpriteMesh(filename,shaderProg,"Model/num1.bmp",TexID); break;
		case 2: digit1 =SpriteMesh(filename,shaderProg,"Model/num2.bmp",TexID); break;
		case 3: digit1 =SpriteMesh(filename,shaderProg,"Model/num3.bmp",TexID); break;
		case 4: digit1 =SpriteMesh(filename,shaderProg,"Model/num4.bmp",TexID); break;
		case 5: digit1 =SpriteMesh(filename,shaderProg,"Model/num5.bmp",TexID); break;
		case 6: digit1 =SpriteMesh(filename,shaderProg,"Model/num6.bmp",TexID); break;
		case 7: digit1 =SpriteMesh(filename,shaderProg,"Model/num7.bmp",TexID); break;
		case 8: digit1 =SpriteMesh(filename,shaderProg,"Model/num8.bmp",TexID); break;
		case 9: digit1 =SpriteMesh(filename,shaderProg,"Model/num9.bmp",TexID); break;
		}
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