#ifndef _STRING_UTILS_INCLUDED_
#define _STRING_UTILS_INCLUDED_

// stl
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdint.h>
//#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/lexical_cast.hpp"

class StringUtils
{
public:

	static void split(const std::string& src, std::vector<std::string>& ret);

	static void split(const std::string& src, const std::string splitStr,
			std::vector<std::string>& ret);

	static std::string replace(std::string& instr, const std::string& src, const std::string& dest);

	static std::string replaceAll(std::string& instr, const std::string& src, const std::string& dest);

	template <typename SourceType>
	static std::string toString(SourceType value)
	{
		 return boost::lexical_cast<std::string>(value);
	}

	//static std::string toString(const boost::posix_time::ptime& time);

	static int toInt32(const std::string& str, int32_t & value);
	static int toUInt32(const std::string& str, uint32_t & value);
	static int toInt64(const std::string& str, int64_t & value);
	static int toUInt64(const std::string& str, uint64_t & value);

	//static void toPtime(const std::string& str,  boost::posix_time::ptime& time);

	static double eval(const std::string& formula);

	static std::string toHexString(int i);

};

#endif //_STRING_UTILS_INCLUDED_
