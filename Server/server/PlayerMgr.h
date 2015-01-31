#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include "Player.h"


/*
 *
 *	Detail: 玩家管理器
 *   
 *  Created by hzd 2015/01/26  
 *
 */
class SWA::NetServer;

class PlayerMgr : public BaseSingle< PlayerMgr >
{

	typedef map< uint64 , Player* >	UIDPlayerMap;  // 
	typedef map< uint64 , Player* >::iterator	UIDPlayerMapIter;

public:

	PlayerMgr();
	~PlayerMgr();

	/*
	 *
	 *	Detail: 初始化所有等待连接的Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Init(SWA::NetServer& rNetServer);	

	/*
	 *
	 *	Detail: 释放Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Release();

	/*
	 *
	 *	Detail: 获得Player
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	Player*	GetPlayer( uint64 nUid);

	/*
	 *
	 *	Detail: 获得该Player(Player与Socket是永远绑定的，这样是为了提高效率)
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	Player&	GetPlayer( SWA::NetSocket& rSocket );
		  
	/*
	 *
	 *	Detail: 广播消息
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void SendToAll( NetMsgHead* msg );

	/*
	 *
	 *	Detail: 更新
	 *   
	 *  Created by hzd 2015/01/26  
	 *
	 */
	void Update(uint32 nDelay);

private:

	Player*			m_arrPlayerCache[MAX_SOCKET_CONNECT];	// 所有玩家缓存(连接与未连接)
	UIDPlayerMap	m_mapUIDPlayer;							// UID对应Player	

};


#endif

