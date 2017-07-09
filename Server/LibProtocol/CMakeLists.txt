file(GLOB_RECURSE headers_LibProtocol_H ./*.h)
file(GLOB_RECURSE sources_LibProtocol_CC ./*.cc)

include_directories(
	${PROTOBUL_INCLUDE_DIR}
)

set(LibProtocol_SRCS
	${headers_LibProtocol_H}
	${sources_LibProtocol_CC}
)

add_library(LibProtocol STATIC
  ${LibProtocol_SRCS}
)

target_link_libraries(LibProtocol
   ${LIB_PROTOBUF_DEBUG}
)

if( WIN32 )
  if ( MSVC )
    SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../Bin/Debug/)
  endif()
endif()
