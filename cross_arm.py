#!/usr/bin/env python


import Utils
import ccroot
from Configure import conftest


@conftest
def find_arm_cc(conf):
    v = conf.env
    v['CC'] = None
    cc = conf.find_program('arm-linux-gcc', path_list=['/usr/arm/bin'], var='CC')
    if not cc: conf.fatal('arm-linux-gcc was not found')
    try:
        if Utils.cmd_output('%s --version' % cc).find('arm-linux-gcc') < 0:
            conf.fatal('arm-linux-gcc was not found, see the result of gcc --version')
    except ValueError:
        conf.fatal('gcc --version could not be executed')
    v['CC'] = v['LINK_CC'] = cc
    v['CC_NAME'] = 'gcc'
    conf.find_program('arm-linux-ar', path_list=['/usr/arm/bin'], var='AR')
    conf.find_program('arm-linux-ranlib', path_list=['/usr/arm/bin'], var='RANLIB')
    ccroot.get_cc_version(conf, [cc], 'CC_VERSION')


@conftest
def find_arm_cxx_cpp(conf):
    v = conf.env
    v['CXX'] = None
    cxx = conf.find_program('arm-linux-g++', path_list=['/usr/arm/bin'])
    if not cxx: conf.fatal('arm-linux-g++ was not found')
    v['CXX'] = v['LINK_CXX'] = v['COMPILER_CXX'] = v['CPP'] = v['LINK_CPP'] = cxx
    v['CXX_NAME'] = 'gcc'

detect = 'find_arm_cc find_arm_cxx_cpp'
