######################################################################
# Automatically generated by qmake (2.01a) Fri May 11 13:24:04 2012
######################################################################

TEMPLATE = app
TARGET = sim-home
#CONFIG += debug
DEPENDPATH += . widgets model planning
QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += . widgets model planning /home/yiding/Documents/LAPKT-public/include /home/yiding/Documents/LAPKT-public/interfaces/agnostic
LIBS += -L./lapkt-lib -llapkt 
QT += xml

# Input
HEADERS += model/SimHomeConfig.hxx model/SimHome.hxx widgets/AppWindow.hxx model/Home.hxx model/Room.hxx widgets/HomeRenderer.hxx widgets/RoomView.hxx widgets/GraphicsFactory.hxx model/WalkTo.hxx widgets/GoalSetView.hxx planning/Observer.hxx planning/Goal.hxx widgets/GoalView.hxx widgets/NewGoalDialog.hxx planning/GoalRecognizer.hxx  model/OpenDoor.hxx model/CloseDoor.hxx model/Door.hxx model/Agent.hxx model/ItemManager.hxx model/Item.hxx model/Drop.hxx model/PickUp.hxx model/StageProp.hxx model/StagePropAction.hxx model/TurnOn.hxx model/TurnOff.hxx model/LieDown.hxx model/GetOff.hxx model/SitDown.hxx model/GetUp.hxx widgets/ObservationsView.hxx planning/PlanningTask.hxx
SOURCES += main.cxx model/SimHomeConfig.cxx model/SimHome.cxx widgets/AppWindow.cxx model/Home.cxx model/Room.cxx widgets/HomeRenderer.cxx widgets/RoomView.cxx widgets/GraphicsFactory.cxx model/WalkTo.cxx widgets/GoalSetView.cxx planning/Observer.cxx planning/Goal.cxx widgets/GoalView.cxx widgets/NewGoalDialog.cxx planning/GoalRecognizer.cxx model/OpenDoor.cxx model/CloseDoor.cxx model/Door.cxx model/Agent.cxx model/ItemManager.cxx model/Item.cxx model/Drop.cxx model/PickUp.cxx model/StageProp.cxx model/StagePropAction.cxx model/TurnOn.cxx model/TurnOff.cxx model/LieDown.cxx model/GetOff.cxx model/SitDown.cxx model/GetUp.cxx widgets/ObservationsView.cxx planning/PlanningTask.cxx
