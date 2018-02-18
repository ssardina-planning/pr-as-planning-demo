# qt - How to convert qmake to cmake? - Stack Overflow http://tinyurl.com/ya3yw4s9
# CMake - Cross Platform Make http://tinyurl.com/yan99qgq
cmake_minimum_required(VERSION 3.5)
project(pr_as_planning_demo)

SET(LAPKT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extras/LAPKT-public)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
set(CMAKE_AUTOUIC ON)

add_definitions(-DLINTEL)
add_definitions(-DQT_MAJOR_VERSION=4)


# Find QT packages
find_package(Qt4 4.8.7 REQUIRED QtCore QtGui QtXml QtSvg QtNetwork)
INCLUDE(${QT_USE_FILE})


# INCLUDE .hxx
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${QT_INCLUDES}
        ${PROJECT_SOURCE_DIR}/widget/
        ${PROJECT_SOURCE_DIR}/model/
        ${PROJECT_SOURCE_DIR}/planning/
        ${LAPKT_DIR}/include
        ${LAPKT_DIR}/interfaces/agnostic
    )


set(HEADERS
        model/SimHomeConfig.hxx model/SimHome.hxx widgets/AppWindow.hxx model/Home.hxx model/Room.hxx widgets/HomeRenderer.hxx widgets/RoomView.hxx widgets/GraphicsFactory.hxx model/WalkTo.hxx widgets/GoalSetView.hxx planning/Observer.hxx planning/Goal.hxx widgets/GoalView.hxx widgets/NewGoalDialog.hxx planning/GoalRecognizer.hxx  model/OpenDoor.hxx model/CloseDoor.hxx model/Door.hxx model/Agent.hxx model/ItemManager.hxx model/Item.hxx model/Drop.hxx model/PickUp.hxx model/StageProp.hxx model/StagePropAction.hxx model/TurnOn.hxx model/TurnOff.hxx model/LieDown.hxx model/GetOff.hxx model/SitDown.hxx model/GetUp.hxx widgets/ObservationsView.hxx planning/PlanningTask.hxx
        )
set(SOURCES
        main.cxx model/SimHomeConfig.cxx model/SimHome.cxx widgets/AppWindow.cxx model/Home.cxx model/Room.cxx widgets/HomeRenderer.cxx widgets/RoomView.cxx widgets/GraphicsFactory.cxx model/WalkTo.cxx widgets/GoalSetView.cxx planning/Observer.cxx planning/Goal.cxx widgets/GoalView.cxx widgets/NewGoalDialog.cxx planning/GoalRecognizer.cxx model/OpenDoor.cxx model/CloseDoor.cxx model/Door.cxx model/Agent.cxx model/ItemManager.cxx model/Item.cxx model/Drop.cxx model/PickUp.cxx model/StageProp.cxx model/StagePropAction.cxx model/TurnOn.cxx model/TurnOff.cxx model/LieDown.cxx model/GetOff.cxx model/SitDown.cxx model/GetUp.cxx widgets/ObservationsView.cxx planning/PlanningTask.cxx
        )
#file(GLOB_RECURSE INC_ALL "model/*.cxx" "model/*.hxx" "widget/*.cxx" "widget/*.hxx")

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lapkt-lib/)


ADD_EXECUTABLE(sim-home ${SOURCES} ${HEADERS})
#add_executable(sim-home ${INC_ALL})


TARGET_LINK_LIBRARIES(sim-home ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(sim-home lapkt)





MESSAGE("===================================")
MESSAGE("QT_INCLUDES: " ${QT_INCLUDES})
MESSAGE("QT_LIBRARIES: " ${QT_LIBRARIES})
MESSAGE("PROJECT SRC DIR: "  ${PROJECT_SOURCE_DIR})
MESSAGE("CMAKE_CURRENT_SOURCE_DIR: "  ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("===================================")

