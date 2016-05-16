#ifndef _CHARACTER_LEVEL_CONFIG_H_
#define _CHARACTER_LEVEL_CONFIG_H_

#include "ConfigBase.h"

using namespace SWA;
using namespace DB;

class CharLevelConfig : public BaseSingle<CharLevelConfig> , public ConfigBase<uint16 ,CharacterLevelCfg >
{
public:
	CharLevelConfig(void){};
	~CharLevelConfig(void){};

	virtual  void	LoadRes( const sbyte* path )
	{
		ptree pt;
		ptree root;

		read_xml( path , pt , boost::property_tree::xml_parser::trim_whitespace );

		FLOG4( "xml loading. %s" , path );

		root = pt.get_child( "root" );

		ptree::assoc_iterator iter = root.ordered_begin();
		for (  ; iter != root.not_found() ; ++iter )
		{
			ptree n = iter->second;
			CharacterLevelCfg item;
			item.level					= n.get< uint32 >(XMLATTR(level));
			item.exp					= n.get< uint32 >(XMLATTR(exp));
			item.power					= n.get< uint32 >(XMLATTR(power));
			item.talent					= n.get< uint32>(XMLATTR(talent));
			items.insert(std::make_pair(item.level,item));
		}

		FLOG4( "xml loaded. %s" , path );
	}

	virtual const CharacterLevelCfg*  GetData( uint16 level )
	{
	    ItemMapType::iterator it = items.find(level);
		if (it == items.end())
			return NULL;
		return &it->second;
	}

private:

};

#define gCharLevelCfg CharLevelConfig::Instance()

#endif
