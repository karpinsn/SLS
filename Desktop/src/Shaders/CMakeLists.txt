include(FileCopyTargets)

#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_VERT_SOURCES

	Depth2Holo.vert
	Mesh2Depth.vert
	Holoencoder.vert
	MedianFilter3x3.vert
	NormalCalculator.vert	
	Phong.vert
	AxisDisplay.vert
	TextureDisplay.vert
    RotationManipulator.vert
	FinalRenderColor.vert
	FinalRenderTexture.vert
)

set(SHADER_FRAG_SOURCES

	Depth2Holo.frag
	Mesh2Depth.frag
    HoloencoderDither.frag
	Holoencoder.frag
	MedianFilter3x3.frag
	NormalCalculator.frag
	Phong.frag
	AxisDisplay.frag
	TextureDisplay.frag
    RotationManipulator.frag
	FinalRenderColor.frag
	FinalRenderTexture.frag
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
add_subdirectory(SixFringe)
add_subdirectory(NineFringe)
