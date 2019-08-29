## pr-as-planning-demo

Demo for Plan Recognition as Planning over Classical Action Theories

### Pre-requisites

The demo requires:

 1. The sources of the automated planning toolkit [https://github.com/LAPKT-dev/LAPKT-public](LAPKT), which you will need to clone.
 2. The Qt4 development libraries.  Note that latest versions of Ubuntu come with Qt5 pre-installed. Getting Qt4 installed 'alongside' Qt5 is trivial in Ubuntu 16.04, all is needed is the following command
 ```
 $ sudo apt install libqt4-dev qt4-dev-tools
 ```


### Build procedure

Some quick and dirty instructions to build this project:

 1. go into the folder lapkt-lib and edit the script SConstruct, changing the paths in lines 18-19 so they point to the location where you cloned LAPKT-public,

 2. issue the command 'scons', this will produce the binaries for the planning algorithms needed by the demo,

 3. go back into the root folder of the demo sources and edit line 10 of the  sim-home.pro file, changing the references to LAPKT-public as appropiate,

 4. issue the command ```qmake``` (or ```qmake-qt4``` if you have *both* Qt4 and Qt5 installed in your system). This will produce a working Makefile script,

 5. build the app with ```make```.

### Sending Action from File

Got to file in folder

```
thor/observations.log
```

and add an action, save it, and if the simhome application is running, and the action is applicable, then it will change the main view. If you want to see the observation, first click in "start observing file menu option".

If you want to send another action, remove the action in observations.log write the new action, save the file, and it will be read automatically again.

In the file

```
thor/available_actions.log
```

you can see the actions available once you load a home.xml file.


### Thor simulation

The Thor 3D home simulation include three parts:
1. Sim-Home application
2. Thor 3D home Unity file
3. Python keyboard controller

The python controller will be used to control the agent working around the 3D Home and reports back the monitored action(through thor/observations.log) to the Sim-Home applicaton.


#### Sim-Home Application
Steps to run Sim-Home:
1. ./sim-home to open the application
2. load the 2D home file called event_demo.xml
3. Add some goals you want to track, you can find the example goals at thor/Goals.txt
4. Click start Observation 

#### Thor 3D home Unity file

This file contain the 3D unity home, please place this file under the folder "/thor". The file size is about 500MB and it's not include in the repository.

#### Python keyboard controller

1. Install the python3 library

* pip3 install pynput
* pip3 install ai2thor
* pip3 install inquirer
* pip3 install Shapely

2. Run the keyboard controller 
python3 keyboard.py

3. The unity screen should come out

4. Controll the agent through the following command:

* WASD for moving around
* t for turn on/off the objects like TV, laptop, lightswitch
* p for pick up the objects like pan, book, credit card
* o for open and close the objects like door, fridge door
* m for move hand forward
* Up and Down arrow to move head up and down
* Left and right arrow to rotate left and right







