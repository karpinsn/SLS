include(FileCopyTargets)
set(SHADER_TARGET CopyShaders)

#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_VERT_SOURCES

	Holoencoder.vert
	MedianFilter3x3.vert
	NormalCalculator.vert
	PhaseCalculator.vert
	FinalRender.vert
	Phong.vert
)

set(SHADER_FRAG_SOURCES

	Holoencoder.frag
	MedianFilter3x3.frag
	NormalCalculator.frag
	PhaseCalculator.frag
	FinalRender.frag
	Phong.vert
)

SET(SHADER_SOURCES ${SHADER_VERT_SOURCES} ${SHADER_FRAG_SOURCES})
set(SHADER_SOURCES "${SHADER_SOURCES}" PARENT_SCOPE)

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------
set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/Shaders")
message(STATUS "Copying shaders to: " ${SHADER_OUTPUT})

add_file_copy_target(${SHADER_TARGET} ${SHADER_OUTPUT} ${SHADER_SOURCES})
set(SHADER_TARGET "${SHADER_TARGET}" PARENT_SCOPE)
