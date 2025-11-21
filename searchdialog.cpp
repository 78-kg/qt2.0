#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QTextCursor>
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent , QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;

    // 设置默认搜索方向为向下
    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btFindNext_clicked()
{
    QString target = ui->searchText->text();
    if (target.isEmpty()) {
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();

    // 获取搜索选项
    bool caseSensitive = ui->cbCaseSensetive->isChecked();  // 修正拼写
    bool searchDown = ui->rbDown->isChecked();

    Qt::CaseSensitivity sensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    int index = -1;
    int startPos = c.position();

    if (searchDown) {
        // 向下搜索：从当前位置开始到文本末尾
        index = text.indexOf(target, startPos, sensitivity);

        // 如果没找到且允许循环，则从头开始搜索
        if (index == -1 && startPos > 0) {
            index = text.indexOf(target, 0, sensitivity);
        }
    } else {
        // 向上搜索：从当前位置向前搜索
        QString textBeforeCursor = text.left(startPos);
        index = textBeforeCursor.lastIndexOf(target, -1, sensitivity);

        // 如果没找到且允许循环，则从末尾开始搜索
        if (index == -1 && startPos < text.length()) {
            index = text.lastIndexOf(target, -1, sensitivity);
        }
    }

    if (index >= 0) {
        c.setPosition(index);
        c.setPosition(index + target.length(), QTextCursor::KeepAnchor);
        pTextEdit->setTextCursor(c);

        // 确保找到的文本在视图中可见
        pTextEdit->ensureCursorVisible();
    } else {
        QMessageBox::information(this, tr("查找"),
                                 tr("找不到\"%1\"").arg(target));
    }
}

void SearchDialog::on_btCancel_clicked()
{
    close();
}
