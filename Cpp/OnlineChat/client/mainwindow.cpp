#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(500, 400);  // 设置窗口大小为800x600
    // adjustSize();  // 自动适应内容调整窗口大小

    // 绑定客户端实例
    client = Client::getInstance();
    client2 = Client2::getInstance();

    createMenus();
    setWindow();
}

/*
添加菜单栏按钮，并绑定事件
*/
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("File"));

    openAction = new QAction(tr("Open"), this);
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All Files (*)"));
    // 处理打开文件的逻辑
    usernameLineEdit->setText(fileName);
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

    setWindowTitle("Login");

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
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    buttonLayout->addWidget(loginButton);

    registerButton = new QPushButton("Register注册", this);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::handleRegister);
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
登录按钮绑定事件
*/
void MainWindow::handleLogin()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString message = "1#" + username + "#" + password + "#" + QString::fromStdString(myUse::getCurrentTimeAsString());

    client->init("127.0.0.1", 12345);
    client2->init("127.0.0.1", 12345);

    // 发送登录消息，并接收登录结果
    std::string recv = client->sendStrToServer(message.toStdString());
    // 处理登录结果
    if (recv._Equal("ok")) {
        // 创建消息发布窗口
        ChatWindow* mainWindow = new ChatWindow();
        mainWindow->setWindowTitle(username);
        mainWindow->show();

        client->setUserName(username.toStdString(), mainWindow);
        std::cout << "开启一个子线程2: 专门接收服务器广播的新消息." << std::endl;
        // 开启一个子线程专门接收服务器广播的新消息
        std::thread receiveThread(&Client::Receive_New_Messages, client);
        receiveThread.detach();

        // 关闭登录页面
        this->close();
        QMessageBox::information(this, "登录成功", QString::fromStdString(recv));
    }
    else {
        QMessageBox::critical(this, "登录失败", QString::fromStdString(recv));
    }
}

/*
注册按钮绑定事件
*/
void MainWindow::handleRegister()
{
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString message = "2#" + username + "#" + password + "#" + QString::fromStdString(myUse::getCurrentTimeAsString());

    client->init("127.0.0.1", 12345);
    client2->init("127.0.0.1", 12345);

    // 发送注册消息，并接收注册结果
    std::string recv = client->sendStrToServer(message.toStdString());
    // 处理注册结果
    if (recv._Equal("ok")) {
        // 创建消息发布窗口
        ChatWindow* mainWindow = new ChatWindow();
        mainWindow->setWindowTitle(username);
        mainWindow->show();

        client->setUserName(username.toStdString(), mainWindow);
        std::cout << "开启一个子线程2: 专门接收服务器广播的新消息." << std::endl;
        // 开启一个子线程专门接收服务器广播的新消息
        std::thread receiveThread(&Client::Receive_New_Messages, client);
        receiveThread.detach();

        // 关闭注册页面
        this->close();
        QMessageBox::information(this, "注册成功", QString::fromStdString(recv));
    }
    else {
        QMessageBox::critical(this, "注册失败", QString::fromStdString(recv));
    }
}
