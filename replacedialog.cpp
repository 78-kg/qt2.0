#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QTextCursor>
#include <QMessageBox>
#include <QDebug>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;

    // 设置默认搜索方向为向下
    ui->rbDown->setChecked(true);

    // 调试信息
    qDebug() << "ReplaceDialog created, textEdit:" << (pTextEdit != nullptr);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btFindNext_clicked()
{
    // 检查文本编辑器是否有效
    if (!pTextEdit) {
        qDebug() << "Error: pTextEdit is null";
        QMessageBox::warning(this, tr("错误"), tr("文本编辑器未初始化"));
        return;
    }

    QString target = ui->searchText->text();
    if (target.isEmpty()) {
        QMessageBox::information(this, tr("提示"), tr("请输入要查找的内容"));
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();

    // 获取搜索选项
    bool caseSensitive = ui->cbCaseSensetive->isChecked();
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
        index = text.lastIndexOf(target, startPos - 1, sensitivity);

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

void ReplaceDialog::on_btReplace_clicked()
{
    // 检查文本编辑器是否有效
    if (!pTextEdit) {
        qDebug() << "Error: pTextEdit is null";
        QMessageBox::warning(this, tr("错误"), tr("文本编辑器未初始化"));
        return;
    }

    // 获取查找和替换文本
    QString target = ui->searchText->text();
    QString replaceText = ui->targetText->text();

    if (target.isEmpty()) {
        QMessageBox::information(this, tr("提示"), tr("请输入要查找的内容"));
        return;
    }

    QTextCursor c = pTextEdit->textCursor();

    // 获取搜索选项
    bool caseSensitive = ui->cbCaseSensetive->isChecked();

    // 检查当前是否有选中的文本，且选中的文本是否与目标匹配
    if (c.hasSelection()) {
        QString selectedText = c.selectedText();
        bool isMatch = caseSensitive ? (selectedText == target)
                                     : (selectedText.compare(target, Qt::CaseInsensitive) == 0);

        if (isMatch) {
            // 替换当前选中的文本
            c.insertText(replaceText);
        }
    }

    // 查找下一个
    on_btFindNext_clicked();
}

void ReplaceDialog::on_btReplaceAll_clicked()
{
    // 检查文本编辑器是否有效
    if (!pTextEdit) {
        qDebug() << "Error: pTextEdit is null";
        QMessageBox::warning(this, tr("错误"), tr("文本编辑器未初始化"));
        return;
    }

    // 获取查找和替换文本
    QString target = ui->searchText->text();
    QString replaceText = ui->targetText->text();

    if (target.isEmpty()) {
        QMessageBox::information(this, tr("提示"), tr("请输入要查找的内容"));
        return;
    }

    QString originalText = pTextEdit->toPlainText();
    QString newText = originalText;

    // 获取搜索选项
    bool caseSensitive = ui->cbCaseSensetive->isChecked();
    Qt::CaseSensitivity sensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // 统计替换次数
    int replaceCount = 0;

    // 执行全部替换
    if (caseSensitive) {
        // 区分大小写 - 简单替换
        replaceCount = originalText.count(target);
        newText = originalText.replace(target, replaceText, sensitivity);
    } else {
        // 不区分大小写 - 需要手动处理
        QString tempText = originalText;
        int index = 0;

        while (index < tempText.length()) {
            index = tempText.indexOf(target, index, sensitivity);
            if (index == -1) {
                break;
            }
            tempText.replace(index, target.length(), replaceText);
            index += replaceText.length();
            replaceCount++;
        }
        newText = tempText;
    }

    // 更新文本编辑器的内容
    if (replaceCount > 0) {
        pTextEdit->setPlainText(newText);
        QMessageBox::information(this, tr("替换完成"),
                                 tr("共替换了 %1 个匹配项").arg(replaceCount));
    } else {
        QMessageBox::information(this, tr("替换"),
                                 tr("没有找到匹配项"));
    }
}

void ReplaceDialog::on_btCancel_clicked()
{
    close();
}
