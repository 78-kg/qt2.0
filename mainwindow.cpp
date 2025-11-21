#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog.h>
#include <QMessageBox>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel.setMaximumWidth(150);
    statusLabel.setText(" Length: " + QString::number(0) + "   lines: " + QString::number(1));//设置行号
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText(" Ln: " + QString::number(0) + "   Col : " + QString::number(1));//设置行号
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("陈志凯"));
        ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg;
    dlg.exec();

}


void MainWindow::on_actionNew_triggered()
{
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text file (*.text) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..","打开文件失败");
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
}


void MainWindow::on_actionSave_triggered()
{

}


void MainWindow::on_actionSaveAs_triggered()
{

}

