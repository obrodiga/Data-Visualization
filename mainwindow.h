#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <qcustomplot.h>

#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QList>

#include <firstmetod.h>
#include <external_method.h>
#include <datastorage.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddGrid();
    void Addelips(int diametr, int x, int y);
    void CreateElips (int x, int y, QVector<double> pointvalue, int multiplier);

private slots:

    void on_quit_triggered();

    void on_developer_triggered();

    void on_guide_triggered();

    void on_SelectAndStart_clicked();

    void on_openData_triggered();

    void on_toggleResize_toggled(bool checked);

    void on_actionTest_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsTextItem *text;
    firstmetod firstMetod;
    external_method externalMethod;

};
#endif // MAINWINDOW_H
