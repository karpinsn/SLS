#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Documentation for wrench library
#	---------------------------------------------------------------------------------------

#	-----------------------------------------
#	Doxygen
#	-----------------------------------------
find_package(Doxygen)

#	-----------------------------------------
#	Create target
#	-----------------------------------------
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} 
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
