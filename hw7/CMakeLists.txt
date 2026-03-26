cmake_minimum_required( VERSION 3.20 )
project( pi VERSION 1.0 )

find_package(OpenMP)
if(OpenMP_C_FOUND)
else()
	message( FATAL_ERROR "Could not find OpenMP" )
endif()

add_executable( pi pi.c )
target_compile_features( pi PRIVATE c_std_99 )
target_link_libraries( pi PUBLIC OpenMP::OpenMP_C )
target_link_libraries( pi PUBLIC OpenMP::OpenMP_C m )

install( TARGETS pi DESTINATION . )
