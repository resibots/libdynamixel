libdynamixel
============
libdynamixel is a C++11, header-only interface to the `Dynamixel <http://en.robotis.com/index/product.php?cate_code=101010>`_ actuators that aims at being lightweight and highly flexible. This library is NOT the `official dynamixel SDK provided by Robotis <TODO>`_, but it provides a similar service.

This library is for Unix-like systems (GNU/Linux and Mac OSX)

We also provide a simple command-line tool that allows to change the IDs, the baudrate, the control mode, the position, etc.

Main features
===============
* Support for Dynamixel series AX, MX, EX, XL and Pro
* Support for all instructions and messages of both version 1 and 2 of the Dynamixel communication protocol (including torque control, speed control, etc.)
* Full C++11 API

Documentation
==============
The full documentation (including tutorials, guides, and API) is available on the `ResiBots website <http://www.resibots.eu/libdynamixel>`_


Projects using this library
===========================
This library is used in most of the robots designed in ERC project `ResiBots <http://www.resibots.eu/photos.html>`_ (funded by the European Research Council), in particular :
* several 18-DOFs hexapod robots (based on MX-28 actuators);
* a mobile manipulator based on Dynamixel Pro actuators and the Kuka Youbot (omnidirectionnal platform);
* a wheel-legged hybrid robot (based on MX-128 actuators);
* several small arms (based on Crustcrawler kits and MX actuators).

*We would be pleased to hear from you if you use this library in one of your projects.*

Scientific publications
-----------------------
This library has been used in many scientific publications, in particular:
* Cully, A., Clune, J. Tarapore, D. and Mouret, J.-B.. "Robots that can adapt like animals." *Nature* 521, no. 7553 (2015): 503-507.
* Cully, A., and J-B. Mouret. "Evolving a Behavioral Repertoire for a Walking Robot." Evolutionary computation (2016).
* Koos, S. and Cully, A. and Mouret, J.-B. (2013). "Fast Damage Recovery in Robotics with the T-Resilience Algorithm". *International Journal of Robotics Research*. Vol 32 No 14 Pages 1700-1723.
* Koos, S. and Mouret, J.-B. and Doncieux, S. (2013). "The Transferability Approach: Crossing the Reality Gap in Evolutionary Robotics". *IEEE Transactions on Evolutionary Computation*. Vol 17 No 1 Pages 122 - 145
* Koos, S. and Mouret, J-B. (2011). "Online Discovery of Locomotion Modes for Wheel-Legged Hybrid Robots: a Transferability-based Approach". *Proceedings of CLAWAR*, World Scientific Publishing Co., publisher. Pages 70-77.
* Koos, S. and Mouret, J.-B. and Doncieux, S. (2010). "Crossing the Reality Gap in Evolutionary Robotics by Promoting Transferable Controllers". *GECCO'10: Proceedings of the 12th annual conference on Genetic and evolutionary computation* ACM, publisher . Pages 119--126.

*Please let us know if you use this library in a scientific publication.*

ROS interface
================
An optional ROS interface (hardware interface) is available in a separate repository (`dynamixel_control_hw<https://github.com/resibots/dynamixel_control_hw/>`_).

Building
========
Dependencies :

* python2.x
* C++
* Boost C++

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

Support
=======
This library is actively used and regularly developped. If you need some help, feel free to `fill an issue <https://github.com/resibots/libdynamixel/issues/new>`_ on this repository. We'll do our best to be responsive.

Alternative software
====================

Inria FLOWERS team developped `pybot <http://poppy-project.github.io/poppy-docs/pypot/doc/index.html>`_, a python library to control a Dynamixel-based robot.

The official `Robotis SDK <TODO>`_.

License and authors
===================
This work is distributed under the terms of the `CeCILL-C <http://www.cecill.info/licences.en.html>`_ license. © UPMC and Inria.

The authors for this library are Jean-Baptiste Mouret, Antoine Cully, Federico Allocati and Dorian Goepp.
