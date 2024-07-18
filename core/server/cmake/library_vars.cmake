
function(DEFINE_LIB_HEADERS TARGET)
    set(${TARGET}_HEADERS_DIR "${ARGN}" CACHE PATH "headers for ${TARGET}")
    message(STATUS "Define headers of target ${TARGET}: ${${TARGET}_HEADERS_DIR}")
endfunction()
