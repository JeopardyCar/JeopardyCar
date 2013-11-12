#ifndef RoadGen_h
#define RoadGen_h

#include <vector>
#include "SpriteMesh.h"
using namespace std; //makes using vectors easy

class RoadGen
{
public:
	RoadGen()
	{
	}
	~RoadGen()
	{
	}
	void init(GLuint shaderProg)
	{
		road1 = SpriteMesh("Model/road1.obj",shaderProg);
		road2 = SpriteMesh("Model/road1.obj",shaderProg);
		road3 = SpriteMesh("Model/road1.obj",shaderProg);
	}
	void update(glm::vec3 carPos){
        printf("update road : %f\n", carPos.y);
		road1.setPosM(glm::vec3(0,(int)(carPos.y)%10,-1));
		road2.setPosM(glm::vec3(0,(int)(carPos.y)%10+10,-1));
		road3.setPosM(glm::vec3(0,(int)(carPos.y)%10+20,-1));
	}
	void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
		road1.show(P,C,M);
		road2.show(P,C,M);
		road3.show(P,C,M);
	}
private:
	SpriteMesh road1;
	SpriteMesh road2;
	SpriteMesh road3;
};
#endif