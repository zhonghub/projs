#include "chatWindow.h"

ChatWindow::ChatWindow(QWidget* parent)
    : QMainWindow(parent)
{
    client2 = Client2::getInstance();
    // ע��QTextCursor����
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
    // ���ð�ť��ˮƽ�����Ͼ��ж���
    buttonLayout->setAlignment(Qt::AlignHCenter);
    layout->addLayout(buttonLayout);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

/*
������ť���¼�
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
��ȡ��ʷ��Ϣ��ť���¼�
*/
void ChatWindow::onGetMessage()
{
    std::string sendstr = "4#" + this->windowTitle().toStdString() + "#get#" + myUse::getCurrentTimeAsString();
    std::string recv = client2->sendStrToServer(sendstr);
    if (!recv.empty() && recv.back() == '\n') {
        recv.pop_back();
    }
    chatDisplay->setText(QString::fromStdString(recv));
    //QString chineseText = "\n����Ϊȫ����ʷ��Ϣ\n";
    //QString utf16String = QString::fromUtf8(chineseText.toUtf8()); // ת��ΪUTF-16�����QString
    //chatDisplay->append(utf16String);
}

