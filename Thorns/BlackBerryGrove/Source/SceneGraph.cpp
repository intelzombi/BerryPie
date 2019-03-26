// Hugh Smith 
// copyright 2015
// all rights reserved
#include "SceneGraph.h"


namespace SG {
SceneGraph::SceneGraph(int ItemCount){
    SceneItemCount = ItemCount;
}
SceneGraph::~SceneGraph(){

}

void SceneGraph::cleanup(){

    int i = 0;

	for (auto si : mp_sceneGraph)
	{
		si.second.cleanup();
	}

    for( i = 0; i < passItems.size(); i++ )
    {

        ZeroMemory(passItems[i], sizeof(SG::PassItem));
        delete passItems[i];
        passItems[i] = NULL;
    }
    passItems.clear();
}

SceneItem::SceneItem(){
}
SceneItem::~SceneItem(){	
}
void SceneItem::cleanup()
{
  std::vector<RenderItem *>::iterator riIter;	
  for ( riIter = renderItemList.begin(); riIter != renderItemList.end(); riIter ++ )
  {
        
        delete (*riIter);
        (*riIter) = 0;

  }
	
}
};