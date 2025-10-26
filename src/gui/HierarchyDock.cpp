#include "HierarchyDock.h"
#include <QHeaderView>

HierarchyDock::HierarchyDock(QWidget *parent)
    : QDockWidget("Hierarchy", parent),
      tree(new QTreeWidget(this))
{
    tree->setHeaderLabel("Design Hierarchy");
    tree->header()->setStretchLastSection(true);

    setWidget(tree);

    connect(tree, &QTreeWidget::itemDoubleClicked,
            this, [this](QTreeWidgetItem* item, int){
        // If item has data "isSignal"
        QVariant isSig = item->data(0, Qt::UserRole + 0);
        if (isSig.isValid() && isSig.toBool()) {
            QVariant sigNameVar = item->data(0, Qt::UserRole + 1);
            QString sigName = sigNameVar.toString();
            if (waveDB) {
                auto it = waveDB->signalsByName.find(sigName.toStdString());
                if (it != waveDB->signalsByName.end()) {
                    emit signalSelected(it->second.get());
                }
            }
        }
    });
}

void HierarchyDock::buildFromWaveDB(WaveformDatabase* db) {
    waveDB = db;
    tree->clear();

    auto rootItem = new QTreeWidgetItem(QStringList() << "top");
    tree->addTopLevelItem(rootItem);

    addScopeRecursive(rootItem, db->root.get(), "");
    tree->expandAll();
}

void HierarchyDock::addScopeRecursive(QTreeWidgetItem* parentItem,
                                      WaveformDatabase::HierNode* node,
                                      const QString& pathPrefix)
{
    // Add signals in this node
    for (Signal* sig : node->signalList) {
        auto *sigItem = new QTreeWidgetItem(parentItem);
        sigItem->setText(0, QString::fromStdString(sig->reference));
        sigItem->setData(0, Qt::UserRole + 0, true); // isSignal = true
        sigItem->setData(0, Qt::UserRole + 1, QString::fromStdString(sig->fullName));
    }

    // Recurse children scopes
    for (auto &kv : node->children) {
        const std::string& childName = kv.first;
        WaveformDatabase::HierNode* childNode = kv.second.get();

        auto *childItem = new QTreeWidgetItem(parentItem);
        childItem->setText(0, QString::fromStdString(childName));
        childItem->setData(0, Qt::UserRole + 0, false); // scope, not signal

        addScopeRecursive(childItem, childNode,
                          pathPrefix + childName.c_str() + ".");
    }
}
