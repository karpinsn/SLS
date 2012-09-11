# - Try to find PointGrey Camera API
# Once done this will define
#  
#  POINTGREY_FOUND        - System has the PointGrey camera API
#  POINTGREY_INCLUDE_DIR  - The PointGrey Camera APIs include directory
#  POINTGREY_LIBRARY_DIR  - The PointGrey Camera APIs library directory
#  POINTGREY_LIBRARIES    - Link these to use the PointGrey Camera API
#   

IF (POINTGREY_FOUND)
    # Already in cache, blow away incase user switched from 32 to 64 bit libs
    SET( POINTGREY_INCLUDE_DIR )
    SET( POINTGREY_LIBRARY_DIR )
    SET( POINTGREY_LIBRARIES)
ENDIF (POINTGREY_FOUND)

if( WIN32 )
    if( MSVC80 )
        set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC" )
    endif( MSVC80 )
    if( MSVC71 )
        set( COMPILER_PATH "C:/Program\ Files/Microsoft\ Visual\ Studio\ .NET\ 2003/Vc7" )
    endif( MSVC71 )

    SET( POINTGREY_NAMES FlyCapture2 )

    # Check which libs we should compile against
    option(POINTGREY_USE_64 "Build with 64 bit libraries" OFF)
    if(POINTGREY_USE_64)
        FIND_PATH( POINTGREY_INCLUDE_DIR FlyCapture2.h
            PATHS "C:/Program\ Files/Point\ Grey\ Research/FlyCapture2/include"
            ${COMPILER_PATH}/PlatformSDK/Include )

        FIND_LIBRARY( POINTGREY_LIBRARIES
            NAMES ${POINTGREY_NAMES}
            PATHS "C:/Program\ Files/Point\ Grey\ Research/FlyCapture2/lib64"
            ${COMPILER_PATH}/PlatformSDK/Lib )      
    else(POINTGREY_USE_64)
        FIND_PATH( POINTGREY_INCLUDE_DIR FlyCapture2.h
            PATHS "C:/Program\ Files (x86)/Point\ Grey\ Research/FlyCapture2/include" 
            ${COMPILER_PATH}/PlatformSDK/Include )

        FIND_LIBRARY( POINTGREY_LIBRARIES
            NAMES ${POINTGREY_NAMES}
            PATHS "C:/Program\ Files (x86)/Point\ Grey\ Research/FlyCapture2/lib" 
            ${COMPILER_PATH}/PlatformSDK/Lib )      

    endif(POINTGREY_USE_64)

    GET_FILENAME_COMPONENT( POINTGREY_LIBRARY_DIR ${POINTGREY_LIBRARIES} PATH )

    IF (POINTGREY_INCLUDE_DIR AND POINTGREY_LIBRARIES)
        SET(POINTGREY_FOUND TRUE)
    ELSE (POINTGREY_INCLUDE_DIR AND POINTGREY_LIBRARIES)
        SET( POINTGREY_FOUND FALSE )
        SET( POINTGREY_LIBRARY_DIR )
        SET( POINTGREY_LIBARIES )
    ENDIF (POINTGREY_INCLUDE_DIR AND POINTGREY_LIBRARIES)

    MARK_AS_ADVANCED(
        POINTGREY_LIBRARIES
        POINTGREY_LIBRARY_DIR
        POINTGREY_INCLUDE_DIR
        )

else( WIN32 )
    message(ERROR "Phantom camera not supported on this OS")
endif( WIN32 )

