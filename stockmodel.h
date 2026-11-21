
#ifndef STOCKMODEL_H    // lagrer kursene slik at QML senere kan vise dem.
#define STOCKMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>

                // Informasjon om én handelsdag
struct StockData
{
    QString date;
    double close;
};

class StockModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit StockModel(QObject *parent = nullptr);

                // Feltene som QML skal kunne lese
    enum StockRoles {
        DateRole = Qt::UserRole + 1,
        CloseRole
    };

                 // Obligatoriske funksjoner i modellen
    int rowCount(
        const QModelIndex &parent = QModelIndex()
        ) const override;

    QVariant data(
        const QModelIndex &index,
        int role = Qt::DisplayRole
        ) const override;

    QHash<int, QByteArray> roleNames() const override;

                     // Legger inn aksjedata i modellen
    void setStockData(const QList<StockData> &data);

                // Grafen trenger en enkel måte å lese aksjekursene fra C++ på
    Q_INVOKABLE int count() const
    {
        return rowCount();
    }

    Q_INVOKABLE double closeAt(int row) const
    {
        if (row < 0 || row >= m_stockData.size())
            return 0.0;

        return m_stockData.at(row).close;
    }

    Q_INVOKABLE QString dateAt(int row) const
    {
        if (row < 0 || row >= m_stockData.size())
            return QString();

        return m_stockData.at(row).date;
    }

private:
    QList<StockData> m_stockData;
};

#endif              // STOCKMODEL_H
