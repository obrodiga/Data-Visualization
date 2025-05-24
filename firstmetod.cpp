#include "firstmetod.h"
#include "ui_firstmetod.h"

firstmetod::firstmetod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstmetod)
{
    ui->setupUi(this);
    setWindowTitle("Математическая обработка данных");
    this->resize(400,390);          // Задание размера окна
    this->setFixedSize(400,390);
    ui->countPoints->setMaximum(4);
    ui->countPoints->setMinimum(1);
    ui->Directory->setReadOnly(true);

    ui->Directory->setToolTip("Внимнеие, измерения относяшиеся к одной точке, должны находится подряд!");
    ui->countPoints->setToolTip("Выберете число точек, которые будут отображены на финальном гарфике");
}

firstmetod::~firstmetod()
{
    delete ui;
}

double firstmetod::fileprocessing(QString filelocation)
{
    QFile file(filelocation);
    file.open(QIODevice::ReadOnly);
    double curentvalue, summ=0, result; //текущее значение, сумма всех чисел и результат
    int count=0; //количество чисел


    QStringList lineData = QString(file.readAll()).split("\n");
    for(int i = 0; i < lineData.length(); i++)
    {
        curentvalue=lineData[i].toDouble();
        if (curentvalue>0.1 || curentvalue==0)          //провеврка на соответсвие пороговому значению и пустую строку
        {
            continue;
        }
        else
        {
            count++;
            summ+=curentvalue;
        }
    }
    result=summ/count;
    return (result);
}

void firstmetod::saveRowToFile(QVector<double> row, QString filename, QString filelock)
{
    QString filefolder=filelock+"/"+filename;
    QFile file(filefolder);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (double value : row)
        {
            out << value << "\n";
        }
        file.close();
    }
}

void firstmetod::on_DirectoryOpen_clicked()
{
    ui->Directory->clear();
    QString DirectoryStr=QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), "C:/Users");

    if (!DirectoryStr.isEmpty())
    {
       ui->Directory->setText(DirectoryStr);
    }
}

void firstmetod::on_start_clicked()
{
    DataStorage::instance().clearData();    //очистка массива с хранимыми данными

    double filevalue; //переменная для сохранения среднего значения файла
    int count;  //переменная для сохранения номера файла из общего списка
    QString filelock;   //строка для сохранения полного/абсолютного пути к файлу
    int countPoint=ui->countPoints->value();
    QDir dir(ui->Directory->text());
    QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);
    int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();
    if ((countFiles%countPoint)!=0)
    {
        ui->textBrowser->setText("Количество заданых точек не соответсвует числу файлов");
    }
    else
    {
        QVector<double> valuesForPoint; //массив для сохранения всех усреднённых значений
        ui->textBrowser->setText("Обрабатываются файлы:\n");
        int temp=countFiles/countPoint;
        for (int i=0; i<countPoint;i++) //проход по всем файлам
        {
            valuesForPoint.clear(); //очистка вектора
            ui->textBrowser->append("Обрабатываются файлы для точки " + QString::number(i+1) + ":");
            for (int j=0; j<temp; j++)  //обработка по N файлов для каждой точки
            {
                count=temp*i+j; //нормер текущего файла
                filelock=ui->Directory->text()+"/"+fileName[count];
                ui->textBrowser->append(filelock);
                filevalue=fileprocessing(filelock);
                ui->textBrowser->append("Среднее значение файла: "+ QString::number(filevalue));
                valuesForPoint.push_back(filevalue);
            }
            DataStorage::instance().addRow(valuesForPoint);
            ui->textBrowser->append("__________\n");
        }
    }
}

void firstmetod::on_cancel_clicked()
{
    QDialog::close();
}

void firstmetod::on_save_clicked()
{
    int i, countPoints=DataStorage::instance().rowCount();
    QDir dir(ui->Directory->text());
    bool flag=dir.mkdir("2");           //создания подпапки "2", если папка существует или не создалась, - вернёт false
    QString filedir=ui->Directory->text()+"/2";
    QDir tempdir(filedir);
    if (flag==false && tempdir.exists() && tempdir.entryList(QStringList()<<"*.txt", QDir::Files).count()!=0)
    {
        tempdir.removeRecursively();
        dir.mkdir("2");
    }
    for (i = 0; i < countPoints; ++i)
    {
        QVector<double> row = DataStorage::instance().getRow(i);
        QString filename = QString("point_%1.txt").arg(i);
        saveRowToFile(row, filename, filedir);
    }
    QDialog::close();
}

