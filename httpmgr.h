#ifndef HTTPMGR_H
#define HTTPMGR_H

#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// C++ CRTP 奇异递归
class HttpMgr : public QObject,
                public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
public:
    /*析构函数设置为public目的是为了让基类能够访问*/
    ~HttpMgr();

private:
    /*设置友元的目的是为了让基类能够访问子类的构造函数*/
    friend class Singleton<HttpMgr>;
    HttpMgr();

    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
    QNetworkAccessManager manager_;

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
