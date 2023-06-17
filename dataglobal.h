#ifndef DATAGLOBAL_H
#define DATAGLOBAL_H

#include <QMap>
#include <QVector>
#include <utility>  // for pair

// avoid multiple definition of
extern QMap<QString, QVector<QMap<QString, QString>>> all_dataSet;

extern QVector<std::pair<int, int>> cmpCode;

#endif  // DATAGLOBAL_H
