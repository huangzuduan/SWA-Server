#ifndef _GAMECOMMAND_H_
#define _GAMECOMMAND_H_

#define MAX_PATH1 260

typedef void	(*CommandFun)( char* );

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

class GameCommand
{
public:

	GameCommand();
	~GameCommand();
    
    void    Init();
	void 	Command( float32 delay );
	bool	ParseCommand( char* command );

protected:


	static void	CommandHelp( char* );
	static void	CommandPlayer( char* );

private:

	static VectorCommand s_sCommands;

	char	m_arrBuffer[ MAX_PATH1 ];
	int32	m_nCommandCount;
};



#endif


