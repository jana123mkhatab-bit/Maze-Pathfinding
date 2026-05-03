cmake_minimum_required(VERSION 3.16)

project(MAZEFINALLL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Add executable with ALL source files
add_executable(appMAZEFINALLL
    main.cpp
    MazeSolverWidget.cpp
    MazeWidget.cpp
    Maze.cpp
    MazeAlgorithms.cpp
    MyVector.h
    MyPriorityQueue.h
    MyVector.tpp
    MyPriorityQueue.tpp
)

# Link against Qt6 Widgets
target_link_libraries(appMAZEFINALLL
    PRIVATE Qt6::Widgets
)

# Set target properties
set_target_properties(appMAZEFINALLL PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
