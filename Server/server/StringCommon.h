#ifndef STRINGCOMMON_H
#define STRINGCOMMON_H
#include "BaseDefine.h"
#include "baseSingle.h"
#include "gameDefine.h"
using namespace std;

class StringCommon : public BaseSingle<StringCommon>
{
public:
	StringCommon(){};
	static	vector<string> mystr_split(string str,string pattern)
	{
		int pos;
		vector<string> result;
		str+=pattern;//扩展字符串以方便操作
		int size=str.size();

		for(int i=0; i<size; i++)
		{
			pos=(int)str.find(pattern,i);
			if(pos<size)
			{
				string s=str.substr(i,pos-i);
				result.push_back(s);
				i=pos+pattern.size()-1;
			}
		}
		return result;
	}
	vector<sint32> mystr_split_int(string str,string pattern)
	{
		int pos;
		vector<sint32> result;
		str+=pattern;//扩展字符串以方便操作
		int size=str.size();

		for(int i=0; i<size; i++)
		{
			pos=(int)str.find(pattern,i);
			if(pos<size)
			{
				string s=str.substr(i,pos-i);
				result.push_back(atoi(s.c_str()));
				i=pos+pattern.size()-1;
			}
		}
		return result;
	}
};
#endif