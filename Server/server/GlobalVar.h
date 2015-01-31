#ifndef __GLOBALVAR_H_
#define __GLOBALVAR_H_

#include "PlayerMgr.h"
#include "ResourceMgr.h"
#include "GameConfig.h"

using namespace SWA;

static PlayerMgr&	g_rPlayerMgr  = *(PlayerMgr::Instance());
static ResourceMgr& g_rResourceMgr = *(ResourceMgr::Instance());
static GameConfig&	g_rGameCfg = *(GameConfig::Instance());

static DatabaseMgr& g_rDbDatabaseMgr = *(DatabaseMgr::Instance());

#endif