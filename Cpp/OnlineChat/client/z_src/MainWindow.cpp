#include "z_include/MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server_ip = "127.0.0.1";
    server_port = "12345";
    loginButton = nullptr;
    registerButton = nullptr;
    setUI1();
}

void MainWindow::setUI1() {
    resize(650, 500);  // 设置窗口大小为800x600
    //adjustSize();  // 自动适应内容调整窗口大小
    createMenus();
    setWindow();
}

/*
添加菜单栏按钮，并绑定事件
*/
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Menu"));

    openAction = new QAction(tr("Set"), this);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);
    fileMenu->addAction(openAction);

    exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onFileExit);
    fileMenu->addAction(exitAction);
}

/*
菜单栏按钮Open绑定事件
*/
void MainWindow::onFileOpen()
{
    // 获取文件路径
    // QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All Files (*)"));
    // 处理打开文件的逻辑
    //
    setWindow2();
    setIPWindow->show();
}

/*
菜单栏按钮Exit绑定事件
*/
void MainWindow::onFileExit()
{
    close();
    // 处理退出程序的逻辑
}

/*
设置登录页面UI,并为按钮绑定事件
*/
void MainWindow::setWindow() {

    std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
    setWindowTitle(QString::fromStdString(str1));

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    // 居中
    mainLayout->setAlignment(Qt::AlignHCenter);

    // Username
    QLabel* usernameLabel = new QLabel("用户: ", this);
    usernameLineEdit = new QLineEdit(this);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameLineEdit);

    // Password
    QLabel* passwordLabel = new QLabel("密码: ", this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordLineEdit);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    loginButton = new QPushButton("Login登录", this);
    // connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    buttonLayout->addWidget(loginButton);

    registerButton = new QPushButton("Register注册", this);
    // connect(registerButton, &QPushButton::clicked, this, &MainWindow::handleRegister);
    buttonLayout->addWidget(registerButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // 设置文本输入框的大小
    usernameLineEdit->setMinimumSize(500, 60);
    usernameLineEdit->setMaximumSize(800, 100);

    passwordLineEdit->setMinimumSize(500, 60);
    passwordLineEdit->setMaximumSize(800, 100);

    // 设置按钮的大小
    registerButton->setFixedSize(200, 50);
    loginButton->setFixedSize(200, 50);
}

/*
设置绑定服务器IP和端口号
*/
void MainWindow::setWindow2() {
    setIPWindow = new QMainWindow(this);

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
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(ipLineEdit);

    // 端口号
    QLabel* passwordLabel = new QLabel("端口: ", setIPWindow);
    portLineEdit = new QLineEdit(setIPWindow);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(portLineEdit);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    setButton = new QPushButton("绑定", setIPWindow);
    connect(setButton, &QPushButton::clicked, this, &MainWindow::handleSetIP);
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

    std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
    setIPWindow->setWindowTitle(QString::fromStdString(str1));

}

/*
* 绑定服务器IP和端口号
*/
void MainWindow::handleSetIP() {
    if (ipLineEdit->text().isEmpty() || portLineEdit->text().isEmpty()) {
        QMessageBox::critical(this->setIPWindow, "警告", "IP或端口号不能为空!");
    }
    else {
        server_ip = ipLineEdit->text().toStdString();
        server_port = portLineEdit->text().toStdString();
        std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
        setWindowTitle(QString::fromStdString(str1));
        setIPWindow->setWindowTitle(QString::fromStdString(str1));
        QMessageBox::information(this, "提示", QString::fromStdString("设置成功!"));
    }
}