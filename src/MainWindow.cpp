#include "MainWindow.h"
#include "WaveformView.h"
#include "gui/HierarchyDock.h"
#include "core/VCDParser.h"

#include <QDockWidget>
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

    setCentralWidget(waveformView);

    // menu
    auto fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open Waveform...", [this]() {
        openVCDFile();
    });
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &MainWindow::close);

    createDockWindows();

    statusBar()->showMessage("Ready");
}

void MainWindow::createDockWindows() {
    // Hierarchy dock (now its own class)
    hierDock = new HierarchyDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, hierDock);

    // Console panel
    auto consoleDock = new QDockWidget("Console", this);
    auto console = new QTextEdit(consoleDock);
    console->setReadOnly(true);
    console->setText("SimVision-like console ready...");
    consoleDock->setWidget(console);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    // connect hierarchy selection to waveform view (Phase 2 will use this)
    connect(hierDock, &HierarchyDock::signalSelected,
            this, [this](Signal* sig){
        // TODO in Phase 2: add this signal to waveformView's list to render
        statusBar()->showMessage(
            QString("Selected signal: %1").arg(QString::fromStdString(sig->fullName)));
    });
}

void MainWindow::openVCDFile() {
    QString file = QFileDialog::getOpenFileName(
        this, "Open VCD file", "", "VCD Files (*.vcd);;All Files (*)");

    if (file.isEmpty())
        return;

    VCDParser parser;
    std::string err;
    WaveformDatabase newDB; // parse into temp; only replace if ok

    bool ok = parser.parseFile(file.toStdString(), newDB, err);
    if (!ok) {
        QMessageBox::critical(this, "Parse Error",
                              "Failed to parse VCD:\n" + QString::fromStdString(err));
        return;
    }

    // swap into app state
    waveDB = std::move(newDB);

    // rebuild hierarchy dock UI
    hierDock->buildFromWaveDB(&waveDB);

    statusBar()->showMessage("Loaded " + file);
}
