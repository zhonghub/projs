#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    std::string server_ip;
    std::string server_port;

    QPushButton* loginButton;
    QPushButton* registerButton;

    MainWindow(QWidget *parent = nullptr);
    void setUI1();

    QString getUsername() {
        return usernameLineEdit->text();
    }

    QString getPassword() {
        return passwordLineEdit->text();
    }

private slots:
    void onFileOpen();
    void onFileExit();
    void handleSetIP();
    // 其他槽函数
    // 登录按钮绑定事件
    // void handleLogin();
    // 注册按钮绑定事件
    // void handleRegister();

   
private:

    QMenu *fileMenu;
    QAction *openAction;
    QAction *exitAction;
    QMainWindow* setIPWindow;
 
    //用户名
    QLineEdit* usernameLineEdit;
    // 密码
    QLineEdit* passwordLineEdit;
    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;
    QPushButton* setButton;

    void createMenus();
    void setWindow();
    void setWindow2();

};

#endif // MAINWINDOW_H
