cmake_minimum_required( VERSION 3.12 )
project( heat_distribution VERSION 1.0 )

# https://cmake.org/cmake/help/latest/module/FindMPI.html
find_package( MPI )

set(CMAKE_CXX_STANDARD 11)

add_executable( heat_distribution heat_distribution.cxx )
target_include_directories(
	heat_distribution PUBLIC
	${MPI_C_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(
	heat_distribution PUBLIC 
	${MPI_C_LIBRARIES} )

install( TARGETS heat_distribution DESTINATION . )
