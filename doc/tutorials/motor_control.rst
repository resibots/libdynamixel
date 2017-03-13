Control the position of auto-detected servos
---------------------------------------------

.. highlight:: c++

This tutorial assumes that you have already been able to compile the library. If not, please refer to the related instructions in the :ref:`building` instructions.

We would like to develop a program that auto-detects all the servos connected to a port and sets their position at :math:`\pi` radians (180 degrees). Our program will have the following usage ::

    ./high_level_protocol{1,2} port

It requires the port name which is the absolute path to the linux device interfacing with the Dynamixel bus.

Depending on the interface device you use, the device name would look like ``ttyACMx`` or ``ttyUSBx`` with x being an integer

.. warning::

    To be able to access this interface, you usually need special rights. On ubuntu, you either have to belong to the *dialout* group or to run the generated programs as superuser.

We begin with including required files:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 1-2

We are expecting the port name as a command line parameter:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 4-10

We select which protocol our servos are utilizing by using the C++ preprocessor:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 12-17

We first create a ``Usb2Dynamixel`` object with default parameters:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 19-20
   :dedent: 4

Next, we want to establish a connection with the desired port:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 22-31
   :dedent: 4

Note that we should always enclose the connection to the serial port with a ``try-catch``, since connecting to serial devices could always have some unexpected errors. Similarly, we would like our ``Usb2Dynamixel`` controller to notify us for possible errors. To do this, we must enable it as follows:

 .. literalinclude:: ../../src/tutorials/high_level.cpp
    :language: c++
    :linenos:
    :lines: 33-34
    :dedent: 4

Next, we try to detect all the connected servos:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 38-40
   :dedent: 8

Finally, for each of the dected servos we enable it and set its position at :math:`\pi` radians:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
   :linenos:
   :lines: 42-56
   :dedent: 8

All these commands should be enclosed into a ``try-catch`` since we enabled the exception throwing.

Here's the full file:

.. literalinclude:: ../../src/tutorials/high_level.cpp
   :language: c++
