cmake_minimum_required(VERSION 3.16)
project(BPF-KV)

if (NOT UNIX)
    message(FATAL_ERROR "Unsupported OS")
endif()

# Common Options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# save the executables/libraries according to GNU standard
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# enable warnings by default for all targets
add_compile_options(-Wall -Wextra -Wpedantic -Wunused)
add_compile_definitions(_GNU_SOURCE)

# enable lto for release build
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE true)

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# use pthread
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(simplekv
    simplekv.c # simplekv.h db_types.h
    helpers.c # helpers.h db_types.h  # helpers.o
    range.c # range.h db_types.h parse.h db_types.h simplekv.h helpers.h  # range.o
    parse.c # parse.h helpers.h  # parse.o
    create.c # create.h parse.h db_types.h simplekv.h  # create.o
    get.c # get.h db_types.h parse.h simplekv.h  # get.o
)
target_link_libraries(simplekv PRIVATE bpf m Threads::Threads)

add_custom_target(xrp-bpf
    DEPENDS xrp-bpf/get.c xrp-bpf/range.c

    COMMAND clang -S -target bpf -D __BPF_TRACING__ -I/build/usr/ -Wall -Wno-unused-value -Wno-pointer-sign -Wno-compare-distinct-pointer-types -Werror -O2 -emit-llvm -c -g -o get.ll get.c
    COMMAND llc -march=bpf -filetype=obj -o get.o get.ll
    COMMAND clang -S -target bpf -D __BPF_TRACING__ -I/build/usr/ -Wall -Wno-unused-value -Wno-pointer-sign -Wno-compare-distinct-pointer-types -Werror -O2 -emit-llvm -c -g -o range.ll range.c
    COMMAND llc -march=bpf -filetype=obj -o range.o range.ll

    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/xrp-bpf
    COMMENT "Generating bpf object files"
    VERBATIM
)
add_dependencies(simplekv xrp-bpf)