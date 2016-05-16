#ifndef _GAMECOMMAND_H_
#define _GAMECOMMAND_H_

#define MAX_PATH1 260

typedef void (*CommandFun)(void);

struct KeyCommand
{
	KeyCommand( const char* id , const char* doc , CommandFun call )
	{
		this->id = id;
		this->doc = doc;
		this->callBack = call;
	}
	string		id;
	string		doc;
	CommandFun	callBack;
};

typedef vector< KeyCommand > VectorCommand;
typedef vector< KeyCommand >::iterator VectorCommandIter;

using namespace SWA;

class GameCommand
{
public:

	GameCommand();
	~GameCommand();
    
    void Init(NetSocket& net_socket);

	void Command( float32 delay );
	bool			ParseCommand( char* command );


	static void	CommandHelp();

	// µ÷ÓÃÃüÁî
	// µÇÂ¼
	static void	CommandLogin( char* );

private:

	static NetSocket*		mCommandSocket;
	static VectorCommand	s_sCommands;

	char			m_arrBuffer[ MAX_PATH1 ];
	int32			m_nCommandCount;
};



#endif


