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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /openmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /openmp")


add_library(pbb_fmm3d SHARED ${incl} ${src})

set(MKL_ROOT
    "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl")

set(FFTW_ROOT "C:/fftw-3.3.5-dll64")

target_include_directories(pbb_fmm3d PUBLIC 
	"include" 
	"${MKL_ROOT}/include"
	#${FFTW_ROOT}
	"${MKL_ROOT}/include/fftw"
	)

target_link_directories(pbb_fmm3d PUBLIC
        "${MKL_ROOT}/lib/intel64"
        "${FFTW_ROOT}"
        )

target_link_libraries(pbb_fmm3d
    mkl_rt
    mkl_lapack95_lp64
		#mkl_core
    mkl_intel_lp64
    mkl_sequential
    #mkl_blas95_lp64
    #mkl_intel_ilp64
    #mkl_intel_lp64
    #mkl_intel_thread
    #mkl_lapack95_ilp64
    #mkl_tbb_thread
    #fftw3
    )

set(MPI_ROOT "C:/Program Files (x86)/IntelSWTools/mpi/2019.6.166/intel64")
target_include_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/include")
target_link_directories(pbb_fmm3d PUBLIC "${MPI_ROOT}/lib")
target_link_libraries(pbb_fmm3d debug "debug/impi")
target_link_libraries(pbb_fmm3d optimized "release/impi")

set_target_properties(pbb_fmm3d PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

# 
# # test
# file(COPY test/data DESTINATION .)
# add_executable(cdf_cpp_test test/cdf_cpp_test.cpp)
# target_link_libraries(cdf_cpp_test cdf_cpp)
# 
# # cdf distribution test
# #
# #enable_testing()
# #function(cdf_test n)
# #    add_executable(${n} tests/${n}.c)
# #    target_link_libraries(${n} cdf)
# #    add_test(NAME ${n} COMMAND ${n})
# #endfunction()
# #
# #cdf_test(qst2c)
# #cdf_test(qst2cOrig)
# #cdf_test(qst2ic)
# #cdf_test(qst2icEPOCH16)
# #cdf_test(qst2icTT2000)
# #cdf_test(qst2Version)
