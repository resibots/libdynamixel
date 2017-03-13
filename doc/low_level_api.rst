Low level API
=============


Controllers
-----------
.. doxygenclass:: dynamixel::controllers::Usb2Dynamixel
  :members:
  :undoc-members:



Instructions Packets
--------------------

Abstract class (InstructionPacket)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
For details about instructions, see:

* protocol 1: http://support.robotis.com/en/product/actuator/dynamixel/communication/dxl_instruction.htm
* protocol 2: http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm

.. doxygenclass:: dynamixel::InstructionPacket
  :members:
  :undoc-members:

Instructions
^^^^^^^^^^^^
.. doxygengroup:: instructions
   :members:

Status packets (StatusPacket)
-----------------------------

.. doxygenclass:: dynamixel::StatusPacket
  :members:
  :undoc-members:

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
