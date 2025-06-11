#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Анализ и сравнение данных");

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

void MainWindow::AddGrid()
{
    int j=-250;
    QPen penGray(Qt::gray); //установка серого цвета кисти
    for (int i=-250; i<=250; i+=50) //проход и рисование линий каждые 50 пикселей
    {
        scene->addLine(i, j, i, j+500, penGray);
        scene->addLine(j, i, j+500, i, penGray);
    }

    scene->addLine(0, 250, 0, -250, QPen(Qt::black));       //осевые разделительные линий по нулевым осям
    scene->addLine(250, 0, -250, 0, QPen(Qt::black));
}

void MainWindow::CreateElips (int x, int y, QVector<double> pointvalue, int multiplier)
{
    QPen penBest(Qt::darkGreen), penLowest(Qt::red);    //кисти для лучшего и худшего значения
    penBest.setWidth(2);    //установка толщины линий кистей
    penLowest.setWidth(2);

    int count=pointvalue.size();
    double maxValue=DataStorage::instance().getStorageMax(), minValue=DataStorage::instance().getStorageMin();  //получение значений
    for (int i=0; i<count; i++)     //проход по всему массиву и визуализация
    {
        double radius=pointvalue[i]*multiplier; //увеличение значения в multiplier раз
        double daimetr=radius*2;    //диаметр для визуализации

        if ((pointvalue[i]!=maxValue) && (pointvalue[i]!=minValue))
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr);    //если не подходит под максимум и минимум среди максимумов
        }
        else if (pointvalue[i]==maxValue)
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penBest);   //если максимум
        }
        else
        {
            scene->addEllipse(x-radius, y-radius, daimetr, daimetr, penLowest); //если минимум
        }
    }
}

void MainWindow::on_SelectAndStart_clicked()
{
    ui->Directory->clear();
    scene->clear();
    DataStorage::instance().clearData();

    QString DirectoryStr=QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), "C:/Users"); //открытие проводника для выбора директории  со стартовым каталогом "C:/Users"

    if (!DirectoryStr.isEmpty())
    {
        ui->Directory->setText(DirectoryStr);
        ui->toggleResize->setEnabled(true);     //разблокировка кнопки для создания графиков
        QString filelock;   //полное путь к файлу
        QDir dir(ui->Directory->text());
        QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);    //получение списка из названия всех файлов в директории с расширением txt
        int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();  //число файлов с расширением txt
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
                    valuesForPoint.push_back(lineData[i].toDouble());   //сохранение во временный массив значения преобразованного из QString в double
                }
            }
            DataStorage::instance().addRow(valuesForPoint);
        }
        double maxValue=DataStorage::instance().findStorageMax();   //вызов метода для определения максимум в хранилище
        DataStorage::instance().findStorageMin();

        while (maxValue<=12.5)  //подсчёт сколько раз можно умножить на 10, чтоб не выйти за границы квадранта
        {
            maxValue*=10;
            multiplier*=10;
        }

        for (int i=0; i<DataStorage::instance().rowCount(); i++)
        {
            CreateElips(mass[i][0], mass[i][1], DataStorage::instance().getRow(i), multiplier); //вызов функции построения окружностей с передачей центральной точки, массива значений и множителя
        }
    }
}

void MainWindow::on_toggleResize_toggled(bool checked)
{
    if (checked==true)  //если кнопка нажата
    {
        //настройка виджета
        this->resize(575,850);          // Задание размера окна
        this->setFixedSize(575,850);    // Фиксация размера окна
        ui->grafik->clearGraphs();
        ui->grafik->setVisible(true);   //отображение поля для графиков

        QPen penfirst(Qt::darkGreen), pensecond(Qt::darkRed), penthird(Qt::darkBlue), penfourth(Qt::darkGray);  //кисти для рисования

        //заготовка для графов
        int xBegin=1, xEnd;    //начало и конец промежутков по X
        double yMin=0.0, yMax=DataStorage::instance().getStorageMax()*1.5; // нижная и верхняя граница по Y
        ui->grafik->yAxis->setRange(yMin, yMax);   //установка границ по У

        int xCount=DataStorage::instance().maxElementCount();
        QVector<double> xPoints;       //массив элементов по оси Х
        for (int i=1; i<=xCount;i++)   //заполнение
        {
            xPoints.push_back(i);
        }
        xEnd=xPoints.size();
        ui->grafik->xAxis->setRange(xBegin, xEnd);  //ограничения оси Х

        for (int i=0; i<DataStorage::instance().rowCount();i++)
        {
            ui->grafik->addGraph();     //создание поля для графиков
            ui->grafik->graph(i)->addData(xPoints, DataStorage::instance().getRow(i));  //добавление одной строки из массива и массива точек Х
            ui->grafik->graph(i)->setName("Точка "+QString::number(i+1));
            switch(i)   //определение цвета графика
            {
            case 0: ui->grafik->graph(i)->setPen(penfirst); break;
            case 1: ui->grafik->graph(i)->setPen(pensecond); break;
            case 2: ui->grafik->graph(i)->setPen(penthird); break;
            case 3: ui->grafik->graph(i)->setPen(penfourth); break;
            default: QPen pendefault(Qt::black); ui->grafik->graph(i)->setPen(pendefault); break;   //если графиков много, они рисуются чёрным цветом
            }
        }
        ui->grafik->yAxis->setLabel("Значения в точках");   //подпись оси У
        ui->grafik->legend->setVisible(true);               //видимость легенды
        ui->grafik->replot();       //отрисовка графика
    }
    else    //если кнопку отжали
    {
        this->resize(575,650);          // Задание размера окна
        this->setFixedSize(575,650);    // Фиксация размера окна
        ui->grafik->clearGraphs();      //очистка поля для грфиков
        ui->grafik->setVisible(false);  //скрытие элемента
    }
}

void MainWindow::on_developer_triggered()
{

    QFile file(":/info/about.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Информация о разработчках", data);    //отображение файла с информацией о разработчиках
    }
}


void MainWindow::on_guide_triggered()
{
    QFile file(":/info/guid.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QByteArray data = file.readAll();
        QMessageBox::about(this, "Руководство по использованию программы", data);   //отображение файла с руководством пользователя
    }
}


void MainWindow::on_quit_triggered()
{
    QApplication::quit();       //кнопка закрыть в контекстном меню
}

void MainWindow::on_openData_triggered()
{
    firstMetod.exec();      //запуск вшитого обработчика данных
}

void MainWindow::on_actionTest_triggered()
{
    externalMethod.show();  //запуск внешнего модуля обработчика
}
