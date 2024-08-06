#include "httpmgr.h"

HttpMgr::~HttpMgr() {

}

HttpMgr::HttpMgr() {
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);

}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    auto self = shared_from_this();
    QNetworkReply* reply = manager_.post(request, data);

    // 查看数据是否发送完毕,并执行lambda函数:
    /*此处不传递this而是使用self的原因是:
     * lambda是replay指针收到finish信息之后调用的,是个回调函数,因此其调用时间我们没法掌握
     * 而在这之间this对象可能会被回收,因此我们用上面的self智能指针,而不是this,才能保证此对
     * 象不会被回收(引用计数+1).
    */
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, req_id, mod](){
        // 处理错误情况:
        if(reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            // 发送信号通知完成
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();
        // 发送信号通知完成
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTER) {
        // 发布信号通知指定模块http的响应结束了
        emit sig_reg_mod_finish(id, res, err);

    }
}




