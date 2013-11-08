//
//  Geometry.h
//  program5
//
//  Created by Storm Max on 13-11-8.
//
//

#ifndef program5_Geometry_h
#define program5_Geometry_h
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "SpriteMesh.h"
#include "Mesh.h"
#include "obj_loader/OBJMeshConvert.h"

class Geometry:public SpriteMesh{
public:
    Geometry(){
        
    }
    
    Geometry(char * filepath){
        FILE *file = NULL;
		file = fopen(filepath, "rb");
		
		if(file)
		{
			fclose(file);
			OBJMeshConvert objConvert;
			mesh = objConvert.loadOBJ(filepath);
        }else{
            printf("cant read file\n");
        }
    }
    
    void init(GLuint shaderProg){
        SpriteMesh::init(&mesh, shaderProg);
    }
    
    
private:
    Mesh mesh;
    
};

#endif
