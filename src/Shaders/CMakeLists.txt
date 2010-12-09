#	---------------------------------------------------------------------------------------
#	List all of the shader sources to be copied over
#	---------------------------------------------------------------------------------------

set(SHADER_SOURCES

	Holoencoder.vert
	Holodecoder.vert
	MedianFilter3x3.vert
	NormalCalculator.vert
	PhaseCalculator.vert
	FinalRender.vert

	Holoencoder.frag
	Holodecoder.frag
	MedianFilter3x3.frag
	NormalCalculator.frag
	PhaseCalculator.frag
	FinalRender.frag
)

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------

message(STATUS "Copying Shaders")

set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Shaders")
message("Shaders will be copied to: ${SHADER_OUTPUT}")

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND}
				   ARGS -E make_directory ${SHADER_OUTPUT}
				   COMMENT "Creating directory: ${SHADER_OUTPUT}")

message(STATUS "Copying the following shaders: " ${SHADER_SOURCES})
foreach(FILENAME ${SHADER_SOURCES})
   	set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
   	set(DST "${SHADER_OUTPUT}/${FILENAME}")

	message("SRC " ${SRC})
	message("DST " ${DST})

	# Copy the shader to the directory
   	add_custom_command(TARGET ${PROJECT_NAME}
   					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} 
    				   ARGS -E copy "${SRC}" "${DST}"
					   COMMENT "Copying ${SRC} to ${DST}")
endforeach(FILENAME)