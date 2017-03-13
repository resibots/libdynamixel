Concepts underlying the design of the library
=============================================

In this document we will describe the way the communication with dynamixel actuators works and how it is done with this library.

The two protocols
-----------------

Robotis designed two version of its communication protocol. We have a class for each protocol containing:

* type definitions for protocol-specific data
* codes for the instructions allowed in the protocol
* methods to pack to be sent and unpack the data that was received

Unless said overwise, everything written bellow is common to versions 1 and 2 of the communication protocols.

Serial interface
----------------

The client interacts with Dynamixels by sending instructions and receiving status reports. These are packed in data frames that are usually sent through a USB to serial interface (such as `USB2AX <http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax>`__ and `USB2DYNAMIXEL <http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1289&keyword=USB2Dynamixel>`__).

.. highlight:: C++

In this library, the interface is managed by an instance of :code:`controllers::Usb2Dynamixel` (compatible with USB2AX).

Instructions and status report
------------------------------

A class is defined for each instruction and templated with the protocol. Here are the instructions currently supported:

* :code:`Ping<Protocol>`: check that an actuator responds
* :code:`Read<Protocol>`: read some data in the RAM or ROM of the actuator
* :code:`Write<Protocol>`: write some data in the RAM or ROM of the actuator
* :code:`RegWrite<Protocol>`: register a write instruction to be performed later, on receival of the action instruction
* :code:`Action<Protocol>`: trigger the registered write instruction
* :code:`FactoryReset<Protocol>`: reset the ROM of the actuator to factory defaults
* :code:`SyncWrite<Protocol>`: simultaneously write data for selveral servos (one memory address, many servos, many data)
* :code:`Reboot<Protocol>` (only for protocol 2)

Likewise, the :code:`StatusPacket<Protocol>` class is for the status report packet. A status packet can report errors in the communication or related to an actuators. For instance, a checksum error or an overheating error. Such packets are sent in response to all packets, unless sent to the broadcast ID. Some actuators also allow to configure after which instructions a status report is sent.

Model traints
-------------

A templated class :code:`ModelTraints<typename Model>`, is explicitely specialised for each supported actuator, holding model-specific information such as the control table (including address, size of each entry). This makes a unified yet extensible interface to all actuators.

Aside from the control table, the :code:`ModelTraits` class also defines the protocol accepted by each servo.

.. todo:: give a link to the list of supported actuators

Base servo, Servo and actuator classes
--------------------------------------

A class is defined for each actuator. For instance, the MX 28 actuators have the class :code:`Mx28`. These are children class of :code:`Servo<typename Protocol>` and :code:`BaseServo<typename Protocol>`. The latter declares the methods to access and modify the fields of all possible control tables but the definition of these methods throws an "not implemented" exception. :code:`Servo` on the other hand defines the least common denimonator of methods. The remainder fields are defined in the actuator's class.

Protocol-specific packets
-------------------------

There are some operations that are comon to all actuators using a given protocol. This templated class is for these operations, such as converting the speed from unit-less integer to radians per second.

Exception classes
-----------------