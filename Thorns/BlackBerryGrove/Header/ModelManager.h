// Hugh Smith 
// copyright 2015
// all rights reserved
#pragma once


#include "Global.h"
#include "Effectsmanager.h"
#include <vector>
#include <stdio.h>
#include "Vector3.h"
#include "D3D11.h"

using std::wstring;
using std::vector;

enum AttributeType{
    ShadowOccluder = 0x1,
    ShadowReciever = 0x2,
    PhysicsBody    = 0x4,
    Bone           = 0x8,
    Transparent    = 0x10,
    GravityFloor   = 0x20,
    AIPath         = 0x40
};

namespace MdlMgr {

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
	wstring materialName;
};

struct AXModel
{
	int numVerts;
    union {
        PNT_VERTEX *pntvertices;
        PT_VERTEX *ptvertices;
    };
	int triCount;
	wstring inputLayoutName;
	wstring objectName;
	int MaterialCount;
	map<wstring, AXModelIndex *> indexMap;
	ILOType                 inputLayoutType;
};

struct ModelIndexBuffer 
{
    ID3D11Buffer    *iBuff;
    D3D11_BUFFER_DESC bDesc;
    unsigned int    faceCount;
	wstring			AttributeName;
	unsigned int    iBufferStart;
    unsigned int    vBufferStart; 
    PassType        EffectMask;       
};

struct ModelItem
{
    vector< ModelIndexBuffer *>  indexLists;
    ID3D11Buffer                *vtx;
    D3D11_BUFFER_DESC           *vtxBufferDesc;
    UINT stride;
    UINT offset; 
	wstring						modelName;
	ILOType                     inputLayoutType;
    AttributeType               AttributeMask;
    GMATRIX                     myWorld;
    vector< PassType *>			EffectMasksLists;                       
};

class ModelManager 
{
public:      
    ModelManager(){};
    ~ModelManager();
 
    ModelItem *     loadModel( wstring modelFile );
    ModelItem *     CreateDXModelItem( AXModel *axm );
    ModelItem *     getModel( wstring modelName);
    void            cleanUp();
    ILOType         getInputLayoutType( wstring type );

	map<wstring, ModelItem *> modelItemMap;

};
};