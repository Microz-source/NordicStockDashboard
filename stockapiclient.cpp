
#include "stockapiclient.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

StockApiClient::StockApiClient(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void StockApiClient::fetchStockData(const QString &symbol)
{
    QUrl url("https://eodhd.com/api/eod/" + symbol);

    QUrlQuery query;
    query.addQueryItem("api_token", qEnvironmentVariable("EODHD_API_TOKEN"));
    query.addQueryItem("fmt", "json");
    query.addQueryItem("period", "d");
    query.addQueryItem("from", "2026-11-01");
    query.addQueryItem("to", "2026-11-15");

    url.setQuery(query);

    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() {

                if (reply->error() == QNetworkReply::NoError) {

                    QByteArray data = reply->readAll();

                    QJsonParseError parseError;
                    QJsonDocument document =                            // leser JSON-svaret.
                        QJsonDocument::fromJson(data, &parseError);

                    if (parseError.error != QJsonParseError::NoError
                        || !document.isArray()) {

                        qDebug() << "Ugyldig JSON-svar";

                    } else {


                        QJsonArray stockData = document.array();

                        QList<StockData> stockList;

                        for (const QJsonValue &value : stockData) {

                            QJsonObject stock = value.toObject();

                            QString date = stock["date"].toString();
                            double close = stock["close"].toDouble();

                            StockData item;
                            item.date = date;
                            item.close = close;

                            stockList.append(item);

                            qDebug() << "Dato:" << date
                                     << "Sluttkurs:" << close << "NOK";
                        }

                        if (m_stockModel) {
                            m_stockModel->setStockData(stockList);
                            qDebug() << "Antall dager i modellen:"
                                     << m_stockModel->rowCount();

                        }
                    }

                } else {
                    qDebug() << "Nettverksfeil:" << reply->errorString();
                }

                reply->deleteLater();
            });
}
                                                        // forteller API-klassen hvilken modell som skal motta aksjedataene
void StockApiClient::setStockModel(StockModel *model)
{
    m_stockModel = model;
}