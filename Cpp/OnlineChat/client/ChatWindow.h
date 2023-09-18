#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "Client2.h"
#include "myUseFun.h"

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <string>
#include <vector>
#include <sstream>
#include <mutex>
#include <WinSock2.h>


// 定义一个互斥量，用于保护对QWidget的访问
static std::mutex guiMutex1;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    Client2* client2;
    ChatWindow(QWidget* parent = nullptr);

private slots:
    void onSendMessage();
    void onGetMessage();

public slots:
    void updateGui1(const QString& newMsg)
    {
        // 这里执行需要在主线程中执行的GUI更新操作
        std::lock_guard<std::mutex> lock(guiMutex1);
        // 更新QWidget的状态或执行其他GUI操作
        this->chatDisplay->append(newMsg);
        // lineEdit1->setText(QString::fromStdString(newVector));
    }

private:

    QTextEdit* chatDisplay;
    QTextEdit* messageInput;
    QPushButton* sendButton;
    QPushButton* getButton;

    void setUI();

    void setText(QTextEdit* chatDisplay, qreal size) {
        QTextCursor cursor = chatDisplay->textCursor();
        QTextCharFormat format;
        // Set the font size
        format.setFontPointSize(size); // Change this value to the desired font size
        cursor.mergeCharFormat(format);
        chatDisplay->mergeCurrentCharFormat(format);
    }
};


#endif // CHATWINDOW_H
