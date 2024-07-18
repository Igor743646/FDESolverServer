
macro(START_BUILD)
    string(TIMESTAMP t "%H:%M:%S:%f")
    message(STATUS "[${t}] Start build ${PROJECT_NAME} in ${PROJECT_SOURCE_DIR}")
endmacro()

