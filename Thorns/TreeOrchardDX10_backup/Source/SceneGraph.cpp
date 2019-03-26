#include "SceneGraph.h"


namespace SG {
SceneGraph::SceneGraph(int ItemCount){
	m_sceneGraph = new SceneItem[ItemCount];
	for(int i = 0; i < ItemCount; i++) {
		m_sceneGraph[i].siVBuffers = new _BufferInfo();
		m_sceneGraph[i].siIBuffers = new _BufferInfo();
		m_sceneGraph[i].siCBuffers = new _BufferInfo[2];
	}
    SceneItemCount = ItemCount;
}
SceneGraph::~SceneGraph(){
	delete []m_sceneGraph;
}

void SceneGraph::cleanup(){

	for(int i = 0; i < SceneItemCount; i++) {
		m_sceneGraph[i].cleanup();
	}

}

SceneItem::SceneItem(){
	siVBuffers = 0;
    siIBuffers = 0;
    pILO = 0;
    pET = 0;
    pRV = 0;
    effect = 0;
    material = 0;
}
SceneItem::~SceneItem(){
	
		delete siVBuffers;
        delete siIBuffers;
	
}
void SceneItem::cleanup(){
	
    if( siVBuffers ) {
        if (siVBuffers->bd){
            delete siVBuffers->bd;
        }
		delete siVBuffers;
    }
    siVBuffers = 0;
    if( siIBuffers ){
        if (siIBuffers->bd){
            delete siIBuffers->bd;
        }

        delete siIBuffers;
    }
    siIBuffers = 0;

    if( siCBuffers ){
        delete []siCBuffers;
    }
    siCBuffers = 0;

    if( effect ){
        delete effect;
    }
    effect = 0;
    if( material ){
        delete material;
    }
    material = 0;
    if( pILO ){
        pILO=NULL;
    }
    pILO = 0;
    pRV = 0;
	
}
};