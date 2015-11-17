libdynamixel
============
libdynamixel is a C++ interface to the `Dynamixel <http://en.robotis.com/index/product.php?cate_code=101010>`_ actuators. It aims at being fast and working with most Dynamixels. This includes the Dynamixel Pro actuators.

Currently, we have library code and a utility that uses it to offer a few functionalities like changing the IDs, the control mode (wheel or joint) and the baudrate.

Features
========
- Dynamixel series AX, MX, EX, XL and Pro are supported
- support for all instructions and messages of both version 1 and 2 of the Dynamixel communication protocol
- full C++ api

Building
========
Dependencies :

- python2.x
- C++
- Boost C++

.. highlight:: shell

The build system for this library is Waf. Don't run away yet. It has the big advantage of requiring no more than Python installed on your computer. If you run a GNU/Linux OS or Mac OS, you should already have it. Here is how we compile and install it:

1. configuration
  run ``./waf configure`` and add ``--prefix PATH/TO/INSTALL`` if you want to install it to a specific location
2. compilation
  is as easy as ``./waf``
3. installation
  is simply done with ``./waf install``, with the required rights (might need sudo if you install globally)
4. setup the proper authorisation
  please ensure that you have the proper rights to access the serial interfaces. On GNU/Linux, you might have to add your user to the `dialout` group and log out.

You get the ``dynamixel`` toolset and the demo programms compiled in the ``build/default/src`` folder.

For now, the demo programs are not at a working stage and should not be used. Conversely, ``dynamixel`` is a working program that we use regularly.

udev rule for USB2AX adapter
============================
For Gnu/Linux only.

If you want your USB2AX serial interface to appear in ``/dev`` as ``usb2axN`` (where N is a kernel-attributed integer), you can install the udev rule. It is as simple as moving the ``usb2ax.rules`` file in this repository to the folder for the udev rules. For ubuntu, it is ``/etc/udev/rules.d``.

Using the utility
=================
Since we are right now writing a brand new utility, the user interface is not settled yet and some commands are not implemented. You can still play with the binary `dynamixel2` (the name will change) with the `--help [command]` option to learn how to use it.

> **Caution:** the `get-speed` command gives you the reference speed used in the control table (if any) and not the current rotational speed of the actuator.

Further documentation
=====================
The documentation for libdynamixel is being written and should be published soon.

Support
=======
This library is actively used and regularly developped. If you need some help, feel free to `fill an issue <https://github.com/resibots/libdynamixel/issues/new>`_ on this repository. We'll do our best to be responsive.

Projects using this library
===========================

Internal projects
-----------------
- `dynamixel_control_hw<https://github.com/resibots/dynamixel_control_hw/>`_
- omnigrasper_bo_recovery

Other projects
--------------
We would be pleased to hear from you if you use this library in one of your projects.

Alternative software
====================

Inria FLOWERS team developped `pybot <http://poppy-project.github.io/poppy-docs/pypot/doc/index.html>`_, a python library to control a Dynamixel-based robot.

Let us know if you developped an other library for Dynamixels.

License and authors
===================
This work is distributed under the terms of the `CeCILL-C <http://www.cecill.info/licences.en.html>`_ license. © UPMC and INRIA.

The authors for this library are Jean-Baptiste Mouret, Antoine Cully, Federico Allocati and Dorian Goepp.