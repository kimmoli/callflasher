TARGET = harbour-callflasher

CONFIG += link_pkgconfig
PKGCONFIG += mlite5

QT += dbus
QT -= gui

# DBus
system(qdbusxml2cpp config/com.kimmoli.callflasher.xml -i src/flasher.h -a src/adaptor)

DEFINES += "APPVERSION=\\\"$${SPECVERSION}\\\""

target.path = /usr/bin

dbusService.files = config/com.kimmoli.callflasher.service
dbusService.path = /usr/share/dbus-1/services/

systemduser.files = config/harbour-callflasher.service
systemduser.path = /etc/systemd/user/

dbusInterface.files = config/com.kimmoli.callflasher.xml
dbusInterface.path = /usr/share/dbus-1/interfaces/

INSTALLS += target dbusService dbusInterface systemduser

message($${DEFINES})

SOURCES += \
    src/callflasher.cpp \
    src/adaptor.cpp \
    src/flasher.cpp

OTHER_FILES += \
    rpm/$${TARGET}.spec \
    config/com.kimmoli.callflasher.service \
    config/com.kimmoli.callflasher.xml \
    config/harbour-callflasher.service

HEADERS += \
    src/adaptor.h \
    src/flasher.h
