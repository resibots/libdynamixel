Control one actuator's position or speed
----------------------------------------

.. highlight:: c++

Let's assume that you installed the dependencies of libdynamixel and that scceded in compiling the ``dynamixel`` tool and the demos. If not, please refer to the related instructions in the :ref:`building` instructions.

Instranciate the object managing the interface ``Usb2Dynamixel``::

    std::string port = "/dev/ttyUSB0";
    int baudrate = B1000000;
    Usb2Dynamixel controller;
    controller.open_serial(port, baudrate);

It requires the port name which is the absolute path to the linux device interfacing with the Dynamixel bus.

Depending on the interface device you use, the device name would ressemble more ``ttyACMx`` or ``ttyUSBx`` with x being an integer

.. warning::

    To be able to access this interface, you usually need special rights. On ubuntu, you either have to belong to the *dialout* group or to run the generated programs as superuser.

The B1000000 constant is defined by the system and is the default baud rate for a newly acquired actuator. See :doc:`/reference/baudrates` for the other possible values.

