#include "firstmetod.h"
#include "ui_firstmetod.h"

firstmetod::firstmetod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstmetod)
{
    ui->setupUi(this);
    setWindowTitle("Математическая обработка данных");
    this->resize(400,390);          // Задание размера окна
    this->setFixedSize(400,390);    //зафиксировать размеры окна
    ui->countPoints->setMaximum(4); //ограничение на поле для чисел: от 1 до 4
    ui->countPoints->setMinimum(1);
    ui->Directory->setReadOnly(true);   //поле с директорией данных только для чтения
    ui->save->setEnabled(false);    //блокировка кнопки "сохранить"

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
    file.open(QIODevice::ReadOnly);     //открытие файла для чтения данных из него
    double curentvalue, summ=0, result; //текущее значение, сумма всех чисел и результат
    int count=0;


    QStringList lineData = QString(file.readAll()).split("\n"); //прочитать все данные с учётом разделителя "\n"
    for(int i = 0; i < lineData.length(); i++)
    {
        curentvalue=lineData[i].toDouble();     //преобразование строки в тип double
        if (curentvalue>0.1 || curentvalue==0)          //провеврка на соответсвие пороговому значению и пустую строку
        {
            continue;
        }
        else    //если проверка пройдена, выполняются действия согласно заданному мат. аппарату, в данном случе вычисляется среднеарифметическое
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
    QString filefolder=filelock+"/"+filename;   //формирование абсолютного пути к файлу
    QFile file(filefolder);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (double value : row)
        {
            out << value << "\n";   //запись значений в файл с разделителем "\n"
        }
        file.close();
    }
}

void firstmetod::on_DirectoryOpen_clicked()
{
    ui->Directory->clear();
    QString DirectoryStr=QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), "C:/Users");     //открытие проводника для выбора директории  со стартовым каталогом "C:/Users"

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
    QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);    //получение списка из названия всех файлов в директории с расширением txt
    int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();  //число файлов с расширением txt
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
    ui->save->setEnabled(true);    //включение кнопки "сохранить"
}

void firstmetod::on_cancel_clicked()
{
    QDialog::close();   //закрытие виджета
}

void firstmetod::on_save_clicked()
{
    int i, countPoints=DataStorage::instance().rowCount();
    QDir dir(ui->Directory->text());
    bool flag=dir.mkdir("2");           //создания подпапки "2", если папка существует или не создалась, - вернёт false
    QString filedir=ui->Directory->text()+"/2";
    QDir tempdir(filedir);
    if (flag==false && tempdir.exists() && tempdir.entryList(QStringList()<<"*.txt", QDir::Files).count()!=0)   //если flag имет значение false, а дериктория существует и не пуста, - она удаляется и создаётся заново
    {
        tempdir.removeRecursively();
        dir.mkdir("2");
    }
    for (i = 0; i < countPoints; ++i)
    {
        QVector<double> row = DataStorage::instance().getRow(i);
        QString filename = QString("point_%1.txt").arg(i+1);    //название файла по шаблону point_х, где х берётся из arg(i+1)
        saveRowToFile(row, filename, filedir);  //вызов функции по сохранению в файл
    }
    QDialog::close();   //закрытие виджета
}
