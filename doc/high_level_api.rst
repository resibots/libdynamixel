High level API
===============

Controllers
-----------
For now there is only one controller class called ``Usb2Dynamixel``. It handles the USB to serial interface, be it the official USB2Dynamixel or the `USB2AX`_. More contcretely, this is the class you use to send and recieve packets on the servos' bus.

.. _USB2AX: http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax

.. doxygenclass:: dynamixel::controllers::Usb2Dynamixel
   :members:
   :undoc-members:

Auto-detection of servos
-------------------------
There are two versions of the auto detection, one that returns a vector and one that returns a map.

.. doxygenfunction:: dynamixel::auto_detect

.. doxygenfunction:: dynamixel::auto_detect_map

Servo object (BaseServo)
------------------------

The auto-detection will return ``BaseServo`` objects (that is, objects of sub-classes of ``BaseServo``)

.. doxygenclass:: dynamixel::servos::BaseServo
  :members:
