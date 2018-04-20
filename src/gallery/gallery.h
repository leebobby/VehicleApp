/*!
 * @file gallery.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef GALLERY_H
#define GALLERY_H

#include <QWidget>
#include <QScrollBar>
#include <QPropertyAnimation>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QFileInfo>
#include <QModelIndex>
#include <QListWidgetItem>

#include "optionwidget.h"
#include "../waittingwidget.h"
#include "picturepreview.h"

namespace Ui {
class Gallery;
}
class ScanPictureFileThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanPictureFileThread(QObject *parent = 0);
    ~ScanPictureFileThread();
protected:
    virtual void run();
signals:
    void scanComplete(QList<QFileInfo>*,QList<QListWidgetItem*>);
private:
    QList<QFileInfo>* fileInfoList;
    QList<QListWidgetItem*> widgetItemList;
};
#if 0
class insertListThread : public QThread
{
    Q_OBJECT
public:
    explicit insertListThread(QObject *parent = 0);
    ~insertListThread();
protected:
    virtual void run();
signals:
    void insertComplete(QList<QFileInfo>* listFileInfo);
private:

};
#endif
class Gallery : public QWidget
{
    Q_OBJECT

public:
    static Gallery *Instance(QWidget *parent = 0);//create window by this interface

    ~Gallery();

    void initOptions();
    void initImageList();


signals:
    void notifyWatitingWidget();

    void sendtoPicView(QList<QFileInfo>,QString,int);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *);

private slots:
    void on_homeButton_clicked();

    void showOptionWidget();

    void onScanComplete(QList<QFileInfo>*,QList<QListWidgetItem*>);

    void showBigPicture(QModelIndex);

    void onNotifyGalleryUnHide();

    void onNotifyGalleryClearList();

    void onNotifyGalleryAddPic();

    void onNotifyGalleryRefreshList();



private:
    static Gallery* self;//this  is for single instance
    explicit Gallery(QWidget *parent = 0);

    Ui::Gallery *ui;

    OptionWidget* pWidget;

    QScrollBar *m_scrollBarV;

    QList<QFileInfo> m_PictureList;
    QList<QListWidgetItem*> m_WidgetListItem;

    WaittingWidget* m_waittingWidget;

    PicturePreview* m_PicPreView;
};

#endif // GALLERY_H
