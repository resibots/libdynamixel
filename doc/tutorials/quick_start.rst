Quick Start
===========

Building
--------

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

For more options and troubleshooting, see the :ref:`Compilation tutorial <download_and_compilation>`.

Command line interface
----------------------

You get the ``dynamixel`` toolset and the demo programms compiled in the ``build/default/src`` folder.

.. note:: the ``get-speed`` command gives you the reference speed used in the control table (if any) and not the current rotational speed of the actuator.

Demo programms
--------------

You get the ``dynamixel`` toolset and the demo programms compiled in the ``build/default/src`` folder.

For now, the demo programs are not at a working stage and should not be used. Conversely, ``dynamixel`` is a working program that we use regularly.

Library
-------