// AssetImport.h
// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once
#include "vector3.h"
#include "stdio.h"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::wstring;
using std::map;

namespace AssetImport {

enum ILOType{
    P,
    PNT,
    PTNT,
    PVSAT,
    PC,
    PT
};



struct PNT_VERTEX
{
	GVECTOR3 Position;
    GVECTOR3 Normal;
    GVECTOR2 TexCoord;
};

struct PT_VERTEX
{
	GVECTOR3 Position;
    GVECTOR2 TexCoord;
};

struct TRIANGLE 
{
    DWORD   vertex[3];
};

struct AXModelIndex 
{
    TRIANGLE *  triangles;
    int         faceCount; 
	wstring     materialName;
};

struct AXMaterial
{
	//MFn::Type		type;	// Lambert || Phong || kBlinn
	wstring		materialName;
	wstring		type;	// Lambert || Phong || kBlinn
	wstring		textureName;
	wstring		FXFile;
	wstring		FXTechnique;
	// lambert shader attributes
	GVCOLOR  		color;
	GVCOLOR			transparency;
	GVCOLOR			ambientColor;
	GVCOLOR			incandescence;
	float			diffuseCoeff;
	float			translucenceCoeff;
	//float			glowIntensity;
	//bool			glowHideSource;

	// reflective Phong/Blinn shaders 
	GVCOLOR			specularColor;
	float			reflectivity;
	GVCOLOR			reflectedColor;
	short			reflectedRayDepthLimit;

	// Blinn shader
	float			eccentricity;
	float			specularRollOff;

	// Phong shader
	float			cosPower;
};

struct AXModel
{
	int numVerts;
    union {
        PNT_VERTEX *pntvertices;
        PT_VERTEX  *ptvertices;
    };
	int		triCount;
	wstring inputLayoutName;
	wstring objectName;
	int		MaterialCount;
	map<wstring, AXModelIndex *> indexMap;
    ILOType						 inputLayoutType;
};

class AIScene
{
public:
    int         objectCount;
    int         materialCount;
	map<wstring, AXModel*> modelsMap;
	map<wstring, AXMaterial* > materialsMap;
    ILOType     getInputLayoutType( wstring type );
	TCHAR		fileInput[256];

	AIScene(const TCHAR *modelFileName)
	{
		FILE *modelFile = NULL;
		float version;

		modelFile = _wfopen( modelFileName, L"r");
		if ( !modelFile )
		{
			printf("Error opening %s", modelFileName);
			exit(1);
		}

		fwscanf( modelFile, L"// %*[^\n]\n");
		fwscanf( modelFile, L"// %*[^\n]\n");
		fwscanf( modelFile, L"// %*[^\n]\n");

		fwscanf( modelFile, L"MODEL\nVERSION %f\n", &version);
        fwscanf( modelFile, L"ObjectCount %d\n", &objectCount);

		for(int modelIdx = 0; modelIdx < objectCount; modelIdx++)
        {
			std::pair<wstring, AXModel*> axModelpair;
			AXModel *axModel = new AXModel();
			fwscanf(modelFile, L"Object   %s\n", &fileInput);
			axModel->objectName = wstring(fileInput);
		    //fwscanf( modelFile, L"Material  %s\n", &models[modelIdx].materialName);
			fwscanf(modelFile, L"InputLayout %s\n", &fileInput);
			axModel->inputLayoutName = wstring(fileInput);
            axModel->inputLayoutType = getInputLayoutType(axModel->inputLayoutName );
		    fwscanf( modelFile, L"NUMVERTS %d\n", &axModel->numVerts);

            switch (axModel->inputLayoutType )
            {
                case ILOType::PNT:
					axModel->pntvertices = new PNT_VERTEX[axModel->numVerts];
		            for ( int i = 0; i < axModel->numVerts; ++i )
		            {
			            fwscanf( modelFile, L"VERT     %*d\n" );
			            fwscanf( modelFile, L"POSITION %f %f %f\n", &(axModel->pntvertices[i].Position.x), &(axModel->pntvertices[i].Position.y), &(axModel->pntvertices[i].Position.z) );
			            fwscanf( modelFile, L"NORMAL   %f %f %f\n", &(axModel->pntvertices[i].Normal.x), &(axModel->pntvertices[i].Normal.y), &(axModel->pntvertices[i].Normal.z) );
			            fwscanf( modelFile, L"TEXCOORD %f %f\n", &(axModel->pntvertices[i].TexCoord.x), &(axModel->pntvertices[i].TexCoord.y) );
		            }
                    break;
                case ILOType::PT:
                    axModel->ptvertices = new PT_VERTEX[axModel->numVerts];
		            for ( int i = 0; i < axModel->numVerts; ++i )
		            {
			            fwscanf( modelFile, L"VERT     %*d\n" );
			            fwscanf( modelFile, L"POSITION %f %f %f\n", &(axModel->ptvertices[i].Position.x), &(axModel->ptvertices[i].Position.y), &(axModel->ptvertices[i].Position.z) );
			            fwscanf( modelFile, L"TEXCOORD %f %f\n", &(axModel->ptvertices[i].TexCoord.x), &(axModel->ptvertices[i].TexCoord.y) );
		            }
                    break;
                default:
                    MessageBox( NULL, L"Invalid input layout in modelmanager loadmodel - bailing out!", L"Error", MB_OK );
                    break;
            }

		    fwscanf( modelFile, L"Face_Count %d\n", &axModel->triCount);
		    fwscanf( modelFile, L"MESH_MATERIAL_COUNT %d\n", &axModel->MaterialCount);
            AXModelIndex * mi = NULL;
            for ( int mc = 0 ; mc < axModel->MaterialCount; mc ++ )
            {
                mi = new AXModelIndex();
				fwscanf(modelFile, L"MATERIAL %s\n", &fileInput);
				mi->materialName = wstring(fileInput);;
		        fwscanf( modelFile, L"FACE_COUNT %d\n", &mi->faceCount );
  		        mi->triangles = new TRIANGLE[mi->faceCount];
		        for ( int i = 0; i < mi->faceCount; ++i )
		        {
			        fwscanf( modelFile, L"%d %d %d\n", &(mi->triangles[i].vertex[0]), &(mi->triangles[i].vertex[1]), &(mi->triangles[i].vertex[2]));
		        }
				axModel->indexMap.insert(std::pair<wstring, AXModelIndex*>(mi->materialName, mi));
				axModelpair.first = axModel->objectName;
				axModelpair.second = axModel;
				modelsMap.insert(axModelpair);
			}

        }

        fwscanf( modelFile, L"NUMMATERIALS %d\n", &materialCount);
        for (int matIdx = 0; matIdx < materialCount; matIdx++)
        {
			AXMaterial* axMaterial = new AXMaterial();
			fwscanf(modelFile, L"MATERIAL %s\n", &fileInput);				axMaterial->materialName	= wstring(fileInput);
			fwscanf(modelFile, L"TEXTURE %s\n", &fileInput);				axMaterial->textureName		= wstring(fileInput);
			fwscanf(modelFile, L"FXFile %s\n", &fileInput);                 axMaterial->FXFile			= wstring(fileInput);
		    //fwscanf( modelFile, L"FXTechnique %s\n", &fileInput);			axMaterial->FXTechnique		= wstring(fileInput);
		    //fwscanf( modelFile, L"FXTechnique %s\n",                      &axMaterial->FXTechnique));
		    //fwscanf( modelFile, L"COLOR           RGBA    %f %f %f %f\n", &axMaterial->color.r), &(materials[matIdx].color.g), &(materials[matIdx].color.b), &(materials[matIdx].color.a) );
		    //fwscanf( modelFile, L"TRANSPARENCY    RGBA    %f %f %f %f\n", &axMaterial->transparency.r), &(materials[matIdx].transparency.g), &(materials[matIdx].transparency.b), &(materials[matIdx].color.a) );
		    //fwscanf( modelFile, L"AMBIENTCOLOR    RGBA    %f %f %f %f\n", &axMaterial->ambientColor.r), &(materials[matIdx].ambientColor.g), &(materials[matIdx].ambientColor.b), &(materials[matIdx].ambientColor.a) );
		    //fwscanf( modelFile, L"INCANDESCENCE   RGBA    %f %f %f %f\n", &axMaterial->incandescence.r), &(materials[matIdx].incandescence.g), &(materials[matIdx].incandescence.b), &(materials[matIdx].incandescence.a) );
		    //fwscanf( modelFile, L"DIFFUSE         COEFF   %f\n",          &axMaterial->diffuseCoeff) );
		    //fwscanf( modelFile, L"TRANSLUCENCE    COEFF   %f\n",          &axMaterial->translucenceCoeff) );
		    //fwscanf( modelFile, L"SPECULARCOLOR   RGBA    %f %f %f %f\n", &axMaterial->specularColor.r), &(materials[matIdx].specularColor.g), &(materials[matIdx].specularColor.b), &(materials[matIdx].specularColor.a) );
		    //fwscanf( modelFile, L"REFLECTIVECOLOR RGBA    %f %f %f %f\n", &axMaterial->reflectedColor.r), &(materials[matIdx].reflectedColor.g), &(materials[matIdx].reflectedColor.b), &(materials[matIdx].reflectedColor.a) );
		    //fwscanf( modelFile, L"REFLECTIVE RAY DEPTH    %d\n",          &axMaterial->reflectedRayDepthLimit));
		    //fwscanf( modelFile, L"REFLECTIVE              %f\n",          &axMaterial->reflectivity) );
		    //fwscanf( modelFile, L"BLINN ECCENTRICITY      %f\n",          &axMaterial->eccentricity) );
		    //fwscanf( modelFile, L"BLINN SPECULAR ROLLOFF  %f\n",          &axMaterial->specularRollOff) );
		    //fwscanf( modelFile, L"PHONG COS POWER         %f\n",          &axMaterial->cosPower) );
			std::pair<wstring, AXMaterial*> material;
			material.first = axMaterial->materialName;
			material.second = axMaterial;
			materialsMap.insert(material);
        }       
		fclose( modelFile );   
    }; 

    ~AIScene()
    {
		// AXModelIndex
		//		TRIANGLE *
		// AXModel
		//		PNT_VERTEX *pntvertices;
		//		PT_VERTEX *ptvertices;
		//		map<wstring, AXModelIndex *> indexMap;
		// 	map<wstring, AXModel*> modelsMap;
		//	map<wstring, AXMaterial* > materialsMap;
		for (auto model : modelsMap)
		{		
			for( auto index : model.second->indexMap )
			{
				if (index.second->triangles)
					delete[] index.second->triangles;
				index.second->triangles = NULL;
				delete index.second;
				index.second = NULL;
			}
			model.second->indexMap.clear();
			if (model.second->inputLayoutType == ILOType::PNT)
			{
				if (model.second->pntvertices)
					delete[] model.second->pntvertices;
				model.second->pntvertices = NULL;
			}
			if (model.second->inputLayoutType == ILOType::PT)
			{
				if (model.second->ptvertices)
					delete[] model.second->ptvertices;
				model.second->ptvertices = NULL;
			}
			delete model.second;
			model.second = NULL;
		}
		modelsMap.clear();
		for (auto m : materialsMap)
		{
			if (m.second != NULL)
			{
				delete m.second;
				m.second = NULL;
			}
		}
		materialsMap.clear();
    };
};
};
