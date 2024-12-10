file(
 GLOB_RECURSE MAZEGENERATION_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE MAZEGENERATION_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this mazeGeneration's asset folder to bin when it is built
add_custom_target(copyAssetsMaze ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${MAZEGENERATION_INC} DESTINATION include/mazeGeneration)
add_executable(mazeGeneration ${MAZEGENERATION_SRC} ${MAZEGENERATION_INC})
target_link_libraries(mazeGeneration PUBLIC core IMGUI glm)
target_include_directories(mazeGeneration PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when mazeGeneration is built
add_dependencies(mazeGeneration copyAssetsMaze)