include(FileCopyTargets)

#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_VERT_SOURCES
	PassThrough.vert
)

set(SHADER_FRAG_SOURCES
	PhaseWrapper.frag
	PhaseUnwrapper.frag
	DepthCalculator.frag
    NormalCalculator.frag
	PhaseFilter.frag
)

set(SHADER_SOURCES ${SHADER_VERT_SOURCES} ${SHADER_FRAG_SOURCES})
set(SHADER_SOURCES "${SHADER_SOURCES}" PARENT_SCOPE)

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------
set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/Shaders/SixFringe")
message(STATUS "Copying shaders to: " ${SHADER_OUTPUT})

add_file_copy_target(SixFringeShaders ${SHADER_OUTPUT} ${SHADER_SOURCES})
add_dependencies(${PROJECT_NAME} SixFringeShaders)
