#include "datastorage.h"


DataStorage& DataStorage::instance() {
    static DataStorage instance;
    return instance;
}

void DataStorage::clearData() {
    data.clear();
}

void DataStorage::addRow(const QVector<double>& row) {
    data.append(row);
}

QVector<double> DataStorage::getRow(int index) const {
    if (index >= 0 && index < data.size()) {
        return data[index];
    } else {
        qWarning() << "Invalid row index:" << index;
        return {};
    }
}

int DataStorage::rowCount() const {
    return data.size();
}
