#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QtCore/QTimer>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtXml>

#include "exam.h"

#define HOMENAME "QT5exam"  // 如果ui已设置title，界面优先使用ui中的

namespace Ui {
class Home;
}

// 配置文件的默认位置，使用文件菜单的重新选择文件更换
static QString GfilePath_xml = "/home/xhp/item.xml";

class XmlStreamReader;

class Home : public QMainWindow {
    Q_OBJECT

   public:
    explicit Home(QWidget *parent = 0);
    ~Home();

    void switchpage(int i);
    QCheckBox *getCheckBox_level4();
    QCheckBox *getCheckBox_level6();
    QCheckBox *getCheckBox_level8();
    // 在表中显示 xml 的 dataSet 标签内容
    QTableWidget *getdataTable();

   private slots:
    void on_btngoHome_clicked();
    void on_btngoAnalysis_clicked();

    void selectConfig();
    void selectRecentFiles();
    // 跳出版本号提示框
    void openAppVersion(QWidget *parent = 0);
    void openManual();

    // 主界面关闭，开启子窗口
    void on_btnStart_clicked();

    void on_btnSuperVision_clicked();

    void on_btnItemLook_clicked();

    void on_dataTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_dataTree_itemPressed(QTreeWidgetItem *item, int column);

   private:
    Ui::Home *ui;
    Exam *examWin;

    XmlStreamReader *reader;

    QTimer *ptrTimer;
};

class XmlStreamReader {
   public:
    XmlStreamReader(QTreeWidget *tree);
    XmlStreamReader(QTreeWidget *tree, QTableWidget *table);

    bool readFile(const QString &fileName);

   private:
    void readXhpclElement();  // 解析根元素
    void readLevelElement(QTreeWidgetItem *parent);
    void readDataSetElement(QTreeWidgetItem *parent);
    QMap<QString, QString> readItemElement();
    void skipUnknownElement();

    QTableWidget *tableWidget;
    QTreeWidget *treeWidget;
    QXmlStreamReader reader;
};
#endif  // HOME_H
