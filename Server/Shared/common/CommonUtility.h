#ifndef INCOMMMONUSED_H_
#define INCOMMMONUSED_H_

#include "BaseDefine.h"

#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  

class Utility
{
public:
	Utility(){};
	~Utility(){};

	/*
	*
	*	Detail: 获得系统时间戳
	*   
	*  Created by hzd 2014-10-7
	*
	*/
	static uint32 GetCurrentSeconds()
	{
		using namespace boost;
		using namespace boost::gregorian;

		posix_time::ptime epoch(date(1970, Jan, 1));
		posix_time::time_duration time_from_epoch = posix_time::second_clock::universal_time() - epoch;
		return time_from_epoch.total_seconds();
	}

	/*
	*
	*	Detail: 获得系统时间戳(微秒)
	*   
	*  Created by hzd 2014-10-7
	*
	*/
	static uint64 GetCurrentMircoseconds()
	{
		using namespace boost;
		using namespace boost::gregorian;

		posix_time::ptime epoch(date(1970, Jan, 1));
		posix_time::time_duration time_from_epoch =posix_time::microsec_clock::universal_time() - epoch;
		return time_from_epoch.total_microseconds();
	}

};




#endif
