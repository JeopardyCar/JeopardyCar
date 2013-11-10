#ifndef __MESH_
#define __MESH_

#include <vector>
#include "glew/glew.h"
#include <SFML/Window.hpp>

class VectorV
{
public:
	float c[3];
};

class Triangle
{
public:
	int vertexIndex[3];
	int normalIndex[3];;
};

class Mesh
{
public:
	std::vector<VectorV> vertices;
	std::vector<VectorV> normals;
	std::vector<Triangle> triangles;
	
	VectorV getCenter()
	{
		VectorV min = getMinBound();
		VectorV max = getMaxBound();
		VectorV center;
		
		for(int c=0; c<3; c++)
        center.c[c] = (min.c[c]+max.c[c]) * 0.5;
		
		return center;
	}
    
    void draw(){
        glBegin(GL_TRIANGLES);
		
		for(int t=0; t<triangles.size(); t++)
		{
			Triangle &tri = triangles[t];
            
			for(int v=0; v<3; v++)
			{
				int vertexIndex = tri.vertexIndex[v];
				int normalIndex = tri.normalIndex[v];
				VectorV vertex = vertices[vertexIndex];
				if(normalIndex > -1)
				{
					VectorV normal = normals[normalIndex];
					glNormal3fv( normal.c );
				}
				glVertex3fv( vertex.c );
			}
		}
		
		glEnd();
    }
	
	VectorV getMinBound()
	{
		VectorV bound;
        
		for(int c=0; c<3; c++)
		{
			bound.c[c] = 9999999.0;
		}
		
		for(int i=0; i<this->vertices.size(); i++)
		{
			for(int c=0; c<3; c++)
			{
				if(vertices[i].c[c] < bound.c[c])
                bound.c[c] = vertices[i].c[c];
			}
		}
		
		return bound;
	}
	
	VectorV getMaxBound()
	{
		VectorV bound;
		
		for(int c=0; c<3; c++)
		{
			bound.c[c] = -9999999.0;
		}
		
		for(int i=0; i<this->vertices.size(); i++)
		{
			for(int c=0; c<3; c++)
			{
				if(vertices[i].c[c] > bound.c[c])
                bound.c[c] = vertices[i].c[c];
			}
		}
		
		return bound;
	}
};

#endif