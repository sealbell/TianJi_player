#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // 函数声明

private slots:
    void on_previous_btn_clicked();

    void on_control_btn_clicked();

    void on_next_btn_clicked();

    void on_sound_btn_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
