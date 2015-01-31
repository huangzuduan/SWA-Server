#include "Server_PCH.h"
#include "GameConfig.h"
#include "ConfigBase.h"

using namespace SWA;

void	GameConfig::LoadRes( const char* path )
{
	ptree pt_root;
	ptree pt_items;
	ptree login , gameconfig,socket,dbdata;
	
	read_xml( path , pt_root , boost::property_tree::xml_parser::trim_whitespace );
	
	FLOG4( "xml loading. %s" , path );
	
	pt_items	= pt_root.get_child("items");

	gameconfig  = pt_items.get_child("gameconfig");
	socket		= pt_items.get_child("socket");
	dbdata		= pt_items.get_child("dbdata");

#define XMLATTR(key) "<xmlattr>."#key 

	strcpy(m_sServerCfg.arrConfigPath,gameconfig.get<string>(XMLATTR(configpath)).c_str());

	strcpy(m_sServerCfg.arrServerHost,socket.get<string>(XMLATTR(host)).c_str());
	m_sServerCfg.nServerPort = socket.get<uint16>(XMLATTR(port));

	strcpy(m_sServerCfg.arrDbdateHost,dbdata.get<string>(XMLATTR(host)).c_str());
	strcpy(m_sServerCfg.arrDbDataDbname,dbdata.get<string>(XMLATTR(dbname)).c_str());
	strcpy(m_sServerCfg.arrDbDataUsername,dbdata.get<string>(XMLATTR(username)).c_str());
	strcpy(m_sServerCfg.arrDbDataPassword,dbdata.get<string>(XMLATTR(password)).c_str());

	m_sServerCfg.nDbDataPort = dbdata.get<uint16>(XMLATTR(port));
}

