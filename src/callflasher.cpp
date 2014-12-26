/*
 * (C) 2014 Kimmo Lindholm <kimmo.lindholm@gmail.com> Kimmoli
 *
 * Callflasher main
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "flasher.h"
#include "adaptor.h"
#include <QtCore/QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    setlinebuf(stdout);
    setlinebuf(stderr);

    printf("callflasher: starting daemon version %s\n", APPVERSION);

    Flasher flasher;
    new CallflasherAdaptor(&flasher);

    flasher.registerDBus();

    static QDBusConnection mceCallStateconn = QDBusConnection::systemBus();
    mceCallStateconn.connect("com.nokia.mce", "/com/nokia/mce/signal", "com.nokia.mce.signal", "sig_call_state_ind",
                          &flasher, SLOT(handleCall(const QDBusMessage&)));

    return app.exec();
}
