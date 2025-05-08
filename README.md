# MachineWallet-RobotsDemo
This is a BoAT-based demo showing a house keeping robot requests A/C service from another robot by paying crypto on blockchain.

Two Fibocom L718 development boards play the role of the two robots. One board acts as a house keeping robot, with a themometer connected on its serial port. The other board acts as an A/C service providing robot that controls a cooling fan. When the ambient temperature rises to higher than 40℃, the house keeping robot will pay one cent (e.g., USDC) to the A/C service provider robot. The A/C robot will turn on the fan for a minute and re-distribute part of the 1 cent revenue to two investors.

Both robots send the transfer transaction with the [BoAT machine wallet SDK](https://github.com/boat-x/BoAT-ProjectTemplate). BoAT is designed dedicated for resource constrains machine, fostering the up-coming decentralized massive machine economy based on such on-chain protocols: requesting service → paying fees → receiving service → revenue sharing, as shown in the demo.

# Preparation

1. Fibocom L718 development boards and SDK are required. The development environment should be linux.
Contact Fibocom for the resources.

2. OWON's Smart Plug (model: WSP-403) is required for the A/C robot to control the cooling fan.

3. Clone this project:

    `git clone https://github.com/boat-x/MachineWallet-RobotsDemo.git` 

4. Copy the entire cloned project to L718 develpment SDK's user app directory:

    `cp -r MachineWallet-RobotsDemo build_env_mini/app/app`

5. Modify **build_env_mini\app\app\Makefile** to configure the toolchain:
```
include ../../global_opt.mak
export GCC_PATH = $(shell pwd)/../../build/usr/bin
export MIN_CC 			= $(GCC_PATH)/arm-linux-gcc
export MIN_LD 			= $(GCC_PATH)/arm-linux-gcc
export MIN_AS       	= $(GCC_PATH)/arm-linux-as $(CPUFLAGS)
export MIN_CXX       	= $(GCC_PATH)/arm-linux-g++ $(CPUFLAGS)
export MIN_AR        	= $(GCC_PATH)/arm-linux-ar
export MIN_OBJCOPY   	= $(GCC_PATH)/arm-linux-objcopy
export MIN_RANLIB    	= $(GCC_PATH)/arm-linux-ranlib
export MIN_STRIPTOOL 	= $(GCC_PATH)/arm-linux-strip
export MIN_STRIP     	= $(GCC_PATH)

INCLUDE += -I../fiboframework/framework/c/inc
INCLUDE += -I../fiboframework/interfaces/atServices
INCLUDE += -I../fiboframework/interfaces/audio
INCLUDE += -I../fiboframework/interfaces/modemServices

LDLIBS +=



.PHONY: all
all: 
	make -C ./MachineWallet-RobotsDemo all


clean:
	make -C ./MachineWallet-RobotsDemo clean
```

6. Configure credentials
Follow comments in **build_env_mini/app/app/MachineWallet-RobotsDemo/ac_robot/ac_robot.c** and **build_env_mini/app/app/MachineWallet-RobotsDemo/housekeeping_robot/housekeeping_robot.c** to create `credentials.key` with your keys.


7. Build
Change directory to **build_env_mini/app/app** and build:
```
cd build_env_mini/app/app
make all
```

The built executables will be placed at:
- The House Keeping Robot: build_env_mini/app/app/MachineWallet-RobotsDemo/build/housekeeping_robot
- The A/C Robot: build_env_mini/app/app/MachineWallet-RobotsDemo/build/ac_robot

To clean:
`make clean`
