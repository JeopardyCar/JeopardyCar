#ifndef RoadGen_h
#define RoadGen_h

#include <vector>
#include "SpriteMesh.h"
using namespace std; //makes using vectors easy




#define R_UP 0
#define R_RIGHT 1
#define R_DOWN 2
#define R_LEFT 3

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

	//Initializes the roads
	void init(GLuint shaderProg,char * filename ="Model/broad.obj",char * edgefilename = "Model/broad_edge.obj")
	{
        direction = R_UP;
        for(int i= 0;i<8;i++){
            SpriteMesh* road =new SpriteMesh(filename,shaderProg,"Model/Road1_T1.bmp",TexID);
            SpriteMesh* edge =new SpriteMesh(edgefilename,shaderProg,"",TexID);
            roads.push_back(road);
            edges.push_back(edge);
        }
	}
    
	//Updates the roads so they follow the car's position
	void update(glm::vec3 carPos){
        for(int i=0 ;i< roads.size();i++){
            float r = ((float) rand() / (RAND_MAX));
            orgX= 0;
            orgY=((int)((carPos.y)/10))*10+10;
            if(r<.01){
                printf("make a turn%f\n",r);                
            }
            if(direction == R_UP){
                SpriteMesh* road = roads[i];
                SpriteMesh* edge = edges[i];
                road->setPosM(glm::vec3(0, orgY-i*10,0));
                edge->setPosM(glm::vec3(0, orgY-i*10,0));
            }      
        }
	}

	//Displays the roads
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
    
    bool testOut(CarSprite car){
        for(int i=0;i<edges.size();i++){
            SpriteMesh* edge = edges[i];
            if(car.testCol(*edge,1)){
                return true;
            }
        }
        return false;
    }
    
    
    
private:
    int direction;
    float orgX;
    float orgY;
    
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