file(GLOB BASE_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB BASE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")



add_library(base STATIC ${BASE_SRC} ${BASE_HEADERS})

if(WIN32)
    target_link_libraries(base ${WINLIBS})
 else(WIN32)
    target_link_libraries(base ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)

target_link_libraries(base glad glfw imgui)