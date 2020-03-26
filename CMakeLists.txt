cmake_minimum_required(VERSION 3.12)
project(PBBFMM3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_GENERATOR_PLATFORM x86)

if (MSVC)
	set(CMAKE_SUPPRESS_REGENERATION ON)
endif()

file(GLOB incl RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	include/*.hpp include/*.h)
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

# Copy dependency DLLs into output directory
add_custom_command(TARGET pbb_fmm3d POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	"${MKL_ROOT}/redist/intel64_win/mkl/mkl_rt.dll"
	"${MKL_ROOT}/redist/intel64_win/mkl/mkl_intel_thread.dll"
	"${MKL_ROOT}/redist/intel64_win/compiler/libiomp5md.dll"
 	$<TARGET_FILE_DIR:pbb_fmm3d>)

# Copy input, output directories into build tree
file(COPY "input" DESTINATION ".")
file(COPY "output" DESTINATION ".")

set(MPI_ROOT "C:/Program Files (x86)/IntelSWTools/mpi/2019.6.166/intel64")
target_include_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/include")
target_link_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/lib")
target_link_libraries(pbb_fmm3d debug "debug/impi")
target_link_libraries(pbb_fmm3d optimized "release/impi")

set_target_properties(pbb_fmm3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

function(add_example name)
	add_executable(${name} "examples/${name}.cpp")
	target_link_libraries(${name} pbb_fmm3d)
	set_property(TARGET ${name} PROPERTY 
		VS_DEBUGGER_WORKING_DIRECTORY 
		$<TARGET_FILE_DIR:${name}>)
endfunction()

add_example(get_input_through_routine_mykernel)
add_example(get_input_through_routine_standard_kernel)
add_example(binary_file_mykernel binary_file_mykernel.cpp)
add_example(binary_file_standard_kernel)
add_example(3d_exp_cov)
add_example(my_test)
