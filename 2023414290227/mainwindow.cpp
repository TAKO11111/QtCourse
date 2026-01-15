#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDate>
#include    <QTableWidgetItem>
#include    <QRandomGenerator>
#include    <QMessageBox>


//为一行的单元格创建 Items
void MainWindow::createItemsARow(int rowNo,QString name,QString sex,QDate birth,QString nation,bool isPM,int score)
{
    uint studID=202105000;  //学号基数
    //姓名
    QTableWidgetItem *item=new  QTableWidgetItem(name, MainWindow::ctName);   //数据项类型为MainWindow::ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studID  +=rowNo;        //学号 =基数 + 行号
    item->setData(Qt::UserRole,QVariant(studID));           //设置studID为用户数据
    ui->tableinfo->setItem(rowNo,MainWindow::colName,item);

    //性别
    QIcon   icon;
    if (sex=="男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");

    item=new  QTableWidgetItem(sex,MainWindow::ctSex);      //type为MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags=Qt::ItemIsSelectable |Qt::ItemIsEnabled;    //不允许编辑
    item->setFlags(flags);
    ui->tableinfo->setItem(rowNo,MainWindow::colSex,item);  //为单元格设置Item

    //出生日期
    QString str=birth.toString("yyyy-MM-dd");   //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);        //type为MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);   //文本对齐格式
    ui->tableinfo->setItem(rowNo,MainWindow::colBirth,item);

    //民族
    item=new  QTableWidgetItem(nation,MainWindow::ctNation);        //type为MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableinfo->setItem(rowNo,MainWindow::colNation,item);

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);      //type为 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    flags= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled;   //不允许编辑，但可以更改复选状态
    item->setFlags(flags);
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));   //设置背景颜色
    ui->tableinfo->setItem(rowNo,MainWindow::colPartyM,item);

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);    //type为MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableinfo->setItem(rowNo,MainWindow::colScore,item);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setCentralWidget(ui->splitterMain);

    //状态栏初始化创建
    labCellIndex = new QLabel("当前单元格坐标：",this);
    labCellIndex->setMinimumWidth(250);

    labCellType=new QLabel("当前单元格类型：",this);
    labCellType->setMinimumWidth(200);

    labStudID=new QLabel("学生ID：",this);
    labStudID->setMinimumWidth(200);

    ui->statusBar->addWidget(labCellIndex); //添加到状态栏
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//设置水平表头
void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    //    ui->tableInfo->setHorizontalHeaderLabels(headerText); //只设置标题
    ui->tableinfo->setColumnCount(headerText.size());      //设置表格列数
    for (int i=0;i<ui->tableinfo->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setBold(true);              //设置为粗体
        font.setPointSize(11);           //字体大小
        headerItem->setForeground(QBrush(Qt::red));  //设置文字颜色
        headerItem->setFont(font);       //设置字体
        ui->tableinfo->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }
}

//设置行数,设置的行数为数据区的行数，不含表头
void MainWindow::on_btnSetRows_clicked()
{
    ui->tableinfo->setRowCount(ui->spinRowCount->value());//设置数据区行数
    ui->tableinfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); //设置交替行背景颜色
}


//初始化表格数据
void MainWindow::on_btnIniData_clicked()
{
    QDate   birth(2001,4,6);        //初始化一个日期
    ui->tableinfo->clearContents(); //只清除工作区，不清除表头
    for (int i=0; i<ui->tableinfo->rowCount(); i++)
    {
        QString strName=QString("学生%1").arg(i);
        QString strSex= ((i % 2)==0)? "男":"女";
        bool isParty= ((i % 2)==0)? false:true;
        int score= QRandomGenerator::global()->bounded(60,100);   //随机数[60,100)
        createItemsARow(i, strName, strSex, birth,"汉族",isParty,score);  //为某一行创建items
        birth=birth.addDays(20);    //日期加20天
    }
}

void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{ //设置表格是否可编辑，以及进入编辑模式的方式
    if (checked)
        //双击或获取焦点后单击，进入编辑状态
        ui->tableinfo->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);
    else
        ui->tableinfo->setEditTriggers(QAbstractItemView::NoEditTriggers); //不允许编辑
}

void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{//是否显示水平表头
    ui->tableinfo->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{//是否显示垂直表头
    ui->tableinfo->verticalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{ //行的底色交替采用不同颜色
    ui->tableinfo->setAlternatingRowColors(checked);
}

void MainWindow::on_rBtnSelectItem_clicked()
{//选择方式：单元格选择
    ui->tableinfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainWindow::on_rBtnSelectRow_clicked()
{//选择方式：行选择
    ui->tableinfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}


void MainWindow::on_btnReadToEdit_clicked()
{
//    QString str;
    QTableWidgetItem    *item;

    ui->textEdit->clear();  //文本编辑器清空
    for (int i=0;i<ui->tableinfo->rowCount();i++)   //逐行处理
    {
        QString str=QString::asprintf("第 %d 行： ",i+1);
        for (int j=0;j<ui->tableinfo->columnCount()-1;j++) //逐列处理，但最后一列是check型，单独处理
        {
            item=ui->tableinfo->item(i,j);      //获取单元格的item
            str=str+item->text()+"   ";         //字符串连接
        }
        item=ui->tableinfo->item(i,colPartyM);  //最后一列，党员
        if (item->checkState()==Qt::Checked)    //根据check状态显示文字
            str=str+"党员";
        else
            str=str+"群众";
        ui->textEdit->appendPlainText(str);     //添加到编辑框作为一行
    }
}

//currentCellChanged()信号的槽函数，当前单元格发生变化时的响应
void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    QTableWidgetItem* item=ui->tableinfo->item(currentRow,currentColumn); //获取单元格的Item
    if  (item==nullptr)
        return;

    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列",currentRow,currentColumn));

    int cellType=item->type();      //获取单元格的类型
    labCellType->setText(QString::asprintf("当前单元格类型：%d",cellType));

    item=ui->tableinfo->item(currentRow,MainWindow::colName);   //取当前行第1列的单元格的item
    uint ID=item->data(Qt::UserRole).toUInt();        //读取用户数据
    labStudID->setText(QString::asprintf("学生ID：%d",ID));      //学生ID
}

//插入一行
void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableinfo->currentRow();     //当前行号

    ui->tableinfo->insertRow(curRow);           //插入一行，但不会自动为单元格创建item
    createItemsARow(curRow, "新学生", "男",
                    QDate::fromString("2002-10-1","yyyy-M-d"),"苗族",true,80 ); //为某一行创建items
}

//添加一行
void MainWindow::on_btnAppendRow_clicked()
{
    int curRow=ui->tableinfo->rowCount();       //当前行号
    ui->tableinfo->insertRow(curRow);           //在表格尾部添加一行
    createItemsARow(curRow, "新生", "女",
                    QDate::fromString("2002-6-5","yyyy-M-d"),"满族",false,76 ); //为某一行创建items
}

//删除当前行及其items
void MainWindow::on_btnDelCurRow_clicked()
{
    int curRow=ui->tableinfo->currentRow();     //当前行号
    ui->tableinfo->removeRow(curRow);           //删除当前行及其items
}

void MainWindow::on_btnAutoHeght_clicked()
{
    ui->tableinfo->resizeRowsToContents();
}

void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableinfo->resizeColumnsToContents();
}

void MainWindow::on_toolsbutton_triggered()
{

    // 1. 清空表格所有内容（包括表头和数据）
    ui->tableinfo->clear();
    ui->tableinfo->setRowCount(0);


    const int colCount = 7;
    ui->tableinfo->setColumnCount(colCount);

    // 3. 设置表头
    QStringList headers = {"学号", "姓名", "性别", "行政班级", "院系", "专业", "修读性质"};
    ui->tableinfo->setHorizontalHeaderLabels(headers);

    // 4. 学生数据
    QVector<QVector<QString>> studentData = {
        {"2023414290223", "卢树杰", "男", "2023软件2班", "计算机学院", "软件工程", "全日制"},
        {"2023414290224", "罗企翔", "男", "2023软件2班", "计算机学院", "软件工程", "全日制"},
        {"2023414290227", "欧阳曾锦培", "男", "2023软件2班", "计算机学院", "软件工程", "全日制"},
        {"2023414290228", "苏童鼎", "男", "2023软件2班", "计算机学院", "软件工程", "全日制"},
        {"2023414290229", "孙斯琦", "女", "2023软件2班", "计算机学院", "软件工程", "全日制"}
    };
    QIcon   icon;

    // 5. 填充表格数据
    ui->tableinfo->setRowCount(studentData.size());  // 设置行数为5
    for (int row = 0; row < studentData.size(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            // 确保数据索引有效
            if (col >= studentData[row].size()) {
                QMessageBox::warning(this, "数据错误", QString("第%1行第%2列数据缺失").arg(row).arg(col));
                continue;
            }
            // 创建单元格并设置内容
            QTableWidgetItem *item = new QTableWidgetItem(studentData[row][col]);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableinfo->setItem(row, col, item);
            if (col == 2) {
                QIcon icon;
                if (studentData[row][col] == "男") {
                    icon.addFile(":/images/icons/boy.ico");
                } else {
                    icon.addFile(":/images/icons/girl.ico");
                }
                item->setIcon(icon);
            }

            if (studentData[row][0] == "2023414290227") {
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(Qt::red));
            }
        }
    }

    // 7. 自动调整列宽以完整显示内容
    ui->tableinfo->resizeColumnsToContents();
    QMessageBox::information(this, "完成", "学生名单设置成功！");

}

