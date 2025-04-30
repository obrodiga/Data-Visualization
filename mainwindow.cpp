#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(575,650);          // Задание размера окна
    this->setFixedSize(575,650);    // Фиксация размера окна
    ui->grafik->setVisible(false);
    ui->toggleResize->setEnabled(false);        //кнопка изначально заблокирована, после ввода данных - доступна

    //настройка graphicsView
    scene=new QGraphicsScene();     // Иницализация графической сцены
    ui->graphicsView->setScene(scene);      // Установка графической сцены в graphicsView
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали
    scene->setSceneRect(-250,-250,500,500); // Устанавливаем область графической сцены

}

MainWindow::~MainWindow()
{
    delete ui;
}

//временные функции, удалятся из финальной версии

void MainWindow::Addelips(int diametr, int x, int y)
{
    int temp=diametr/2;
    int anglX=x-temp;
    int anglY=y-temp;
    scene->addEllipse(anglX, anglY, diametr, diametr);
}

//______________

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

void MainWindow::CreateElips (int x, int y, QVector<double> pointvalue, int multiplier)
{
    QPen penBest(Qt::darkGreen), penLowest(Qt::red);
    penBest.setWidth(2);
    penLowest.setWidth(2);

    int count=pointvalue.size();
    double maxValue=DataStorage::instance().getStorageMax(), minValue=DataStorage::instance().getStorageMin();
    for (int i=0; i<count; i++)
    {
        double radius=pointvalue[i]*multiplier;
        double daimetr=radius*2;

        if ((pointvalue[i]!=maxValue) && (pointvalue[i]!=minValue))
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr);
        }
        else if (pointvalue[i]==maxValue)
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penBest);
        }
        else
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penLowest);
        }
    }
}

void MainWindow::on_SelectAndStart_clicked()
{
    ui->Directory->clear();
    scene->clear();
    DataStorage::instance().clearData();

    QString DirectoryStr=QFileDialog::getExistingDirectory(0, "Выбор каталога", ui->Directory->text());

    if (!DirectoryStr.isEmpty())
    {
        ui->Directory->setText(DirectoryStr);
        ui->toggleResize->setEnabled(true);     //разблокировка кнопки для создания графиков
        QString filelock;   //полное путь к файлу
        QDir dir(ui->Directory->text());
        QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);
        int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();
        int multiplier=1;    //вычисляет максимальное кол-во нулей чтобы домножать числа при отображении

        AddGrid();

        int mass[4][2];         //центры окружностей
        mass[0][0]=-125;
        mass[0][1]=-125;
        mass[1][0]=125;
        mass[1][1]=-125;
        mass[2][0]=-125;
        mass[2][1]=125;
        mass[3][0]=125;
        mass[3][1]=125;

        QVector<double> valuesForPoint; //массив для сохранения значений из файла
        for (int i=0; i<countFiles;i++) //проход по числу точек
        {
            filelock=ui->Directory->text()+"/"+fileName[i];
            QFile file(filelock);
            file.open(QIODevice::ReadOnly);
            valuesForPoint.clear();
            QStringList lineData = QString(file.readAll()).split("\n");
            for(int i = 0; i < lineData.length(); i++)
            {
                if (lineData[i].toDouble()==0)
                {
                    continue;
                }
                else
                {
                    valuesForPoint.push_back(lineData[i].toDouble());
                }

            }
            DataStorage::instance().addRow(valuesForPoint);
        }
        double maxValue=DataStorage::instance().findStorageMax();
        double minValue=DataStorage::instance().findStorageMin();

        while (maxValue<=12.5)  //подсчёт сколько раз можно умножить на 10, чтоб не выйти за границы квадранта
        {
            maxValue*=10;
            multiplier*=10;
        }

        for (int i=0; i<DataStorage::instance().rowCount(); i++)
        {
            CreateElips(mass[i][0], mass[i][1], DataStorage::instance().getRow(i), multiplier);
        }
    }
}

void MainWindow::on_toggleResize_toggled(bool checked)
{
    if (checked==true)
    {
        //настройка виджета
        this->resize(575,850);          // Задание размера окна
        this->setFixedSize(575,850);    // Фиксация размера окна
        ui->grafik->clearGraphs();
        ui->grafik->setVisible(true);

        //заготовка для графов
        int xBegin=0, xEnd;    //начало и конец промежутков по X
        double yMin=0.0, yMax=DataStorage::instance().getStorageMax()*1.2; // нижная и верхняя граница по Y
        ui->grafik->yAxis->setRange(yMin, yMax);   //установка границ по У

        QPen penfirst(Qt::darkGreen), pensecond(Qt::darkRed), penthird(Qt::darkBlue), penfourth(Qt::darkGray);

        QVector<double> xPoints;
        for (int i=0; i<DataStorage::instance().getRow(0).size();i++)   //подсчёт кол-ва элементов в первой строке
        {
            xPoints.push_back(i);
        }
        xEnd=xPoints.size()-1;
        ui->grafik->xAxis->setRange(xBegin, xEnd);

        for (int i=0; i<DataStorage::instance().rowCount();i++)
        {
            ui->grafik->addGraph();
            ui->grafik->graph(i)->addData(xPoints, DataStorage::instance().getRow(i));
            ui->grafik->graph(i)->setName("Точка "+QString::number(i+1));
            switch(i)
            {
            case 0: ui->grafik->graph(i)->setPen(penfirst); break;
            case 1: ui->grafik->graph(i)->setPen(pensecond); break;
            case 2: ui->grafik->graph(i)->setPen(penthird); break;
            case 3: ui->grafik->graph(i)->setPen(penfourth); break;
            default: QPen pendefault(Qt::black); ui->grafik->graph(i)->setPen(pendefault); break;
            }
        }
        ui->grafik->yAxis->setLabel("Значения в точках");
        ui->grafik->legend->setVisible(true);
        ui->grafik->replot();       //отрисовка графика
    }
    else
    {
        this->resize(575,650);          // Задание размера окна
        this->setFixedSize(575,650);    // Фиксация размера окна
        ui->grafik->clearGraphs();
        ui->grafik->setVisible(false);
    }
}

void MainWindow::on_developer_triggered()
{

    QFile file(":/info/about.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Информация о разработчках", data);
    }
}


void MainWindow::on_guide_triggered()
{
    QFile file(":/info/guid.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Руководство по использованию программы", data);
    }
}


void MainWindow::on_quit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_openData_triggered()
{
    firstMetod.exec();
}
