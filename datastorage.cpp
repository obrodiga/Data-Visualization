#include "datastorage.h"


DataStorage& DataStorage::instance()
{
    static DataStorage instance;
    return instance;
}

void DataStorage::clearData()
{
    data.clear();
    maxStoredValue=0.0;
    minStoredValue=0.0;
    maxElementCountInRow=0;
}

void DataStorage::addRow(const QVector<double>& row)
{
    data.append(row);
}

QVector<double> DataStorage::getRow(int index)
{
    if (index >= 0 && index < data.size())
    {
        return data[index];
    }
    else
    {
        return {};
    }
}

int DataStorage::rowCount()
{
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
            if (row[j] > maxValue)
            {
                maxValue = row[j];
            }
        }
    }

    maxStoredValue = maxValue;
    return maxValue;
}

double DataStorage::getStorageMax()
{
    return maxStoredValue;
}

void DataStorage::findStorageMin()
{
    double minValue = numeric_limits<double>::max();;        //для сохранения минимального среди всех максимумов в точках
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

    for (int i=0; i<maxRowValue.size(); i++)
    {
        if (maxRowValue[i]<minValue)      //поиск наименьшего числа
        {
            minValue=maxRowValue[i];
        }
    }

    minStoredValue = minValue;
}

double DataStorage::getStorageMin()
{
    return minStoredValue;
}

int DataStorage::maxElementCount()
{
    if (maxElementCountInRow==0)
    {
        int count=0;  //для хранения максимального числа элементов в строке
        for (int i=0; i<data.size(); i++)
        {
            if (data[i].size()>count)
            {
                count=data[i].size();
            }
        }
        maxElementCountInRow=count;
    }
    return maxElementCountInRow;
}
