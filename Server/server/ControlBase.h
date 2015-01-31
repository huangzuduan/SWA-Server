#ifndef CONTROL_BASE_H_
#define CONTROL_BASE_H_

/*
 *
 *	Detail: 控制器父类
 *   
 *  Created by hzd 2014-10-29
 *
 */
template<class IK,class IV>
class ControlBase
{
	typedef std::map<IK,IV*> ItemMapType;

public:

	ControlBase(){}

	~ControlBase(){}

	virtual svoid UpdateAll(suint32 now_time) = 0;

	// 释放times中的内存对象，内存分配交给子控制器
	virtual svoid Release() = 0;

	virtual sbool Remove(IK k)
	{
		ItemMapType::iterator it = mItems.find(k);
		if(it == mItems.end())
			return false;
		mItems.erase(it);
		return true;
	}

protected:
	
	ItemMapType mItems;
};

#endif