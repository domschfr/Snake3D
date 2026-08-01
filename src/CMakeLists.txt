set(SOURCES
    main.cpp
        Game.cpp
)

add_executable(Snake3D ${SOURCES})

target_include_directories(Snake3D PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Snake3D PRIVATE
        OpenGL::GL
        SDL2::SDL2
        glm::glm
)