 cmake_minimum_required(VERSION 3.9)
 project(cgen)
 
 add_executable(cgen src/main.cpp)

target_compile_features(cgen
        PRIVATE
        cxx_std_17
)

# add_definitions("-std=c++17 -lstdc++fs")

 target_link_libraries(cgen stdc++fs)

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 target_link_libraries(cgen ${CONAN_LIBS})
