set( CMAKE_C_FLAGS "-wall -g" )

file( GLOB SRCS *.cpp *.h )

add_executable( mineout ${SRCS} )

