set(DIRS interpreter utils memory)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
