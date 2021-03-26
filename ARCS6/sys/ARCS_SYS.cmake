include_directories(${CMAKE_CURRENT_LIST_DIR})

set(
        ARCS_SYSS_files
        ${CMAKE_CURRENT_LIST_DIR}/ARCSassert.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCSassert.hh
        ${CMAKE_CURRENT_LIST_DIR}/ARCScommon.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCScommon.hh
        ${CMAKE_CURRENT_LIST_DIR}/ARCSeventlog.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCSeventlog.hh
        ${CMAKE_CURRENT_LIST_DIR}/ARCSprint.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCSprint.hh
        ${CMAKE_CURRENT_LIST_DIR}/ARCSscreen.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCSscreen.hh
        ${CMAKE_CURRENT_LIST_DIR}/ARCSthread.cc
        ${CMAKE_CURRENT_LIST_DIR}/ARCSthread.hh
)

set(
        ARCS_CC_file
        ${CMAKE_CURRENT_LIST_DIR}/ARCS.cc
)