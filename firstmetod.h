#ifndef FIRSTMETOD_H
#define FIRSTMETOD_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>

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

private slots:

    void on_DirectoryOpen_clicked();

    void on_start_clicked();

private:
    Ui::firstmetod *ui;
};

#endif // FIRSTMETOD_H
