cmake_minimum_required(VERSION 3.14)
project(arduino-core VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch dependencies
include(FetchContent)

# Fetch arduinolib4
FetchContent_Declare(
    arduinolib4
    GIT_REPOSITORY https://github.com/compilerNayan/arduinolib4.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(arduinolib4)

# Create header-only library (INTERFACE library)
add_library(arduino-core INTERFACE)

# Set target properties
set_target_properties(arduino-core PROPERTIES
    VERSION ${PROJECT_VERSION}
)

# Link to arduinolib4
# Try the namespaced target first, fallback to non-namespaced
if(TARGET arduinolib4::arduinolib4)
    target_link_libraries(arduino-core INTERFACE arduinolib4::arduinolib4)
elseif(TARGET arduinolib4)
    target_link_libraries(arduino-core INTERFACE arduinolib4)
else()
    # Fallback: manually add include directory if target doesn't exist
    FetchContent_GetProperties(arduinolib4)
    if(arduinolib4_SOURCE_DIR)
        target_include_directories(arduino-core INTERFACE
            $<BUILD_INTERFACE:${arduinolib4_SOURCE_DIR}/src>
        )
    endif()
endif()

# Note: For INTERFACE libraries, we don't need to add header files to target_sources
# The include directories are sufficient for consumers to find the headers

