cmake_minimum_required(VERSION 3.29)
project(pacman)

set(CMAKE_CXX_STANDARD 20)

add_executable(pacman main.cpp
        Cubo.cpp
        Cubo.h
        Escene.cpp
        Escene.h
        Utils.cpp
        Utils.h
        pm_maps.cpp
        pm_maps.h
)

# Añadir las rutas de las cabeceras de freeglut
include_directories(/usr/local/opt/freeglut/include)

# Añadir las rutas de las bibliotecas de freeglut
link_directories(/usr/local/opt/freeglut/lib)

# Encontrar las bibliotecas de OpenGL y GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


# Enlazar las bibliotecas de OpenGL y GLUT
target_link_libraries(pacman OpenGL::GL GLUT::GLUT)