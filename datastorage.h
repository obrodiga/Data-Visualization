#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QString>
#include <QVector>
#include <QDebug>

using namespace std;

class DataStorage
{
public:
    static DataStorage& instance();                        // Получить Singleton

    void clearData();                                      // Очистить все данные
    void addRow(const QVector<double>& row);               // Добавить строку
    QVector<double> getRow(int index) const;               // Получить строку по индексу
    int rowCount() const;                                  // Кол-во строк

    double findStorageMax();                              //максимум среди значений
    double getStorageMax() const;                           //запросить максимум
    double findStorageMin();                              //минимум среди максимумов линий
    double getStorageMin() const;                           //запросить минум

private:
    DataStorage() = default;
    QVector<QVector<double>> data;                         // Хранилище данных
    double maxStoredValue = 0.0;                           // Максимальное значение, если =0, - невычислили или хранилище пусто
    double minStoredValue = 0.0;                           // Минимальное значение среди максимумов, если =0, - невычислили или хранилище пусто
};

#endif // DATASTORAGE_H
