#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include <QVector>

#include <firstmetod.h>

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
    void CreateElips (int x, int y, QVector<double> pointvalue);

private slots:
    void on_Button1_clicked();

    void on_Button2_clicked();

    void on_Button3_clicked();

    void on_quit_triggered();

    void on_developer_triggered();

    void on_guide_triggered();

    void on_pushButton_clicked();

    void on_openData_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsTextItem *text;
    firstmetod firstMetod;

};
#endif // MAINWINDOW_H
