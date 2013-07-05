include(FileCopyTargets)

#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_VERT_SOURCES

    Mesh2Depth.vert
    Depth2Holo.vert
)

set(SHADER_FRAG_SOURCES

    Mesh2Depth.frag
    Depth2Holo.frag   
)

set(SHADER_SOURCES ${SHADER_VERT_SOURCES} ${SHADER_FRAG_SOURCES})
set(SHADER_SOURCES "${SHADER_SOURCES}" PARENT_SCOPE)

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------
set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/Shaders/DepthHoloencoder")
message(STATUS "Copying shaders to: " ${SHADER_OUTPUT})

add_file_copy_target(DepthHoloencoderShaders ${SHADER_OUTPUT} ${SHADER_SOURCES})
add_dependencies(${PROJECT_NAME} DepthHoloencoderShaders)
