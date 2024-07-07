cmake_minimum_required(VERSION 3.28)
project(OOOHDLib VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 26)

# Add source files
set(SOURCES
        src/DatabaseObject.cpp
        src/Query.cpp
        src/QueryResult.cpp
        src/OOHD.cpp
        src/OOHDQuery.cpp
        src/oohd_c_api.cpp
)

# Add header files
set(HEADERS
        include/DatabaseObject.hpp
        include/Query.hpp
        include/QueryResult.hpp
        include/OOHDQuery.hpp
        include/OOHD.hpp
        include/OOHDList.hpp
        include/DynamicObject.hpp
        include/oohd_c_api.h
)

# Create the library
add_library(OOOHDLib SHARED ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(OOOHDLib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable warnings
if(MSVC)
    target_compile_options(OOOHDLib PRIVATE /W4)
else()
    target_compile_options(OOOHDLib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Export the library for use in other projects
install(TARGETS OOOHDLib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)