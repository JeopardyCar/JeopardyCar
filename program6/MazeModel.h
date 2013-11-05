#ifndef __MAZEMODEL
#define __MAZEMODEL

#include <vector>
#include "Maze.h"
using namespace std; //makes using vectors easy

class MazeModel
{
public:
	MazeModel()
	{
	}
	
	MazeModel(Maze const & maze)
	{
		size_t h = maze.getHeight();
		size_t w = maze.getWidth();
		float unit = this->getUnitSize();
		bool includeBase = true;
		usedWalls=0;
		//int maxWalls = w*(h+1) + h*(w+1);
		size_t maxMazeWalls = w*(h+1) + h*(w+1);
		size_t maxElements = maxMazeWalls*2;
		float wallW = .08;
		float wallH = .5;
		positions.push_back(-1);
		positions.push_back(-1);
		positions.push_back(-.5);
		positions.push_back(1);
		positions.push_back(-1);
		positions.push_back(-.5);
		positions.push_back(-1);
		positions.push_back(1);
		positions.push_back(-.5);
		positions.push_back(1);
		positions.push_back(1);
		positions.push_back(-.5);

		positions.push_back(-1);
		positions.push_back(-1);
		positions.push_back(.5);
		positions.push_back(1);
		positions.push_back(-1);
		positions.push_back(.5);
		positions.push_back(-1);
		positions.push_back(1);
		positions.push_back(.5);
		positions.push_back(1);
		positions.push_back(1);
		positions.push_back(.5);

		elements.push_back(0);
		elements.push_back(1);
		elements.push_back(2);

		elements.push_back(1);
		elements.push_back(2);
		elements.push_back(3);
		
		elements.push_back(4);
		elements.push_back(5);
		elements.push_back(6);

		elements.push_back(5);
		elements.push_back(6);
		elements.push_back(7);
		/*
		elements.push_back(1);
		elements.push_back(2);
		elements.push_back(6);

		elements.push_back(2);
		elements.push_back(6);
		elements.push_back(7);

		*/
	}
	
	int getMax(int w, int h)
	{
		int maxMazeWalls = w*(h+1) + h*(w+1);
		return maxMazeWalls*2;
	}
	 
	int getUnused(int w, int h){
		int maxWalls = w*(h+1) + h*(w+1);
		return maxWalls-usedWalls;
	}


	GLfloat const * getPosition() const
	{ return &positions[0]; }
	
	GLuint const * getElements() const
	{ return &elements[0]; }

	GLfloat const * getNormals() const
	{ return &normals[0]; }

	GLfloat const * getColors() const
	{ return &colors[0]; }

	int getElementCountInt() const
	{ return elements.size(); }

	size_t getElementCount() const
	{ return elements.size(); }
	
	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }

	size_t getNormalBytes() const
	{ return normals.size()*sizeof(GLfloat); }

	size_t getColorBytes() const
	{ return colors.size()*sizeof(GLfloat); }

	size_t getVertexCount() const
	{ return positions.size()/3; }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	float getUnitSize()
	{ return 1.0f; }
	
private:
	vector<GLfloat> positions;
	vector<GLuint> elements;
	vector<GLfloat> colors;
	vector<GLfloat> normals;
	int usedWalls;
};

#endif
