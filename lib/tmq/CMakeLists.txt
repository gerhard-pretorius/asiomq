
#----------------------------------------------------------
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/lib)

#----------------------------------------------------------
set(LIB tmq)

set(PROTO_SRC
  tmq.proto
)

#----------------------------------------------------------
PROTOBUF_GENERATE_CPP(PROTO_CPP PROTO_HDR ${PROTO_SRC})

add_library(${LIB} OBJECT ${PROTO_CPP})
set_property(TARGET ${LIB} PROPERTY POSITION_INDEPENDENT_CODE 1)

#----------------------------------------------------------
set(HDR_FILES
  ${PROTO_HDR}
  ${PROTO_SRC}
  tmqproto.h
)

install(FILES ${HDR_FILES} DESTINATION ${HDR_DIR} COMPONENT header)


