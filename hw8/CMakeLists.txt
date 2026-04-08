cmake_minimum_required( VERSION 3.20 )
project( tasks VERSION 1.0 )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
else()
	message( FATAL_ERROR "Could not find OpenMP" )
endif()

add_executable( tasks tasks.cxx )
target_compile_features( tasks PRIVATE cxx_std_11 )
target_link_libraries( tasks PUBLIC OpenMP::OpenMP_CXX )


install( TARGETS tasks DESTINATION . )
