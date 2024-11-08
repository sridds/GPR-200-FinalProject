file(
 GLOB_RECURSE BRODYHELLOTRIANGLECOPY_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE BRODYHELLOTRIANGLECOPY_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this brodyHelloTriangleCopy's asset folder to bin when it is built
add_custom_target(copyAssetsBrody ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${BRODYHELLOTRIANGLECOPY_INC} DESTINATION include/assignment1)
add_executable(brodyHelloTriangleCopy ${BRODYHELLOTRIANGLECOPY_SRC} ${BRODYHELLOTRIANGLECOPY_INC})
target_link_libraries(brodyHelloTriangleCopy PUBLIC core IMGUI glm)
target_include_directories(brodyHelloTriangleCopy PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when brodyHelloTriangleCopy is built
add_dependencies(brodyHelloTriangleCopy copyAssetsBrody)