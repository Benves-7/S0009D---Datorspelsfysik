#--------------------------------------------------------------------------
# S0009D project
#--------------------------------------------------------------------------

PROJECT(S0009D)
FILE(GLOB S0009D_headers code/*.h)
FILE(GLOB S0009D_sources code/*.cc)
FILE(GLOB debug code/debug/*.*)
FILE(GLOB data code/data/*.*)
FILE(GLOB shader code/shader/*.*)

SET(files ${S0009D_headers} ${S0009D_sources} ${debug} ${shader})
SOURCE_GROUP("S0009D" FILES ${files})

foreach(FILE ${data})
	configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)

ADD_EXECUTABLE(S0009D ${files})
TARGET_LINK_LIBRARIES(S0009D core render)
ADD_DEPENDENCIES(S0009D core render)