cmake_minimum_required( VERSION 3.12 )
project( hello VERSION 1.0 )

# https://cmake.org/cmake/help/latest/module/FindMPI.html
find_package( MPI )

add_executable( hello hello.c )
target_include_directories(
	hello PUBLIC
	${MPI_C_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(
	hello PUBLIC 
	${MPI_C_LIBRARIES} )

install( TARGETS hello DESTINATION . )
