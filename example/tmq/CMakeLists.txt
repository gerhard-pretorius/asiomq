
#----------------------------------------------------------
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR}/lib)

list(APPEND PROTOBUF_IMPORT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")


#----------------------------------------------------------
PROTOBUF_GENERATE_CPP(MTYPE_PROTO_CPP PROTO_HDR mtype.proto)
add_executable(typegen typegen.cc ${MTYPE_PROTO_CPP})

target_link_libraries(typegen
  ${Protobuf_PROTOC_LIBRARIES}
  ${Protobuf_LIBRARIES}
)

#----------------------------------------------------------
set(TYPEGEN "typegen=${CMAKE_CURRENT_BINARY_DIR}/typegen")
PROTOBUF_GENERATE_CPP_PLUGIN(PROTO_CPP PROTO_HDR PLUGIN ${TYPEGEN} foo.proto)

#----------------------------------------------------------
add_executable(client1 client1.cc ${PROTO_CPP} ${MTYPE_PROTO_CPP})

target_link_libraries(client1 asmq
  ${Protobuf_PROTOC_LIBRARIES}
  ${Protobuf_LIBRARIES}
)


#----------------------------------------------------------
add_executable(server1 server1.cc ${PROTO_CPP} ${MTYPE_PROTO_CPP})

target_link_libraries(server1 asmq
  ${Protobuf_PROTOC_LIBRARIES}
  ${Protobuf_LIBRARIES}
)

