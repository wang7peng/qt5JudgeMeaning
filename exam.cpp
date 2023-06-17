#include "exam.h"

#include <QDebug>
#include <QTime>

#include "dataglobal.h"
#include "home.h"
#include "ui_exam.h"

QVector<std::pair<int, int>> cmpCode(5);

// 获取从 xml 存好的数据，存入QList
void setContent(QList<ItemData> *list, Home *parent) {
    QString dataSetName;

    int scale4[5] = {1, 3, 4, 1, 3};
    int scale6[5] = {2, 2, 2, 2, 2};
    int scale8[5] = {5, 5, 5, 5, 5};

    qsrand(QTime::currentTime().msec());
    int index = 1 + qrand() % 4;  // in [1, 5]

    if (!parent->getCheckBox_level8()->isChecked()) {
        dataSetName = QString("第%1次").arg(scale8[index]);
    } else if (!parent->getCheckBox_level6()->isChecked()) {
        dataSetName = QString("第%1次").arg(scale6[index]);
    } else {
        dataSetName = QString("第%1次").arg(scale4[index]);
    }
    qDebug() << dataSetName;

    QVector<QMap<QString, QString>> vec = all_dataSet[dataSetName];

    for (auto iterV = vec.begin(); iterV != vec.end(); ++iterV) {
        QMap<QString, QString> itemsMap = *iterV;
        ItemData dataOne;

        for (int i = 0; i < itemsMap.size(); ++i) {
            dataOne.content = itemsMap["content"];
            dataOne.optionA = itemsMap["optionA"];
            dataOne.optionB = itemsMap["optionB"];

            dataOne.val = itemsMap["val"] == "T" ? true : false;
        }
        *list << dataOne;
    }

    // 题数限制，保持5条
    while (list->length() < 5) {
        *list << ItemData{"Flick", "轻抚", "轻拍", true};
    }
}

Exam::Exam(QWidget *parent) : QDialog(parent), ui(new Ui::Exam) {
    ui->setupUi(this);
    progressVal = 0;
    home = (Home *)parent;

    ui->progressBar->setRange(0, 20);
    ptrTimer = new QTimer(this);
    connect(ptrTimer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    ptrTimer->start(1000);

    ui->table->setRowCount(5);
    ui->table->setColumnCount(4);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table->setAlternatingRowColors(true);

    QStringList headitem;
    headitem << "content"
             << "A选项"
             << "B选项"
             << "只选对的";
    ui->table->setHorizontalHeaderLabels(headitem);

    itemlist.clear();
    setContent(&itemlist, home);
    if (itemlist.isEmpty()) {
        qDebug() << "no data";
    }
    QTableWidgetItem *itemCheck = new QTableWidgetItem();
    itemCheck->setCheckState(Qt::Unchecked);

    for (int i = 0; i < itemlist.size(); ++i) {
        ui->table->setItem(i, 0, new QTableWidgetItem(itemlist.at(i).content));
        ui->table->setItem(i, 1, new QTableWidgetItem(itemlist.at(i).optionA));
        ui->table->setItem(i, 2, new QTableWidgetItem(itemlist.at(i).optionB));

        QTableWidgetItem *itemCheck = new QTableWidgetItem();
        itemCheck->setCheckState(Qt::Unchecked);
        itemCheck->setText("T");
        ui->table->setItem(i, 3, itemCheck);
    }

    connect(ui->btnCommit, SIGNAL(clicked()), this, SLOT(updateLabelInfo()));
}

Exam::~Exam() { delete ui; }

void Exam::updateProgress() {
    ui->progressBar->setValue(++progressVal);
    ui->progressBar->update();
    // 自动提交
    if (progressVal > ui->progressBar->maximum()) {
        progressVal = 0;
        ui->btnCommit->click();
    }
}

void Exam::updateLabelInfo() { ui->labelMsg->setText("已提交"); }

// 先处理数据再操作页面
void Exam::on_btnCommit_clicked() {
    std::pair<int, int> t = std::make_pair(0, 0);
    // 0表示打勾，2表示未打
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        t.first = itemlist.at(i).val == true ? 2 : 0;
        t.second = ui->table->item(i, 3)->checkState();
        cmpCode.insert(i, t);
    }

    ui->btnCommit->setEnabled(false);
    home->switchpage(2);
    home->setVisible(true);

    ptrTimer->stop();  // 提交完成不再倒计时
}
