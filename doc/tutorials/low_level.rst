Control servos with the low level interface
=============================================

.. highlight:: c++

Description
------------

This tutorial assumes that you have already been able to compile and install the library. If not, please refer to the related instructions in the :ref:`building/installation instructions <download_and_compilation>`.

We have a robot that uses 4 dynamixel actuators: two MX106 ones and two MX28 ones. We would like to develop a program that connects to the actuators, enables them, sets their maximum speed so that they are slower than usual and sets their position at :math:`\pi` radians (180 degrees). To do so, we are going to use the low level interface of the libdynamixel library. Our program will have the following usage ::

    ./low_level_control port

It requires the port name which is the absolute path to the linux device interfacing with the Dynamixel bus.

Depending on the interface device you use, the device name would look like ``ttyACMx`` or ``ttyUSBx`` with x being an integer

.. warning::

    To be able to access this interface, you usually need special rights. On ubuntu, you either have to belong to the *dialout* group or to run the generated programs as superuser.

C++ Code with comments
-----------------------

We begin with including the required files:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 1-4

We are expecting the port name as a command line parameter:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 6-12

We first create a ``Usb2Dynamixel`` object with default parameters:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 14-15
   :dedent: 4

Next, we want to establish a connection with the desired port:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 17-26
   :dedent: 4

Note that we should always enclose the connection to the serial port with a ``try-catch``, since connecting to serial devices could always have some unexpected errors. Similarly, we would like our ``Usb2Dynamixel`` controller to notify us for possible errors. To do this, we must enable it as follows:

 .. literalinclude:: ../../src/tutorials/low_level.cpp
    :language: c++
    :linenos:
    :lines: 28-29
    :dedent: 4

We create 2 arrays with the IDs of each type of actuators:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 31-33
   :dedent: 4

Finally, for each actuator we enable it, set its maximum speed and set its position at :math:`\pi` radians:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++
   :linenos:
   :lines: 39-72
   :dedent: 8

All these commands should be enclosed into a ``try-catch`` since we enabled the exception throwing.

Here's the **low_level_control.cpp** file:

.. literalinclude:: ../../src/tutorials/low_level.cpp
   :language: c++

Buildind with Waf
------------------

Now we need to create a `wscript` file for our project to compile it with waf (see the :ref:`compilation tutorial <download_and_compilation>` for details):

.. code-block:: python

    #!/usr/bin/env python

    def options(opt):
        pass

    def configure(conf):
      # Get locations where to search for libdynamixel's headers
      includes_check = ['/usr/include', '/usr/local/include']

      try:
        # Find the headers of libdynamixel
        conf.start_msg('Checking for libdynamixel includes')
        conf.find_file('dynamixel/dynamixel.hpp', includes_check)
        conf.end_msg('ok')

        conf.env.INCLUDES_LIBDYNAMIXEL = includes_check
      except:
        conf.end_msg('Not found', 'RED')
      return

    def build(bld):
        libs = 'LIBDYNAMIXEL BOOST'

        obj = bld(features = 'cxx',
                  source = 'low_level_control.cpp',
                  includes = '.',
                  target = 'low_level_control',
                  uselib =  libs)
