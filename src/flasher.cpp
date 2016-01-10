#include <stdio.h>
#include "flasher.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

static const char *SERVICE = SERVICE_NAME;
static const char *PATH = "/";

Flasher::Flasher(QObject *parent) :
    QObject(parent)
{
    m_dbusRegistered = false;
    m_enabled = false;
    m_incomingCall = false;

    m_blinkTimer = new QTimer(this);
    m_blinkTimer->setInterval(500);
    connect(m_blinkTimer, SIGNAL(timeout()), this, SLOT(blinkTimerTimeout( )));

    enable();
}

Flasher::~Flasher()
{
    if (m_dbusRegistered)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject(PATH);
        connection.unregisterService(SERVICE);

        printf("callflasher: unregistered from dbus sessionBus\n");
    }
}

void Flasher::registerDBus()
{
    if (!m_dbusRegistered)
    {
        // DBus
        QDBusConnection connection = QDBusConnection::sessionBus();
        if (!connection.registerService(SERVICE))
        {
            QCoreApplication::quit();
            return;
        }

        if (!connection.registerObject(PATH, this))
        {
            QCoreApplication::quit();
            return;
        }
        m_dbusRegistered = true;

        printf("callflasher: succesfully registered to dbus sessionBus \"%s\"\n", SERVICE);
    }
}

void Flasher::quit()
{
    printf("callflasher: quit requested from dbus\n");
    QCoreApplication::quit();
}

void Flasher::enable()
{
    m_enabled = true;
    printf("callflasher: enabled\n");
    showNotification("Call flasher enabled");
}

void Flasher::toggle()
{
    m_enabled = !m_enabled;
    
    if (m_enabled)
    {
        printf("callflasher: enabled\n");
        showNotification("Call flasher enabled");
    }
    else
    {
        m_blinkTimer->stop();
        printf("callflasher: disabled\n");
        showNotification("Call flasher disabled");
    }
}

void Flasher::handleCall(const QDBusMessage & msg)
{
    QList<QVariant> args = msg.arguments();

    printf("callflasher: sig_call_state_ind says: %s\n", qPrintable(args.at(0).toString()));

    if ( !(QString::localeAwareCompare( args.at(0).toString(), "ringing")) )
    {
        printf("callflasher: Incoming call\n");
        m_incomingCall = true;
        if (m_enabled)
            m_blinkTimer->start();
    }
    else if ( m_incomingCall && !(QString::localeAwareCompare( args.at(0).toString(), "active")) )
    {
        printf("callflasher: Call answered or placing new call when missed call indicated\n");
        m_incomingCall = false;
        m_blinkTimer->stop();
    }
    else if (!(QString::localeAwareCompare( args.at(0).toString(), "none")) )
    {
        printf("callflasher: Call ended\n");
        m_incomingCall = false;
        m_blinkTimer->stop();
    }
}

void Flasher::showNotification(QString text)
{
    MNotification notification(MNotification::DeviceEvent, "", text);
    notification.setImage("/usr/share/themes/jolla-ambient/meegotouch/icons/icon-system-resources.png");
    notification.publish();
}

void Flasher::blinkTimerTimeout()
{
    if (!m_enabled)
    {
        m_blinkTimer->stop();
        return;
    }

    int fd = open("/sys/kernel/debug/flash_adp1650/mode", O_WRONLY);

    int tmp;
    Q_UNUSED(tmp);

    if (!(fd < 0))
    {
        tmp = write (fd, "1", 1);
        close(fd);
    }
    QThread::msleep(10);

    fd = open("/sys/kernel/debug/flash_adp1650/mode", O_WRONLY);

    if (!(fd < 0))
    {
        tmp = write (fd, "0", 1);
        close(fd);
    }
}
