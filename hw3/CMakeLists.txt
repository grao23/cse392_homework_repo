cmake_minimum_required( VERSION 3.12 )
project( sorting VERSION 1.0 )

# https://cmake.org/cmake/help/latest/module/FindMPI.html
find_package( MPI )

set(CMAKE_CXX_STANDARD 11)

add_executable( sorting sorting.cxx )
target_include_directories(
	sorting PUBLIC
	${MPI_C_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(
	sorting PUBLIC 
	${MPI_C_LIBRARIES} )

install( TARGETS sorting DESTINATION . )
