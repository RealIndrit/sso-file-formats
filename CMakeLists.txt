cmake_minimum_required(VERSION 3.29)
project(sso_file_format)

set(CMAKE_C_STANDARD 11)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(_CCX SHARED ccx/CCX.c)
add_library(_CSA_HEADER SHARED csaheader/CSAHEADER.c)
