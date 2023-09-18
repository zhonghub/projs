#include "mywindow.h"

MyWindow* MyWindow::instance = nullptr; // 初始化静态指针为 nullpt

MyWindow* MyWindow::getInstance(QWidget* parent)
{
    if (instance == nullptr) // 如果实例尚未创建
    {
        instance = new MyWindow(parent); // 创建一个新实例
    }
    return instance; // 返回现有实例
}

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 创建菜单栏
    fileMenu = menuBar()->addMenu("menu");

    openAction = new QAction(tr("Option1"), this);
    connect(openAction, &QAction::triggered, this, &MyWindow::onOption1Clicked);
    fileMenu->addAction(openAction);

    exitAction = new QAction(tr("Option2"), this);
    connect(exitAction, &QAction::triggered, this, &MyWindow::onOption2Clicked);
    fileMenu->addAction(exitAction);

    // 创建文本框
    QHBoxLayout* layout = new QHBoxLayout;
    lineEdit1 = new QTextEdit(this);
    lineEdit2 = new QTextEdit(this);
    // textEdit2->setReadOnly(true);  // 第二个文本框设置为只读
    lineEdit1->setMinimumHeight(800);
    lineEdit2->setMinimumHeight(800);

    lineEdit1->setMinimumWidth(500);
    lineEdit2->setMaximumWidth(700);
    lineEdit2->setMinimumWidth(200);
    lineEdit2->setMaximumWidth(300);

    layout->addWidget(lineEdit1);
    layout->addWidget(lineEdit2);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
