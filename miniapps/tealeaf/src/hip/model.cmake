
register_flag_required(CMAKE_CXX_COMPILER
        "Absolute path to the AMD HIP C++ compiler")

register_flag_optional(MANAGED_ALLOC "Use UVM (hipMallocManaged) instead of the device-only allocation (hipMalloc)"
        "OFF")

register_flag_optional(SYNC_ALL_KERNELS
        "Fully synchronise all kernels after launch, this also enables synchronous error checking with line and file name"
        "OFF")


macro(setup)
    enable_language(HIP) # CMake 3.21
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        #set(CUSAN_ARCH_FLAG --cuda-gpu-arch=${CUDA_ARCH})
    else()
        #set(CUSAN_ARCH_FLAG -arch=${CUDA_ARCH})
    endif()
    set(CMAKE_HIP_FLAGS "${CMAKE_HIP_FLAGS} -std=c++17 -fPIC ${CUSAN_ARCH_FLAG} ${HIP_EXTRA_FLAGS}")

    if (MANAGED_ALLOC)
        register_definitions(CLOVER_MANAGED_ALLOC)
    endif ()

    if (SYNC_ALL_KERNELS)
        register_definitions(CLOVER_SYNC_ALL_KERNELS)
    endif ()

endmacro()

macro(setup_target NAME)
    # Treat everything as HIP source
    get_target_property(PROJECT_SRC "${NAME}" SOURCES)
    foreach (SRC ${PROJECT_SRC})
        set_source_files_properties("${SRC}" PROPERTIES LANGUAGE HIP)
    endforeach ()
endmacro()
