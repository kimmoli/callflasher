#ifndef FLASHER_H
#define FLASHER_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QTimer>
#include <QThread>
#include <mlite5/MNotification>
#include <mlite5/mdesktopentry.h>

#define SERVICE_NAME "com.kimmoli.callflasher"

class QDBusInterface;
class Flasher : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", SERVICE_NAME)

public:
    explicit Flasher(QObject *parent = 0);
    virtual ~Flasher();
    void registerDBus();
    void showNotification(QString text);

public slots:
    void enable();
    void toggle();
    void quit();

    void handleCall(const QDBusMessage&);
    void blinkTimerTimeout();

private:
    bool m_dbusRegistered;
    bool m_enabled;
    bool m_incomingCall;
    QTimer *m_blinkTimer;

};

#endif // FLASHER_H
