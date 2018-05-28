#!/usr/bin/env python
# encoding: utf-8
import sys
sys.path.insert(0, './waf_tools')

import os
import glob

VERSION = '0.0.2'
APPNAME = 'dynamixel-demos'

srcdir = '.'
blddir = 'build'


def options(opt):
    opt.load('compiler_cxx')
    opt.add_option('--tests', action='store_true', help='compile tests or not', dest='tests')
    opt.add_option('--demos', action='store_true', help='compile demos or not', dest='demos')

    opt.recurse('src/tools')
    # opt.recurse('src/tests')


def configure(conf):
    conf.load('compiler_cxx')
    conf.env['CXXFLAGS'] = '-D_REENTRANT -Wall -finline-functions -Wno-inline  -fPIC -O3 -std=c++11 -ftemplate-depth-128 -Wno-sign-compare'.split(' ')

    conf.recurse('src/tools')
    # conf.recurse('src/tests')

    print 'CXXFLAGS:' + str(conf.env['CXXFLAGS'])


def build(bld):
    bld.recurse('src/tools')
    if bld.options.demos:
        bld.recurse('src/demos')
    if bld.options.tests:
        bld.recurse('src/tests')

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
