set(TARGET_NAME "glad")

file(GLOB_RECURSE ALL_HEADER_FILES *.h)
file(GLOB_RECURSE ALL_SOURCE_FILES *.c)
set(ALL_CODE_FILES ${ALL_HEADER_FILES} ${ALL_SOURCE_FILES})

GROUP_FILES(ALL_CODE_FILES)

add_library(${TARGET_NAME} STATIC ${ALL_CODE_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# append the _d (or whatever) for debug builds as needed.
#if(HAS_BUILD_SUFFIX AND BUILD_SUFFIX)
#    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME_DEBUG "${TARGET_NAME}${BUILD_SUFFIX}")
#endif()
