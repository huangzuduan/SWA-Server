#ifndef SWA_LOGGER_H_
#define SWA_LOGGER_H_

#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <boost/log/core.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/detail/format.hpp>
#include <boost/log/detail/thread_id.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


#include <iostream>
#include <string>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

enum LogSeverityLevel {
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL,
	LOG_REPORT
};
void InitLog() {

	std::string logDir = "./log";
	typedef sinks::synchronous_sink<sinks::text_file_backend> TextSink;  
	if (boost::filesystem::exists(logDir) == false)
	{
		boost::filesystem::create_directories(logDir);
	}
	// init sink1  
	boost::shared_ptr<sinks::text_file_backend> backend1 = boost::make_shared<sinks::text_file_backend>(  
		keywords::file_name = logDir + "/%Y%m%d_%H%M.%N.log",  
		keywords::rotation_size = 10 * 1024 * 1024,  
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),  
		keywords::min_free_space = 30 * 1024 * 1024);  
	boost::shared_ptr<TextSink> sink_file(new TextSink(backend1));  
	sink_file->set_formatter (  
		expr::format("[%1%]<%2%>(%3%): %4%")  
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")  
		% expr::attr<LogSeverityLevel>("Severity")  
		% expr::attr<attrs::current_thread_id::value_type >("ThreadID")  
		% expr::smessage  
		);  
	sink_file->set_filter(expr::attr<LogSeverityLevel>("Severity") >= LOG_WARNING);  
	logging::core::get()->add_sink(sink_file);  

	logging::add_common_attributes();  
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());  

}
// 单个logger对象，线程安全my_logger::get()可获得logger对象
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt<LogSeverityLevel>)

	/************************************************************************/
	/* 使用方法
	
	
	InitLog();   // 程序 启动调用
	src::severity_logger_mt<LogSeverityLevel>& lg = my_logger::get();  
	BOOST_LOG_SEV(lg, LOG_TRACE) << "A trace severity message";  
	BOOST_LOG_SEV(lg, LOG_DEBUG) << "A debug severity message";  
	BOOST_LOG_SEV(lg, LOG_INFO) << "An informational severity message";  
	BOOST_LOG_SEV(lg, LOG_WARNING) << "A warning severity message";  
	BOOST_LOG_SEV(lg, LOG_ERROR) << "An error severity message";  
	BOOST_LOG_SEV(lg, LOG_FATAL) << "A fatal severity message";  
	BOOST_LOG_SEV(lg, LOG_REPORT) << "A report severity message";  
	logging::core::get()->remove_all_sinks();  // 程序结束前调用
	
	
	*/
	/************************************************************************/

#endif