#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "sql.h"

#include <QMenuBar>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

#include <vector>
#include <string>
#include <thread>
#include <mutex>


// 定义一个互斥量，用于保护对QWidget的访问
static std::mutex guiMutex1;
static std::mutex guiMutex2;

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget* parent = nullptr); // 私有构造函数
    static MyWindow* getInstance(QWidget* parent = nullptr);

    std::string server_ip;
    std::string server_port;
    QTextEdit* lineEdit1;
    QTextEdit* lineEdit2;
    QAction* startAction;
    QAction* stopAction;

public slots:
    void updateGui1(const QString& newVector)
    {
        // 这里执行需要在主线程中执行的GUI更新操作
        std::lock_guard<std::mutex> lock(guiMutex1);
        // 更新QWidget的状态或执行其他GUI操作
        lineEdit1->setText(newVector);
        // lineEdit1->setText(QString::fromStdString(newVector));
    }
    void updateGui2(const QString& newVector)
    {
        // 这里执行需要在主线程中执行的GUI更新操作
        std::lock_guard<std::mutex> lock(guiMutex2);
        // 更新QWidget的状态或执行其他GUI操作
        lineEdit2->setText(newVector);
    }

private slots:
    void onOption1Clicked();
    void onOption2Clicked();
    void handleSetPort();
    // void getIP();
   

private:
    // MyWindow(QWidget* parent = nullptr); // 私有构造函数
    static MyWindow* instance; // 静态指针，用于保存单例对象

    QMenu* fileMenu;
    QAction* openAction;
    QAction* exitAction;

    QMainWindow* setPortWindow;
    QLineEdit* ipLineEdit;
    QLineEdit* portLineEdit;
    QPushButton* setButton;
    void MyWindow::getSetPortWindow();
};


#endif // MYWINDOW_H
