
#include "stockmodel.h"

StockModel::StockModel(QObject *parent)
    : QAbstractListModel(parent)
{
}
                                                        //forteller hvor mange handelsdager modellen inneholder.
int StockModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_stockData.size();
}
                                                        // henter dato eller sluttkurs for én handelsdag
QVariant StockModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()
        || index.row() < 0
        || index.row() >= m_stockData.size()) {
        return {};
    }

    const StockData &stock = m_stockData.at(index.row());

    switch (role) {
    case DateRole:
        return stock.date;

    case CloseRole:
        return stock.close;

    case OpenRole:
        return stock.open;

    default:
        return {};
    }
}
                                                    // henter dato eller sluttkurs for én handelsdag
QHash<int, QByteArray> StockModel::roleNames() const
{
    return {
        {DateRole, "date"},
        {CloseRole, "close"},
        {OpenRole, "open"}
    };
}
                                                    // legger aksjedata inn i modellen og gir beskjed om at innholdet er oppdatert
void StockModel::setStockData(const QList<StockData> &data)
{
    beginResetModel();

    m_stockData = data;

    endResetModel();
}
