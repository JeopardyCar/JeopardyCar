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
        for(int i=0;i<roads.size();i++){
            delete roads[i];
        }
	}
	void init(GLuint shaderProg,char * filename ="Model/broad.obj")
	{
        for(int i= 0;i<6;i++){
           SpriteMesh* road =new SpriteMesh(filename,shaderProg,"Model/road1_T.bmp",TexID);
            roads.push_back(road);
        }
	}
    
	void update(glm::vec3 carPos){
        for(int i=0 ;i< roads.size();i++){
            SpriteMesh* road = roads[i];
            printf("%f\n",road->getPos().y);
            road->setPosM(glm::vec3(0,((int)((carPos.y)/10))*10-i*10+20,-1));
        }
    
	}
	void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
        for(int i=0;i<roads.size();i++){
            roads[i]->show(P,C,M);
        }
	}
    
    vector<SpriteMesh*> getRoads(){
        return roads;
    }
    
private:
    
    vector<SpriteMesh*> roads;
    GLuint TexID;
};
#endif