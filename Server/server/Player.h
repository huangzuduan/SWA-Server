#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

class SWA::NetSocket;
struct NetMsgHead;

// 玩家状态
enum EPlayerStatus
{
	EPLAYER_STATUS_INITED		 ,	// 初始化完成 
	EPLAYER_STATUS_CONNECTED	 ,  // 已经接受了连接完成 
	EPLAYER_STATUS_ENCRYPTED	 ,	// 获得密钥成功完成 
	EPLAYER_STATUS_LOGINED		 ,	// 登录完成完成 
	EPLAYER_STATUS_SELECTED		 ,  // 选择角色完成 
	EPLAYER_STATUS_OK			 ,	// 可以进行数据交互完成  
};

/*
 *
 *	Detail: 玩家类
 *   
 *  Created by hzd 2015/01/26  
 *
 */

struct PlayerData
{

};

class Player
{
public:

	Player(SWA::NetSocket& rSocket);
	virtual ~Player();

	/*
	 *
	 *	Detail: 加载数据
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void LoadData(const PlayerData& rData);

	/*
	 *
	 *	Detail: 获得角色ID
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	uint64	UID();

	/*
	 *
	 *	Detail: 发送消息
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SendMsg( NetMsgHead* pMsg);
	
	/*
	 *
	 *	Detail: 获得平台账号
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void GetAccount(char o_arrAccount[MAX_ACCOUNT_LENG]);

	/*
	 *
	 *	Detail: no
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SetStatus(EPlayerStatus eStatus);
	
	/*
	 *
	 *	Detail: 获得状态
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	EPlayerStatus GetStatus(){ return m_eStatus; };

	/*
	 *
	 *	Detail: 设置通信密钥
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void SetEncryptKey(char arrNewKey[MAX_ENCRYPT_LENTH]);

	/*
	 *
	 *	Detail: 获得通信密钥
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void GetEncryptKey(char o_arrEncryptKey[MAX_ACCOUNT_LENG]);

	/*
	 *
	 *	Detail: 玩家退出
	 *   
	 *  Created by hzd 2015-1-21
	 *
	 */
	void Exist();

	/*
	 *
	 *	Detail: 定时循环，参数为延时毫秒
	 *   
	 *  Created by hzd 2015/01/21  
	 *
	 */
	void Update(uint32 nDelay);
		
private:

	uint64				m_nID;								// 角色ID
	char				m_arrAccount[MAX_ACCOUNT_LENG];	// 平台ID
	SWA::NetSocket&		m_rSocket;							// socket
	EPlayerStatus		m_eStatus;							// 工作流程状态

	int32				mSendPackCount;						//发包数量（校验包顺序、丢包情况）
	char				m_arrEncryptKey[MAX_ENCRYPT_LENTH];	//通信密钥

};

#endif

