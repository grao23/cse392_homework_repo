cmake_minimum_required( VERSION 3.12 )
project( exercise2 VERSION 1.0 )

# https://cmake.org/cmake/help/latest/module/FindMPI.html
find_package( MPI )

set(CMAKE_CXX_STANDARD 11)

add_executable( exercise2 exercise2.cxx )
target_include_directories(
        exercise2 PUBLIC
        ${MPI_C_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(
        exercise2 PUBLIC
        ${MPI_C_LIBRARIES} )

install( TARGETS exercise2 DESTINATION . )
