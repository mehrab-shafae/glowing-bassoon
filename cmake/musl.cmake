_set_if_nonempty(MUSL_CROSS_ROOT "$ENV{MUSL_CROSS_ROOT}")

set(MUSL_CROSS_ROOT "/opt/raiden" CACHE PATH "path to musl cross root")
set(CMAKE_SYSROOT "${MUSL_CROSS_ROOT}/x86_64-unknown-linux-musl")

set(CMAKE_C_COMPILER   "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-gcc")
set(CMAKE_CXX_COMPILER "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-g++")
set(CMAKE_AR           "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-ar")
set(CMAKE_RANLIB       "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-ranlib")
set(CMAKE_NM           "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-nm")
set(CMAKE_OBJCOPY      "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-objcopy")
set(CMAKE_STRIP        "${MUSL_CROSS_ROOT}/bin/x86_64-unknown-linux-musl-strip")

set(CMAKE_FIND_ROOT_PATH "${MUSL_CROSS_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(_possible_pc_dirs
    "${MUSL_CROSS_ROOT}/lib/pkgconfig"
    "${MUSL_CROSS_ROOT}/lib64/pkgconfig"
    "${MUSL_CROSS_ROOT}/x86_64-unknown-linux-musl/lib/pkgconfig"
    "${MUSL_CROSS_ROOT}/x86_64-unknown-linux-musl/lib64/pkgconfig"
    "${MUSL_CROSS_ROOT}/usr/local/lib/pkgconfig"
    "${MUSL_CROSS_ROOT}/usr/local/share/pkgconfig"
    "${MUSL_CROSS_ROOT}/share/pkgconfig"
)
set(_pc_dirs "")

foreach(_d IN LISTS _possible_pc_dirs)
    if(EXISTS "${_d}")
        list(APPEND _pc_dirs "${_d}")
    endif()
endforeach()

if(_pc_dirs)
    list(JOIN _pc_dirs ":" _pc_env_string)
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${CMAKE_SYSROOT}")
    set(ENV{PKG_CONFIG_LIBDIR} "${_pc_env_string}")
endif()

_set_if_nonempty(CMUSL_CFLAGS "$ENV{CFLAGS}")
_set_if_nonempty(CMUSL_CXXFLAGS "$ENV{CXXFLAGS}")
_set_if_nonempty(CMUSL_LDFLAGS "$ENV{LDFLAGS}")
_set_if_nonempty(CMUSL_LTOFLAGS "$ENV{LTOFLAGS}")
