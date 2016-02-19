#!/usr/bin/env python
# encoding: utf-8

import copy
import os
import glob

VERSION = '0.0.2'
APPNAME = 'dynamixel-demos'

srcdir = '.'
blddir = 'build'


def options(opt):
    opt.load('compiler_cxx')
    opt.add_option('--arm', action='store_true', help='enable arm cross-compilation', dest='arm')


def configure(conf):
    conf.load('compiler_cxx')
    conf.env['CXXFLAGS'] = '-D_REENTRANT -Wall -finline-functions -Wno-inline  -fPIC -O3 -ftemplate-depth-128 -Wno-sign-compare'

    if conf.options.arm:
        conf.setenv('arm', conf.env)
        conf.env['ENABLE_ARM'] = True
        conf.load('cross_arm', tooldir='waf_tools')
        conf.find_arm_cc()
        conf.find_arm_cxx_cpp()
        conf.env['CXXFLAGS'] = '-D_REENTRANT -DDBG_ENABLED -Wall -O3 -ftemplate-depth-128 -Wno-sign-compare'
    print 'CXXFLAGS:' + conf.env['CXXFLAGS']


def build(bld):
    bld.recurse('src/demos')

    if ('arm' in bld.all_envs) and (bld.all_envs['arm']['ENABLE_ARM'] is True):
        print "arm enabled"
        for obj in copy.copy(bld.all_task_gen):
            obj.clone('arm')

    p = bld.srcnode.abspath() + '/src/dynamixel/'

    r = glob.glob(p + '*/**')
    for i in r:
        k = os.path.split(i)
        d = os.path.split(k[0])
        bld.install_files('${PREFIX}/include/dynamixel/' + d[1], i)

    r = glob.glob(p + '*.hpp')
    for i in r:
        k = os.path.split(i)
        d = os.path.split(k[0])
        bld.install_files('${PREFIX}/include/' + d[1], i)
