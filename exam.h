#ifndef EXAM_H
#define EXAM_H

#include <QDialog>
#include <QtCore/QTimer>

class Home;

namespace Ui {
class Exam;
}

struct ItemData {
    QString content;
    QString optionA;
    QString optionB;
    bool val;
};

class Exam : public QDialog {
    Q_OBJECT

   public:
    explicit Exam(QWidget *parent = 0);
    ~Exam();

   private slots:
    void on_btnCommit_clicked();
    void updateProgress();
    void updateLabelInfo();

   private:
    Ui::Exam *ui;
    Home *home;

    int progressVal;
    QTimer *ptrTimer;

    QList<ItemData> itemlist;
};

#endif  // EXAM_H
