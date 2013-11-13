#ifndef RoadGen_h
#define RoadGen_h

#include <vector>
#include "SpriteMesh.h"
using namespace std; //makes using vectors easy




#define 


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
	void init(GLuint shaderProg,char * filename ="Model/broad.obj",char * edgefilename = "Model/broad_edge.obj")
	{
        direction = 0;
        for(int i= 0;i<8;i++){
            SpriteMesh* road =new SpriteMesh(filename,shaderProg,"Model/Road1_T1.bmp",TexID);
            SpriteMesh* edge =new SpriteMesh(edgefilename,shaderProg,"",TexID);
            roads.push_back(road);
            edges.push_back(edge);
        }
	}
    
	void update(glm::vec3 carPos){
        for(int i=0 ;i< roads.size();i++){
            SpriteMesh* road = roads[i];
            SpriteMesh* edge = edges[i];
            //printf("%f\n",road->getPos().y);
            road->setPosM(glm::vec3(0,((int)((carPos.y)/10))*10-i*10+10,0));
            edge->setPosM(glm::vec3(0,((int)((carPos.y)/10))*10-i*10+10,0));
        }
	}
	void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
        for(int i=0;i<roads.size();i++){
            roads[i]->show(P,C,M);
            edges[i]->show(P,C,M);
        }
	}
    
    vector<SpriteMesh*> getRoads(){
        return roads;
    }
    
    vector<SpriteMesh*> getEdges(){
        return edges;
    }
    
    
private:
    
    int direction;
    
    
    
    vector<SpriteMesh*> edges;
    vector<SpriteMesh*> roads;
    vector<SpriteMesh*> strightroads;
    vector<SpriteMesh*> strightroadedges;
    vector<SpriteMesh*> rightturns;
    vector<SpriteMesh*> rightturnedges;
    vector<SpriteMesh*> leftturns;
    vector<SpriteMesh*> leftturnedges;
    vector<SpriteMesh*> horizontals;
    vector<SpriteMesh*> horizontaledges;
    
    
    GLuint TexID;
};
#endif