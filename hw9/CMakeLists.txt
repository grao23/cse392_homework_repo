cmake_minimum_required( VERSION 3.12 )
project( cudaHW1 VERSION 1.0 LANGUAGES CUDA CXX )

add_executable( cudaHW1 cudaHW1.cu )

set_target_properties(
	cudaHW1 PROPERTIES
	CUDA_ARCHITECTURES 75 )

target_include_directories(
	cudaHW1 PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR} )

install( TARGETS cudaHW1 DESTINATION . )
