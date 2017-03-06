Libdynamixel's documentation
============================

libdynamixel
------------
libdynamixel is a C++ library for the `Dynamixel <http://en.robotis.com/index/product.php?cate_code=101010>`_ actuators. Its goals are to be fast and to work with most Dynamixels, including the Dynamixel Pro actuators.

This library comes with a command line utility that can, among other things, be very convenient for the configuration of your actuators.

Features
--------

* supports Dynamixel series AX, MX, EX, XL and Pro
* supports all instructions and messages of both version 1 and 2 of the Dynamixel communication protocol
* full C++ api

  * template-based API for fast and low-level access
  * class-based unified API abstracting away the differences between the actuators (as long as they use the same Dynamixel protocol).

Contents
--------

.. toctree::
  :hidden:
  :caption: Libdynamixel

.. toctree::
  :maxdepth: 2

  self
  tutorials/index
  reference/index

Indices and tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

Support
-------

If you need some help, feel free to `fill an issue <https://github.com/resibots/libdynamixel/issues/new>`_ on this repository. We'll do our best to be responsive.

License and authors
-------------------

This work is distributed under the terms of the `CeCILL-C <http://www.cecill.info/licences.en.html>`_ license. © UPMC and INRIA.

The authors for this library are Jean-Baptiste Mouret, Antoine Cully, Federico Allocati and Dorian Goepp.
