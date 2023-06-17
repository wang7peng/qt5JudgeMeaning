#include "home.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <string>

#include "dataglobal.h"
#include "ui_home.h"

#define iconPath "/usr/share/qt5/doc/global/template/images/Qt-logo.png"
// #define iconPath "../qt5JudgeMeaning/images/ubuntu_logo.png"
#define pdfPath "/usr/local/etc/manual_qt5JudgeMeaning.pdf"

QMap<QString, QVector<QMap<QString, QString>>> all_dataSet;

void addSkinCircle(QWidget *w, int radiu, int color = 0);

void Home::switchpage(int i) {
    // 如果跳转到分析页，要显示当前数据的分析
    if (i == 2) {
        int count = 0;
        for (int i = 0; i < 5; ++i) {
            if (cmpCode.at(i).first == cmpCode.at(i).second) count++;
        }

        addSkinCircle(ui->led1, 10,
                      cmpCode.at(0).first == cmpCode.at(0).second ? 2 : 1);
        addSkinCircle(ui->led2, 10,
                      cmpCode.at(1).first == cmpCode.at(1).second ? 2 : 1);
        addSkinCircle(ui->led3, 10,
                      cmpCode.at(2).first == cmpCode.at(2).second ? 2 : 1);
        addSkinCircle(ui->led4, 10,
                      cmpCode.at(3).first == cmpCode.at(3).second ? 2 : 1);
        addSkinCircle(ui->led5, 10,
                      cmpCode.at(4).first == cmpCode.at(4).second ? 2 : 1);
        ui->labelRes->setText(QString("对 %1 题").arg(count));
    }
    ui->stackedWidget->setCurrentIndex(i);
}

Home::Home(QWidget *parent) : QMainWindow(parent), ui(new Ui::Home) {
    ui->setupUi(this);

    ui->timeWidget->setDigitCount(8);
    ui->timeWidget->setStyleSheet("border: 0px;");

    ptrTimer = new QTimer(this);
    connect(ptrTimer, &QTimer::timeout, ui->timeWidget, [=] {
        QDateTime cur = QDateTime::currentDateTime();
        QString timeStr = cur.toString("hh:mm:ss");
        ui->timeWidget->display(timeStr);
    });
    ptrTimer->start(1000);

    // 读取xml文件内容, 用两个控件显示
    reader = new XmlStreamReader(ui->dataTree, ui->dataTable);

    // 两边控件宽度比例 3:5
    ui->splitter_2T->setStretchFactor(0, 3);
    ui->splitter_2T->setStretchFactor(1, 5);

    ui->dataTable->setColumnCount(4);
    ui->dataTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    addSkinCircle(ui->led1, 10, 3);
    addSkinCircle(ui->led2, 10);
    addSkinCircle(ui->led3, 10);
    addSkinCircle(ui->led4, 10);
    addSkinCircle(ui->led5, 10);

    connect(ui->action_select, SIGNAL(triggered()), this, SLOT(selectConfig()));
    connect(ui->action_recent, &QAction::triggered, this,
            [=]() { selectRecentFiles(); });

    // about 选项的内容
    connect(ui->action_appV, SIGNAL(triggered()), this, SLOT(openAppVersion()));
    connect(ui->action_qtV, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_manual, SIGNAL(triggered()), this, SLOT(openManual()));
}

Home::~Home() { delete ui; }

void Home::selectConfig() {
    // 获取的是文件的绝对路径
    QString absFilename = QFileDialog::getOpenFileName(
        this, "选择配置文件", "/home",
        "XHP Config File (*.xml);;Subst+ Private File (*.scd *.rcd)");

    // 未选文件直接关闭窗口
    if (absFilename.isEmpty()) {
        qDebug() << "未选好文件";
        return;
    }

    GfilePath_xml = absFilename;

    qDebug() << "get a config file: " << GfilePath_xml;
    reader->readFile(GfilePath_xml);
}

void Home::selectRecentFiles() { qDebug() << "recent 3 files"; }

QCheckBox *Home::getCheckBox_level4() { return ui->level4; }
QCheckBox *Home::getCheckBox_level6() { return ui->level6; }
QCheckBox *Home::getCheckBox_level8() { return ui->level8; }

QTableWidget *Home::getdataTable() { return ui->dataTable; }

void Home::openAppVersion(QWidget *parent) {
    QMessageBox appVersion;
    appVersion.setStyleSheet(
        "QLabel{"
        "max-width: 320px;"
        "}");

    appVersion.setWindowTitle(
        QString("About %1").arg(QCoreApplication::applicationName()));
    appVersion.setText(QString("<h2>一词多义判断题系统 Ver%1</h2>")
                           .arg(QCoreApplication::applicationVersion()) +
                       "<p><font size=4>release time 2023.6.15</font></p>"
                       "<p><font face='times New Roman'>Copyright &copy; "
                       "2023 NewHope Power Inc.</font></p>");
    appVersion.setIconPixmap(QPixmap(iconPath));
    appVersion.exec();
}

void Home::openManual() {
    QProcess *process = new QProcess;
    std::string cmdOpenPDF = "evince " pdfPath;
    // system(cmdOpenPDF.c_str());
    process->start(QString::fromStdString(cmdOpenPDF));
}

void Home::on_btnStart_clicked() {
    if (ui->rbtn_remote->isChecked()) {
        QMessageBox::warning(this, "在线读取不支持", "敬请期待...");
        return;
    }
    // 主页单选必须选上后才能下一步
    if ((ui->level4->isChecked() || ui->level6->isChecked()) &&
        (ui->rbtn_local->isChecked() || ui->rbtn_remote->isChecked())) {
        setVisible(false);
        examWin = new Exam(this);
        examWin->setVisible(true);
    } else {
        QMessageBox::warning(this, "Warning", "no select");
        ui->btnStart->setEnabled(true);
    }
}

void Home::on_btngoHome_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void Home::on_btnSuperVision_clicked() {
    qDebug() << "当前使用的xml: " << GfilePath_xml;
    ui->stackedWidget->setCurrentIndex(1);
}
void Home::on_btngoAnalysis_clicked() { ui->stackedWidget->setCurrentIndex(2); }

// ----------------------------
XmlStreamReader::XmlStreamReader(QTreeWidget *tree) : treeWidget(tree) {
    tableWidget = nullptr;
}

XmlStreamReader::XmlStreamReader(QTreeWidget *tree, QTableWidget *table) {
    treeWidget = tree;
    tableWidget = table;
}

bool XmlStreamReader::readFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        // QMessageBox::warning(this, "读取配置文件出错", file.errorString());
        std::cerr << "Error: Cannot read file " << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    reader.setDevice(&file);
    // 确认文档有内容
    if (reader.readNext() && !reader.isStartDocument()) {
        qDebug() << "no content in file!";
        return false;
    }

    // 也可以直接调用 QXmlStreamReader 的 enum
    enum TokenType {
        StartDocument = 2,
        StartElement = 4,
        Characters = 6,
        Comment = 7,
    };

    // readNext 从第2次开始真正有用
    int ret = reader.readNext();
    while (!reader.atEnd()) {
        qDebug() << ret;
        switch (reader.tokenType()) {
            case Comment:  // =7, skip comment
                break;
            case QXmlStreamReader::StartElement:  // =4
            {
                QString eleName = reader.name().toString();
                qDebug() << QString("获取到标签 " + eleName);
                if (eleName == "XHPCL") {
                    treeWidget->clear();  // 清理老树
                    readXhpclElement();
                } else {
                    reader.raiseError(QObject::tr("Not a XHP config file"));
                }
                break;
            }
            default:
                break;
        }
        // 如果是结尾,注释等各种用不着的行，直接跳过
        ret = reader.readNext();
    }

    file.close();
    if (reader.hasError()) {
        std::cerr << "Error: Failed to parse file " << qPrintable(fileName)
                  << ": " << qPrintable(reader.errorString()) << std::endl;
        return false;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
}

void XmlStreamReader::readXhpclElement() {
    reader.readNext();
    while (!reader.atEnd()) {
        //        if (reader.isEndElement())
        //        {
        //            reader.readNext();
        //            break;
        //        }
        if (reader.isStartElement()) {
            // 找到顶级索引level的开始标签
            if (reader.name() == "Level") {
                qDebug() << QString("获取到元素 %1").arg("Level");
                readLevelElement(treeWidget->invisibleRootItem());
            } else {
                // skipUnknownElement();
                reader.readNext();
            }
        } else {
            reader.readNext();
        }
    }
}

// 传入参数是用来设置 tree 中显示位置的，与 reader 无关
void XmlStreamReader::readLevelElement(QTreeWidgetItem *parent) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, reader.attributes().value("type").toString());

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if (reader.isStartElement()) {
            if (reader.name() == "DataSet") {
                qDebug() << QString("获取到元素 %1").arg("DataSet");
                readDataSetElement(item);
            } else {
                // skipUnknownElement();
                reader.readNext();
            }
        } else {
            reader.readNext();
        }
    }
}

void XmlStreamReader::readDataSetElement(QTreeWidgetItem *parent) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    // 设置树最小项的名字，后续用 item->text(0) 访问
    item->setText(0, reader.attributes().value("name").toString());

    QVector<QMap<QString, QString>> vec;
    vec.clear();

    int count = 0;

    while (reader.readNext()) {
        if (reader.isEndElement()) {  //  同级末尾结束循环
            reader.readNext();
            break;
        }

        switch (static_cast<int>(reader.tokenType())) {
            case QXmlStreamReader::EndElement: {
                qDebug() << "获取到元素" << reader.name().toString();
                break;
            }
            case QXmlStreamReader::StartElement: {
                QString eleName = reader.name().toString();
                if (eleName == "item") {
                    qDebug() << eleName;
                    QMap<QString, QString> a = readItemElement();
                    vec.insert(count++, a);
                } else {
                    // skipUnknownElement();
                    reader.readNext();
                }
                break;
            }
            case QXmlStreamReader::Comment:
                qDebug() << "ignore xxx comment";
                break;
            default:
                break;
        }
    }

    all_dataSet[item->text(0)] = vec;
}

QMap<QString, QString> XmlStreamReader::readItemElement() {
    QMap<QString, QString> itemMap;

    while (reader.readNextStartElement()) {
        // elements consist of content, optionA, optionB, etc.
        QString eleName = reader.name().toString();
        itemMap[eleName] = reader.readElementText();
    }

    for (auto iterM = itemMap.begin(); iterM != itemMap.end(); ++iterM) {
        qDebug() << iterM.key() << iterM.value();
    }

    return itemMap;
}

void XmlStreamReader::skipUnknownElement() {
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}

void addSkinCircle(QWidget *w, int radiu, int color) {
    QString width = QString("min-width: %1 px; max-width: %2 px; ")
                        .arg(radiu * 2)
                        .arg(radiu * 2);
    QString high = QString("min-height: %1 px; max-height: %2 px; ")
                       .arg(radiu * 2)
                       .arg(radiu * 2);

    QString colorStr;
    switch (color) {
        case 1:  // red
            colorStr = "rgb(255, 0, 0)";
            break;
        case 2:  // green
            colorStr = "rgb(60, 179, 113)";
            break;
        case 3:  // like yellow
            colorStr = "rgb(255, 215, 0)";
            break;
        default:  // grey
            colorStr = "rgb(211, 211, 211)";
            break;
    }

    QString strStyle = width + high + QString("background: %1;").arg(colorStr) +
                       QString("border-radius: %1px;").arg(radiu);
    w->setStyleSheet("QLabel{" + strStyle + "}");
}

bool SearchChild(QTreeWidgetItem *item, QString &text) {
    bool bshow = false;
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *CurChildItem = item->child(i);
        if (SearchChild(CurChildItem, text)) {
            CurChildItem->setHidden(false);

            //  item->setSelected(false);
            bshow = true;
            continue;
        } else {
            item->setExpanded(false);
        }
        QString ChildNodeText = CurChildItem->text(0);
        if (ChildNodeText.contains(text, Qt::CaseInsensitive)) {
            CurChildItem->setHidden(false);
            CurChildItem->setSelected(true);  // 自己保持选中状态
            bshow = true;
            continue;
        } else {
            CurChildItem->setSelected(false);
            //  CurChildItem->setHidden(true);
        }
    }
    return bshow;
}

// 根据提供的项目名查找树目录对应
void Home::on_btnItemLook_clicked() {
    QString strInput = ui->itemInput->text();
    if (strInput.length() > 0) {
        qDebug() << strInput;
    }

    // 遍历tree叶子
    for (int i = 0; i < ui->dataTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *curItem = ui->dataTree->topLevelItem(i);

        // 先确认自己是不是要找的
        QString TopNodeText = curItem->text(0);
        if (TopNodeText.contains(strInput, Qt::CaseInsensitive)) {
            curItem->setHidden(false);
            curItem->setSelected(true);
        } else {
            curItem->setSelected(false);
        }

        if (SearchChild(curItem, strInput)) {
            curItem->setHidden(false);
            continue;
        } else {
            // TODO 不符合的节点把叶子收缩起来
        }
    }
    ui->dataTree->expandAll();
}

// 点击树的节点，在右侧表窗口显示该节点具体的数据
void Home::on_dataTree_itemClicked(QTreeWidgetItem *item, int column) {
    // 只在表中显示树叶子标签的数据
    if (0 != item->childCount()) {
        int rows = ui->dataTable->rowCount();
        for (rows; rows > 0; --rows) {
            ui->dataTable->removeRow(rows - 1);
        }
        return;
    }

    QVector<QMap<QString, QString>> vec = all_dataSet[item->text(column)];

    ui->dataTable->setRowCount(vec.length());

    for (auto row = 0; row < vec.length(); ++row) {
        QMap<QString, QString> itemsMap = vec.at(row);
        ui->dataTable->setItem(row, 0,
                               new QTableWidgetItem(itemsMap["content"]));
        ui->dataTable->setItem(row, 1,
                               new QTableWidgetItem(itemsMap["optionA"]));
        ui->dataTable->setItem(row, 2,
                               new QTableWidgetItem(itemsMap["optionB"]));

        QTableWidgetItem *judge = new QTableWidgetItem(itemsMap["val"]);
        judge->setTextAlignment(Qt::AlignCenter);
        ui->dataTable->setItem(row, 3, judge);
    }
}

void Home::on_dataTree_itemPressed(QTreeWidgetItem *item, int column) {
    // ignore
}
