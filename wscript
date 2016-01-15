#!/usr/bin/env python
# encoding: utf-8

import Options
import copy
VERSION='0.0.2'
APPNAME='dynamixel-demos'

srcdir = '.'
blddir = 'build'

def set_options(opt):
    opt.tool_options('compiler_cxx')
    opt.add_option('--arm', type='string', help='enable arm cross-compilation', dest='arm')
    opt.tool_options('boost_detect')

def configure(conf):
    conf.check_tool('compiler_cxx')
    conf.env['CXXFLAGS']='-D_REENTRANT -Wall -finline-functions -Wno-inline  -fPIC -O3 -ftemplate-depth-128 -Wno-sign-compare'

    env = conf.env.copy()
    env.set_variant('arm')
    conf.set_env_name('arm', env)

    conf.check_tool('boost_detect')
    boost = conf.check_boost(lib='program_options', min_version='1.35')
    if boost:
      conf.env['CPPFLAGS'] += ['-DUSE_BOOST']

    if Options.options.arm:
        conf.setenv('arm')
        conf.env['ENABLE_ARM'] = True
        conf.check_tool('cross_arm', tooldir='.')
        conf.env['CXXFLAGS']='-D_REENTRANT -DDBG_ENABLED -Wall -O3 -ftemplate-depth-128 -Wno-sign-compare'
    print 'CXXFLAGS:' + conf.env['CXXFLAGS']

def build(bld):
    #bld.add_subdirs('src/dynamixel')
    bld.add_subdirs('src/demos')
    #bld.add_subdirs('src/tools')

    if ('arm' in bld.all_envs) and (bld.all_envs['arm']['ENABLE_ARM'] == True):
        print "arm enabled"
        for obj in copy.copy(bld.all_task_gen):
            obj.clone('arm')

