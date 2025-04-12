#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QString>
#include <QVector>
#include <QDebug>

class DataStorage
{
public:
    static DataStorage& instance();                        // Получить Singleton

    void clearData();                                      // Очистить все данные
    void addRow(const QVector<double>& row);               // Добавить строку
    QVector<double> getRow(int index) const;               // Получить строку по индексу
    int rowCount() const;                                  // Кол-во строк

private:
    DataStorage() = default;
    QVector<QVector<double>> data;                         // Хранилище данных
};

#endif // DATASTORAGE_H
