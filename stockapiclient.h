
#ifndef STOCKAPICLIENT_H        // henter historiske aksjekurser fra EODHD.
#define STOCKAPICLIENT_H

#include <QObject>                  //grunnklassen vi bruker for å arbeide med Qt.
#include <QNetworkAccessManager>   // sender forespørsler til internett.
#include <QNetworkReply>            // brukes til å motta svar.
#include "stockmodel.h"

class StockApiClient : public QObject
{
    Q_OBJECT

public:
    explicit StockApiClient(QObject *parent = nullptr);

    Q_INVOKABLE void fetchStockData(const QString &symbol, int days = 30); //er funksjonen som skal hente aksjedata
    void setStockModel(StockModel *model);

signals:                                    // errorOccurred er et signal som C++ kan sende til QML når noe går galt
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager *networkManager;     //er objektet som håndterer nettverkskommunikasjonen
    StockModel *m_stockModel = nullptr;
};

#endif // STOCKAPICLIENT_H


// Q_INVOKABLE gjør at QML kan kalle funksjonen direkte

