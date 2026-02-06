# musl find system flags
if(NOT DEFINED CMUSL_CFLAGS OR CMUSL_CFLAGS STREQUAL "")
    if(EXISTS "/etc/makepkg.conf")
        execute_process(COMMAND bash -c "source /etc/makepkg.conf >/dev/null 2>&1 && echo \"\$CFLAGS\""
                        OUTPUT_VARIABLE _mpk_cflags OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        execute_process(COMMAND bash -c "source /etc/makepkg.conf >/dev/null 2>&1 && echo \"\$CXXFLAGS\""
                        OUTPUT_VARIABLE _mpk_cxxflags OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        execute_process(COMMAND bash -c "source /etc/makepkg.conf >/dev/null 2>&1 && echo \"\$LDFLAGS\""
                        OUTPUT_VARIABLE _mpk_ldflags OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        execute_process(COMMAND bash -c "source /etc/makepkg.conf >/dev/null 2>&1 && echo \"\$LTOFLAGS\""
                        OUTPUT_VARIABLE _mpk_ltoflags OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        _set_if_nonempty(CMUSL_CFLAGS "${_mpk_cflags}")
        _set_if_nonempty(CMUSL_CXXFLAGS "${_mpk_cxxflags}")
        _set_if_nonempty(CMUSL_LDFLAGS "${_mpk_ldflags}")
        _set_if_nonempty(CMUSL_LTOFLAGS "${_mpk_ltoflags}")
    else()
        message(WARNING "warn: '/etc/makepkg.conf' not found!")
    endif()
endif()
