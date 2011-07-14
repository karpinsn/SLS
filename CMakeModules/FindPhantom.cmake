# - Try to find Phantom Camera API
# Once done this will define
#  
#  PHANTOM_FOUND        - System has the Phatom camera API
#  PHANTOM_INCLUDE_DIR  - The Phantom Camera APIs include directory
#  PHANTOM_LIBRARY_DIR  - The Phantom Camera APIs library directory
#  PHANTOM_LIBRARIES      - Link these to use the Phantom Camera API
#   

IF (PHANTOM_FOUND)
  # Already in cache, be silent
  SET(PHANTOM_FIND_QUIETLY TRUE)
ENDIF (PHANTOM_FOUND)

if( WIN32 )
   if( MSVC80 )
       set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC" )
   endif( MSVC80 )
   if( MSVC71 )
       set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ .NET\ 2003/Vc7" )
   endif( MSVC71 )
   FIND_PATH( PHANTOM_INCLUDE_DIR phcon.h PhFile.h phint.h
	   PATHS "C:/Program\ Files/Phantom/PhSDK/Inc" "C:/Program\ Files (x86)/Phantom/PhSDK/Inc" 
	   ${COMPILER_PATH}/PlatformSDK/Include )
   SET( PHANTOM_NAMES PhCon PhFile PhInt )
   FIND_LIBRARY( PHANTOM_LIBRARIES
                 NAMES ${PHANTOM_NAMES}
		 PATHS "C:/Program\ Files/Phantom/PhSDK/Lib" "C:/Program\ Files (x86)/Phantom/PhSDK/Lib" 
		 ${COMPILER_PATH}/PlatformSDK/Lib )
		 
		 
		 
GET_FILENAME_COMPONENT( PHANTOM_LIBRARY_DIR ${PHANTOM_LIBRARIES} PATH )

IF (PHANTOM_INCLUDE_DIR AND PHANTOM_LIBRARIES)
   SET(PHANTOM_FOUND TRUE)
ELSE (PHANTOM_INCLUDE_DIR AND PHANTOM_LIBRARIES)
   SET( PHANTOM_FOUND FALSE )
   SET( PHANTOM_LIBRARY_DIR )
   SET( PHANTOM_LIBARIES )
ENDIF (PHANTOM_INCLUDE_DIR AND PHANTOM_LIBRARIES)

MARK_AS_ADVANCED(
  PHANTOM_LIBRARIES
  PHANTOM_LIBRARY_DIR
  PHANTOM_INCLUDE_DIR
)

else( WIN32 )
   message(ERROR "Phantom camera not supported on this OS")
endif( WIN32 )

