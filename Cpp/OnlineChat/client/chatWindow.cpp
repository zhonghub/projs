#include "chatWindow.h"

ChatWindow::ChatWindow(QWidget* parent)
    : QMainWindow(parent)
{
    client2 = Client2::getInstance();
    // 注册QTextCursor类型
    qRegisterMetaType<QTextCursor>("QTextCursor");
    setUI();
}

void ChatWindow::setUI() {
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
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);

    getButton = new QPushButton("Get History Msg", this);
    getButton->setFixedSize(230, 60);
    connect(getButton, &QPushButton::clicked, this, &ChatWindow::onGetMessage);

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

/*
发布按钮绑定事件
*/
void ChatWindow::onSendMessage()
{
    // QString::fromStdString(recv)
    // message0.toStdString()
    QString message0 = messageInput->toPlainText();
    std::string str1 = "3#" + this->windowTitle().toStdString() + "#"; // GetLocalIPAddress()
    std::string message = str1 + message0.toStdString() + "#" + myUse::getCurrentTimeAsString();
    if (!message0.isEmpty())
    {
        // chatDisplay->append(this->windowTitle() + ": " + message0);
        messageInput->clear();
        std::string recv = client2->sendStrToServer(message);
        // chatDisplay->append(QString::fromStdString("Recived: " + recv));
    }
}

/*
获取历史消息按钮绑定事件
*/
void ChatWindow::onGetMessage()
{
    std::string sendstr = "4#" + this->windowTitle().toStdString() + "#get#" + myUse::getCurrentTimeAsString();
    std::string recv = client2->sendStrToServer(sendstr);
    if (!recv.empty() && recv.back() == '\n') {
        recv.pop_back();
    }
    chatDisplay->setText(QString::fromStdString(recv));
    //QString chineseText = "\n以上为全部历史消息\n";
    //QString utf16String = QString::fromUtf8(chineseText.toUtf8()); // 转换为UTF-16编码的QString
    //chatDisplay->append(utf16String);
}

