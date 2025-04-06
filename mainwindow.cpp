#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(575,850);          // Задание размера окна
    this->setFixedSize(575,850);    // Фиксация размера окна

    //настройка graphicsView
    scene=new QGraphicsScene();     // Иницализация графической сцены
    ui->graphicsView->setScene(scene);      // Установка графической сцены в graphicsView
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали
    scene->setSceneRect(-250,-250,500,500); // Устанавливаем область графической сцены

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddGrid()
{
    int j=-250;
    QPen penGray(Qt::gray);
    for (int i=-250; i<=250; i+=50)
    {
        scene->addLine(i, j, i, j+500, penGray);
        scene->addLine(j, i, j+500, i, penGray);
    }

    scene->addLine(0, 250, 0, -250, QPen(Qt::black));
    scene->addLine(250, 0, -250, 0, QPen(Qt::black));
}

void MainWindow::Addelips(int diametr, int x, int y)
{
    int temp=diametr/2;
    int anglX=x-temp;
    int anglY=y-temp;
    scene->addEllipse(anglX, anglY, diametr, diametr);
}

void MainWindow::CreateElips (int x, int y, qreal diametr, int count)
{
    QPen penBest(Qt::darkGreen), penLowest(Qt::red);
    penBest.setWidth(2);
    penLowest.setWidth(2);
    for (int i=1; i<=count; i++)
    {
        int radius=diametr/2;
        if (i==count && count==3)
        {
            scene->addEllipse(x-radius, y-radius, diametr, diametr, penBest);
        }
        else if (i==count && count==1)
        {
            scene->addEllipse(x-radius, y-radius, diametr, diametr, penLowest);
        }
        else
        {
            scene->addEllipse(x-radius, y-radius, diametr, diametr);
        }
        diametr+=7.5;
    }
}


void MainWindow::on_Button1_clicked()
{
    scene->clear();
    //AddGrid();
    scene->addEllipse(0, 0, 0.5, 0.25);

    //Addelips(50, 0, 0);
    //Addelips(55, 0, 0);
    //Addelips(60, 0, 0);

}


void MainWindow::on_Button2_clicked()
{
    scene->clear();
    AddGrid();

    scene->addEllipse(50, 50, 50, 50);
    scene->addEllipse(50, 50, 55, 55);
    scene->addEllipse(50, 50, 60, 60);

    scene->addEllipse(-100, -100, 50, 50);
    scene->addEllipse(-100, -100, 55, 55);
}


void MainWindow::on_Button3_clicked()
{
    scene->clear();
    AddGrid();
    CreateElips(100, 100, 50, 3);
    CreateElips(100, -100, 50, 2);
    CreateElips(-100, 100, 50, 2);
    CreateElips(-100, -100, 50, 1);
    QString text1="1";
    QGraphicsTextItem *text = scene->addText(text1);
    text->setPos(101, 101);
}


void MainWindow::on_quit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_developer_triggered()
{
    QFile file(":/info/about.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //ui->textBrowser->setPlainText("Не удалось открыть файл 'Информация о разработчках'");
    }
    else
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Информация о разработчках", data);
    }
}


void MainWindow::on_guide_triggered()
{
    QFile file(":/info/guid.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //ui->textBrowser->setPlainText("Не удалось открыть файл 'Информация о разработчках'");
    }
    else
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Руководство по использованию программы", data);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray data;
    QString FileName;
    FileName = QFileDialog::getOpenFileName(this, tr("Open TXT File"), "", tr("TXT file (*.txt)"));
    ui->lineEdit->setText(FileName);
    QFile file(FileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->textBrowser->setPlainText("Не удалось открыть файл");
    }
    else
    {
        data=file.readAll();
        ui->textBrowser->setText(data);
    }
}

void MainWindow::on_openData_triggered()
{
    firstMetod.exec();
}

