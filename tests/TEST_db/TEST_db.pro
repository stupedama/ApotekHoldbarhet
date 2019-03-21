QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

win32: LIBS += -lshell32

SOURCES +=  tst_test_db.cpp

include(../tests.pri)

# copy FEST files
unix {
copydata.commands = $(COPY_DIR) $$PWD/fest $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
}
