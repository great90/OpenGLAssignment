set(TARGET_NAME "OpenGL")

file(GLOB_RECURSE ALL_HEADER_FILES *.h)
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp)
file(GLOB_RECURSE ALL_SHADER_FILES shader/*)

set(ALL_CODE_FILES ${ALL_HEADER_FILES} ${ALL_SOURCE_FILES} ${ALL_SHADER_FILES})

GROUP_FILES(ALL_CODE_FILES)

add_executable(${TARGET_NAME} ${ALL_CODE_FILES})

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")

# append the _d (or whatever) for debug builds as needed.
if(HAS_BUILD_SUFFIX AND BUILD_SUFFIX)
    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${TARGET_NAME}${BUILD_SUFFIX}")
endif()

target_link_libraries(${TARGET_NAME} opengl32 glad glfw)

if (MSVC)
    if (NOT ${CMAKE_VERSION} VERSION_LESS "3.6.0")
        set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
    endif ()
endif (MSVC)
