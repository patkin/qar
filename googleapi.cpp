#include "googleapi.h"

static QString const keyApi = "";

GoogleApi::GoogleApi()
{

}

QStringList GoogleApi::httpReq(QString latlon){


    QEventLoop eventLoop;
    connectionStatus=true;

    QNetworkAccessManager netMgr;
    QObject::connect(&netMgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));


    QNetworkRequest netReq( QUrl( QString("https://maps.googleapis.com/maps/api/elevation/xml?locations="+latlon+"&key="+keyApi) ) );
    QNetworkReply *netReply = netMgr.get(netReq);
    eventLoop.exec();

    //Connection stasus is ok

    if (netReply->error() == QNetworkReply::NoError) {

        QXmlStreamReader xml(netReply->readAll());
        int temp;
        QString status;
        QStringList feetList;

        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                QString name = xml.name().toString();

                if(name == "elevation"){
                    feetList.append(xml.readElementText());
                }
                if(name == "status"){
                    status=xml.readElementText();
                }
            }
        }

        //Problems with xml file, return empty list and false status
        if (xml.hasError() || !status.contains("OK"))
        {
            QMessageBox msgBox;
            msgBox.setText("Somthing goes wrong, try once again.");
            msgBox.exec();
            qDebug()<< "XML error: " << xml.errorString();
            connectionStatus=false;
            return QStringList();
        }

        delete netReply;

        if(connectionStatus){
            for(int k=0;k<feetList.size();k++){
                temp=QString(feetList.at(k)).toFloat();
                temp*=3.280839895;  //Meters to feet converter
                feetList.replace(k,QString::number(temp,'f',4));
            }
        }


        return feetList;
    }

    //Connection problem
    else {

        QMessageBox msgBox;
        msgBox.setText("Connection problem, try once again or check internet connection.");
        msgBox.exec();
        connectionStatus=false;
        delete netReply;
        qDebug() << "Failure" <<netReply->errorString();
        return QStringList();

    }

}

bool GoogleApi::getConnectionStatus(){
    return connectionStatus;
}
