#include "WaveformView.h"
#include <QPainter>

WaveformView::WaveformView(QWidget *parent)
    : QOpenGLWidget(parent) {}

void WaveformView::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void WaveformView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void WaveformView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Simple demo rendering — fake waveform lines
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::green, 2));

    int midY = height() / 2;
    int step = width() / 20;

    for (int i = 0; i < 20; ++i) {
        int y = (i % 2 == 0) ? midY - 50 : midY + 50;
        painter.drawLine(i * step, midY, (i + 1) * step, y);
    }

    painter.end();
}
