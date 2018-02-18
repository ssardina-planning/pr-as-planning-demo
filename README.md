## pr-as-planning-demo

Demo for Plan Recognition as Planning over Classical Action Theories

### Pre-requisites

The demo requires:

 1. The sources of the automated planning toolkit [LAPKT](https://github.com/LAPKT-dev/LAPKT-public), which you will 
 need to clone.
 2. The Qt4 development libraries.  Note that latest versions of Ubuntu come with Qt5 pre-installed. Getting Qt4 installed 'alongside' Qt5 is trivial in Ubuntu 16.04, all is needed is the following command
 ```
 $ sudo apt install libqt4-dev qt4-dev-tools
 ```


### Build procedure

#### Build via QMAKE

Some quick and dirty instructions to build this project:

1. Go into the folder lapkt-lib and edit the script SConstruct, changing the paths in lines 18-19 so they point to 
the location where you cloned LAPKT-public,
2. Issue the command 'scons', this will produce the binaries for the planning algorithms needed by the demo,
3. Go back into the root folder of the demo sources and edit line 10 of the  sim-home.pro file, changing the 
references to LAPKT-public as appropiate,
4. Produce the `Makefile` script via:
    * `qmake -o Makefile sim-home.pro` (if you have just Qt4 installed in your system)
    * `qmake-qt4 -o Makefile sim-home.pro` (if you have both Qt4 and Qt5 installed in your system)
5. Build the app with ```make```.
6. Run `./sim-house`


#### Build via CMAKE

1. Set variable `LAPKT_DIR` in `CMakeList.txt` to the source directory for LAPKT.
2. Get into a directory to build: `mkdir build ; cd build`
3. Run CMAKE to generate building files: `cmake ..`
4. Compile using `make`
5. Run sim-house: `cd .. ; ./build/sim-house`
    * Note working directory has to be the root directory (not `build/`).