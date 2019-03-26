// Hugh Smith 
// copyright 2015
// all rights reserved
#include "ModelManager.h"

namespace MdlMgr {

 ILOType ModelManager::getInputLayoutType( wstring type )
 {
     if(  wcscmp( type.c_str(), L"PNT" ) == 0 )
         return ILOType::PNT;
     if(  wcscmp( type.c_str(), L"PT" ) == 0 )
         return ILOType::PT;

     return ILOType::PNT;
 }

 ModelItem * ModelManager::CreateDXModelItem( AXModel *axm )
 {

    D3D11_SUBRESOURCE_DATA vInitData;
    D3D11_SUBRESOURCE_DATA iInitData;
	HRESULT hr = S_OK;

    // get for dup
	auto model = modelItemMap.find(axm->objectName);
	if (model != modelItemMap.end())
		return model->second;

    ModelItem *mi = new ModelItem;
	mi->modelName = axm->objectName;

    //create vertex buffer
 	mi->vtxBufferDesc = new D3D11_BUFFER_DESC[1];
	unsigned int vertexCount = axm->numVerts;
 	mi->vtxBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	mi->vtxBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	mi->vtxBufferDesc->CPUAccessFlags = 0;
	mi->vtxBufferDesc->MiscFlags = 0;
    axm->inputLayoutType = getInputLayoutType( axm->inputLayoutName );
    mi->inputLayoutType = axm->inputLayoutType;
    switch ( axm->inputLayoutType ) 
    {
        case ILOType::PNT:
            {
        	    mi->vtxBufferDesc->ByteWidth = sizeof( PNT_VERTEX ) * vertexCount;
                vInitData.pSysMem = (PNT_VERTEX*)axm->pntvertices;
                mi->stride = sizeof(PNT_VERTEX);
                mi->offset = 0; 
            }
            break;
        case ILOType::PT:
            {
        	    mi->vtxBufferDesc->ByteWidth = sizeof( PT_VERTEX ) * vertexCount;
                vInitData.pSysMem = (PT_VERTEX*)axm->ptvertices;
                mi->stride = sizeof(PT_VERTEX);
                mi->offset = 0; 
            }
            break;

        default:
            {
                MessageBox( NULL, L"Invalid input layout in modelmanager CreateDXModelItem - bailing out!", L"Error", MB_OK );
                return NULL;
            }
            break;
    }


    hr = g_pd3dDevice->CreateBuffer( mi->vtxBufferDesc, &vInitData, &mi->vtx );


    if ( hr != S_OK )
    {
        MessageBox( NULL, L"Failed to CreateBuffer (vertex) in modelmanager CreateDXModelItem - bailing out!", L"Error", MB_OK );
        return NULL;
    }


    //Load Model World Matrix To be implemented
    GMATRIX world( 1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f );
    mi->myWorld = world;

	for (auto index : axm->indexMap)
	{
        ModelIndexBuffer *mib = new ModelIndexBuffer[1];
        //create index buffer
        
        unsigned int indexCount  = index.second->faceCount * 3;
	    mib->bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	    mib->bDesc.ByteWidth = sizeof(DWORD) * indexCount;
	    mib->bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    mib->bDesc.CPUAccessFlags = 0;
	    mib->bDesc.MiscFlags = 0;
		mib->AttributeName = index.second->materialName;
		mib->faceCount = index.second->faceCount;
        iInitData.pSysMem = (DWORD*)index.second->triangles;
        hr = g_pd3dDevice->CreateBuffer( &mib->bDesc, &iInitData, &mib->iBuff );
        mib->iBufferStart = 0;
        mib->vBufferStart = 0;// example:  calc sizeof( PNT_VERTEX );
        mi->indexLists.push_back(mib);

    }
	std::pair<wstring, ModelItem * > miPair;
	miPair.first = mi->modelName;
	miPair.second = mi;
	modelItemMap.insert(miPair);

    return mi;
 }
 // Not Used.  Needs workover
 // AXModels *models created but not filled
 // Object cound not initialized be for use;
 // No return value.
 // AXModel needs to be identified and converted to a ModelItem
ModelItem *  ModelManager::loadModel( wstring modelFileName )
{
        int objectCount;
        int materialCount;
        AXModel *models;
		FILE *modelFile = NULL;
		int version = 0;

		modelFile = _wfopen( modelFileName.c_str(), L"r");

		if ( !modelFile )
		{
			printf("Error opening %s", modelFileName);
			exit(1);
		}

		fwscanf( modelFile, L"// %*[^\n]\n");
		fwscanf( modelFile, L"// %*[^\n]\n");
		fwscanf( modelFile, L"// %*[^\n]\n");
		fwscanf( modelFile, L"MODEL\nVERSION %d\n", &version);
        //fwscanf( modelFile, L"ObjectCount %d\n", &objectCount);
        models = new AXModel[1];

		for(int modelIdx = 0; modelIdx < objectCount; modelIdx++)
        {
            fwscanf( modelFile, L"Object   %s\n", &models[modelIdx].objectName);
		    //fwscanf( modelFile, L"Material  %s\n", &models[modelIdx].materialName);
            fwscanf( modelFile, L"InputLayout %s\n", &models[modelIdx].inputLayoutName);
			models[modelIdx].inputLayoutType = getInputLayoutType( models[modelIdx].inputLayoutName );
		    fwscanf( modelFile, L"NUMVERTS %d\n", &models[modelIdx].numVerts);

            switch ( models[modelIdx].inputLayoutType )
            {
            case ILOType::PNT:
                models[modelIdx].pntvertices = new PNT_VERTEX[models[modelIdx].numVerts];
		        for ( int i = 0; i < models[modelIdx].numVerts; ++i )
		        {
			        fwscanf( modelFile, L"VERT     %*d\n" );
			        fwscanf( modelFile, L"POSITION %f %f %f\n", &(models[modelIdx].pntvertices[i].Position.x), &(models[modelIdx].pntvertices[i].Position.y), &(models[modelIdx].pntvertices[i].Position.z) );
			        fwscanf( modelFile, L"NORMAL   %f %f %f\n", &(models[modelIdx].pntvertices[i].Normal.x), &(models[modelIdx].pntvertices[i].Normal.y), &(models[modelIdx].pntvertices[i].Normal.z) );
			        fwscanf( modelFile, L"TEXCOORD %f %f\n", &(models[modelIdx].pntvertices[i].TexCoord.x), &(models[modelIdx].pntvertices[i].TexCoord.y) );
		        }
                break;

            default:

                MessageBox( NULL, L"Invalid input layout in modelmanager loadmodel - bailing out!", L"Error", MB_OK );
                return NULL;
                break;
            }

		    fwscanf( modelFile, L"Face_Count %d", &models[modelIdx].triCount);
		    fwscanf( modelFile, L"MESH_MATERIAL_COUNT %d", &models[modelIdx].MaterialCount);
            AXModelIndex * mi = NULL;
            for ( int mc = 0 ; mc < models[modelIdx].MaterialCount; mc ++ )
            {
                mi = new AXModelIndex();
		        fwscanf( modelFile, L"MATERIAL %s\n", &(mi->materialName) );
		        fwscanf( modelFile, L"FACE_COUNT %d", &mi->faceCount );
  		        mi->triangles = new TRIANGLE[mi->faceCount];

		        for ( int i = 0; i < mi->faceCount; ++i )
		        {
			        fwscanf( modelFile, L"%d %d %d\n", &(mi->triangles[i].vertex[0]), &(mi->triangles[i].vertex[1]), &(mi->triangles[i].vertex[2]));
		        }
				std::pair<wstring, AXModelIndex *> miPair;
				miPair.first = mi->materialName;
				miPair.second = mi;
				// create ModelItem and return.
				models[modelIdx].indexMap.insert(miPair);
           }
        }
        fwscanf( modelFile, L"NUMMATERIALS %d\n", &materialCount);        
		fclose( modelFile );

    return NULL;
}

ModelItem *  ModelManager::getModel(wstring modelName)
{
    return NULL;
}

ModelManager::~ModelManager()
{
    cleanUp();
}
void ModelManager::cleanUp()
{
	//AXModelIndex
	//   TRIANGLE *
	//AXModel
	//   AXModelIndex * from indexMap
	//ModelIndexBuffer
	//   ID3D11Buffer *
	//ModelItem
	//   ModelIndexBuffer * from indexLists
	//   ID3D11Buffer * 
	//   D3D11_BUFFER_DESC *
	//ModelItem * from modelItemMap

	for(map<wstring, ModelItem*>::iterator mit = modelItemMap.begin(); mit != modelItemMap.end(); mit++)
	//for (auto model : modelItemMap)
	{
		for(vector< ModelIndexBuffer *>::iterator axmi = mit->second->indexLists.begin(); axmi != mit->second->indexLists.end(); axmi++)
		{
			if ((*axmi)->iBuff != NULL)
			{
				(*axmi)->iBuff->Release();
			}
			delete[] *axmi;
		}
		mit->second->indexLists.clear();
		if (mit->second->vtx != NULL)
		{
			mit->second->vtx->Release();
			mit->second->vtx = NULL;
		}
		if (mit->second->vtxBufferDesc != NULL)
		{
			delete mit->second->vtxBufferDesc;
			mit->second->vtxBufferDesc = NULL;
		}
		delete mit->second;
	}
	modelItemMap.clear();
	modelItemMap.empty();
	return;
}
};
