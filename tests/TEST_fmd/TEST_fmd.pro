QT += testlib sql core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

win32: LIBS += -lshell32

SOURCES +=  tst_test_fmd.cpp

include(../tests.pri)

