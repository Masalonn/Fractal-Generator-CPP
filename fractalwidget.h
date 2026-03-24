#ifndef FRACTALWIDGET_H
#define FRACTALWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QElapsedTimer>

class FractalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FractalWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QImage image;

    double minRe = -2.0;
    double maxRe = 1.0;
    double minIm = -1.5;
    double maxIm = 1.5;
    int maxIter = 80;

    bool dragging = false;
    QPoint lastMousePos;
    QElapsedTimer timer;

    void generateFractal();
};

#endif // FRACTALWIDGET_H
