  #include "MainWindow.h"
#include "WaveformView.h"
#include <QDockWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), waveformView(new WaveformView(this))
{
    setWindowTitle("SimVision Clone (Qt6)");
    resize(1200, 800);

    // Central widget: waveform area
    setCentralWidget(waveformView);

    // Menu bar
    auto fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open Waveform...", [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Open VCD file", "", "VCD Files (*.vcd)");
        if (!file.isEmpty()) {
            QMessageBox::information(this, "Stub", "Would parse: " + file);
        }
    });
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &MainWindow::close);

    // Dockable panels
    createDockWindows();

    // Status bar
    statusBar()->showMessage("Ready");
}

void MainWindow::createDockWindows() {
    // Hierarchy panel
    auto hierarchyDock = new QDockWidget("Hierarchy", this);
    hierarchyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    auto tree = new QTreeWidget(hierarchyDock);
    tree->setHeaderLabel("Design Hierarchy");
    tree->addTopLevelItem(new QTreeWidgetItem(QStringList() << "top"));
    hierarchyDock->setWidget(tree);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);

    // Console panel
    auto consoleDock = new QDockWidget("Console", this);
    auto console = new QTextEdit(consoleDock);
    console->setReadOnly(true);
    console->setText("SimVision-like console ready...");
    consoleDock->setWidget(console);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
}
