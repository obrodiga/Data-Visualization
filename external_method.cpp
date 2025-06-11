#include "external_method.h"
#include "ui_external_method.h"

external_method::external_method(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::external_method)
{
    ui->setupUi(this);
    setWindowTitle("Обработка данных внешним модулем"); //название окна
    ui->countPoints->setMaximum(4); //ограничение на поле для чисел: от 1 до 4
    ui->countPoints->setMinimum(1);
    ui->save->setEnabled(false);    //блокировка кнопки "сохранить"
    ui->Directory->setReadOnly(true);   //поля с директориями только для чтения
    ui->Directory_exe->setReadOnly(true);
}

external_method::~external_method()
{
    delete ui;
}

void external_method::on_DirectoryOpen_clicked()
{
    ui->Directory->clear();
    QString DirectoryStr=QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), "C:/Users");     //вызов проводника для выбора директории с указанием стартового католога "C:/Users"
    if (!DirectoryStr.isEmpty())
    {
        ui->Directory->setText(DirectoryStr);
    }
}

void external_method::on_DirectoryOpen_exe_clicked()
{
    ui->Directory_exe->clear();
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open EXE File"), "C:/Users", tr("EXE file (*.exe)")); //вызов проводника для выбора exe файла с указанием стартового католога "C:/Users"
    if (!FileName.isEmpty())
    {
        ui->Directory_exe->setText(FileName);
    }
}


void external_method::on_starProcessing_clicked()
{
    DataStorage::instance().clearData();       //очистка хранимых данных
    ui->textBrowser->clear();
    int countPoint=ui->countPoints->value();    //число точек из элемента SpinBox
    QString program = ui->Directory_exe->text(), filedir=ui->Directory->text(); //название программы и директории
    QDir dir=filedir;
    QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);    //получение списка из названия всех файлов в директории с расширением txt
    int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();  //число файлов в директории с расширением txt

    QProcess *process = new QProcess(this);     //создание объёкта для запуска процесса

    if ((countFiles%countPoint)==0)
    {
        QVector<double> valuesForPoint; //массив для сохранения всех усреднённых значений
        int temp=countFiles/countPoint;
        ui->textBrowser->setText("Обрабатываются файлы:\n");
        for (int i=0; i<countPoint;i++) //проход по всем файлам
        {
            valuesForPoint.clear(); //очистка вектора
            ui->textBrowser->append("Обрабатываются файлы для точки " + QString::number(i+1) + ":");
            for (int j=0; j<temp; j++)  //обработка по N файлов для каждой точки
            {
                int count=temp*i+j; //нормер текущего файла
                QString filelock=ui->Directory->text()+"/"+fileName[count]; //абсолютный путь к файлу
                QFile file(filelock);
                file.open(QIODevice::ReadOnly);
                QStringList lineData = QString(file.readAll()).split("\n");
                process->start(program, lineData);      //запуск консольного приложения с передачей ему значений
                process->waitForFinished();     //Ожидание завершения работы консольного ПО
                QByteArray output = process->readAllStandardOutput();       // Получаем результат
                QString result(output.trimmed());   //Сохранение в строку с удалением пробелов и символов форматирования
                valuesForPoint.push_back(result.toDouble());
                ui->textBrowser->append(filelock);
                ui->textBrowser->append("Среднее значение файла: "+ result);
            }
            DataStorage::instance().addRow(valuesForPoint);
            ui->textBrowser->append("__________\n\n");
        }
        ui->save->setEnabled(true); //разблокировка кнопки "Сохранить"
    }
}

void external_method::on_save_clicked()
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
        QString filefolder=filedir+"/"+filename;
        QFile file(filefolder);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))  //открытие файла для записи
        {
            QTextStream out(&file);
            for (double value : row)
            {
                out << value << "\n";       //сохранение значения в файл
            }
            file.close();
        }
    }
    this->close();
}


void external_method::on_cancel_clicked()
{
    this->close();  //закрытие виджета
}
