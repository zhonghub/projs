#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMenuBar>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QTextEdit>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

// ����һ�������������ڱ�����QWidget�ķ���
static std::mutex guiMutex1;
static std::mutex guiMutex2;

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow(QWidget* parent = nullptr); // ˽�й��캯��
    static MyWindow* getInstance(QWidget* parent = nullptr);

    QTextEdit* lineEdit1;
    QTextEdit* lineEdit2;

public slots:
    void updateGui1(const QString& newVector)
    {
        // ����ִ����Ҫ�����߳���ִ�е�GUI���²���
        std::lock_guard<std::mutex> lock(guiMutex1);
        // ����QWidget��״̬��ִ������GUI����
        lineEdit1->setText(newVector);
        // lineEdit1->setText(QString::fromStdString(newVector));
    }
    void updateGui2(const QString& newVector)
    {
        // ����ִ����Ҫ�����߳���ִ�е�GUI���²���
        std::lock_guard<std::mutex> lock(guiMutex2);
        // ����QWidget��״̬��ִ������GUI����
        lineEdit2->setText(newVector);
    }

private slots:
    void onOption1Clicked()
    {
        // ����ѡ��1�ĵ���¼�
        lineEdit1->setText("Clear the messages!");
    }

    void onOption2Clicked()
    {
        // ����ѡ��2�ĵ���¼�
        lineEdit2->setText("Clear the users!");
    }

private:
    // MyWindow(QWidget* parent = nullptr); // ˽�й��캯��
    static MyWindow* instance; // ��ָ̬�룬���ڱ��浥������

    QMenu* fileMenu;
    QAction* openAction;
    QAction* exitAction;
};

#endif // MYWINDOW_H
