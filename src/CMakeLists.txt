if(MSVC)
    # Sets the compilation flags to report warnings through Level 4. We used to use "-Wall" until
    # the MSVC compiler started freaking out all the time about secure scanf, etc.
    set( CMAKE_CXX_FLAGS "-W4" )
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
else()
    # adds debug information for all other OSs
    add_definitions(-Wall -g -std=c++11)
endif()

file( GLOB SRCS *.cpp *.h )

# Define the target application executable (named prog01) and the list of CXX source
# and header files needed for the executable.

find_package(SDL2 REQUIRED)

include_directories (${SDL2_INCLUDE_DIRS})
add_executable (${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
