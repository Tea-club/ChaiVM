set(DIRS interpreter utils)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
