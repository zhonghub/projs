#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatWindow.h"
#include "client.h"
#include "Client2.h"

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

/*
登录/注册界面
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onFileOpen();
    void onFileExit();
    // 其他槽函数
    // 登录按钮绑定事件
    void handleLogin();
    // 注册按钮绑定事件
    void handleRegister();
   
private:
    Client* client;
    Client2* client2;

    QMenu *fileMenu;
    QAction *openAction;
    QAction *exitAction;

    QPushButton* loginButton;
    QPushButton* registerButton;
 
    //用户名
    QLineEdit* usernameLineEdit;
    // 密码
    QLineEdit* passwordLineEdit;

    void createMenus();
    void setWindow();

};

#endif // MAINWINDOW_H
