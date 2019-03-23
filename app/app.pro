#-------------------------------------------------
#
# Project created by QtCreator 2019-02-22T21:16:49
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt c++14

TARGET = ApotekHoldbarhet

win32: LIBS += -lshell32

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# libraies
INCLUDEPATH += \
            external/tinyxml2 \
            external/date/include/date \
            src/fest_xml \


SOURCES += \
        external/tinyxml2/tinyxml2.cpp \
        src/main.cpp \
        src/ui/apotekholdbarhet.cpp \
        src/fest_xml/fest_reader.cpp \
        src/database.cpp \
        src/ui/calendar_widget.cpp \
        src/ui/about_apotekholdbarhet.cpp \
        src/ui/about_fest.cpp \
        src/ui/add_vare.cpp \
        src/download.cpp \
        src/ui/fest_update.cpp \
        src/product.cpp \
        src/check_numbers.cpp

# tests
#SOURCES += \
#        tests/test_database.cpp

HEADERS += \
        external/date/include/date/date.h \
        src/ui/apotekholdbarhet.h \
        src/version.h \
        src/fest_xml/fest_reader.h \
        src/database.h \
        src/ui/calendar_widget.h \
        src/ui/about_apotekholdbarhet.h \
        src/ui/about_fest.h \
        src/table_colors.h \
        src/ui/add_vare.h \
        src/download.h \
        src/ui/fest_update.h \
        src/product.h \
        src/constants.h \
        src/apotekholdbarhet_init.h \
        src/database_init.h \
    src/error_messages.h \
    src/table_names.h \
    src/check_numbers.h

FORMS += \
        ui/apotekholdbarhet.ui \
        ui/calendarwidget.ui \
        ui/about_apotekholdbarhet.ui \
        ui/about_fest.ui \
        ui/add_vare.ui \
        ui/fest_update.ui

RESOURCES += \
        images.qrc



# copy FEST files
unix {
copydata.commands = $(COPY_DIR) $$PWD/fest $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
}
