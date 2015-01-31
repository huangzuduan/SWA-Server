#ifndef CONFIG_BASE_H_
#define CONFIG_BASE_H_

#define XMLATTR(key) "<xmlattr>."#key 

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace boost::property_tree;

template<class TK, class TV>
class  ConfigBase 
{
	typedef map<TK,TV> ItemMapType;
public:
	virtual  void LoadRes(const char* path) = 0 ;
	virtual  const TV*	GetData(TK id) = 0;
protected:
	ItemMapType items;
};


#endif