#include "firstmetod.h"
#include "ui_firstmetod.h"

firstmetod::firstmetod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstmetod)
{
    ui->setupUi(this);
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
        if (curentvalue>0.1 || curentvalue==0)
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


void firstmetod::on_DirectoryOpen_clicked()
{
    ui->Directory->clear();
    QString DirectoryStr=QFileDialog::getExistingDirectory(0, "Выбор каталога", ui->Directory->text());

    if (!DirectoryStr.isEmpty()) {
       ui->Directory->setText(DirectoryStr);
    }
}
void firstmetod::on_start_clicked()
{
    double filevalue; //переменная для сохранения среднего значения файла
    int count;  //переменная для сохранения номера файла из обчего списка
    QString filelock;   //полное путь к файлу
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
        ui->textBrowser->setText("Обрабатываются файлы:\n");
        int temp=countFiles/countPoint;
        for (int i=0; i<countPoint;i++) //проход по числу точек
        {
            ui->textBrowser->append("Обрабатываются файлы для точки " + QString::number(i+1) + ":");
            for (int j=0; j<temp; j++)  //обработка по N файлов для каждой точки
            {
                count=temp*i+j; //нормер текущего файла
                filelock=ui->Directory->text()+"/"+fileName[count];
                ui->textBrowser->append(filelock);
                filevalue=fileprocessing(filelock);
                ui->textBrowser->append("Среднее значение файла: "+ QString::number(filevalue));
            }
            ui->textBrowser->append("__________\n");
        }
    }
}


void firstmetod::on_cancel_clicked()
{
    QDialog::close();
}

