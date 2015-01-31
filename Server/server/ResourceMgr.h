#ifndef RESOURCEMGR_H_
#define RESOURCEMGR_H_

#include "BaseDefine.h"
#include "BaseSingle.h"



/*
 *
 *	Detail: 资源加载管理器 
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class ResourceMgr : public BaseSingle<ResourceMgr>
{
public:
	ResourceMgr(){}
	~ResourceMgr(){}

	void	LoadAllRes(string resourePath)
	{
		//gCharLevelCfg->LoadRes((resourePath + "character_level.xml").c_str());			
		FLOG4( "all config Inited." );

	}

private:

};

#endif





