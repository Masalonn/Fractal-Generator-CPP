#include "fractalwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>

FractalWidget::FractalWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    image = QImage(size(), QImage::Format_RGB32);
    timer.start();
    generateFractal();
}

void FractalWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    image = QImage(size(), QImage::Format_RGB32);
    generateFractal();
    update();
}

void FractalWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawImage(0, 0, image);
}

void FractalWidget::generateFractal()
{
    int w = image.width();
    int h = image.height();
    if (w <= 0 || h <= 0) return;

    double reRange = maxRe - minRe;
    double imRange = maxIm - minIm;

    for (int y = 0; y < h; ++y) {
        double cIm = maxIm - (double)y / (h - 1) * imRange;

        for (int x = 0; x < w; ++x) {
            double cRe = minRe + (double)x / (w - 1) * reRange;

            double zRe = 0.0;
            double zIm = 0.0;
            int iter = 0;

            while (iter < maxIter) {
                double zRe2 = zRe * zRe - zIm * zIm + cRe;
                double zIm2 = 2.0 * zRe * zIm + cIm;
                zRe = zRe2;
                zIm = zIm2;

                if (zRe * zRe + zIm * zIm > 4.0)
                    break;

                ++iter;
            }

            QColor color;
            if (iter == maxIter) {
                color = QColor(0, 0, 0);
            } else {
                int hue = (iter * 10) % 360;
                color = QColor::fromHsv(hue, 255, 255);
            }

            image.setPixelColor(x, y, color);
        }
    }
}

void FractalWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        lastMousePos = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void FractalWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!dragging)
        return;

    QPoint delta = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    int w = width();
    int h = height();
    if (w <= 0 || h <= 0) return;

    double reRange = maxRe - minRe;
    double imRange = maxIm - minIm;

    double deltaRe = (double)delta.x() * reRange / w;
    double deltaIm = (double)delta.y() * imRange / h;

    minRe -= deltaRe;
    maxRe -= deltaRe;
    minIm += deltaIm;
    maxIm += deltaIm;

    if (timer.elapsed() > 60) {
        generateFractal();
        update();
        timer.restart();
    }
}

void FractalWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        generateFractal();
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void FractalWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    if (delta == 0) {
        event->ignore();
        return;
    }

    double zoomFactor = (delta > 0) ? 0.8 : 1.25;

    int w = width();
    int h = height();
    if (w <= 0 || h <= 0) return;

    double reRange = maxRe - minRe;
    double imRange = maxIm - minIm;

    QPointF pos = event->position();
    double mouseRe = minRe + pos.x() / (w - 1) * reRange;
    double mouseIm = maxIm - pos.y() / (h - 1) * imRange;

    double newReRange = reRange * zoomFactor;
    double newImRange = imRange * zoomFactor;

    minRe = mouseRe - newReRange / 2.0;
    maxRe = mouseRe + newReRange / 2.0;
    minIm = mouseIm - newImRange / 2.0;
    maxIm = mouseIm + newImRange / 2.0;

    generateFractal();
    update();
    event->accept();
}
