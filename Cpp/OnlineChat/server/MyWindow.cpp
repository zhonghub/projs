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
    server_ip = my_sql_use::getLocalIPAddress();
    server_port = "12345";
    // 创建菜单栏
    fileMenu = menuBar()->addMenu("menu");

    openAction = new QAction(tr("set"), this);
    connect(openAction, &QAction::triggered, this, &MyWindow::onOption1Clicked);
    fileMenu->addAction(openAction);

    startAction = new QAction(tr("start"), this);
    // connect(openAction, &QAction::triggered, this, &MyWindow::onOption1Clicked);
    fileMenu->addAction(startAction);

    stopAction = new QAction(tr("stop"), this);
    fileMenu->addAction(stopAction);

    exitAction = new QAction(tr("exit"), this);
    connect(exitAction, &QAction::triggered, this, &MyWindow::onOption2Clicked);
    fileMenu->addAction(exitAction);

    // 创建文本框
    QHBoxLayout* layout = new QHBoxLayout;
    lineEdit1 = new QTextEdit(this);
    lineEdit2 = new QTextEdit(this);

    lineEdit1->setReadOnly(true);
    lineEdit2->setReadOnly(true);  // 第二个文本框设置为只读

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

void MyWindow::onOption1Clicked()
{
    // 处理选项1的点击事件
    setPortWindow();
    setIPWindow->show();
}

void MyWindow::onOption2Clicked()
{
    // 处理选项2的点击事件
    close();
}

/*
设置绑定服务器IP和端口号
*/
void MyWindow::setPortWindow() {
    setIPWindow = new QMainWindow();
    setIPWindow->resize(550, 350);  // 设置窗口大小为800x600

    setIPWindow->setWindowTitle("绑定IP和端口号");

    QWidget* centralWidget = new QWidget(setIPWindow);
    setIPWindow->setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    // 居中
    mainLayout->setAlignment(Qt::AlignHCenter);

    // 服务器IP
    QLabel* usernameLabel = new QLabel("服务器IP: ", setIPWindow);
    ipLineEdit = new QLineEdit(setIPWindow);
    ipLineEdit->setReadOnly(true);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(ipLineEdit);

    // 端口号
    QLabel* passwordLabel = new QLabel("端口: ", setIPWindow);
    portLineEdit = new QLineEdit(setIPWindow);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(portLineEdit);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    setButton = new QPushButton("设置端口号", setIPWindow);
    connect(setButton, &QPushButton::clicked, this, &MyWindow::handleSetPort);
    buttonLayout->addWidget(setButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // 设置文本输入框的大小
    ipLineEdit->setMinimumSize(400, 60);
    ipLineEdit->setMaximumSize(600, 100);

    portLineEdit->setMinimumSize(400, 60);
    portLineEdit->setMaximumSize(600, 100);

    // 设置按钮的大小
    setButton->setFixedSize(200, 50);

    ipLineEdit->setText(QString::fromStdString(server_ip));
    portLineEdit->setText(QString::fromStdString(server_port));//QString::fromStdString(server_port + "111")

    //std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
    //setIPWindow->setWindowTitle(QString::fromStdString(str1));

}

void MyWindow::handleSetPort() {
    if (portLineEdit->text().isEmpty()) {
        QMessageBox::critical(this->setIPWindow, "警告", "端口号不能为空!");
    }
    else {
        server_port = portLineEdit->text().toStdString();
        std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
        setWindowTitle(QString::fromStdString(str1));
        setIPWindow->setWindowTitle(QString::fromStdString(str1));
        QMessageBox::information(this, "提示", QString::fromStdString("设置成功!"));
    }
}

