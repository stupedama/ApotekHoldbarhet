QT += testlib sql core

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

win32: LIBS += -lshell32

TEMPLATE = app

SOURCES +=  tst_test_fmd_xml.cpp

include(../tests.pri)
