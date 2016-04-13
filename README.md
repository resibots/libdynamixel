libdynamixel
============

libdynamixel is a C++ interface to the dynamixel actuators. Currently, we have library code and an utility that uses it for a few tasks like changing the IDs, the baudrate and reading angles.

The work is in progress, the documentation too, please be clement.

Compilation and installation
============================
The build system for this library is Waf. Don't run away yet. It has the big advantage of requiring no more than Python installed on your computer. If you run a GNU/Linux OS or Mac OS, you should already have it. Here is how we compile and install it:

1. configuration  
  run `./waf configure` and add `--prefix PATH/TO/INSTALL` if you want to install it to a specific location
2. compilation  
  is as easy as `./waf`
3. installation  
  is simply done with `./waf install`, with the required rights (might need sudo if you install globally)

Using the utility
=================
Since we are right now writing a brand new utility, the user interface is not settled yet and some commands are not implemented. You can still play with the binary `dynamixel2` (the name will change) with the `--help [command]` option to learn how to use it.

Projects using this library
===========================

Internal projects
-----------------
- omnigrasper_bo_recovery
- [dynamixel_control_hw](https://github.com/resibots/dynamixel_control_hw/)

Other projects
--------------
We would be pleased to hear from you if you use this library in one of your projects.