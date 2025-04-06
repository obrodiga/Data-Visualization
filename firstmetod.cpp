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
}

firstmetod::~firstmetod()
{
    delete ui;
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
        for (int i=0; i<countFiles; i++)
        {
            ui->textBrowser->append(fileName[i]);
        }
        //ui->textBrowser->setText(QString::number(countFiles));
    }
}

