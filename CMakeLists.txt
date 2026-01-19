# by MRB 
cmake_minimum_required(VERSION 3.25...9.9.9)
project(APP LANGUAGES C CXX)

include(cmake/system.cmake)
include(cmake/func.cmake)

include(cmake/musl.cmake)
include(cmake/musl_fsystem_flags.cmake) # Comment this to disable default system build flags
include(cmake/musl_flags.cmake)

include(cmake/in_out.cmake)
include(cmake/in_packages.cmake)

include(cmake/finally_flags.cmake)
