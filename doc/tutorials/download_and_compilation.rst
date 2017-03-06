.. _download_and_compilation:

Download and Compilation
========================

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

udev rule for USB2AX adapter
----------------------------

For Gnu/Linux only.

If you want your USB2AX serial interface to appear in ``/dev`` as ``usb2axN`` (where N is a kernel-attributed integer), you can install the udev rule. It is as simple as moving the ``usb2ax.rules`` file in this repository to the folder for the udev rules. For ubuntu, it is ``/etc/udev/rules.d``.

