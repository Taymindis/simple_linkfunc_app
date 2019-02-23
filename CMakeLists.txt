cmake_minimum_required(VERSION 3.3.2)
if (APPLE)
  cmake_policy(SET CMP0042 NEW)
endif()

set(project_name "mySampleApp")

project(${project_name})

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`
file(GLOB_RECURSE sources src/main/*.c src/main/*.cpp src/main/*.h src/main/*.hpp)
file(GLOB_RECURSE sources_test src/test/*.cpp src/main/*.h src/main/*.hpp)
file(GLOB_RECURSE data resources/*)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

link_directories(/usr/local/lib /usr/lib)

###############################################################################
## target definitions #########################################################
###############################################################################

# add the data to the target, so it becomes visible in some IDE
add_library(${project_name} SHARED ${sources} ${data})
set_property(TARGET ${project_name} PROPERTY POSITION_INDEPENDENT_CODE ON)

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif (APPLE)   

if(NOT ${CMAKE_VERSION} LESS 3.2)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Checking compiler flags for C++11 support.")
    # Set C++11 support flags for various compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++11 is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "C++0x is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        endif()
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

#add_definitions(" -g")
# just for ${project_name} add some compiler flags
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_compile_options(${project_name} PUBLIC)
if(${CMAKE_VERSION} VERSION_LESS "3.13.0") 
  target_link_libraries(${project_name} PUBLIC -Wl,-undefined,dynamic_lookup)
else()  
  target_link_options(${project_name} PUBLIC -Wl,-undefined,dynamic_lookup)
endif()
else()
  target_compile_options(${project_name} PUBLIC)
if(${CMAKE_VERSION} VERSION_LESS "3.13.0") 
  target_link_libraries(${project_name} PUBLIC -rdynamic)
else()    
  target_link_options(${project_name} PUBLIC -rdynamic)
endif()
endif()

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(${project_name} PUBLIC src src/main)

# this copies all resource files in the build directory
# we need this, because we want to work with paths relative to the executable
file(COPY ${data} DESTINATION ${PROJECT_BINARY_DIR})
# this defines the variables Boost_LIBRARIES that contain all library names
# that we need to link to
# find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)
SET(LIBPATH /usr/lib /usr/local/lib)
include_directories(src/main src/nlohmann_json /usr/local/include /usr/include)


###############################################################################
## packaging ##################################################################
###############################################################################

# all install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS ${project_name} DESTINATION installed_destination)
# this is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it
# in the package
install(DIRECTORY resources DESTINATION installed_destination)

# now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# you need to set for some package types, but we want to
# be minimal here
set(CPACK_PACKAGE_NAME ${project_name})
set(CPACK_PACKAGE_VERSION "1.0.0")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
