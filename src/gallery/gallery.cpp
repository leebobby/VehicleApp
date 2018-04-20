/*!
 * @file gallery.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "gallery.h"
#include "ui_gallery.h"


#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QDir>
#include <QPropertyAnimation>
#include <QListWidgetItem>

/*!
 * @pGlobalMainDesk
 * this global var is defined in maindesk, for widget return
 */
#include "src/maindesk.h"
extern MainDesk* pGlobalMainDesk;

Gallery *Gallery::self = NULL;
Gallery *Gallery::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            self = new Gallery(parent);
        }
    }
    return self;
}

Gallery::Gallery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gallery)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);//the ~CameraSetting will be call when func close() run

    initOptions();

    m_scrollBarV = ui->imageList->verticalScrollBar();
    ui->imageList->viewport()->installEventFilter(this);
    connect(ui->imageList,SIGNAL(clicked(QModelIndex)),this,SLOT(showBigPicture(QModelIndex)));

    m_PicPreView = new PicturePreview;
    connect(this,SIGNAL(sendtoPicView(QList<QFileInfo>,QString,int)),\
            m_PicPreView,SLOT(receiveFileList(QList<QFileInfo>,QString,int)));
    connect(m_PicPreView,SIGNAL(notifyGalleryUnHide()),\
            this,SLOT(onNotifyGalleryUnHide()));
}

Gallery::~Gallery()
{
    delete ui;
    self = NULL;

}
void Gallery::showBigPicture(QModelIndex pos)
{
    QListWidgetItem* item = ui->imageList->currentItem();
    qDebug()<<"filename"<<item->text();
    qDebug()<<"which file:"<<pos.row();

    QString filePathToShowBig = m_PictureList.at(pos.row()).absoluteFilePath();

    qDebug()<<"file path"<<filePathToShowBig;
    emit sendtoPicView(m_PictureList,filePathToShowBig,pos.row());

    this->hide();
    m_PicPreView->show();

}
void Gallery::onNotifyGalleryUnHide()
{
    this->setHidden(false);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief ScanMusicFileThread::ScanMusicFileThread
/// this thread is used for scan music file from udisk and other path
ScanPictureFileThread::ScanPictureFileThread(QObject *parent)
    : QThread(parent)
{
    qDebug() << "ScanPictureFileThread Thread : " << QThread::currentThreadId();
}
ScanPictureFileThread::~ScanPictureFileThread()
{
    qDebug()<<"~ScanPictureFileThread";
}

void ScanPictureFileThread::ScanPictureFileThread::run()
{
    qDebug() << "ScanPictureFileThread Run Thread : " << QThread::currentThreadId() ;

    QDir* dir = new QDir("/mnt/usb/sdc4/picture/");
    QStringList filter;
    filter<<"*.jpg";
    dir->setNameFilters(filter);
    fileInfoList = new QList<QFileInfo>(dir->entryInfoList(filter));
    for(int i = 0;i < fileInfoList->count();i++){
        QString filePath = fileInfoList->at(i).absoluteFilePath();
        QString fileName = fileInfoList->at(i).fileName();
        qDebug()<<"file's path"<<filePath<<"filename:"<<fileName;
        QPixmap pix = QPixmap(filePath);
        if(pix.isNull()){
            qDebug()<<"this pix is NULL";
        }
        QListWidgetItem* pItem = new QListWidgetItem(QIcon(pix.scaled(QSize(195,110))),fileName);
        pItem->setSizeHint(QSize(195,130));
        widgetItemList.push_back(pItem);
    }
    emit scanComplete(fileInfoList,widgetItemList);
    return ;
}
/////////////////////////////////////////////////////////////////////////////////////
/// \brief initImageList
/// init image listwidget show images
void Gallery::initImageList()
{
    m_waittingWidget = new WaittingWidget(ui->waittingDialogWidget);

    connect(this,SIGNAL(notifyWatitingWidget()),m_waittingWidget,SLOT(onStopMovie()));
    ScanPictureFileThread* thread = new ScanPictureFileThread(this);
    connect(thread,SIGNAL(scanComplete(QList<QFileInfo>*,QList<QListWidgetItem*>)),\
            this,SLOT(onScanComplete(QList<QFileInfo>*,QList<QListWidgetItem*>)));

    ui->waittingDialogWidget->show();
    m_waittingWidget->show();
    m_waittingWidget->startMovie();

    thread->start();
}
void Gallery::onScanComplete(QList<QFileInfo>* listFileInfo,QList<QListWidgetItem*> widgetItemList)
{
    qDebug()<<"onScanComplete"<<listFileInfo->count();
    m_WidgetListItem = widgetItemList;
    m_PictureList = *listFileInfo;
    for(int i = 0;i < m_WidgetListItem.count();i++){
        QListWidgetItem* pItem = m_WidgetListItem.at(i);
        if(pItem == NULL){
            qDebug()<<"pItem is not NULl";
        }
        ui->imageList->insertItem(i,pItem);
    }

    ui->waittingDialogWidget->hide();
    emit notifyWatitingWidget();
}
/////////////////////////////////////////////////////////////////////////////////////
/// \brief Gallery::initOptions
///optionWidget show and hide
void Gallery::initOptions()
{
    ui->titleWidget->setFixedWidth(this->width());
    pWidget = new OptionWidget(ui->upWidget);
    pWidget->setFixedSize(this->width(), 80);
    pWidget->move(0, ui->titleWidget->height());

    connect(ui->optionButton,SIGNAL(clicked()),this,SLOT(showOptionWidget()));
    connect(pWidget,SIGNAL(notifyGalleryClearList()),this,SLOT(onNotifyGalleryClearList()));
    connect(pWidget,SIGNAL(notifyGalleryAddPic()),this,SLOT(onNotifyGalleryAddPic()));
    connect(pWidget,SIGNAL(notifyGalleryRefreshList()),this,SLOT(onNotifyGalleryRefreshList()));
}
void Gallery::showOptionWidget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->upWidget, "fixSizeHeight");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::InQuad);
     //高于最小高度，代表处于展开状态
    if (ui->upWidget->height() > 70){
        animation->setEndValue(70);
    }
     //否做就是收缩状态
    else{
        animation->setEndValue(pWidget->height() + 70);
    }
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
void Gallery::resizeEvent(QResizeEvent *)
{
    ui->titleWidget->setFixedWidth(this->width());
    ui->optionButton->move(964, 10);
}
void Gallery::onNotifyGalleryClearList()
{
    showOptionWidget();
    ui->imageList->clear();

}
void Gallery::onNotifyGalleryAddPic()
{
    qDebug()<<"onNotifyGalleryAddPic";
//    showOptionWidget();

}
void Gallery::onNotifyGalleryRefreshList()
{
    showOptionWidget();
    ui->imageList->clear();
    initImageList();
}
//////////////////////////////////////////////////////////////////////////////
/// \brief Gallery::eventFilter
///scrollarea with flow
bool Gallery::eventFilter(QObject *obj, QEvent *event)
{
    static int press_y   = 0;
    static int move_y    = -1;
    static int release_y = 0;
    static QDateTime pressDateTime;
    static QPropertyAnimation *animation = new QPropertyAnimation();

    if("qt_scrollarea_viewport" != obj->objectName()){
        return false;
    }

    int scrollV_max = m_scrollBarV->maximum ();
    int scrollV_min = m_scrollBarV->minimum ();

    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
    if(event->type() == QEvent::MouseButtonPress){
        //记录按下的时间、坐标
        pressDateTime = QDateTime::currentDateTime();
        move_y  = QCursor::pos().y();
        press_y = move_y;

        animation->stop();
    }
    else if(event->type() == QEvent::MouseButtonRelease){
        //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离
        if(animation->targetObject() != m_scrollBarV){
            animation->setTargetObject(m_scrollBarV);
            animation->setPropertyName("value");
        }
        move_y = -1;
        release_y = QCursor::pos().y();
        QObject *parent_obj = obj->parent();
        if(parent_obj != 0 || parent_obj->inherits("QAbstractItemView")){
            QTimer::singleShot(150, (QAbstractItemView *)parent_obj
                               , SLOT(clearSelection()));
        }
        int endValue;
        int pageStep;
        if(release_y - press_y != 0 && qAbs(release_y - press_y) > 45){
            //qDebug()<<"obj->objectName()="<<obj->objectName();
            int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
//            qDebug()<<"mseconds="<<mseconds;

            int limit = 440;
            pageStep = 220;//scrollBarV->pageStep();
//            qDebug()<<"pageStep="<<pageStep;
            if(mseconds > limit){
                //滑动的时间大于某个值的时候，不再滚动(通过增加分母)
                mseconds = mseconds + (mseconds - limit) * 20;
            }

            if(release_y - press_y > 0){
                endValue = m_scrollBarV->value()
                        - pageStep * (200.0 / mseconds);//.0避免避免强制转换为整形
                if(scrollV_min > endValue){
                    endValue = scrollV_min;
                }
            }
            else if(release_y - press_y < 0){
                    endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
                    if(endValue > scrollV_max){
                        endValue = scrollV_max;
                    }
            }
            if(mseconds > limit){
                mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
            }
            animation->setDuration(mseconds+550);
            animation->setEndValue(endValue);
            animation->setEasingCurve(QEasingCurve::OutQuad);
            animation->start();
            return true;
        }
    }
    else if(event->type() == QEvent::MouseMove && move_y >= 0)
    {
        //窗口跟着鼠标移动
        int move_distance = QCursor::pos().y() - move_y;
        int endValue = m_scrollBarV->value() - move_distance;
        if(scrollV_min > endValue){
            endValue = scrollV_min;
        }

        if(endValue > scrollV_max){
            endValue = scrollV_max;
        }
        m_scrollBarV->setValue(endValue);
        move_y = QCursor::pos().y();
    }
    return false;
}

void Gallery::on_homeButton_clicked()
{
    pGlobalMainDesk->setHidden(false);
    this->close();
}
