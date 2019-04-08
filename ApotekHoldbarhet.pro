TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
        app \
        tests/TEST_xml \
        tests/TEST_db \
        tests/TEST_fmd \
        tests/TEST_fmd_xml


DISTFILES += \
        TODO.md \
        README.md \
        CHANGELOG.md

