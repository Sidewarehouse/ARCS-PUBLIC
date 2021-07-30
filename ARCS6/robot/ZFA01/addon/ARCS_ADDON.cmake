include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/c
)

set(
        ARCS_ADDON_files
        ${CMAKE_CURRENT_LIST_DIR}/AddonTemplate.cc
        ${CMAKE_CURRENT_LIST_DIR}/AddonTemplate.hh
        ${CMAKE_CURRENT_LIST_DIR}/c/AddonFunction.c
        ${CMAKE_CURRENT_LIST_DIR}/c/AddonFunction.h
)