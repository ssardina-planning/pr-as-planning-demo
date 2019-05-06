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

