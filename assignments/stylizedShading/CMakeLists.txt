file(
 GLOB_RECURSE STYLIZEDSHADING_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE STYLIZEDSHADING_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this assignment6's asset folder to bin when it is built
add_custom_target(copyAssetsStylized ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${STYLIZEDSHADING_INC} DESTINATION include/stylizedShading)
add_executable(stylizedShading ${STYLIZEDSHADING_SRC} ${STYLIZEDSHADING_INC})
target_link_libraries(stylizedShading PUBLIC core IMGUI glm)
target_include_directories(stylizedShading PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when assignment6 is built
add_dependencies(stylizedShading copyAssetsStylized)