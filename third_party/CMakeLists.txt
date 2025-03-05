set(DIRS googletest A5)

foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

if (CHAI_BENCH)
    add_subdirectory(benchmark)
endif ()
