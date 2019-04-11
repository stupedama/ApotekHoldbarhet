#-------------------------------------------------
#
# Project created by QtCreator 2019-02-22T21:16:49
#
#-------------------------------------------------

QT       += core gui sql network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt c++14
QMAKE_CFLAGS_RELEASE = -O2 -MD -MP2

TARGET = ApotekHoldbarhet

# windows 32 libs
win32: LIBS += -lshell32
win32: LIBS += -L./external/openssl/lib -lssleay32
win32: LIBS += -L./external/openssl/lib -llibeay32
win32: INCLUDEPATH += ./external/openssl/lib

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
            src/fmd


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
        src/ui/fest_update.cpp \
        src/product.cpp \
        src/check_numbers.cpp \
        src/fmd/fmd_decoder.cpp \
        src/fmd/fmd_xml_reader.cpp \
        src/ui/print_durability.cpp \
        src/calculate_months.cpp \
        src/download_manager.cpp

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
        src/ui/fest_update.h \
        src/product.h \
        src/constants.h \
        src/apotekholdbarhet_init.h \
        src/database_init.h \
        src/error_messages.h \
        src/table_names.h \
        src/check_numbers.h \
        src/fmd/fmd_decoder.h \
        src/fmd/escape_codes.h \
        src/fmd/fmd_xml_reader.h \
        src/fmd/fmd_code.h \
        src/ui/print_durability.h \
        src/calculate_months.h \
        src/download_manager.h

FORMS += \
        ui/apotekholdbarhet.ui \
        ui/calendarwidget.ui \
        ui/about_apotekholdbarhet.ui \
        ui/about_fest.ui \
        ui/add_vare.ui \
        ui/fest_update.ui \
        ui/print_durability.ui

RESOURCES += \
        images.qrc

# copy FEST files
unix {
copydata1.commands = $(COPY_DIR) $$PWD/xml/fest $$OUT_PWD
first.depends = $(first) copydata1
export(first.depends)
export(copydata1.commands)
QMAKE_EXTRA_TARGETS += first copydata1
}

# copy FMD files
unix {
copydata2.commands = $(COPY_DIR) $$PWD/xml/fmd $$OUT_PWD
first.depends = $(first) copydata2
export(first.depends)
export(copydata2.commands)
QMAKE_EXTRA_TARGETS += first copydata2
}
