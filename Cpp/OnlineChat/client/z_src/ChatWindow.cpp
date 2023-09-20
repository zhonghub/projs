#include "z_include/chatWindow.h"

ChatWindow::ChatWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 注册QTextCursor类型
    qRegisterMetaType<QTextCursor>("QTextCursor");
    setUI();
}


void ChatWindow::setUI(){
    // Create UI elements
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    setText(chatDisplay, 12);

    messageInput = new QTextEdit(this);
    setText(messageInput, 13);

    chatDisplay->setMinimumHeight(400);
    messageInput->setMinimumHeight(200);

    chatDisplay->setMinimumWidth(800);
    messageInput->setMinimumWidth(800);

    sendButton = new QPushButton("Send", this);
    sendButton->setFixedSize(230, 60);
    // connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);
    
    getButton = new QPushButton("Get History Msg", this);
    getButton->setFixedSize(230, 60);
    // connect(getButton, &QPushButton::clicked, this, &ChatWindow::onGetMessage);

    // Set up layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignHCenter);

    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(getButton);
    // 设置按钮在水平方向上居中对齐
    buttonLayout->setAlignment(Qt::AlignHCenter);
    layout->addLayout(buttonLayout);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

