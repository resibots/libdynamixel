Low level API
=============

Instructions Packets
--------------------

.. note:: For *instruction packets* to be sent and *status packet* to be received, one needs a :ref:`controller<controllers>`, such as :cpp:class:`dynamixel::controllers::Usb2Dynamixel`.

Abstract class (InstructionPacket)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For details about instructions, see Robotis' documentation for `protocol 1 <http://support.robotis.com/en/product/actuator/dynamixel/communication/dxl_instruction.htm>`__ and `protocol 2 <http://support.robotis.com/en/product/actuator/dynamixel_pro/communication/instruction_status_packet.htm>`__.

.. doxygenclass:: dynamixel::InstructionPacket
   :members:
   :undoc-members:

Instructions
^^^^^^^^^^^^

.. doxygengroup:: instructions
   :members:

Status packets (StatusPacket)
-----------------------------

.. note:: For *instruction packets* to be sent and *status packet* to be received, one needs a :ref:`controller<controllers>`, such as :cpp:class:`dynamixel::controllers::Usb2Dynamixel`.

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
