TARGET = harbour-callflasher

CONFIG += link_pkgconfig
PKGCONFIG += mlite5

QT += dbus
QT -= gui

# DBus
system(qdbusxml2cpp config/com.kimmoli.callflasher.xml -i src/flasher.h -a src/adaptor)

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

target.path = /usr/bin/

dbusService.files = config/com.kimmoli.callflasher.service
dbusService.path = /usr/share/dbus-1/services/

dbusInterface.files = config/com.kimmoli.callflasher.xml
dbusInterface.path = /usr/share/dbus-1/interfaces/

desktop.files = config/harbour-callflasher.desktop
desktop.path = /usr/share/applications/

icon.files = config/harbour-callflasher.png
icon.path = /usr/share/icons/hicolor/86x86/apps/

INSTALLS += target dbusService dbusInterface desktop icon

message($${DEFINES})

SOURCES += \
    src/callflasher.cpp \
    src/adaptor.cpp \
    src/flasher.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    config/com.kimmoli.callflasher.service \
    config/com.kimmoli.callflasher.xml \
    config/harbour-callflasher.png \
    config/harbour-callflasher.desktop

HEADERS += \
    src/adaptor.h \
    src/flasher.h
