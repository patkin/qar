#ifndef GOOGLEAPI_H
#define GOOGLEAPI_H
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QTimer>


class GoogleApi
{
public:
    GoogleApi();

    QStringList httpReq(QString);
    bool getConnectionStatus();

private:
    bool connectionStatus;
    QString lon;
    QString lat;


};

#endif // GOOGLEAPI_H
