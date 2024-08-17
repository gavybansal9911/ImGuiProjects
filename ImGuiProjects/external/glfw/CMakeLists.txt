target_link_directories(GLFW PUBLIC
    "${PROJECT_SOURCE_DIR}/vendor/glfw/lib-vc2022/")

target_link_libraries(GLFW PUBLIC "glfw3.lib")

target_include_directories(GLFW PUBLIC
    "${PROJECT_SOURCE_DIR}/vendor/glfw/include/")