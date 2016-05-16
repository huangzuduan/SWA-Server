#ifndef CLIENT_PLAYER_H_
#define CLIENT_PLAYER_H_

class ClientPlayer
{
public:
	ClientPlayer(void);
	~ClientPlayer(void);

	void SendMsg();
	void PackMsg( NetMsgHead* msg );

	void SetSocket( NetSocket& socket );
	NetSocket* GetSocket();


	uint64 GetUid() { return 0;}

	const char*	GetEncryptKey() { return mEncryptKey;}

	void SetEncryptKey(const char arrNewKey[MAX_ENCRYPT_LENTH])
	{
		memcpy(mEncryptKey,arrNewKey,MAX_ENCRYPT_LENTH);
	}

protected:

	NetSocket*  mSocket;
	char mEncryptKey[MAX_ENCRYPT_LENTH];//Õ®–≈√‹‘ø
};


#endif
