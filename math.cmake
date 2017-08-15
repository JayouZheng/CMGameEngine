
ADD_DEFINITIONS("-DMATH_USE_OPENGL")
ADD_DEFINITIONS("-DMATH_RIGHTHANDED_CAMERA")

INCLUDE_DIRECTORIES(${CMGDK_PATH}/3rdpty/MathGeoLib/src)

IF(WIN32)
    link_directories(${CMGDK_PATH}/3rdpty/MathGeoLib/${CMGDK_BUILD_TYPE})
ELSE(WIN32)
    link_directories(${CMGDK_PATH}/3rdpty/MathGeoLib)
ENDIF(WIN32)

