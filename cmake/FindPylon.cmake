set(PYLON_ROOT $ENV{PYLON_ROOT})
if (NOT "${PYLON_ROOT}")
	#default install folder
    #set(PYLON_ROOT "/opt/pylon5")
    
    #here you set the install folder if you have installed in non-default location
    set(PYLON_ROOT "/home/jelavice/Programs/pylon/pylon5")
endif()

set(_PYLON_CONFIG "${PYLON_ROOT}/bin/pylon-config")
if (EXISTS "${_PYLON_CONFIG}")
    set(Pylon_FOUND TRUE)
    execute_process(COMMAND ${_PYLON_CONFIG} --cflags-only-I OUTPUT_VARIABLE HEADERS_OUT)
    execute_process(COMMAND ${_PYLON_CONFIG} --libs-only-l OUTPUT_VARIABLE LIBS_OUT)
    execute_process(COMMAND ${_PYLON_CONFIG} --libs-only-L OUTPUT_VARIABLE LIBDIRS_OUT)
    string(REPLACE " " ";" HEADERS_OUT "${HEADERS_OUT}")
    string(REPLACE "-I" "" HEADERS_OUT "${HEADERS_OUT}")
    string(REPLACE "\n" "" Pylon_INCLUDE_DIRS "${HEADERS_OUT}")

    string(REPLACE " " ";" LIBS_OUT "${LIBS_OUT}")
    string(REPLACE "-l" "" LIBS_OUT "${LIBS_OUT}")
    string(REPLACE "\n" "" Pylon_LIBRARIES "${LIBS_OUT}")

    string(REPLACE " " ";" LIBDIRS_OUT "${LIBDIRS_OUT}")
    string(REPLACE "-L" "" LIBDIRS_OUT "${LIBDIRS_OUT}")
    string(REPLACE "\n" "" LIBDIRS_OUT "${LIBDIRS_OUT}")

    foreach (LIBDIR ${LIBDIRS_OUT})
        link_directories(${LIBDIR})
    endforeach()
else()
    set(Pylon_FOUND FALSE)
endif()