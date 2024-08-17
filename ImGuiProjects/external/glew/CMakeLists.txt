target_link_directories(GLEW PUBLIC
    "${PROJECT_SOURCE_DIR}/vendor/glew/lib/Release/Win32")

target_link_libraries(GLEW PUBLIC "glew32.lib")

target_include_directories(GLEW PUBLIC
    "${PROJECT_SOURCE_DIR}/vendor/glew/include")