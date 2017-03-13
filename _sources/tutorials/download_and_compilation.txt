.. _download_and_compilation:

Download and Compilation
========================

Download
--------

To get **libdynamixel**, simply clone the source code from https://github.com/resibots/libdynamixel.git with git, or `download it as a zip <https://github.com/resibots/libdynamixel/archive/master.zip>`__.

.. todo:: We whould handle versions here and propose to download version 2.1 that should be published at the same time as the documentation.

Dependencies
~~~~~~~~~~~~

* python2.x
* C++
* `Boost <http://www.boost.org>`_ , with the program_options librariy; `Boost` is mainly used for the command line interface.

Compilation
-----------

We use  the `WAF <https://waf.io>`_  build system. Don't run away yet! It has the big advantage of requiring no more than Python installed on your computer. If you use GNU/Linux or Mac OS, you should already have it.

To know why we use waf (and not CMAKE, SCONS, traditional makefiles, etc.), see the :ref:`Limbo FAQ <faq-waf>`.

Like most build systems, it has a configuration and build steps, described bellow.

.. highlight:: shell

Configuration
~~~~~~~~~~~~~

.. caution::
    Make sure that the waf file has execution rights.

The first step is to configure your waf environment. For this, assuming that you are in the root directory of libdynamixel, you have to run the command::

    ./waf configure

.. note::
    add ``--prefix PATH/TO/INSTALL`` if you want to install it to a specific location

If everything is okay, you should expect an output like this::

    Setting top to                           : /path/to/libdynamixel
    Setting out to                           : /path/to/libdynamixel
    Checking for 'g++' (C++ compiler)        : not found
    Checking for 'clang++' (C++ compiler)    : /usr/bin/clang++
    Checking boost includes                  : 1_58
    Checking boost libs                      : ok
    CXXFLAGS:['-D_REENTRANT', '-Wall', '-finline-functions', '-Wno-inline',
        '',     '-fPIC', '-O3', '-std=c++11', '-ftemplate-depth-128',
        '-Wno-sign-compare']
    'configure' finished successfully (0.221s)

The actual ouput may differ, depending on your configuration and installed libraries.

Waf should automatically detect the libraries if they where installed in the default folders, but if it doesn't,
you can use the following command-line options to indicate where they are:

* ``--boost-includes /path/to/boost-includes`` [.h]
* ``--boost-libs /path/to/boost-libraries`` [.a, .so, .dynlib]

Build
~~~~~

The second step is to run the build command::

    ./waf build

Depending on your compiler, there may be some warnings, but the output should end with the following line:

.. code-block:: none

    'build' finished successfully (time in sec)

Installation
~~~~~~~~~~~~

It is possible, although not necessary, to have the library installed globally (or to the path given with ``--prefix``)::

    ./waf install

When globally installed, this command should require super-user rights (e.g. ``sudo``).

Configuration of the serial interfaces
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Please ensure that you have the proper rights to access the serial interfaces. On GNU/Linux, you might have to add your user to the ``dialout`` group and log out.

Building the documentation
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
    This section is only useful for developers who need to update the documentation.

Install sphinx via pip: ::

    sudo pip install Sphinx
    sudo pip install sphinxcontrib-bibtex

.. warning::

  On Mac OSX, do not use `brew install sphinx` because this is not the right sphinx

Install the Resibots theme for Sphinx::

    git clone https://github.com/resibots/sphinx_resibots_theme
    export SPHINX_RESIBOTS_THEME="/home/me/path/to/sphinx_resibots_theme"

Install `breathe <https://breathe.readthedocs.io/en/latest/>`_ via pip::

    sudo pip install breathe

Install `doxygen <http://www.stack.nl/~dimitri/doxygen/>`_ via your package manager (e.g. apt-get / brew)::

    apt-get install doxygen

In the `doc` directory::

    make html

About sphinx and ReStructuredText:
  - `There is a tutorial <http://sphinx-doc.org/tutorial.html>`_,
  - `Primer for ReStructuredText <http://sphinx-doc.org/rest.html>`_, the markup language of Sphinx,
  - `markup specific to Sphinx <http://sphinx-doc.org/markup/index.html>`_,
  - `About C++ in Sphinx <http://sphinx-doc.org/domains.html#id2>`_
  - `Breathe (bridge between sphinx and doxygen) <https://breathe.readthedocs.org/en/latest/>`_

udev rule for USB2AX adapter
----------------------------

.. note:: For Gnu/Linux only

To have the `USB2AX <http://www.xevelabs.com/doku.php?id=product:usb2ax:usb2ax>`__ serial interface appear as :file:`/dev/usb2ax{N}` (where N is a kernel-attributed integer), you can install the provided udev rule. It is as simple as moving the :file:`usb2ax.rules` file from the root of the library to the for the udev rules folder. For ubuntu, it is :file:`/etc/udev/rules.d`.

