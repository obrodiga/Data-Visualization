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
        else if (pointvalue[i]==minValue)
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penLowest);
        }
        else
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penBest);
        }
    }
}


void MainWindow::on_Button1_clicked()
{
    scene->clear();
    scene->addEllipse(10, 10, 0.5, 0.25);

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
    QString text1="1";
    QGraphicsTextItem *text = scene->addText(text1);
    text->setPos(101, 101);
}



void MainWindow::on_pushButton_clicked()
{
    ui->Directory->clear();
    ui->textBrowser->clear();
    DataStorage::instance().clearData();

    QString DirectoryStr=QFileDialog::getExistingDirectory(0, "Выбор каталога", ui->Directory->text());

    if (!DirectoryStr.isEmpty()) {
        ui->Directory->setText(DirectoryStr);
    }

    QString filelock;   //полное путь к файлу
    QDir dir(ui->Directory->text());
    QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);
    int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();
    int multiplier=1;    //вычисляет максимальное кол-во нулей чтобы домножать числа при отображении
    AddGrid();

    int mass[4][2];
    mass[0][0]=-125;
    mass[0][1]=-125;
    mass[1][0]=125;
    mass[1][1]=-125;
    mass[2][0]=-125;
    mass[2][1]=125;
    mass[3][0]=125;
    mass[3][1]=125;

    QVector<double> valuesForPoint; //массив для сохранения всех усреднённых значений
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
        ui->textBrowser->append(filelock);
    }
    double maxValue=DataStorage::instance().findStorageMax();
    double minValue=DataStorage::instance().findStorageMin();
    ui->textBrowser->append(QString::number(maxValue));
    ui->textBrowser->append(QString::number(minValue));

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

