#include "datastorage.h"


DataStorage& DataStorage::instance()
{
    static DataStorage instance;
    return instance;
}

void DataStorage::clearData()   //очистка вестора и зануление значений
{
    data.clear();
    maxStoredValue=0.0;
    minStoredValue=0.0;
    maxElementCountInRow=0;
}

void DataStorage::addRow(const QVector<double>& row)
{
    data.append(row);   //запись новой строки в вектор
}

QVector<double> DataStorage::getRow(int index)
{
    if (index >= 0 && index < data.size())
    {
        return data[index]; //возврат строки по индексу
    }
    else
    {
        return {};      //в случае если индекс находится вне допустимого диапазона - возвращается пустой вектор QVector<double> (чтобы не вылетать в ошибку)
    }
}

int DataStorage::rowCount()
{
    return data.size();     //вывод значения строк в векторе
}

double DataStorage::findStorageMax()
{
    double maxValue = numeric_limits<double>::lowest();  //минимально возможное значение для типа

    for (int i = 0; i < data.size(); i++)      //проход по всем строкам вектора
    {
        const QVector<double>& row = data[i];  //передача строки по ссылке, без копирования

        for (int j = 0; j < row.size(); j++)
        {
            if (row[j] > maxValue)      //если текущее значение больше чем хранящиеся в переменной, она перезаписывается
            {
                maxValue = row[j];
            }
        }
    }

    maxStoredValue = maxValue;
    return maxStoredValue;
}

double DataStorage::getStorageMax()
{
    if (maxStoredValue==0)      //проверка на наличие значения, если нулевое - вычислить и вывести
    {
        return DataStorage::findStorageMax();
    }
    else        //если не нулевое, - вывести значение
    {
        return maxStoredValue;
    }
}

void DataStorage::findStorageMin()
{
    QVector<double> maxRowValue;            //вектор для хранения максимальных значений во всех строках
    for (int i = 0; i < data.size(); i++)   //проход по всем строкам вектора
    {
        const QVector<double>& row = data[i];  //передача строки по ссылке, без копирования

        double maxValue = numeric_limits<double>::lowest(); //запись минимально возможное значение для типа

        for (int j = 0; j < row.size(); j++)    //определение максимума в строке
        {
            double value = row[j];
            if (value > maxValue)
            {
                maxValue = value;
            }
        }
        maxRowValue.push_back(maxValue);
    }

    double minValue = numeric_limits<double>::max();;        //в переменную записывается максимально возможное значение для типа double

    for (int i=0; i<maxRowValue.size(); i++)    //проход по массиву максимумов
    {
        if (maxRowValue[i]<minValue)      //поиск наименьшего числа
        {
            minValue=maxRowValue[i];
        }
    }

    minStoredValue = minValue;  //сохранения наименьшего значения среди максимумов
}

double DataStorage::getStorageMin()
{
    if (minStoredValue==0)      //проверка на наличие значения, если нулевое - вычислить и только после выводить
    {
        DataStorage::findStorageMin();
    }
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
