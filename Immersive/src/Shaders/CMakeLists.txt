#	-----------------------------------------------------------------------------
#							copy_shaders macro
#
#	This macro copies all the .vert and .frag shaders in the current directory to
#	${APP_PATH}/../Shaders/
#	-----------------------------------------------------------------------------
#macro(copy_shaders)
#endmacro(copy_shaders)

#	-----------------------------------------------------------------------------
#							Copy Shaders to build
#	-----------------------------------------------------------------------------

message(STATUS "Copying shaders to the build directory")

get_target_property(APP_LOCATION ${PROJECT_NAME} LOCATION)
get_filename_component(APP_PATH ${APP_LOCATION} PATH)
message(${APP_PATH}/../Shaders/)

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND}
				   ARGS -E make_directory ${APP_PATH}/../Shaders/
				   COMMENT "Creating directory: ${APP_PATH}/../Shaders/")

message(STATUS "Looking for shaders in the following directory: " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SHADERS_TO_COPY
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.frag" "*.vert")

message(STATUS "Copying the following shaders: " ${SHADERS_TO_COPY})
foreach(FILENAME ${SHADERS_TO_COPY})
   	set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}")
   	set(DST "${APP_PATH}/../Shaders/${FILENAME}")

	message("SRC " ${SRC})
	message("DST " ${DST})

	# Copy the shader to the directory
   	add_custom_command(TARGET ${PROJECT_NAME}
   					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} 
    				   ARGS -E copy ${SRC} ${DST})
endforeach(FILENAME)