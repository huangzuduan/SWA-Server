# The name of our project is "HELLO". CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (SWA_Server_Cmake)

# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE "Release")
endif()

IF(MSVC)

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm500")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
   
ELSEIF(UNIX)
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
    ELSE()
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
    ENDIF()
ENDIF()

include(cmake/options.cmake)
include(cmake/FindPCHSupport.cmake)
include(cmake/cxx_flag_overrides.cmake)

# foreach(flag_var
        # CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        # CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   # if(${flag_var} MATCHES "/MD")
      # string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   # endif(${flag_var} MATCHES "/MD")
   
   # # if(${flag_var} MATCHES "/Zi")
      # # string(REGEX REPLACE "/Zi" "/C7" ${flag_var} "${${flag_var}}")
   # # endif(${flag_var} MATCHES "/Zi")
   
# endforeach(flag_var)

# turn off PCH totally if enabled (hidden setting, mainly for devs)
if( NOPCH )
  set(USE_COREPCH 0)
endif()

#find_package(PCHSupport)

# Find Lib path:

#
#  原该项目为win32，现在将boost,mysql编译为64位，其他的引用暂时无空编译64，有空再弄
#
#


IF(WIN32)
    IF(MSVC)
        # /MD and /MDd are the standard values - if someone wants to use
        # others, the libnames have to change here too
        # use also ssl and ssleay32 in debug as fallback for openssl < 0.9.8b
        # TODO: handle /MT and static lib
        # In Visual C++ naming convention each of these four kinds of Windows libraries has it's standard suffix:
        #   * MD for dynamic-release
        #   * MDd for dynamic-debug
        #   * MT for static-release
        #   * MTd for static-debug

        # Implementation details:
        # We are using the libraries located in the VC subdir instead of the parent directory eventhough :
        # libeay32MD.lib is identical to ../libeay32.lib, and
        # ssleay32MD.lib is identical to ../ssleay32.lib
		
		FIND_PATH(MYSQL_INCLUDE_DIR
          NAMES
            mysql/mysql.h
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/mysql/include
        )
        MARK_AS_ADVANCED(MYSQL_INCLUDE_DIR)
		
		FIND_PATH(DEPENDENCE_INCLUDE_DIR
          NAMES
            audiere/audiere.h
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence
        )
        MARK_AS_ADVANCED(DEPENDENCE_INCLUDE_DIR)
		
		FIND_LIBRARY(LIB_MYSQL_COMMON
          NAMES
            libmysql
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/mysql/lib
		  NO_DEFAULT_PATH
        )

        FIND_LIBRARY(LIB_MYSQL_DEBUG
          NAMES
            mysqlclient_debug
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/mysql/lib
        )

        FIND_LIBRARY(LIB_MYSQL_RELEASE
          NAMES
            mysqlclient_release
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/mysql/lib
        )

        MARK_AS_ADVANCED(LIB_MYSQL_COMMON LIB_MYSQL_DEBUG LIB_MYSQL_RELEASE)
		
		FIND_PATH(PROTOBUL_INCLUDE_DIR
          NAMES
            google/protobuf/any.h
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/protobuf/include
        )
		MARK_AS_ADVANCED(PROTOBUL_INCLUDE_DIR)
		
		FIND_LIBRARY(LIB_PROTOBUF_DEBUG
          NAMES
            libprotobufd
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/protobuf/lib
        )
		MARK_AS_ADVANCED(LIB_PROTOBUF_DEBUG)

        FIND_PATH(BOOST_INCLUDE_DIR
          NAMES
            boost/unordered_map.hpp
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/include
        )
        MARK_AS_ADVANCED(BOOST_INCLUDE_DIR)
		
		FIND_PATH(PROTOBUL_INCLUDE_DIR
          NAMES
            google/protobuf/any.h
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/protobuf/include
        )
		MARK_AS_ADVANCED(PROTOBUL_INCLUDE_DIR)
		
		FIND_LIBRARY(LIB_PROTOBUF_DEBUG
          NAMES
            libprotobufd
          PATHS
            ${HMX_Server_Cmake_SOURCE_DIR}/dependence/protobuf/lib
        )
		MARK_AS_ADVANCED(LIB_PROTOBUF_DEBUG)

        FIND_LIBRARY(LIB_BOOST_SYSTEM_DEBUG
          NAMES
            libboost_system-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
		  NO_DEFAULT_PATH
        )
		
		FIND_LIBRARY(LIB_BOOST_SYSTEM_RELEASE
          NAMES
            libboost_system-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_DATA_TIME_DEBUG
          NAMES
            libboost_date_time-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_THREAD_DEBUG
          NAMES
            libboost_thread-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_FILESYSTEM_DEBUG
          NAMES
            libboost_filesystem-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_PROGRAM_OPTIONS_DEBUG
          NAMES
            libboost_program_options-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_CHRONO_DEBUG
          NAMES
            libboost_chrono-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_REGEX_DEBUG
          NAMES
            libboost_regex-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_LOG_DEBUG
          NAMES
            libboost_log-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_LOG_SETUP_DEBUG
          NAMES
            libboost_log_setup-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_ATOMIC_DEBUG
          NAMES
            libboost_atomic-VC140-mt-gd-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_DATA_TIME_RELEASE
          NAMES
            libboost_date_time-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_THREAD_RELEASE
          NAMES
            libboost_thread-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_FILESYSTEM_RELEASE
          NAMES
            libboost_filesystem-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_PROGRAM_OPTIONS_RELEASE
          NAMES
            libboost_program_options-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_CHRONO_RELEASE
          NAMES
            libboost_chrono-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        FIND_LIBRARY(LIB_BOOST_REGEX_RELEASE
          NAMES
            libboost_regex-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_LOG_RELEASE
          NAMES
            libboost_log-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_LOG_SETUP_RELEASE
          NAMES
            libboost_log_setup-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )
		
		FIND_LIBRARY(LIB_BOOST_ATOMIC_RELEASE
          NAMES
            libboost_atomic-VC140-mt-1_60
          PATHS
            ${SWA_Server_Cmake_SOURCE_DIR}/dependence/boost/lib
        )

        MARK_AS_ADVANCED(   LIB_BOOST_SYSTEM_DEBUG
                            LIB_BOOST_SYSTEM_RELEASE
                            LIB_BOOST_DATA_TIME_DEBUG
                            LIB_BOOST_DATA_TIME_RELEASE
                            LIB_BOOST_THREAD_DEBUG
                            LIB_BOOST_THREAD_RELEASE
                            LIB_BOOST_FILESYSTEM_DEBUG
                            LIB_BOOST_FILESYSTEM_RELEASE
                            LIB_BOOST_PROGRAM_OPTIONS_DEBUG
                            LIB_BOOST_PROGRAM_OPTIONS_RELEASE
                            LIB_BOOST_CHRONO_DEBUG
                            LIB_BOOST_CHRONO_RELEASE
                            LIB_BOOST_REGEX_DEBUG
                            LIB_BOOST_REGEX_RELEASE
							LIB_BOOST_LOG_DEBUG
							LIB_BOOST_LOG_RELEASE
							LIB_BOOST_ATOMIC_DEBUG
							LIB_BOOST_ATOMIC_RELEASE
							LIB_BOOST_LOG_SETUP_DEBUG
							LIB_BOOST_LOG_SETUP_RELEASE
							)

        IF( CMAKE_BUILD_TYPE )
          SET( MYSQL_LIBRARIES
			debug ${LIB_MYSQL_COMMON}
            debug ${LIB_MYSQL_DEBUG}
			optimized ${LIB_MYSQL_COMMON}
            optimized ${LIB_MYSQL_RELEASE}
          )
          SET( BOOST_LIBRARIES
            debug ${LIB_BOOST_SYSTEM_DEBUG}
            debug ${LIB_BOOST_DATA_TIME_DEBUG}
            debug ${LIB_BOOST_THREAD_DEBUG}
            debug ${LIB_BOOST_FILESYSTEM_DEBUG}
            debug ${LIB_BOOST_PROGRAM_OPTIONS_DEBUG}
            debug ${LIB_BOOST_CHRONO_DEBUG}
            debug ${LIB_BOOST_REGEX_DEBUG}
			debug ${LIB_BOOST_LOG_DEBUG}
			debug ${LIB_BOOST_LOG_SETUP_DEBUG}
			debug ${LIB_BOOST_ATOMIC_DEBUG}
            optimized ${LIB_BOOST_SYSTEM_RELEASE}
            optimized ${LIB_BOOST_DATA_TIME_RELEASE}
            optimized ${LIB_BOOST_THREAD_RELEASE}
            optimized ${LIB_BOOST_FILESYSTEM_RELEASE}
            optimized ${LIB_BOOST_PROGRAM_OPTIONS_RELEASE}
            optimized ${LIB_BOOST_CHRONO_RELEASE}
            optimized ${LIB_BOOST_REGEX_RELEASE}
			optimized ${LIB_BOOST_LOG_RELEASE}
			optimized ${LIB_BOOST_LOG_SETUP_RELEASE}
			optimized ${LIB_BOOST_ATOMIC_RELEASE}
          )
        ELSE()
          SET( MYSQL_LIBRARIES
            ${LIB_MYSQL_RELEASE}
          )
          SET( BOOST_LIBRARIES
            ${LIB_BOOST_SYSTEM_RELEASE}
            ${LIB_BOOST_DATA_TIME_RELEASE}
            ${LIB_BOOST_THREAD_RELEASE}
            ${LIB_BOOST_FILESYSTEM_RELEASE}
            ${LIB_BOOST_PROGRAM_OPTIONS_RELEASE}
            ${LIB_BOOST_CHRONO_RELEASE}
            ${LIB_BOOST_REGEX_RELEASE}
			${LIB_BOOST_LOG_RELEASE}
          )
        ENDIF()

    ENDIF(MSVC)
ENDIF(WIN32)


# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (Client)
add_subdirectory (Server)
add_subdirectory (Shared)
add_subdirectory (LibProtocol)


