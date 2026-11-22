
#include "stockapiclient.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>

StockApiClient::StockApiClient(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}
void StockApiClient::fetchStockData(const QString &symbol, int days)
{
    if (m_activeReply) {
        m_activeReply->abort();
        m_activeReply = nullptr;
    }

    emit loadingChanged(true);

    QUrl url("https://eodhd.com/api/eod/" + symbol);

    QUrlQuery query;
    query.addQueryItem("api_token", qEnvironmentVariable("EODHD_API_TOKEN"));
    query.addQueryItem("fmt", "json");
    query.addQueryItem("period", "d");
    QDate today = QDate::currentDate();
    QDate fromDate = today.addDays(-days);

    query.addQueryItem("from", fromDate.toString("yyyy-MM-dd"));
    query.addQueryItem("to", today.toString("yyyy-MM-dd"));

    url.setQuery(query);

    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);
    m_activeReply = reply;

    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() {

                if (reply != m_activeReply) {
                    reply->deleteLater();
                    return;
                }

                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();

                    QJsonParseError parseError;
                    QJsonDocument document =                            // leser JSON-svaret.
                        QJsonDocument::fromJson(data, &parseError);

                    if (parseError.error != QJsonParseError::NoError
                        || !document.isArray()) {

                        qDebug() << "Ugyldig JSON-svar";

                        emit errorOccurred(
                            "Kunne ikke lese aksjedata fra API-et."
                            );

                    } else {

                        QJsonArray stockData = document.array();

                        if (stockData.isEmpty()) {
                            emit errorOccurred("Ingen aksjekurser funnet for valgt periode.");
                            emit loadingChanged(false);
                            m_activeReply = nullptr;
                            reply->deleteLater();
                            return;
                        }

                        QList<StockData> stockList;

                        for (const QJsonValue &value : stockData) {

                            QJsonObject stock = value.toObject();

                            QString date = stock["date"].toString();
                            double open = stock["open"].toDouble();
                            double close = stock["close"].toDouble();

                            if (date.isEmpty()
                                || !stock["open"].isDouble()
                                || !stock["close"].isDouble()) {
                                qDebug() << "Hopper over ugyldig kursregistrering";
                                continue;
                            }// Dette gjør at programmet ikke forsøker å vise en kursregistrering som mangler dato eller gyldig sluttkurs.

                            StockData item;
                            item.date = date;
                            item.open = open;
                            item.close = close;

                            stockList.append(item);

                            qDebug() << "Dato:" << date
                                     << "Sluttkurs:" << close << "NOK";
                        }

                        if (stockList.isEmpty()) {
                            emit errorOccurred("Ingen gyldige aksjekurser funnet.");
                            emit loadingChanged(false);
                            m_activeReply = nullptr;
                            reply->deleteLater();
                            return;
                        }// Dette sikrer at vi ikke erstatter eksisterende aksjekurser med en tom liste når alle registreringene er ugyldige

                        if (m_stockModel) {
                            m_stockModel->setStockData(stockList);
                            qDebug() << "Antall dager i modellen:"
                                     << m_stockModel->rowCount();

                        }
                    }

                } else {
                    QString errorMessage =
                        "Kunne ikke hente aksjekurser: " + reply->errorString();

                    qDebug() << errorMessage;

                    emit errorOccurred(errorMessage);
                }

                emit loadingChanged(false);
                m_activeReply = nullptr;
                reply->deleteLater();
    });
}
                                                        // forteller API-klassen hvilken modell som skal motta aksjedataene
void StockApiClient::setStockModel(StockModel *model)
{
    m_stockModel = model;
}