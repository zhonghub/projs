#include "z_include/mywindow.h"

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
    server_ip = my_sql_use::getLocalIPAddress();
    server_port = "12345";
    // �����˵���
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

    // �����ı���
    QHBoxLayout* layout = new QHBoxLayout;
    lineEdit1 = new QTextEdit(this);
    // lineEdit1->setText("Server is not started!");
    lineEdit2 = new QTextEdit(this);
    lineEdit2->setText("There is not user online!");

    lineEdit1->setReadOnly(true);
    lineEdit2->setReadOnly(true);  // �ڶ����ı�������Ϊֻ��

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
    // ����ѡ��1�ĵ���¼�
    getSetPortWindow();
    setPortWindow->show();
}

void MyWindow::onOption2Clicked()
{
    // ����ѡ��2�ĵ���¼�
    close();
}

/*
���ð󶨷�����IP�Ͷ˿ں�
*/
void MyWindow::getSetPortWindow() {
    setPortWindow = new QMainWindow(this);
    setPortWindow->resize(550, 350);  // ���ô��ڴ�СΪ800x600

    setPortWindow->setWindowTitle("��IP�Ͷ˿ں�");

    QWidget* centralWidget = new QWidget(setPortWindow);
    setPortWindow->setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    // ����
    mainLayout->setAlignment(Qt::AlignHCenter);

    // ������IP
    QLabel* usernameLabel = new QLabel("������IP: ", setPortWindow);
    ipLineEdit = new QLineEdit(setPortWindow);
    ipLineEdit->setReadOnly(true);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(ipLineEdit);

    // �˿ں�
    QLabel* passwordLabel = new QLabel("�˿�: ", setPortWindow);
    portLineEdit = new QLineEdit(setPortWindow);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(portLineEdit);

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    setButton = new QPushButton("���ö˿ں�", setPortWindow);
    connect(setButton, &QPushButton::clicked, this, &MyWindow::handleSetPort);
    buttonLayout->addWidget(setButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // �����ı������Ĵ�С
    ipLineEdit->setMinimumSize(400, 60);
    ipLineEdit->setMaximumSize(600, 100);

    portLineEdit->setMinimumSize(400, 60);
    portLineEdit->setMaximumSize(600, 100);

    // ���ð�ť�Ĵ�С
    setButton->setFixedSize(200, 50);

    ipLineEdit->setText(QString::fromStdString(server_ip));
    portLineEdit->setText(QString::fromStdString(server_port));//QString::fromStdString(server_port + "111")

    //std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
    //setPortWindow->setWindowTitle(QString::fromStdString(str1));

}

void MyWindow::handleSetPort() {
    if (portLineEdit->text().isEmpty()) {
        QMessageBox::critical(this->setPortWindow, "����", "�˿ںŲ���Ϊ��!");
    }
    else {
        server_port = portLineEdit->text().toStdString();
        std::string str1 = "Server: " + server_ip + "    Port: " + server_port;
        setWindowTitle(QString::fromStdString(str1));
        setPortWindow->setWindowTitle(QString::fromStdString(str1));
        QMessageBox::information(this, "��ʾ", QString::fromStdString("���óɹ�!"));
    }
}

