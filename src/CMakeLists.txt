set(SOURCES
    main.cpp
        Game.cpp
        Snake.cpp
        Shader.cpp
        Segment.cpp
)

add_executable(Snake3D ${SOURCES})

target_include_directories(Snake3D PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Snake3D PRIVATE
        OpenGL::GL
        SDL2::SDL2
        glm::glm
        assimp::assimp
)

target_include_directories(Snake3D PRIVATE ${CMAKE_SOURCE_DIR}/vendor/imgui)

target_sources(Snake3D PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)