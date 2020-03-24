cmake_minimum_required(VERSION 3.12)
project(PBBFMM3D)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

if (MSVC)
	set(CMAKE_SUPPRESS_REGENERATION ON)
endif()

file(GLOB incl RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.hpp)
source_group("Include Files" FILES ${incl})

set(src src/kernel_Types.cpp
 	src/H2_3D_Tree.cpp
 	src/read_metadata.cpp
 	src/read_sources.cpp
 	src/write_Into_Binary_File.cpp)
source_group("Source Files" FILES ${src})

add_library(pbb_fmm3d SHARED ${incl} ${src})
target_include_directories(pbb_fmm3d PUBLIC "include")

set(MKL_ROOT 
	"C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows")
target_include_directories(pbb_fmm3d PUBLIC
	"${MKL_ROOT}/mkl/include"
	"${MKL_ROOT}/mkl/include/fftw")
target_link_directories(pbb_fmm3d PUBLIC
	"${MKL_ROOT}/redist/intel64/mkl"
	"${MKL_ROOT}/mkl/lib/intel64")
target_link_libraries(pbb_fmm3d mkl_rt)

set(MPI_ROOT "C:/Program Files (x86)/IntelSWTools/mpi/2019.6.166/intel64")
target_include_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/include")
target_link_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/lib")
target_link_libraries(pbb_fmm3d debug "debug/impi")
target_link_libraries(pbb_fmm3d optimized "release/impi")

set_target_properties(pbb_fmm3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)


function(add_example name file)
	add_executable(${name}
		"examples/${file}")
	target_link_libraries(${name} pbb_fmm3d)
endfunction()

add_example(get_input_through_routine_mykernel
	get_input_through_routine_mykernel.cpp)

add_example(get_input_through_routine_standard_kernel
	get_input_through_routine_standard_kernel.cpp)

add_example(binary_file_mykernel
	binary_file_mykernel.cpp)

add_example(binary_file_standard_kernel
	binary_file_standard_kernel.cpp)

add_example(tomography
	3d_exp_cov.cpp)
