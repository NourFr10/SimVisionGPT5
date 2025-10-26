#pragma once
#include <QDockWidget>
#include <QTreeWidget>
#include "../core/WaveformDatabase.h"

class HierarchyDock : public QDockWidget {
    Q_OBJECT
public:
    explicit HierarchyDock(QWidget *parent = nullptr);

    void buildFromWaveDB(WaveformDatabase* db);

signals:
    void signalSelected(Signal* sig);  // emitted on double-click

private:
    QTreeWidget *tree;
    WaveformDatabase* waveDB = nullptr;

    void addScopeRecursive(QTreeWidgetItem* parentItem,
                           WaveformDatabase::HierNode* node,
                           const QString& pathPrefix);
};
