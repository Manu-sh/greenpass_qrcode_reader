cmake_minimum_required(VERSION 3.17)
project(greenpass_string_reader C)
set(CMAKE_C_STANDARD 11)

set(INC_PATH  ${PROJECT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} main.c)


# BASE45
include(FetchContent)
#FetchContent_Declare(base45-ansi-C GIT_REPOSITORY https://github.com/ehn-dcc-development/base45-ansi-C.git GIT_TAG 9c4777f13d00f9231049b1311018808a07cc9ef4) # the commit hash

FetchContent_Declare(base45-ansi-C GIT_REPOSITORY https://github.com/manu-sh/base45-ansi-C.git GIT_TAG b04ff75d2d3358f6dfab7142a4736729c8c15ca6) # the commit hash
FetchContent_GetProperties(base45-ansi-C)

string(TOLOWER "base45-ansi-C" lcName)
if(NOT ${lcName}_POPULATED)
    # Fetch the content using previously declared details
    FetchContent_Populate(base45-ansi-C)

    # Bring the populated content into the build
    #add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR})
    #target_include_directories(base45-ansi-C PUBLIC "${${lcName}_SOURCE_DIR}")
    include_directories("${${lcName}_SOURCE_DIR}")
endif()

find_package(ZLIB)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

FetchContent_Declare(COSE-C GIT_REPOSITORY https://github.com/cose-wg/COSE-C.git GIT_TAG 97d1805e71b7a6770093c5e6790d46611680d563) # the commit hash
FetchContent_MakeAvailable(COSE-C)
target_link_libraries(${PROJECT_NAME} PRIVATE cn-cbor::cn-cbor cose-c::cose-c)

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_PATH})
target_compile_options(${PROJECT_NAME}
        PRIVATE
        -Wall #-Wextra
       	-Wno-unused-function -pipe -pedantic
        -fstack-protector-all -fstack-protector-strong
        #-fsanitize=address -fsanitize=undefined
        -O3 -ffast-math -funroll-loops -march=native
)
