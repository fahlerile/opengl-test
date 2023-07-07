add_executable (${PROJECT_NAME} main.cpp "shaders.cpp"    )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib/include)
target_include_directories(${PROJECT_NAME} PUBLIC headers)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/GLEW-x64/glew32s.lib)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/lib-glfw3-vc2022/glfw3.lib)
target_link_libraries(${PROJECT_NAME} opengl32.lib)
