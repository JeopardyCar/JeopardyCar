//
//  Sprite.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_SpriteMesh_h
#define program5_SpriteMesh_h



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Utilities.h"
#include "obj_loader/OBJMeshConvert.h"

class SpriteMesh{
public:
    SpriteMesh(){
        baseTrans= glm::mat4(1);
        baseRot =glm::mat4(1);
        posM = glm::mat4(1);
        acc = glm::vec3(0);
        pos = glm::vec3(0);
        M = glm::mat4(1);
    }
    
    SpriteMesh(char * meshfile, GLuint shaderProg, char * texfile = NULL, GLuint n = -1){
        baseTrans= glm::mat4(1);
        baseRot =glm::mat4(1);
        posM = glm::mat4(1);
        acc = glm::vec3(0);
        pos = glm::vec3(0);
        M = glm::mat4(1);
        
        TextUnit = n;
        
        FILE *file = NULL;
		file = fopen(meshfile, "rb");
		
		if(file)
		{
			fclose(file);
			OBJMeshConvert objConvert;
			me = objConvert.loadOBJ(meshfile);
            this->mesh = &me;
        }else{
            printf("cant read file %s\n",meshfile);
        }
        
        if (texfile!=NULL && n!=-1){
            GLuint Texture = loadBMP(texfile,&TexID);
        }
        
        this->shaderProg = shaderProg;
        normalSlot = glGetAttribLocation(shaderProg, "norm");
        
        matSlot = glGetUniformLocation(shaderProg, "M");
        pSlot = glGetUniformLocation(shaderProg, "P");
        cSlot = glGetUniformLocation(shaderProg, "C");
        SamplerSlot = glGetUniformLocation(shaderProg, "Sampler");
        
        
    }
    
    //Rebuilds the object with a new model   
    void rebuildSprite(char * meshfile, char * texfile = NULL, GLuint n = -1){
        TextUnit = n;
        
        FILE *file = NULL;
		file = fopen(meshfile, "rb");
		
		if(file)
		{
			fclose(file);
			OBJMeshConvert objConvert;
			me = objConvert.loadOBJ(meshfile);
            this->mesh = &me;
        }else{
            printf("cant read file %s\n",meshfile);
        }
        
        if (texfile!=NULL && n!=-1){
            GLuint Texture = loadBMP(texfile,&TexID);
        }


    }
    
    //Updates the object based on its velocity and acceleration and displays it
    void show(glm::mat4 P, glm::mat4 C, glm::mat4 M){
        glm::mat4 T= P*C*M;
        
        velocity += acc;
        
        baseTrans*=glm::translate(glm::mat4(1), velocity);
        T*=baseTrans;
        P*=baseTrans;
        C*=baseTrans;
        
        T*=baseRot;
        P*=baseRot;
        C*=baseRot;

        
        pos.x = (M*baseTrans)[3][0];
        pos.y = (M*baseTrans)[3][1];
        pos.z = (M*baseTrans)[3][2];
        
        glUseProgram(shaderProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
        glUniform1i(SamplerSlot, 0);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glUniformMatrix4fv(pSlot, 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(cSlot, 1, GL_FALSE, &C[0][0]);
        
        draw();
        glUseProgram(0);
    }

	//Displays the object without accounting for its velocity
	void showNoMove(glm::mat4 P, glm::mat4 C, glm::mat4 M){
		glm::mat4 T= P*C*M;
        T*=baseTrans;
        P*=baseTrans;
        C*=baseTrans;
        
        T*=baseRot;
        P*=baseRot;
        C*=baseRot;

		pos.x = (M*baseTrans)[3][0];
        pos.y = (M*baseTrans)[3][1];
        pos.z = (M*baseTrans)[3][2];


		glUseProgram(shaderProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
        glUniform1i(SamplerSlot, 0);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glUniformMatrix4fv(pSlot, 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(cSlot, 1, GL_FALSE, &C[0][0]);
		draw();
        glUseProgram(0);
	}

	//Displays the object in a static position on the screen
  	void showStatic(glm::vec3 posNum){
		 
		glm::mat4 C = glm::lookAt(glm::vec3(posNum.x,1,posNum.z), glm::vec3(posNum.x,0,posNum.z), glm::vec3(0,0,posNum.z+1));
		glm::mat4 P = glm::mat4(
						1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1
						);
		glm::mat4 M = glm::mat4(
						1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1
						);
		glm::mat4 T= C*M;
		

		baseTrans=glm::translate(glm::mat4(1), glm::vec3(0,0,0));
        T*=baseTrans;
        P*=baseTrans;
        C*=baseTrans;

		pos.x = (M*baseTrans)[3][0];
        pos.y = (M*baseTrans)[3][1];
        pos.z = (M*baseTrans)[3][2];


		glUseProgram(shaderProg);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
        glUniform1i(SamplerSlot, 0);
        glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
        glUniformMatrix4fv(pSlot, 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(cSlot, 1, GL_FALSE, &C[0][0]);
		draw();
        glUseProgram(0);
	}

	void setPosM(glm::vec3 m){
        baseTrans = glm::translate(glm::mat4(1), m);
        pos.x = (baseTrans)[3][0];
        pos.y = (baseTrans)[3][1];
        pos.z = (baseTrans)[3][2];
    }
    
    void setPos(float x, float y, float z){
        baseTrans = glm::translate(glm::mat4(1), glm::vec3(x,y,z));
        pos.x = (baseTrans)[3][0];
        pos.y = (baseTrans)[3][1];
        pos.z = (baseTrans)[3][2];
    }
    
    glm::vec3 getPos(){
        return pos;
    }

    glm::vec3 getV(){
        return velocity;
    }
    
    void setV(glm::vec3 v){
        velocity = v;
    }
    
    void setFriction(float f){
        if(f<0){
            f = 0.01;
        }
        if(f>1){
            f =.99;
        }
        friction = f;
    }
    
    void setAccelerate(glm::vec3 v){
        acc =v;
    }
    
    glm::vec3 getAccelerate(){
        return acc;
    }
    
    std::vector<VectorV> getNormals(){
        return me.normals;
    }
    
    vector<VectorV> getVertices(){
        return me.vertices;
    }
    vector<Triangle> getTriangles(){
        return me.triangles;
    }
    vector<VectorV> getUV(){
        return me.UV;
    }
    
    void draw(){
        
        glBindTexture(GL_TEXTURE_2D, TexID);
        glBegin(GL_TRIANGLES);
		
		for(int t=0; t<me.triangles.size(); t++)
		{
			Triangle &tri = me.triangles[t];
            
			for(int v=0; v<3; v++)
			{
				int vertexIndex = tri.vertexIndex[v];
				int normalIndex = tri.normalIndex[v];
                int uvIndex = tri.uvIndex[v];
				VectorV vertex = me.vertices[vertexIndex];
                
				if(normalIndex > -1)
				{
					VectorV normal = me.normals[normalIndex];
					glNormal3fv( normal.c );
				}
                if (me.UV.size()!=0){
                    VectorV UVcord = me.UV[uvIndex];
                    glTexCoord2f( UVcord.c[1],UVcord.c[0] );
                }
                
				glVertex3fv( vertex.c );
			}
		}
		
		glEnd();
        
    }

    
protected:
    GLuint shaderProg;
    GLint normalSlot;
    GLint matSlot;
    GLint pSlot;
    GLint cSlot;
    GLint vertexUVSlot;
    GLint SamplerSlot;
    GLuint TexID;
    Mesh * mesh;
    Mesh me;
    
    GLint TextUnit;
    
    float friction;
    glm::mat4 posM;
    glm::vec3 pos;
    glm::mat4 M ;
    vector<glm::vec2> UV;

    glm::vec3 acc;
    glm::mat4 baseTrans;
    glm::mat4 baseRot;
    
    glm::vec3 velocity;
    
    
};

#endif
