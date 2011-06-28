# - Try to find JAI Camera API
# Once done this will define
#  
#  JAI_FOUND        - System has the JAI camera API
#  JAI_INCLUDE_DIR  - The JAI Camera APIs include directory
#  JAI_LIBRARY_DIR  - The JAI Camera APIs library directory
#  JAI_LIBRARY      - Link these to use the JAI Camera API
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
              PATHS c:/Program Files/JAI ${COMPILER_PATH}/PlatformSDK/Include )
   SET( JAI_NAMES Jai_Factory )
   FIND_LIBRARY( JAI_LIBRARY
                 NAMES ${JAI_NAMES}
                 PATHS c:/Program Files/JAI ${COMPILER_PATH}/PlatformSDK/Lib )
else( WIN32 )
   message(ERROR "JAI camera not supported on this OS")
endif( WIN32 )

GET_FILENAME_COMPONENT( JAI_LIBRARY_DIR ${JAI_LIBRARY} PATH )

IF (JAI_INCLUDE_DIR AND JAI_LIBRARY)
   SET(JAI_FOUND TRUE)
    SET( JAI_LIBRARY_DIR ${JAI_LIBRARY} )
ELSE (JAI_INCLUDE_DIR AND JAI_LIBRARY)
   SET( JAI_FOUND FALSE )
   SET( JAI_LIBRARY_DIR )
ENDIF (JAI_INCLUDE_DIR AND JAI_LIBRARY)

MARK_AS_ADVANCED(
  JAI_LIBRARY
  JAI_INCLUDE_DIR
)
