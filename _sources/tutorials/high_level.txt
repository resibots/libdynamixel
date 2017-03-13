Detect and control servos with the high level interface
=========================================================

.. highlight:: c++

Description
------------

This tutorial assumes that you have already been able to compile and install the library. If not, please refer to the related instructions in the :ref:`building/installation instructions <download_and_compilation>`.

We would like to develop a program that auto-detects all the servos connected to a port, sets their position at :math:`\pi` radians (180 degrees) and then reads the position that they have to verify that they got to the desired one. To do so, we are going to use the high level interface of the libdynamixel library. Our program will have the following usage ::

    ./high_level_control port

It requires the port name which is the absolute path to the linux device interfacing with the Dynamixel bus.

Depending on the interface device you use, the device name would look like ``ttyACMx`` or ``ttyUSBx`` with x being an integer

.. warning::

    To be able to access this interface, you usually need special rights. On ubuntu, you either have to belong to the *dialout* group or to run the generated programs as superuser.

C++ Code with comments
-----------------------

We begin with including the required files:

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
   :lines: 42-73
   :dedent: 8

All these commands should be enclosed into a ``try-catch`` since we enabled the exception throwing.

Here's the **high_level_control.cpp** file:

.. literalinclude:: ../../src/tutorials/high_level.cpp
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
                  source = 'high_level_control.cpp',
                  includes = '.',
                  target = 'high_level_control',
                  uselib =  libs,
                  defines = ['PROTOCOL1']) # change this to PROTOCOL2 if you want to use servos that operate with PROTOCOL2
