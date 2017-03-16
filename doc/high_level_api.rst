High level API
===============

Controllers
-----------

.. doxygenclass:: dynamixel::controllers::Usb2Dynamixel
   :members:
   :undoc-members:

Auto-detection of servos
-------------------------
There are two versions of the auto detection, one that returns a vector and one that returns a map.

.. doxygenfunction:: dynamixel::auto_detect

.. doxygenfunction:: dynamixel::auto_detect_map

Servo object (ServoBase)
------------------------

The auto-detection will return ``ServoBase`` objects (that is, objects of sub-classes of ``ServoBase``)

.. doxygenclass:: dynamixel::servos::BaseServo
  :members:
