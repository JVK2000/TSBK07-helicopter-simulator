# TSBK07 Project

The project is a third-person helicopter simulator created with [`MicroGLUT`](https://computer-graphics.se/packages/microglut.html). The simulator allows the user to navigate a helicopter through a terrain with mountains generated using the `Perlin` noise function from the `libnoise` library. Additionally, a skybox has been implemented that transitions from day to night, providing a sense of time progression.

## Prerequisites

Before running the `TSBK07 Project`, you need to install the `libnoise` package.

### Installation

To install `libnoise`, follow these steps:

#### Linux (not tested)

1. Install libnoise by running the following commands in your terminal: 
```shell
$ sudo apt-get install libnoise-dev
```
2. Follow the on-screen prompts to complete the installation.


#### Mac ARM
1. Install Homebrew by running the following command in your terminal:
```shell
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Install CMake by running the following command in your terminal:
```shell
$ brew install cmake
```

3. Install libnoise by running the following commands in your terminal:
```shell
$ git clone https://github.com/qknight/libnoise.git
$ cd Iibnoise
$ mkdir build cd build
$ cmake
$ make
$ sudo make install
```

### Run
To run the simulator, open your terminal and navigate to the project directory. Then, run the following commands:
```shell
make
./project
```

##Screencast
Please see the file  `Screencast.mp4` for a video demonstration of the simulator in action.