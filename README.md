# clion-cmake-test
It is used to demostrate how to develop c++ project using clion&cmake for to build project and normally easily test or check open source code at in this template project

## Build on Ubuntu/MacOs
### Check the required tools
Make sure your computer have installed the required packages below:
* [git](https://www.git-scm.com/downloads)
* [cmake](https://cmake.org/download)
* [wget](https://www.gnu.org/software/wget)

### Clone source code
Open a terminal, go to `/home/xxx/dev`
```shell
$ cd /home/xxx/dev/
$ git clone git@github.com:woodzcl-zhangcl/clion-cmake-test.git
$ cd clion-cmake-test
```

### Build clion-cmake-test

Create a build directory `cmake-build-release`
```shell
$ cd /home/xxx/dev/clion-cmake-test
$ mkdir cmake-build-release
```

Execute cmake command to generate Makefile, and make
```shell
$ cd cmake-build-release
$ cmake ..
$ make
```

## Build for Android
### Check the required tools
Make sure your computer have installed the required packages below:
* [git](https://www.git-scm.com/downloads)
* [cmake](https://cmake.org/download)
* [wget](https://www.gnu.org/software/wget)
* [ndk](https://developer.android.com/ndk/downloads/)

### Install ndk
`NDK` version: r16+

Unzip to somewhere, for example `/Users/xxx/dev/android-ndk-r16`

Set system environment variable `ANDROID_NDK` to `/Users/xxx/dev/android-ndk-r16`

### Clone source code
Open terminal, go to `/Users/xxx/dev`
```shell
$ cd /Users/xxx/dev/
$ git clone git@github.com:woodzcl-zhangcl/clion-cmake-test.git
$ cd clion-cmake-test
```

### Build clion-cmake-test

Create a build directory `cmake-build-ndk-release`
```shell
$ cd /Users/xxx/dev/clion-cmake-test
$ mkdir cmake-build-ndk-release
```

Execute cmake command to generate Makefile, and make
```shell
$ cd cmake-build-ndk-release
$ cmake -DFOR_ANDROID=ON ..
$ make
```
