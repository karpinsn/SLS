# - Try to find JAI Camera API
# Once done this will define
#  
#  JAI_FOUND        - System has the JAI camera API
#  JAI_INCLUDE_DIR  - The JAI Camera APIs include directory
#  JAI_LIBRARY_DIR  - The JAI Camera APIs library directory
#  JAI_LIBRARIES    - Link these to use the JAI Camera API
#   

IF (JAI_FOUND)
  # Already in cache, be silent
  SET(JAI_FIND_QUIETLY TRUE)
ENDIF (JAI_FOUND)

if( WIN32 )
   if( MSVC80 )
       set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC" )
   endif( MSVC80 )
   if( MSVC71 )
       set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ .NET\ 2003/Vc7" )
   endif( MSVC71 )
   FIND_PATH( JAI_INCLUDE_DIR Jai_Factory.h
	   PATHS "C:/Program\ Files/JAI/SDK/library/CPP/include" ${COMPILER_PATH}/PlatformSDK/Include )
   SET( JAI_NAMES Jai_Factory )
   FIND_LIBRARY( JAI_LIBRARIES
                 NAMES ${JAI_NAMES}
		 PATHS "C:/Program\ Files/JAI/SDK/library/CPP/lib/Win32_i86" ${COMPILER_PATH}/PlatformSDK/Lib )

 
GET_FILENAME_COMPONENT( JAI_LIBRARY_DIR ${JAI_LIBRARIES} PATH )

IF (JAI_INCLUDE_DIR AND JAI_LIBRARIES)
   SET(JAI_FOUND TRUE)
ELSE (JAI_INCLUDE_DIR AND JAI_LIBRARIES)
   SET( JAI_FOUND FALSE )
   SET( JAI_LIBRARY_DIR )
ENDIF (JAI_INCLUDE_DIR AND JAI_LIBRARIES)

MARK_AS_ADVANCED(
  JAI_LIBRARY
  JAI_LIBRARIES
  JAI_INCLUDE_DIR
  )

else( WIN32 )
   message(ERROR "JAI camera not supported on this OS")
endif( WIN32 )
