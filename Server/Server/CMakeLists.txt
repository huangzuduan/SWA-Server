########### SWA_Server ###############

file(GLOB_RECURSE SWA_ServerSRCS *.h *.cpp *.cc)

# precompiled header
if (USE_COREPCH)
  set(Server_PCH_HDR Server_PCH.h)
  set(Server_PCH_SRC Server_PCH.cpp)
endif ()

include_directories(
    ${CMAKE_SOURCE_DIR}/Shared/Common
	${PROTOBUL_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
)

add_executable(SWA_Server
    ${SWA_ServerSRCS}
)

target_link_libraries(SWA_Server
    Shared
	LibProtocol
    ${MYSQL_LIBRARIES}
    ${BOOST_LIBRARIES}
)

IF(UNIX)
    target_link_libraries(SWA_Server
        pthread
        rt
        dl
    )

    if( CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../Bin/Debug/)
    else()
        SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../Bin/Release/)
    endif()
ENDIF(UNIX)

if( WIN32 )
  if ( MSVC )
    SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../Bin/)

    add_custom_command(TARGET SWA_Server
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/server_cfg.xml $(TargetDir)
	  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/server_cfg.xml ${PROJECT_BINARY_DIR}/Server/SWA_Server
    )
  endif()
endif()

# Generate precompiled header
if (USE_COREPCH)
        add_cxx_pch(SWA_Server ${Server_PCH_HDR} ${Server_PCH_SRC})
endif ()
