cmake_minimum_required(VERSION 3.10)

project(dep_and_aslr)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fstack-protector-strong")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Main executables
add_executable(main main.cpp)
add_executable(main_protected main.cpp)
target_compile_definitions(main_protected PRIVATE PROTECTED_BUILD=1)

# Platform-specific variants
if(UNIX)
    add_executable(main_execstack main.cpp)
    target_link_options(main_execstack PRIVATE "-z" "execstack")
    target_compile_definitions(main_execstack PRIVATE EXECSTACK_BUILD=1)
    
    add_executable(main_noprotect main.cpp)
    target_compile_options(main_noprotect PRIVATE "-fno-stack-protector")
    target_compile_definitions(main_noprotect PRIVATE NO_STACK_PROTECTOR=1)
elseif(WIN32 AND MSVC)
    target_link_options(main PRIVATE "/NXCOMPAT" "/DYNAMICBASE")
    target_link_options(main_protected PRIVATE "/NXCOMPAT" "/DYNAMICBASE")
    
    add_executable(main_win_noprotect main.cpp)
    target_link_options(main_win_noprotect PRIVATE "/NXCOMPAT:NO" "/DYNAMICBASE:NO")
    target_compile_definitions(main_win_noprotect PRIVATE WIN_NO_PROTECT=1)
endif()

# Custom targets
add_custom_target(_dep
    COMMAND main
    DEPENDS main
)

if (WIN32)
    set(EXIT VER>NUL)
else()
    set(EXIT true)
endif()



add_custom_target(test_aslr
    COMMAND echo "n" | main || EXIT
    COMMAND echo "n" | main || EXIT  
    DEPENDS main
)