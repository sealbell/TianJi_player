#include "widget.h"
#include "ui_widget.h"

// 全局变量
int control = 0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_previous_btn_clicked()
{
    // 上一曲的槽函数
}

void Widget::on_control_btn_clicked()
{
    // 控制按钮的槽函数
    if(control){
        // 播放按钮
    }else{
        // 暂停按钮
    }
    return ;
}

void Widget::on_next_btn_clicked()
{
    // 下一曲按钮
}

void Widget::on_sound_btn_clicked()
{
    // 音量按钮
}
