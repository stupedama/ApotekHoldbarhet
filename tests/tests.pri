# libraies
INCLUDEPATH += \
            ../../app/external/tinyxml2 \
            ../../app/external/date/include/date \
            ../../app/src/ \
            ../../app/src/fest_xml \
            ../../app/src/fmd

SOURCES += \
        ../../app/external/tinyxml2/tinyxml2.cpp \
        ../../app/src/fest_xml/fest_reader.cpp \
        ../../app/src/database.cpp \
        ../../app/src/product.cpp \
        ../../app/src/check_numbers.cpp \
        ../../app/src/fmd/fmd_decoder.cpp \
        ../../app/src/fmd/fmd_xml_reader.cpp

HEADERS += \
        ../../app/external/date/include/date/date.h \
        ../../app/src/version.h \
        ../../app/src/product.h \
        ../../app/src/fest_xml/fest_reader.h \
        ../../app/src/database.h \
        ../../app/src/table_colors.h \
        ../../app/src/check_numbers.h \
        ../../app/src/constants.h \
        ../../app/src/fmd/fmd_decoder.h \
        ../../app/src/fmd/escape_codes.h \
        ../../app/src/fmd/fmd_xml_reader.h
