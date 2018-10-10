QT += network
QT += core

QMAKE_CXXFLAGS += -std=c++11

TARGET = Tcp
TEMPLATE = lib

DEFINES += TCP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


HEADERS += \
    tcp_link.h \
    tcp_configuration.h \
    tcp_global.h \
    i_link_events.h \
    tcp_setup.h

SOURCES += \
    tcp_link.cpp \
    tcp_configuration.cpp \
    tcp_setup.cpp


# Unix lib Install
unix:!symbian {
    target.path = $$PWD/../lib
    INSTALLS += target
}

lib.path    = $$PWD/../lib
win32:CONFIG(release, debug|release):       lib.files   += release/Tcp.lib release/Tcp.dll
else:win32:CONFIG(debug, debug|release):    lib.files   += debug/Tcp.lib debug/Tcp.dll
INSTALLS += lib


#Header file copy
headers.path    = $$PWD/../include
headers.files   += $$HEADERS
INSTALLS       += headers
