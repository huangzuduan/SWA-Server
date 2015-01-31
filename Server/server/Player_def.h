#ifndef PLAYER_DEF_H_
#define PLAYER_DEF_H_


//完整的好友信息表给前端发，非存储
struct FrindRelationInfo
{
	suint64 fri_uid;//好友（陌生人）ID
	suint32 relation;//关系标识(0删除,1好友,2陌生人)
	suint32 level;
	sbyte	nick[MAX_NICK_LENGTH + 1];
};


#endif