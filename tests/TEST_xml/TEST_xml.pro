QT += testlib
QT += gui sql core
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

win32: LIBS += -lshell32

SOURCES +=  tst_test_xml.cpp


include(../tests.pri)

# copy FEST files
unix {
copydata.commands = $(COPY_DIR) $$PWD/fest $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
}
