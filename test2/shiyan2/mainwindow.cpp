#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutbutton.h"
#include "checkbutton.h"
#include "repacesbutton.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QColorDialog"
#include "QFontDialog"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statuLabel.setMaximumHeight(180);
    statuLabel.setText("length:"+QString::number(0)+" lines:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statuLabel);


    statusCursorLabel.setMaximumHeight(180);
    statusCursorLabel.setText("length:"+QString::number(0)+" Cols:"+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author=new QLabel(ui->statusbar);
    author->setText(tr("欧阳曾锦培"));
    ui->statusbar->addPermanentWidget(author);

    ui->actioncopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionpaste->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actionUndo->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode =ui->TextEdit->lineWrapMode();

    if(mode==QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionwrap->setChecked(false);
    }else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionwrap->setChecked(true);
    }

    ui->actionstatus->setChecked(true);
    ui->actiontools->setChecked(true);
    ui->actionnum->setChecked(false);
    on_actionnum_triggered(false);
}

void MainWindow::on_TextEdit_textChanged()
{
    if(!textChange){
        this->setWindowTitle("*"+this->windowTitle());
        textChange=true;
    }
    statuLabel.setText("length:"+QString::number(ui->TextEdit->toPlainText().length())+" lines:"+QString::number(ui->TextEdit->document()->lineCount()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionabout_triggered()
{
    aboutBUTTON about;
    about.exec();
}


void MainWindow::on_checkaction_triggered()
{
    checkBUTTON check(this,ui->TextEdit);
    check.exec();
}



void MainWindow::on_repacesaction_triggered()
{
    repacesBUTTON repaces(this,ui->TextEdit);
    repaces.exec();
}


void MainWindow::on_actionNew_triggered()
{
    ui->TextEdit->clear();
    this->setWindowTitle("新建文本文件");
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename =QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files(*.txt);;ALL(*.*)"));
    QFile file(filename);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"..","打开文件夹");
        return;
    }
    filePath=filename;
    QTextStream in(&file);
    QString text=in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
}


void MainWindow::on_actionSave_triggered()
{
    QString filename;

    if (filePath.isEmpty()) {
        filename = QFileDialog::getSaveFileName(
            this,
            "保存文件",
            ".",
            tr("Text files(*.txt);;ALL(*.*)")
            );
        if (filename.isEmpty()) {
            return;
        }
    } else {
        filename = filePath;
    }

    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(
            this,
            "错误",
            "无法打开文件进行保存：" + file.errorString()
            );
        filename = QFileDialog::getSaveFileName(
            this,
            "重新选择保存路径",
            ".",
            tr("Text files(*.txt);;ALL(*.*)")
            );
        if (filename.isEmpty()) {
            return;
        }
        file.setFileName(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(
                this,
                "错误",
                "保存失败：" + file.errorString()
                );
            return;
        }
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    filePath = filename;
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
}



void MainWindow::on_actionSaveAs_triggered()
{
    // 弹出“另存为”对话框
    QString filename = QFileDialog::getSaveFileName(
        this,
        "另存为",
        ".",
        tr("Text files(*.txt);;ALL(*.*)")
        );

    // 如果用户取消选择，直接返回
    if (filename.isEmpty()) {
        return;
    }

    // 尝试打开文件
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(
            this,
            "错误",
            "无法打开文件进行保存：" + file.errorString()
            );
        return;
    }

    // 写入文件内容
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    // 更新文件路径和窗口标题
    filePath = filename;
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    // 保存后，取消“已修改”标记
    textChange = false;
}


void MainWindow::on_actionEdit_triggered()
{
    // 如果文件未被修改，直接退出
    if (!textChange) {
        this->close();
        return;
    }

    // 弹出询问对话框
    QMessageBox::StandardButton ret = QMessageBox::question(
        this,
        "提示",
        "文件已修改，是否保存？",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

    switch (ret) {
    case QMessageBox::Yes:
        // 保存文件后退出
        on_actionSave_triggered();
        this->close();
        break;
    case QMessageBox::No:
        // 不保存，直接退出
        this->close();
        break;
    case QMessageBox::Cancel:
        // 取消退出
        break;
    default:
        break;
    }
}


void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_actioncopy_triggered()
{
    ui->TextEdit->copy();
    ui->actionpaste->setEnabled(true);
}


void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
    ui->actionpaste->setEnabled(true);
}


void MainWindow::on_actionpaste_triggered()
{
    ui->TextEdit->paste();
}



void MainWindow::on_actionredo_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actioncopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionredo->setEnabled(b);
}


void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_action_color_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {color:%1}").arg(color.name()));
    }

}


void MainWindow::on_actionbeicolor_triggered()
{
    QColor color=QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {beicolor:%1}").arg(color.name()));
    }
}


void MainWindow::on_actionzibeicolor_triggered()
{

}


void MainWindow::on_actionwrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode =ui->TextEdit->lineWrapMode();

    if(mode==QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionwrap->setChecked(true);
    }else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionwrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok,this);

    if(ok){
        ui->TextEdit->setFont(font);
    }
}


void MainWindow::on_actionstatus_triggered()
{
    bool visible=ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionstatus->setChecked(!visible);
}


void MainWindow::on_actiontools_triggered()
{
    bool visible=ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actiontools->setChecked(!visible);
}


void MainWindow::on_actionselectall_triggered()
{
    ui->TextEdit->selectAll();
}




void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int col=0;
    int ln=0;
    int flg=-1;
    int pos=ui->TextEdit->textCursor().position();
    QString text=ui->TextEdit->toPlainText();

    for(int i=0;i<pos;i++){
        if(text[i]=='\n'){
            ln++;
            flg=i;
        }
    }
    flg++;
    col=pos-flg;
    statusCursorLabel.setText("length:"+QString::number(ln+1)+" Cols:"+QString::number(col+1));
}





void MainWindow::on_actionnum_triggered(bool checked)
{
    ui->TextEdit->hidelinenum(!checked);
}

