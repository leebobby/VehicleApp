#ifndef PictureRegion_H
#define PictureRegion_H

#include <QWidget>

class QGestureEvent;
class QPanGesture;
class QPinchGesture;
class QSwipeGesture;
class PictureRegion;

class PictureRegion : public QWidget
{
    Q_OBJECT
public:
    PictureRegion(QWidget *parent = 0);

    void setPicture(QImage & image);

    void clearPicture();

protected:
    // 放大/缩小
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 平移
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;


public Q_SLOTS:
    void zoomIn();  //
    void zoomOut();  //
    void zoom(float scale); //
    void translate(QPointF delta);  //move


private:
    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture*);
    void pinchTriggered(QPinchGesture*);

    QImage loadImage(const QString &fileName);

    QImage currentImage;

    qreal horizontalOffset;
    qreal verticalOffset;

    qreal scaleFactor;
    qreal currentStepScaleFactor;
    Qt::MouseButton m_translateButton;
    bool m_bMouseTranslate;
    qreal m_zoomDelta;
    QPoint m_lastMousePos;
};

#endif // PictureRegion_H
