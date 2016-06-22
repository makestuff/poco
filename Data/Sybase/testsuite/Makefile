#
# Makefile
#
# $Id: //poco/1.5/Data/Sybase/testsuite/Makefile#1 $
#
# Makefile for Poco Sybase testsuite
#

include $(POCO_BASE)/build/rules/global

SYBASE_OCS ?= /opt/sap/OCS-16_0

CXXFLAGS := -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -pedantic-errors -std=c++98 -DSYB_LP64

INCLUDE += -isystem $(SYBASE_OCS)/include

# Note: linking order is important, do not change it.
SYSLIBS += -lz -lpthread

objects = SybaseTestSuite Driver SybaseTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoDataSybase PocoData PocoFoundation PocoCppUnit

include $(POCO_BASE)/build/rules/exec
