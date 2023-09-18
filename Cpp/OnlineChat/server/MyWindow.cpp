#include "mywindow.h"

MyWindow* MyWindow::instance = nullptr; // ��ʼ����ָ̬��Ϊ nullpt

MyWindow* MyWindow::getInstance(QWidget* parent)
{
    if (instance == nullptr) // ���ʵ����δ����
    {
        instance = new MyWindow(parent); // ����һ����ʵ��
    }
    return instance; // ��������ʵ��
}

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // �����˵���
    fileMenu = menuBar()->addMenu("menu");

    openAction = new QAction(tr("Option1"), this);
    connect(openAction, &QAction::triggered, this, &MyWindow::onOption1Clicked);
    fileMenu->addAction(openAction);

    exitAction = new QAction(tr("Option2"), this);
    connect(exitAction, &QAction::triggered, this, &MyWindow::onOption2Clicked);
    fileMenu->addAction(exitAction);

    // �����ı���
    QHBoxLayout* layout = new QHBoxLayout;
    lineEdit1 = new QTextEdit(this);
    lineEdit2 = new QTextEdit(this);
    // textEdit2->setReadOnly(true);  // �ڶ����ı�������Ϊֻ��
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
