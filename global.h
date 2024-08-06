#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include <memory.h>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include "QStyle"

// repolish 用来刷新qss
extern std::function<void(QWidget*)> repolish;

enum ReqId {
    ID_GET_VERIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002,        // 注册用户
};

enum Modules {
    REGISTER = 0, // 注册模块

};

enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON,   // json解析失败
    ERR_NETWORK,// 网络错误
};



#endif // GLOBAL_H
