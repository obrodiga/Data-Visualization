#include "datastorage.h"


DataStorage& DataStorage::instance() {
    static DataStorage instance;
    return instance;
}

void DataStorage::clearData() {
    data.clear();
    maxStoredValue=0.0;
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

double DataStorage::findAndStoreMax()
{
    double maxVal = std::numeric_limits<double>::lowest();  //минимально возможное значение для типа

    for (int i = 0; i < data.size(); ++i)
    {
        const QVector<double>& row = data[i];  // строка по ссылке, без копирования

        for (int j = 0; j < row.size(); ++j)
        {
            double value = row[j];
            if (value > maxVal)
            {
                maxVal = value;
            }
        }
    }

    maxStoredValue = maxVal;
    return maxVal;
}

double DataStorage::getStoredMax() const
{
    return maxStoredValue;
}
