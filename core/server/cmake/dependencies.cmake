
list(APPEND CONAN_DEPENDENCIES antlr4-runtime protobuf gRPC absl Boost Catch2)

foreach(package_name IN LISTS CONAN_DEPENDENCIES)
    message(CHECK_START "Search ${package_name}")
    set(${package_name}_FIND_QUIETLY ON)
    find_package(${package_name} REQUIRED CONFIG)
    if (${package_name}_FOUND)
        message(CHECK_PASS "Found ${package_name}")
    else()
        message(CHECK_FAIL "No package ${package_name}")
    endif()
endforeach()
