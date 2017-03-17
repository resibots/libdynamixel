Architecture of the library
===========================

.. highlight:: C++

In this document we will describe how the library communicates with dynamixel actuators.

The two protocols
-----------------

The client interacts with Dynamixels by sending instructions and receiving status reports, both in forms of packets. Robotis has two versions of its communication protocol defining diferent packet formats and contents. We have a class for each protocol containing:

* type definitions for protocol-specific data
* the instructions allowed in the protocol
* methods to build a packet from data (`pack_data`) and extract data from incoming packet (`unpack_data`)

.. note:: Unless said overwise, everything written bellow is common to versions 1 and 2 of the communication protocols.

Serial interface
----------------

All communication with the dynamixel has to go through a serial interface. We support both `USB2AX <http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax>`__ and `USB2DYNAMIXEL <http://www.robotis-shop-en.com/?act=shop_en.goods_view&GS=1289&keyword=USB2Dynamixel>`__ in the class :cpp:class:`controllers::Usb2Dynamixel`. Other serial interfaces should also work.

Instructions and status report
------------------------------

Instructions are packets sent to the actuators and status reports are the replies that the user may get (not for every instruction). There are different instruction kinds and a class is defined for each instruction.

Supported instructions:

* :cpp:class:`Ping\<class Protocol> <instructions::Ping>`: check availability of an actuator with given ID
* :cpp:class:`Read\<class Protocol> <instructions::Read>`: read some data in the RAM or ROM of an actuator
* :cpp:class:`Write\<class Protocol> <instructions::Write>`: write some data in the RAM or ROM of an actuator
* :cpp:class:`RegWrite\<class Protocol> <instructions::RegWrite>`: register a write instruction to be performed later (on receival of the *action* instruction)
* :cpp:class:`Action\<class Protocol> <instructions::Action>`: trigger the registered write instruction
* :cpp:class:`FactoryReset\<class Protocol> <instructions::FactoryReset>`: reset the ROM of the actuator to factory defaults
* :cpp:class:`SyncWrite\<class Protocol> <instructions::SyncWrite>`: simultaneously write data for several servos (one memory address, many servos, many data)
* :cpp:class:`Reboot\<class Protocol> <instructions::Reboot>` (only for protocol 2)

.. note::
    The following instructions, specific to protocol 2, are not implemented yet:

    * Sync read: same as *sync write* but for reading
    * Bulk write: simultaneously write data for several (many addresses, many servos, many data)
    * Bulk read: same as *bulk write* but for reading

Likewise, the :cpp:class:`StatusPacket\<class Protocol> <dynamixel::StatusPacket>` class is for the status report packet. A status packet can report errors in the communication or related to an actuator. It could be for instance a checksum error or an overheating error. Such packets are sent in response to any packet, unless it was sent to the broadcast ID. Some actuators also allow to configure which instructions trigger a status report.

.. note::
    For more details on how instructions and status packet work, refer to `Robotis' documentation <http://support.robotis.com/en/>`__ then go to :menuselection:`Product Information --> Actuator --> Communication 1.0/2.0`.

Model traits
-------------

For a more easy to use client interface, the information specific to each actuator model (including the control table) is stored in the library. We use the trait class :cpp:class:`ModelTraits\<class Model> <dynamixel::servos::ModelTraits>`, templated by the servo. This makes a unified yet extensible interface to all actuators.

Along the control table, the :code:`ModelTraits` class also defines the protocol version used by each servo.

.. todo:: give a link to the list of supported actuators

BaseServo, Servo and actuator classes
--------------------------------------

The :cpp:class:`BaseServo\<class Protocol> <dynamixel::servos::BaseServo>` abstract class declares all possible methods to interact with an actuator. They all throw a "not implemented" exception. Then, :cpp:class:`Servo <dynamixel::servos::Servo>` inherits of :cpp:class:`BaseServo <dynamixel::servos::BaseServo>` and defines the methods to access the fields common to all actuators. The remainder fields are defined in the actuator secific classes. For instance, the MX 28 actuators have the class :code:`Mx28`. One such class is defined for every servo model.

Protocol-specific packets
-------------------------

Some operations are common to all actuators that use a given protocol. The templated class :cpp:class:`ProtocolSpecificPackets\<class Model, class Protocol> <dynamixel::protocols::ProtocolSpecificPackets>` is for these operations:

* convert the speed from unit-less integer to radians per second