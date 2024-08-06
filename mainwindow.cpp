#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login_dlg_ = new LoginDialog(this);
    setCentralWidget(login_dlg_);
    // login_dlg_->show();

    // 创建和注册消息连接
    connect(login_dlg_, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchRegister);
    reg_dlg_ = new RegisterDialog(this);

    login_dlg_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    reg_dlg_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    reg_dlg_->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    // if(login_dlg_) {
    //     delete login_dlg_;
    //     login_dlg_ = nullptr;
    // }
    // if(reg_dlg_) {
    //     delete reg_dlg_;
    //     reg_dlg_ = nullptr;
    // }

}

void MainWindow::SlotSwitchRegister()
{
    // 切换界面时，先将central widget切换为要切换的窗口
    setCentralWidget(reg_dlg_);
    login_dlg_->hide();
    reg_dlg_->show();
}
