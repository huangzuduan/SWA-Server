#ifndef EQUIPSTAGE_H
#define EQUIPSTAGE_H

#include "gameDefine.h"


struct EquipStageItem
{
	fint16	id;
	fint16	value;
	fint32	goodsId;
	fint16	goodsNum;
};


class EquipStageConfig : public baseSingle<EquipStageConfig>
{
public:
	EquipStageConfig(void);
	~EquipStageConfig(void);

	fvoid	Load(const fbyte* path);
	fbool	CheckId(fint16 id);
	const	EquipStageItem& GetItem(fint16 id);

private:
	vector<EquipStageItem> dataList;

};

extern	EquipStageConfig* gEquipStageConfig;

#endif
