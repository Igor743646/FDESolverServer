
function(DEFINE_LIB_HEADERS TARGET)
    set(${TARGET}_HEADERS_DIR "${ARGN}" CACHE PATH "headers for ${TARGET}")
endfunction()
