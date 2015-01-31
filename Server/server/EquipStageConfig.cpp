#include "EquipStageConfig.h"
#include "netConfig.h"

EquipStageConfig* gEquipStageConfig = F_NULL;
EquipStageConfig::EquipStageConfig(void)
{
	gEquipStageConfig = this;
}


EquipStageConfig::~EquipStageConfig(void){}

fvoid	EquipStageConfig::Load(const fbyte* path)
{
	ptree pt;
	ptree root;
	read_xml( path , pt , boost::property_tree::xml_parser::trim_whitespace );

	FLOG4( "xml loading. %s" , path );

	root = pt.get_child("equipstage");

	ptree::assoc_iterator iter = root.ordered_begin();
	int index = 0;
	for (; iter != root.not_found() ;++iter )
	{
		ptree n = iter->second;
		EquipStageItem ritem;
		ritem.id = n.get< fint16 >( "<xmlattr>.id" );
		ritem.value = n.get< fint16 >( "<xmlattr>.value" );
		ritem.goodsId = n.get< fint32 >( "<xmlattr>.goodsId" );
		ritem.goodsNum = n.get< fint16 >( "<xmlattr>.goodsNum" );
		dataList.push_back(ritem);
	}
}

fbool	EquipStageConfig::CheckId(fint16 id)
{
	if( 0 < id && id < dataList.size() ){
		return	F_TRUE;
	}
	return	F_FALSE;
}

const	EquipStageItem& EquipStageConfig::GetItem(fint16 id)
{
	return dataList[id - 1];
}
