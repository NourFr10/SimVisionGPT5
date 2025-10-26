#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class WaveformView : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit WaveformView(QWidget *parent = nullptr);
    ~WaveformView() override = default;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
