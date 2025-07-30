cmake_minimum_required(VERSION 3.10)

add_executable(main main.cpp)

if (MSVC)
    # Enable buffer security checks (/GS is default in Release)
    target_compile_options(main PRIVATE /GS)
    message(STATUS "MSVC: Stack protection enabled (/GS)")
else()
    # For GCC or Clang: add stack protector flags
    target_compile_options(main PRIVATE -fstack-protector-strong)
    message(STATUS "GCC/Clang: Stack protection enabled (-fstack-protector-strong)")
endif()

# Executable WITHOUT stack protection
add_executable(main_unprotected main.cpp)
if (MSVC)
    # Disable buffer security check (/GS-) for MSVC
    target_compile_options(main_unprotected PRIVATE /GS-)
else()
    # Disable stack protector for GCC/Clang
    target_compile_options(main_unprotected PRIVATE -fno-stack-protector)
endif()