High level API
===============

.. _controllers:

Controllers
-----------
For now there is only one controller class called ``Usb2Dynamixel``. It handles the USB to serial interface, be it the official USB2Dynamixel or the `USB2AX`_. More contcretely, this is the class you use to send and recieve packets on the servos' bus.

.. _USB2AX: http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax

.. doxygenclass:: dynamixel::controllers::Usb2Dynamixel
   :members:
   :undoc-members:


.. _auto_detection:

Auto-detection of servos
-------------------------
Doing auto-detection will scan the bus for connected servos and build a collection of objects handling each servo that was found. There are two versions of the auto detection, one that returns a vector and one that returns a map.

.. note:: Bear in mind that only servos using the same baudrate as set in the controller will be found.

.. doxygenfunction:: dynamixel::auto_detect

.. doxygenfunction:: dynamixel::auto_detect_map

Servo object (BaseServo)
------------------------

The :ref:`auto-detection <auto_detection>` will return ``BaseServo`` objects (actually objects of sub-classes of ``BaseServo``). In addition to the methods bellow, this class also defines methods to :ref:`access and change <baseservo_define_methods>` all elements of the control table for the actuator.

Class definition
^^^^^^^^^^^^^^^^

``BaseServo`` is defined as follows, with the addition of the methods described in :ref:`baseservo_define_methods`.

.. doxygenclass:: dynamixel::servos::BaseServo
  :members:

..  :undoc-members:

.. _baseservo_define_methods:

Methods for access to the control table
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The following methods are defined for all fields found in at least a servo's control table. If one tries to access or change a field that does not exist in the control table of the servo at hand, an exception will be raised. The exception is of type :cpp:class:`dynamixel::errors::Error` and contains the message "*some_method* not implemented in model" where *some_method* is the name of a method.

.. note:: see :ref:`supported_actuators_and_fields` for the fields available for each supported servo model and their names in Libdynamixel.

.. cpp:function:: virtual InstructionPacket<protocol_t> BaseServo::get_NAME() const

  Request the NAME field in the control table; the response from the servo must be parsed with :code:`parse_NAME`.

.. cpp:function:: virtual long long int parse_NAME(const StatusPacket<protocol_t>& st) const

  The response (stored in :code:`StatusPacket`) from :code:`get_NAME` is processed here and returned as an integer.

.. cpp:function:: virtual InstructionPacket<protocol_t> set_NAME(long long int value) const

  Set the value for field NAME of the control table; to set speed and position values in radians per second and radians, use :code:`set_goal_position_angle <BaseServo::set_goal_position_angle>` and similar methods (see previous section).

.. cpp:function:: virtual InstructionPacket<protocol_t> reg_NAME(long long int value) const

  Register the instruction to set the value of the field NAME of the control table; for speed and position values, use :code:`reg_goal_position_angle<BaseServo::reg_goal_position_angle>` and similar methods (see previous section).

  The walue is written when the servo receives an :cpp:class:`Action\<class T>` instruction, which can be broadcasted to all servos.

  Example::

    serial_interface.send(servos.at(id)->reg_goal_position_angle(angle));

    StatusPacket<Protocol> status;
    serial_interface.recv(status);

    serial_interface.send(
      dynamixel::instructions::Action<Protocol>(Protocol::broadcast_id));

Other utility functions
-----------------------

.. doxygenfunction:: dynamixel::get_baudrate_id