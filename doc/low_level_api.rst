Low level API
============================


Controllers
--------------
.. doxygenclass:: dynamixel::controllers::Usb2Dynamixel
  :members:
  :undoc-members:



Instructions Packets
---------------------

Errors
------

Abstract class (Error)
^^^^^^^^^^^^^^^^^^^^^^^
All errors derive from ``dynamixel::error::Error``:

.. doxygenclass:: dynamixel::errors::Error
  :members:
  :undoc-members:


Error classes
^^^^^^^^^^^^^^
.. doxygengroup:: errors
   :members:
