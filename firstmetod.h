#ifndef FIRSTMETOD_H
#define FIRSTMETOD_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QVector>
#include <QTextStream>

#include <datastorage.h>

namespace Ui {
class firstmetod;
}

class firstmetod : public QDialog
{
    Q_OBJECT

public:
    explicit firstmetod(QWidget *parent = nullptr);
    ~firstmetod();

    double fileprocessing (QString filelocation);

    void saveRowToFile(QVector<double> row, QString filename, QString filelock);

private slots:

    void on_DirectoryOpen_clicked();

    void on_start_clicked();

    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::firstmetod *ui;
};

#endif // FIRSTMETOD_H
