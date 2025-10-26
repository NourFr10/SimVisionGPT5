#pragma once
#include <QMainWindow>
#include "core/WaveformDatabase.h"

class WaveformView;
class HierarchyDock;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void createDockWindows();
    void openVCDFile();

    WaveformView *waveformView;
    HierarchyDock *hierDock;

    WaveformDatabase waveDB; // <-- store parsed data here
};
