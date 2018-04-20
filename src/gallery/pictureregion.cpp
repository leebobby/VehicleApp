#include "pictureregion.h"
#include <QEvent>
#include <QPainter>
#include <QGesture>
#include <QPointF>
#include <QPinchGesture>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>

PictureRegion::PictureRegion(QWidget *parent)
    : QWidget(parent),
    horizontalOffset(0),
    verticalOffset(0),
    scaleFactor(1),
    currentStepScaleFactor(1),
    m_translateButton(Qt::LeftButton),
    m_bMouseTranslate(false),
    m_zoomDelta(0.2)
{
    this->setFocusPolicy(Qt::ClickFocus);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
}
void PictureRegion::setPicture(QImage &image)
{
    if(image.format() == QImage::Format_Mono)
    {
        qDebug() << "format_mono";
        currentImage = image.convertToFormat(QImage::Format_Mono);
    }
    else if(image.format()  == QImage::Format_MonoLSB)
    {
        qDebug() << "format_monoLSB";
        currentImage = image.convertToFormat(QImage::Format_MonoLSB);
    }
    else if(image.format()  == QImage::Format_Indexed8)
    {
        qDebug() << "QImage::Format_Indexed8";
        currentImage = image.convertToFormat(QImage::Format_Indexed8);
    }
    else if(image.format()  == QImage::Format_RGB32)
    {
        qDebug() << "QImage::Format_RGB32";
        currentImage = image.convertToFormat(QImage::Format_RGB32);
    }
    else if(image.format()  == QImage::Format_ARGB32)
    {
        qDebug()<<"QImage::Format_ARGB32";
        currentImage = image.convertToFormat(QImage::Format_ARGB32);
    }

    else if(image.format()  ==QImage::Format_ARGB32_Premultiplied)
    {
         qDebug()<<"QImage::Format_ARGB32_Premultiplied";
         currentImage = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    else if(image.format()  == QImage::Format_RGB16)
    {
        qDebug("QImage::Format_RGB16");
        currentImage = image.convertToFormat(QImage::Format_RGB16);
    }

    else if(image.format()  == QImage::Format_ARGB8565_Premultiplied)
    {
        qDebug()<<"QImage::Format_ARGB8565_Premultiplied";
        currentImage = image.convertToFormat(QImage::Format_ARGB8565_Premultiplied);
    }

    else if(image.format()  == QImage::Format_RGB666)
    {
        qDebug()<<"QImage::Format_RGB666";
        currentImage = image.convertToFormat(QImage::Format_RGB666);
    }

    else if(image.format()  == QImage::Format_ARGB6666_Premultiplied)
    {
        qDebug()<<"QImage::Format_ARGB6666_Premultiplied";
        currentImage = image.convertToFormat(QImage::Format_ARGB6666_Premultiplied);
    }

    else if(image.format()  == QImage::Format_RGB555)
    {
        qDebug()<<"QImage::Format_RGB555";
        currentImage = image.convertToFormat(QImage::Format_RGB555);
    }

    else if(image.format()  == QImage::Format_ARGB8555_Premultiplied)
    {
        qDebug()<<"QImage::Format_ARGB8555_Premultiplied";
        currentImage = image.convertToFormat(QImage::Format_ARGB8555_Premultiplied);
    }

    else if(image.format()  == QImage::Format_RGB888)
    {
        qDebug()<<"QImage::Format_RGB888";
        currentImage = image.convertToFormat(QImage::Format_RGB888);
    }

    else if(image.format() == QImage::Format_RGB444)
    {
        qDebug()<<"QImage::Format_RGB444";
        currentImage = image.convertToFormat(QImage::Format_RGB444);
    }
    else if(image.format()  == QImage::Format_ARGB4444_Premultiplied)
    {
        qDebug()<<"QImage::Format_ARGB4444_Premultiplied";
        currentImage = image.convertToFormat(QImage::Format_ARGB4444_Premultiplied);
    }
    else
    {
        qDebug()<<"unkown format";
    }
    update();
}

void PictureRegion::clearPicture()
{

}

bool PictureRegion::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

void PictureRegion::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QImage image = currentImage;


    if(!image.isNull()){
        image = image.scaled(this->width()*currentStepScaleFactor * scaleFactor,
                             this->height()*currentStepScaleFactor * scaleFactor,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }
    const qreal iw = image.width();
    const qreal ih = image.height();
    const qreal wh = height();
    const qreal ww = width();

    painter.translate(ww/2, wh/2);
    painter.translate(horizontalOffset, verticalOffset);
    //painter.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
    painter.translate(-iw/2, -ih/2);
    painter.drawImage(0,0,image);
}
//还原
void PictureRegion::mouseDoubleClickEvent(QMouseEvent *)
{
    scaleFactor = 1;
    currentStepScaleFactor = 1;
    verticalOffset = 0;
    horizontalOffset = 0;
    update();
}

bool PictureRegion::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void PictureRegion::panTriggered(QPanGesture *gesture)
{
#ifndef QT_NO_CURSOR
    switch (gesture->state()) {
        case Qt::GestureStarted:
        case Qt::GestureUpdated:
            setCursor(Qt::SizeAllCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
    }
#endif
    QPointF delta = gesture->delta();
    horizontalOffset += delta.x();
    verticalOffset += delta.y();
    update();
}

void PictureRegion::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        currentStepScaleFactor = gesture->totalScaleFactor();
    }
    if (gesture->state() == Qt::GestureFinished) {
        scaleFactor *= currentStepScaleFactor;
        currentStepScaleFactor = 1;
    }
    update();
}

void PictureRegion::resizeEvent(QResizeEvent*e)
{
    update();
    QWidget::resizeEvent(e);
}

void PictureRegion::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        QPointF mouseDelta = event->pos() - m_lastMousePos;
        translate(mouseDelta);
    }

    m_lastMousePos = event->pos();

    QWidget::mouseMoveEvent(event);
}

void PictureRegion::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "PictureRegion::mousePressEvent";
    if (event->button() == m_translateButton) {
        m_bMouseTranslate = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::OpenHandCursor);
    }

    QWidget::mousePressEvent(event);
}

void PictureRegion::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton)
    {
        m_bMouseTranslate = false;
        setCursor(Qt::ArrowCursor);
    }

    QWidget::mouseReleaseEvent(event);
}


void PictureRegion::zoomIn()
{
    zoom(1 + m_zoomDelta);
}


void PictureRegion::zoomOut()
{
    zoom(1 - m_zoomDelta);
}


void PictureRegion::zoom(float scale)
{
    scaleFactor *= scale;
    update();
}


void PictureRegion::translate(QPointF delta)
{
    horizontalOffset += delta.x();
    verticalOffset += delta.y();
    update();
}
