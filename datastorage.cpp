#include "datastorage.h"


DataStorage& DataStorage::instance() {
    static DataStorage instance;
    return instance;
}

void DataStorage::clearData() {
    data.clear();
    maxStoredValue=0.0;
    minStoredValue=0.0;
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

double DataStorage::findStorageMax()
{
    double maxValue = numeric_limits<double>::lowest();  //минимально возможное значение для типа

    for (int i = 0; i < data.size(); i++)
    {
        const QVector<double>& row = data[i];  // строка по ссылке, без копирования

        for (int j = 0; j < row.size(); j++)
        {
            double value = row[j];
            if (value > maxValue)
            {
                maxValue = value;
            }
        }
    }

    maxStoredValue = maxValue;
    return maxValue;
}

double DataStorage::getStorageMax() const
{
    return maxStoredValue;
}

double DataStorage::findStorageMin()
{
    double minValue;        //для сохранения минимального среди всех максимумов в точках
    QVector<double> maxRowValue;            // сохранять максимальные значения во всех строках
    for (int i = 0; i < data.size(); i++)
    {
        const QVector<double>& row = data[i];  // строка по ссылке, без копирования

        double maxValue = numeric_limits<double>::lowest(); //минимально возможное значение для типа

        for (int j = 0; j < row.size(); j++)
        {
            double value = row[j];
            if (value > maxValue)
            {
                maxValue = value;
            }
        }
        maxRowValue.push_back(maxValue);
    }

    double minDoubleValue = numeric_limits<double>::max();

    for (int i=0; i<maxRowValue.size(); i++)
    {
        if (maxRowValue[i]<minDoubleValue)      //поиск наименьшего числа
        {
            minValue=maxRowValue[i];
        }
    }

    minStoredValue = minValue;
    return minValue;
}

double DataStorage::getStorageMin() const
{
    return minStoredValue;
}
