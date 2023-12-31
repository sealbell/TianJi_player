#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//- 定义一个继承自 QMainWindow 的类，包括 UI 组件和事件处理。

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pervious_btn_clicked();

    void on_play_btn_clicked();

    void on_suspend_btn_clicked();

    void on_next_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
