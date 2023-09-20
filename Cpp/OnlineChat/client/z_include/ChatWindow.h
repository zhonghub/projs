#ifndef CHATWINDOW_H
#define CHATWINDOW_H


#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <string>
#include <mutex>


// ����һ�������������ڱ�����QWidget�ķ���
static std::mutex guiMutex1;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton* sendButton;
    QPushButton* getButton;

    ChatWindow(QWidget* parent = nullptr);

    QString getMsg() {
        return messageInput->toPlainText();
    }
    void clearMsg() {
        messageInput->clear();
    }
    void setChatText(std::string recv) {
        chatDisplay->setText(QString::fromStdString(recv));
    }

public slots:
    void updateGui1(const QString& newMsg)
    {
        // ����ִ����Ҫ�����߳���ִ�е�GUI���²���
        std::lock_guard<std::mutex> lock(guiMutex1);
        // ����QWidget��״̬��ִ������GUI����
        this->chatDisplay->append(newMsg);
    }

private:

    QTextEdit* chatDisplay;
    QTextEdit* messageInput;

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
