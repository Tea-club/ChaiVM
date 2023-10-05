set(DIRS googletest)

foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
