# BoAT Integration Guide for Fibocom L610


## 1. Introduction

This document explains how to use the `BoAT Infra Arch` infrastructure within the `Fibocom L610` development environment to compile the related BoAT static libraries and generate blockchain DEMO applications.

The static libraries used in this example from the `BoAT Infra Arch` infrastructure include:

- `BoAT-SupportLayer`: Provides an abstraction of platform operating system calls and driver application interfaces for applications and middleware based on BoAT Infra Arch.
- `BoAT-Engine`: Provides blockchain access application interfaces for applications based on BoAT Infra Arch.

The SDK version for `Fibocom L610` is `core_sdk_16000.1000.00.96.64.04`.

## 2. Code Cloning and Configuration

Assume `<L610 Root>` is the root directory of the `Fibocom L610` platform SDK.

Taking `Ethereum` as an example:

1. Use the `git clone` command to download the `BoAT-ProjectTemplate` repository into the `<L610 Root>` directory:

  ```
  git clone -b main git@github.com:boat-x/BoAT-ProjectTemplate.git
  ```
  or
  ```
  git clone -b main https://github.com/boat-x/BoAT-ProjectTemplate.git
  ```

2. Navigate to the `<L610 Root>/BoAT-ProjectTemplate/` directory and modify the `BoATLibs.conf` file and replace the contents of `BoATLibs.conf` with the following:

```
BoAT-SupportLayer
BoAT-Engine
```


3、Run the configuration script in the `<L610 Root>/BoAT-ProjectTemplate/` directory:

```
python3 config.py
```
Follow the prompts to configure dependencies:

```  
We will clone the BoAT-SupportLayer repository, which may take several minutes

Input the branch name or null:
```
Type ‘main’ to select main branch of BoAT-SupporLayer repository.
``` 
Input the branch name or null:main
branch name is [ -b main]

git clone -b dev git@github.com:boat-x/BoAT-SupportLayer.git

Cloning into 'BoAT-SupportLayer'...
remote: Enumerating objects: 2930, done.
remote: Counting objects: 100% (704/704), done.
remote: Compressing objects: 100% (327/327), done.
remote: Total 2930 (delta 441), reused 589 (delta 362), pack-reused 2226
Receiving objects: 100% (2930/2930), 3.40 MiB | 21.00 KiB/s, done.
Resolving deltas: 100% (1826/1826), done.
git cmd succ


We will clone the BoAT-Engine repository, which may take several minutes

Input the branch name or null:
```
Type ‘main’ to select main branch of BoAT-Engine repository.

```
Input the branch name or null:main
branch name is [ -b main]

git clone -b dev git@github.com:boat-x/BoAT-Engine.git

Cloning into 'BoAT-Engine'...
remote: Enumerating objects: 900, done.
remote: Counting objects: 100% (39/39), done.
remote: Compressing objects: 100% (27/27), done.
remote: Total 900 (delta 18), reused 22 (delta 12), pack-reused 861
Receiving objects: 100% (900/900), 527.23 KiB | 37.00 KiB/s, done.
Resolving deltas: 100% (567/567), done.
git cmd succ


overwrite the Makefile?(Y/n):
```
Type 'Y' to generate Makefile
```
Yes

 Select blockchain list as below:
 [1] ETHEREUM          : 
 [2] PLATON            : 
 [a] QUORUM            : 
 [0] All block chains
 Example:
  Select blockchain list as below:
  input:1a
  Blockchain selected:
   [1] ETHEREUM
   [a] QUORUM

input:
```
Type '1' to select ETHEREUM
```
input:1
Blockchain selected:
 [1] ETHEREUM

Select the platform list as below:
[1] linux-default             : Default linux platform
[2] Fibocom-L610              : Fibocom's LTE Cat.1 module
[3] create a new platform
```
Type '2' to select Fibocom-L610
```
2
 
platform is : Fibocom-L610

include BoAT-SupportLayer.conf

include BoAT-Engine.conf


./BoAT-SupportLayer/demo/ False
./BoAT-Engine/demo/ True
Configuration completed
```
Configuration completes with the following directory structure:
```
<L610 Root>
|
`-- BoAT-ProjectTemplate
      |-- BoAT-SupportLayer
      |-- BoAT-Engine
      |-- BoATLibs.conf
      |-- config.py
      |-- Makfile
      |-- README.md
```

4. Create a `demo.c` source file in the `<L610 Root>` directory to implement the blockchain application.

Reference `<L610 Root>/BoAT-ProjectTemplate/BoAT-Engine/demo/demo_ethereum` for implementation details.

The updated directory structure is like:

```
<L610 Root>
|
`-- BoAT-ProjectTemplate
      |-- BoAT-SupportLayer
      |-- BoAT-Engine
      |-- BoATLibs.conf
      |-- config.py
      |-- Makfile
      |-- README.md
      |-- README_en.md
`-- cmake
      |-- toolchain-gcc.cmake
|-- CMakeLists.txt
|-- demo.c
```

## 3. File Modifications

1. Modify `<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/external.env` to configure the cross-compiler:

  ```
  CC := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
  AR := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-ar
  ```

2. Disable SSL in httpclient.c

Edit `<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal/http/httpclient.c` and comment out or remove the SSL enable macro:：
```
//#define BOAT_HTTPCLIENT_SSL_ENABLE
```
3. Add Paths for Demo Source Files, Header Files, and BoAT Library Links

  3.1 Modify `<L610 Root>/CMakeLists.txt`:

  Add demo source and header file paths in `include_directories`:
  ```
include_directories(components/include 
                    components/newlib/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/include        
                    BoAT-ProjectTemplate/BoAT-SupportLayer/tests
                    BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/log
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/osal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/third-party/cJSON
                    BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
                    BoAT-ProjectTemplate/BoAT-Engine/include
                    BoAT-ProjectTemplate/
                   )

  ```
Add the required source files and link libraries under `if(CONFIG_APPIMG_LOAD_FLASH)`:

  ```
      add_appimg(${target} ${flash_ldscript}
          demo.c)
      target_link_libraries(${target} PRIVATE ${libboatengine_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name} )
  ```
Note that the order of the libraries is sensitive. libboatengine must appear before libboatvendor. Otherwise, linking may fail.
The exact library file name is defined in  `<L610 Root>/cmake/toolchain-gcc.cmake` .

   3.2 Modify `<L610 Root>//cmake/toolchain-gcc.cmake` to add BoAT static library paths under `if(CONFIG_CPU_ARM_CA5)`:

  ```
  if(CONFIG_CPU_ARM_CA5)
      set(abi_options -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4
          -mfloat-abi=hard -mno-unaligned-access)
      set(partial_link_options)
      if(L610_CN_02_90_ALIEVK)
      message(STATUS "BUILD L610_CN_02_90_ALIEVK")
      set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/alievk/libc.a)
      else()
      set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libc.a)
      endif()
      set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libm.a)
      set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a)
      set(libboatengine_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a)
  endif()
  ```


## 4. Compiling BoAT Static Libraries and Firmware

### 1. Compile BoAT Static Libraries on Linux


   Navigate to the `BoAT-ProjectTemplate` directory and compile the BoAT static libraries:
   ```
   cd <L610 Root>/BoAT-ProjectTemplate
   make clean
   make all
   ```

   This generates two static libraries in `<L610 Root>/BoAT-ProjectTemplate/lib`:
   ```
   libboatvendor.a
   libboatengine.a
   ```

### 2. Compile L610 Firmware

   Navigate to `<L610 Root>` and compile the firmware:
   ```
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
  The compiled firmware `hello_flash.pac` is generated in `<L610 Root>/out/appimage_debug/hex`.

## 5. Run the `demo`

Download `hello_flash.pac` to the Fibocom L610 module and restart the module to run the demo. Use the module's debugging tools to check execution logs. For details on module code downloading and debugging, refer to the L610 platform operation guide.

