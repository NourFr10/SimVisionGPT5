#pragma once
#include <QMainWindow>

class WaveformView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void createDockWindows();
    WaveformView *waveformView;
};
