include(FileCopyTargets)

#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_VERT_SOURCES

	Holoencoder.vert
	MedianFilter3x3.vert
	NormalCalculator.vert	
	Phong.vert
	AxisDisplay.vert
	TextureDisplay.vert
)

set(SHADER_FRAG_SOURCES

	Holoencoder.frag
	MedianFilter3x3.frag
	NormalCalculator.frag
	Phong.frag
	AxisDisplay.frag
	TextureDisplay.frag
)

set(SHADER_SOURCES ${SHADER_VERT_SOURCES} ${SHADER_FRAG_SOURCES})
set(SHADER_SOURCES "${SHADER_SOURCES}" PARENT_SCOPE)

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------
set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/Shaders")
message(STATUS "Copying shaders to: " ${SHADER_OUTPUT})

add_file_copy_target(MainShaders ${SHADER_OUTPUT} ${SHADER_SOURCES})
add_dependencies(${PROJECT_NAME} MainShaders)

#	---------------------------------------------------------------------------------------
#	Add More Shaders
#	---------------------------------------------------------------------------------------
add_subdirectory(Holovideo)
