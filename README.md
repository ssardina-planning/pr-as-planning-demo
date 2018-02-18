## pr-as-planning-demo

Demo for Plan Recognition as Planning over Classical Action Theories

### Pre-requisites

The demo requires:

 1. The sources of the automated planning toolkit [LAPKT](https://github.com/LAPKT-dev/LAPKT-public), which you will need to clone.
    * A snapshot is already included in `extas/LAPKT-public`
 2. The Qt4 development libraries.  Note that latest versions of Ubuntu come with Qt5 pre-installed. Getting Qt4 installed 'alongside' Qt5 is trivial in Ubuntu 16.04, all is needed is the following command
 ```
 $ sudo apt install libqt4-dev qt4-dev-tools
 ```


### Build procedure

First of all, compile to produce the LAPKT library `lapkt-lib/liblapkt.a` by going into folder `lapkt-lib/` and issuing the command `scons`. This will use the LAPKT source snapshops in `../extras/LAPKT-public` and produce the binaries for the planning algorithms needed by the demo.

Second, we can compile the demo either via QMAKE or CMAKE.

#### Build via QMAKE

1. In the root folder of the project, produce the `Makefile` script via:
    * `qmake -o Makefile sim-home.pro` (if you have just Qt4 installed in your system)
    * `qmake-qt4 -o Makefile sim-home.pro` (if you have both Qt4 and Qt5 installed in your system)
2. Build the app with ```make```.
3. Binary `./sim-house` will be created and all temp build files will remain in `.build/`


#### Build via CMAKE

1. Get into a directory to build: `mkdir build ; cd build`
2. Run CMAKE to generate building files: `cmake ..`
3. Compile using `make`
4. Run sim-house: `cd .. ; ./build/sim-house`
    * Note working directory has to be the root directory (not `build/`).
    
    
  ![screenshot](extras/screenshot02.png)  
