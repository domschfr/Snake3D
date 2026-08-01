set(SOURCES
    main.cpp
        Game.cpp
        Snake.cpp
)

add_executable(Snake3D ${SOURCES})

target_include_directories(Snake3D PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Snake3D PRIVATE
        OpenGL::GL
        SDL2::SDL2
        glm::glm
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)