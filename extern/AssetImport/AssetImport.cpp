
// Hugh Smith 
// copyright 2015
// all rights reserved
#include "AssetImport.h"
namespace AssetImport {
ILOType AIScene::getInputLayoutType( wstring type )
 {
     if(  wcscmp( type.c_str(), L"PNT" ) == 0 )
         return ILOType::PNT;
     if(  wcscmp( type.c_str(), L"PT" ) == 0 )
         return ILOType::PT;
     return ILOType::PNT;
 }

}