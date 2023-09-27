#ifndef ServerController_H
#define ServerController_H

#include "sql.h"
#include "server.h"
#include "MyWindow.h"

#include <QObject>
#include <QPushButton>

class ServerController : public QObject
{
    Q_OBJECT

public slots:
    void handleStart();
    void ServerController::handleStop();

public:
    ServerController();
    ~ServerController();

    Server * mainServer;
    MyWindow * myWindow;

};

#endif // ServerController

