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

    if (!QDBusConnection::systemBus().isConnected())
    {
        printf("callflasher: Cannot connect to the D-Bus systemBus\n%s\n", qPrintable(QDBusConnection::systemBus().lastError().message()));
        return EXIT_FAILURE;
    }

    QDBusInterface ssuCall("org.nemo.ssu", "/org/nemo/ssu", "org.nemo.ssu", QDBusConnection::systemBus());
    ssuCall.setTimeout(1000);

    QList<QVariant> args;
    args.append(2);

    QDBusMessage ssuCallReply = ssuCall.callWithArgumentList(QDBus::Block, "displayName", args);

    if (ssuCallReply.type() == QDBusMessage::ErrorMessage)
    {
        printf("callflasher: Error: %s\n", qPrintable(ssuCallReply.errorMessage()));
        return EXIT_FAILURE;
    }

    QList<QVariant> outArgs = ssuCallReply.arguments();
    if (outArgs.count() == 0)
    {
        printf("callflasher: ssu reply is epmty\n");
        return EXIT_FAILURE;
    }

    QString deviceName = outArgs.at(0).toString();

    printf("callflasher: device name is %s\n", qPrintable(deviceName));

    if (outArgs.at(0).toString() == "JP-1301") /* The one and only original Jolla phone */
    {
        flasher.setPath("/sys/kernel/debug/flash_adp1650/mode", "1", "0");
    }
    else if (outArgs.at(0).toString() == "onyx") /* OneplusX */
    {
        flasher.setPath("/sys/class/leds/led:flash_torch/brightness", "50", "0");
    }
    else if (outArgs.at(0).toString() == "fp2-sibon")
    {
        flasher.setPath("/sys/class/leds/led:flash_torch/brightness", "50", "0");
    }
    else
    {
        printf("callflasher: device not supported\n");
        return EXIT_FAILURE;
    }

    if (!flasher.registerDBus())
    {
        printf("callflasher: failed to reqister to dbus\n");
        return EXIT_FAILURE;
    }

    static QDBusConnection mceCallStateconn = QDBusConnection::systemBus();
    mceCallStateconn.connect("com.nokia.mce", "/com/nokia/mce/signal", "com.nokia.mce.signal", "sig_call_state_ind",
                          &flasher, SLOT(handleCall(const QDBusMessage&)));

    return app.exec();
}
