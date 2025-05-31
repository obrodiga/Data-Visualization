#ifndef EXTERNAL_METHOD_H
#define EXTERNAL_METHOD_H

#include <QWidget>
#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QVector>
#include <QTextStream>

#include <QProcess>

#include <QDebug>

#include <datastorage.h>

namespace Ui {
class external_method;
}

class external_method : public QWidget
{
    Q_OBJECT

public:
    explicit external_method(QWidget *parent = nullptr);
    ~external_method();

private slots:
    void on_DirectoryOpen_clicked();

    void on_DirectoryOpen_exe_clicked();

    void on_starProcessing_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

private:
    Ui::external_method *ui;
};

#endif // EXTERNAL_METHOD_H
