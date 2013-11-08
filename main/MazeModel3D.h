//
//  MazeModel3D.h
//  program5
//
//  Created by X. Ding on 10/10/13.
//
//

#ifndef program5_MazeModel3D_h
#define program5_MazeModel3D_h

#include <vector>
#include "MazeModel.h"
using namespace std; //makes using vectors easy

class MazeModel3D
{
public:
	MazeModel3D()
	{
	}
	
	MazeModel3D(MazeModel  & model)
	{
        float halfLangth3D = model.getUnitSize() * 0.2 / 2.0;
        
        float max_x = 0;
        float max_y = 0;
        float min_x = 0;
        float min_y = 0;
        
        
        //generate 8 points for the wall from 2 points
		for(int k = 0; k < model.getElementCount(); k+=2)
        {
            int m = model.getElements()[k];
            int n = model.getElements()[k + 1];
            
            if(m != n){
                float x1 = model.getPosition()[m * 3];
                float y1 = model.getPosition()[m * 3 + 1];
                float z1 = model.getPosition()[m * 3 + 2];
                
                if(x1 < min_x)
                    min_x = x1;
                else if(x1 > max_x)
                    max_x = x1;
                
                if(y1 < min_y)
                    min_y = y1;
                else if(y1 > max_y)
                    max_y = y1;
                
                
                
                float x2 = model.getPosition()[n * 3];
                float y2 = model.getPosition()[n * 3 + 1];
                float z2 = model.getPosition()[n * 3 + 2];
                
                if(x2 < min_x)
                    min_x = x2;
                else if(x2 > max_x)
                    max_x = x2;
                
                if(y2 < min_y)
                    min_y = y2;
                else if(y2 > max_y)
                    max_y = y2;
            
                
                //the wall is vertical
                if( (abs(x2 - x1) < 0.005) && (abs(y2 - y1) > 0.005 )){
                    // the plain defined by (x1, y1)
                    positions3D.push_back(x1 - halfLangth3D);
                    positions3D.push_back(y1);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x1 - halfLangth3D);
                    positions3D.push_back(y1);
                    positions3D.push_back(halfLangth3D);
                    
                    positions3D.push_back(x1 + halfLangth3D);
                    positions3D.push_back(y1);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x1 + halfLangth3D);
                    positions3D.push_back(y1);
                    positions3D.push_back(halfLangth3D);
                    
                    //the plain defined by (x2, y2)
                    positions3D.push_back(x2 - halfLangth3D);
                    positions3D.push_back(y2);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x2 - halfLangth3D);
                    positions3D.push_back(y2);
                    positions3D.push_back(halfLangth3D);
                    
                    positions3D.push_back(x2 + halfLangth3D);
                    positions3D.push_back(y2);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x2 + halfLangth3D);
                    positions3D.push_back(y2);
                    positions3D.push_back(halfLangth3D);
                    
                }
                
                //the wall is horizontal
                else if ((abs(y2 - y1) < 0.005) && (abs(x2 - x1) > 0.005)){
                    // the plain defined by (x1, y1)
                    positions3D.push_back(x1);
                    positions3D.push_back(y1 - halfLangth3D);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x1);
                    positions3D.push_back(y1 - halfLangth3D);
                    positions3D.push_back(halfLangth3D);
                    
                    positions3D.push_back(x1);
                    positions3D.push_back(y1 + halfLangth3D);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x1);
                    positions3D.push_back(y1 + halfLangth3D);
                    positions3D.push_back(halfLangth3D);
                    
                    // the plain defined by (x2, y2)
                    positions3D.push_back(x2);
                    positions3D.push_back(y2 - halfLangth3D);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x2);
                    positions3D.push_back(y2 - halfLangth3D);
                    positions3D.push_back(halfLangth3D);
                    
                    positions3D.push_back(x2);
                    positions3D.push_back(y2 + halfLangth3D);
                    positions3D.push_back(-halfLangth3D);
                    
                    positions3D.push_back(x2);
                    positions3D.push_back(y2 + halfLangth3D);
                    positions3D.push_back(halfLangth3D);
                }
                
            }
        }
        
        int first_floor_point = positions3D.size() / 3;
        
        
        positions3D.push_back(min_x);
        positions3D.push_back(max_y);
        positions3D.push_back(-halfLangth3D);
        
        positions3D.push_back(min_x);
        positions3D.push_back(min_y);
        positions3D.push_back(-halfLangth3D);
        
        positions3D.push_back(max_x);
        positions3D.push_back(min_y);
        positions3D.push_back(-halfLangth3D);
        
        positions3D.push_back(max_x);
        positions3D.push_back(max_y);
        positions3D.push_back(-halfLangth3D);

        
        
        GLuint ele[] = {
            0, 1, 5,
            0, 4, 5,
            0, 2, 6,
            0, 4, 6,
            2, 6, 7,
            2, 3, 7,
            1, 5, 7,
            1, 3, 7,
            0, 1, 3,
            0, 2, 3,
            5, 4, 7,
            4, 7, 6,
        };
        
        
        for(int i = 0; i < positions3D.size() / 24; i++){
            for(int j = 0; j < 36; j++)
            {
                elements3D.push_back(i * 8 + ele[j]);
            }
        }

//        printf();
        
        elements3D.push_back(first_floor_point);
        elements3D.push_back(first_floor_point + 1);
        elements3D.push_back(first_floor_point + 2);
        
        elements3D.push_back(first_floor_point);
        elements3D.push_back(first_floor_point + 2);
        elements3D.push_back(first_floor_point + 3);
        
//        elements3D.push_back(0);
//        elements3D.push_back(0 + 1);
//        elements3D.push_back(0 + 2);
//        
//        elements3D.push_back();
//        elements3D.push_back(0 + 2);
//        elements3D.push_back(0 + 3);
//        
        
	}
	
	GLfloat const * get3DPosition() const
	{ return &positions3D[0]; }
	
	GLuint const * get3DElements() const
	{ return &elements3D[0]; }
	
	size_t get3DElementCount() const
	{ return elements3D.size(); }
	
	size_t get3DElementBytes() const
	{ return elements3D.size()*sizeof(GLuint); }
	
	size_t get3DPositionBytes() const
	{ return positions3D.size()*sizeof(GLfloat); }
    
    size_t get3DPositionCount() const
	{ return positions3D.size(); }
    
    
    glm::vec3 getCentroid()
	{
		glm::vec3 center = glm::vec3(0);
		float positionCount = 1.0f/(positions3D.size()/3.0f);
		
		for(int i=0; i<positions3D.size(); i+=3)
		{
			center[0] += positions3D[i] * positionCount;
			center[1] += positions3D[i+1] * positionCount;
			center[2] += positions3D[i+2] * positionCount;
		}
		
		return center;
	}

	
private:
	vector<GLfloat> positions3D;
	vector<GLuint> elements3D;
    GLuint x[4], y[4];
};

#endif

