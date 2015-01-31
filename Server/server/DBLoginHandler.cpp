#include "Server_PCH.h"
#include "DBLoginHandler.h"
#include "NetLoginHandler.h"
#include "PlayerMgr.h"

#include "GameDB.h"

void DBLoginRepAccount(void* pData ,const void* dbdata)
{

	Player& rPlayer = *static_cast<Player*>(pData);
	
	const DbRecordSet& rDbSet = *static_cast<const DbRecordSet*>(dbdata);
	if(rDbSet.CountRecord())
	{
		const DBAccountLogin& rLogin = *static_cast<const DBAccountLogin*>(rDbSet.GetRecordBuff(0));
		FLOG4("Login successful %s",rLogin.arrUsername);
	}

}

void DBSelectRoleList(void* pData ,const void* dbdata)
{

}

// Ñ¡Ôñ½ÇÉ«·µ»Ø
void DBLoginRepSelectRole(void* pData ,const void* dbdata)
{

}

void NoDBCharacterLoadAllDataFinished(void* pData)
{

}

void DBLoginRepCreateRole(void* pData ,const void* dbdata)
{

}


