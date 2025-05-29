# cmake_uninstall.cmake.in
if(NOT EXISTS "/home/meteoriw/CPP4_3DViewer_v2.0-1/src/build/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: /home/meteoriw/CPP4_3DViewer_v2.0-1/src/build/install_manifest.txt")
endif()

file(READ "/home/meteoriw/CPP4_3DViewer_v2.0-1/src/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach(file ${files})
    if(EXISTS "${file}")
        message(STATUS "Removing file: ${file}")
        exec_program(
            "/usr/bin/cmake" ARGS "-E remove \"${file}\""
            OUTPUT_VARIABLE rm_out
            RETURN_VALUE rm_retval
        )
        if(NOT "${rm_retval}" STREQUAL 0)
            message(FATAL_ERROR "Problem when removing ${file}")
        endif()
    else()
        message(STATUS "File does not exist: ${file}")
    endif()
endforeach()
