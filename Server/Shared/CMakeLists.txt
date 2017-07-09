file(GLOB_RECURSE headers_CommonDef_H Common/*.h)

file(GLOB_RECURSE sources_CommonDef_CC Common/*.cc)
file(GLOB_RECURSE sources_CommonDef_CPP Common/*.cpp)

include_directories(
	${MYSQL_INCLUDE_DIR}
	${BOOST_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/Shared/Common
	${PROTOBUL_INCLUDE_DIR}
)

set(Shared_SRCS
  ${headers_CommonDef_H}
  ${sources_CommonDef_CC}
  ${sources_CommonDef_CPP}
)

add_library(Shared STATIC
  ${Shared_SRCS}
)

target_link_libraries(Shared
    ${BOOST_LIBRARIES}
    ${MYSQL_LIBRARIES}
)

if( WIN32 )
  if ( MSVC )
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../Bin/)
  endif()
endif()
