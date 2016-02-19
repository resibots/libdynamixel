#!/usr/bin/env python

from waflib.Configure import conf


@conf
def find_arm_cc(conf):
    v = conf.env
    v['CC'] = None
    cc = conf.find_program('arm-linux-gnueabi-gcc', path_list=['/usr/arm/bin', '/usr/bin'], var='CC')
    print cc
    if not cc or not len(cc):
        conf.fatal('arm-linux-gnueabi-gcc was not found')
    try:
        if conf.cmd_and_log('%s --version' % cc[0]).find('arm-linux-gnueabi-gcc') < 0:
            conf.fatal('arm-linux-gnueabi-gcc was not found, see the result of gcc --version')
    except ValueError:
        conf.fatal('gcc --version could not be executed')
    v['CC'] = v['LINK_CC'] = cc
    v['CC_NAME'] = 'gcc'
    conf.find_program('arm-linux-gnueabi-ar', path_list=['/usr/arm/bin'], var='AR')
    conf.find_program('arm-linux-gnueabi-ranlib', path_list=['/usr/arm/bin'], var='RANLIB')


@conf
def find_arm_cxx_cpp(conf):
    v = conf.env
    v['CXX'] = None
    cxx = conf.find_program('arm-linux-g++', path_list=['/usr/arm/bin'])
    if not cxx:
        conf.fatal('arm-linux-g++ was not found')
    v['CXX'] = v['LINK_CXX'] = v['COMPILER_CXX'] = v['CPP'] = v['LINK_CPP'] = cxx
    v['CXX_NAME'] = 'gcc'

detect = 'find_arm_cc find_arm_cxx_cpp'
