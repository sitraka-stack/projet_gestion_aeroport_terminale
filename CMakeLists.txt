cmake_minimum_required(VERSION 3.16)
project(AirportSimulationTUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(AirportSimulation
    src/main.cpp
    src/Avion.h
    src/Avion.cpp
    src/Aeroport.h
    src/Aeroport.cpp
)

target_link_libraries(AirportSimulation PRIVATE ftxui::screen ftxui::dom ftxui::component)