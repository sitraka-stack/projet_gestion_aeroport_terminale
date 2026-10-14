cmake_minimum_required(VERSION 3.16)
project(AirportSimulationConsole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activer l'exportation pour l'autocomplétion VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ajouter les fichiers sources de ton projet
add_executable(AirportSimulation
    src/main.cpp
    src/Avion.h
    src/Avion.cpp
    src/Aeroport.h
    src/Aeroport.cpp
)