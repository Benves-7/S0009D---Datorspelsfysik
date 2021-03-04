#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(S0009D)
FILE(GLOB S0009D_headers code/*.h)
FILE(GLOB S0009D_sources code/*.cc)

SET(files ${S0009D_headers} ${S0009D_sources})
SOURCE_GROUP("S0009D" FILES ${files})

ADD_EXECUTABLE(S0009D ${files})
TARGET_LINK_LIBRARIES(S0009D core render)
ADD_DEPENDENCIES(S0009D core render)