cmake_minimum_required(VERSION 3.25...9.9.9)
project(APP LANGUAGES C CXX)

include(cmake/system.cmake)
include(cmake/func.cmake)

include(cmake/musl.cmake)
include(cmake/musl_fsystem_flags.cmake)
include(cmake/musl_flags.cmake)

include(cmake/out.cmake)
include(cmake/packages.cmake)

include(cmake/pre.cmake)
