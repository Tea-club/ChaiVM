set(DIRS googletest benchmark)

foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
