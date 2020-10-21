
#----------------------------------------------------------
set(LIB asmq)

add_subdirectory(tmq)

add_library(${LIB} SHARED
  $<TARGET_OBJECTS:tmq>
)


#----------------------------------------------------------
set_target_properties(${LIB} PROPERTIES VERSION ${VERSION} SOVERSION ${MAJOR_VERSION})

install(TARGETS ${LIB} DESTINATION ${LIB_DIR} COMPONENT lib)

