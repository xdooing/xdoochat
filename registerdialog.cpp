#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);

    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish,
            this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    // example@163.com
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match) {
        // 发送http验证码
    }else {
        showTip(tr("邮箱地址不正确！"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS) {
        // 为了安全起见,这里可以不用写的很清楚
        showTip(tr("请求无效"), false);
        return;
    }

    // 解析json字符串 --> res转换为QByteArray --> 转换为字节数组方便反序列化
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()) {
        showTip(tr("Json 解析失败!"), false);
        return;
    }
    if(!jsonDoc.isObject()) {
        showTip(tr("未转换为Json对象!"), false);
        return;
    }

    // 调用处理函数
    handles_[id](jsonDoc.object());
    return;
}

void RegisterDialog::showTip(QString str, bool is_ok)
{
    if(is_ok)
        ui->err_tip->setProperty("state", "normal");
    else
        ui->err_tip->setProperty("state", "err");

    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回包逻辑
    handles_.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject& json){
        int error = json["error"].toInt();
        if(error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误!"), false);
            return;
        }

        auto email = json["email"].toString();
        showTip(tr("验证码已发送到邮箱,注意查收!"), true);
        qDebug() << "email is " << email;
    });
}

