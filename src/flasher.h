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
    bool registerDBus();
    void showNotification(QString text);
    void setPath(QString path, QString on, QString off);

public slots:
    void enable();
    void toggle();
    void test();
    void quit();

    void handleCall(const QDBusMessage&);
    void blinkTimerTimeout();

private:
    bool m_dbusRegistered;
    bool m_enabled;
    bool m_incomingCall;
    QString m_path;
    QString m_on;
    QString m_off;
    QTimer *m_blinkTimer;

};

#endif // FLASHER_H
