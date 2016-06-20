#
# Makefile
#
# $Id: //poco/1.5/Data/Sybase/Makefile#1 $
#
# Makefile for Poco Sybase
#

include $(POCO_BASE)/build/rules/global

SYBASE_OCS ?= /opt/sap/OCS-16_0

CXXFLAGS := -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare -pedantic-errors -std=c++98 -DSYB_LP64

INCLUDE += -isystem $(SYBASE_OCS)/include

SYSLIBS += -L$(SYBASE_OCS)/lib -lsybct64 -lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64 -Wl,-rpath,$(SYBASE_OCS)/lib

objects = SessionImpl Connector SybaseException SybaseStatementImpl SessionHandle StatementExecutor Extractor Callbacks CTLibNames

target         = PocoDataSybase
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
