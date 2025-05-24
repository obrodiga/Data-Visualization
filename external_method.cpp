#include "external_method.h"
#include "ui_external_method.h"

external_method::external_method(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::external_method)
{
    ui->setupUi(this);
    setWindowTitle("Обработка данный внешним модулем");
    ui->countPoints->setMaximum(4);
    ui->countPoints->setMinimum(1);
    ui->save->setEnabled(false);
    ui->Directory->setReadOnly(true);
    ui->Directory_exe->setReadOnly(true);
}

external_method::~external_method()
{
    delete ui;
}

void external_method::on_DirectoryOpen_clicked()
{
    ui->Directory->clear();   
    QString DirectoryStr=QFileDialog::getExistingDirectory(this, tr("Выбор каталога"), "C:/Users");
    if (!DirectoryStr.isEmpty())
    {
       ui->Directory->setText(DirectoryStr);
    }
}

void external_method::on_DirectoryOpen_exe_clicked()
{
    ui->Directory_exe->clear();
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open EXE File"), "C:/Users", tr("EXE file (*.exe)"));
    if (!FileName.isEmpty())
    {
       ui->Directory_exe->setText(FileName);
    }
}


void external_method::on_starProcessing_clicked()
{
    DataStorage::instance().clearData();
    int countPoint=ui->countPoints->value();
    QString program = ui->Directory_exe->text(), filedir=ui->Directory->text();
    QDir dir=filedir;
    QStringList fileName=dir.entryList(QStringList()<<"*.txt", QDir::Files);
    int countFiles=dir.entryList(QStringList()<<"*.txt", QDir::Files).count();

    QProcess *process = new QProcess(this);     //создание объёкта для процесса

    if ((countFiles%countPoint)==0)
    {
        QVector<double> valuesForPoint; //массив для сохранения всех усреднённых значений
        int temp=countFiles/countPoint;
        for (int i=0; i<countPoint;i++) //проход по всем файлам
        {
            valuesForPoint.clear(); //очистка вектора
            for (int j=0; j<temp; j++)  //обработка по N файлов для каждой точки
            {
                int count=temp*i+j; //нормер текущего файла
                QString filelock=ui->Directory->text()+"/"+fileName[count];
                QFile file(filelock);
                file.open(QIODevice::ReadOnly);
                QStringList lineData = QString(file.readAll()).split("\n");
                process->start(program, lineData);      //запуск консоли с передачей ей значений
                process->waitForFinished();     //Ожидание завершения работы консольного ПО
                QByteArray output = process->readAllStandardOutput();       // Получаем результат
                QString result(output.trimmed());   //Сохранение в строку с удалением пробелов и символов форматирования
                valuesForPoint.push_back(result.toDouble());
            }
            DataStorage::instance().addRow(valuesForPoint);
        }
        ui->save->setEnabled(true);
    }
}


void external_method::on_save_clicked()
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
        QString filefolder=filedir+"/"+filename;
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
    this->close();
}

