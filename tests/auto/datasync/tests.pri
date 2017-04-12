QT += datasync

HEADERS += \
	$$PWD/testdata.h \
    $$PWD/mocklocalstore.h \
    $$PWD/tst.h \
    $$PWD/mockstateholder.h \
    $$PWD/mockremoteconnector.h \
    $$PWD/mockdatamerger.h \
    $$PWD/testobject.h

SOURCES += \
	$$PWD/testdata.cpp \
    $$PWD/mocklocalstore.cpp \
    $$PWD/tst.cpp \
    $$PWD/mockstateholder.cpp \
    $$PWD/mockremoteconnector.cpp \
    $$PWD/mockdatamerger.cpp \
    $$PWD/testobject.cpp

INCLUDEPATH += $$PWD

mac: QMAKE_LFLAGS += '-Wl,-rpath,\'$$OUT_PWD/../../../../lib\''