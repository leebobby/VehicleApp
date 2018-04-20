/*!
 * @file playlist.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "playlist.h"
#include "ui_playlist.h"
#include "../frmmessagebox.h"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QPropertyAnimation>
#include <QFileInfo>
#include <QDir>
#include <QByteArray>
#include <QTextCodec>
#include <QThread>
#include <QTimer>

////////////////////////////////////////////////////////////////////////////////////
/// \brief PlayList::self
/// ensure this class only has one instance
PlayList *PlayList::self = NULL;
PlayList *PlayList::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            self = new PlayList(parent);
        }
    }
    return self;
}
PlayList::PlayList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayList)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);//the ~CameraSetting will be call when func close() run

    ui->stackedWidget->setCurrentWidget(ui->mediaFilePositionSelect);

    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setColumnWidth(0,251);
    ui->tableWidget->setColumnWidth(1,30);
    ui->tableWidget->setColumnWidth(2,30);
}

PlayList::~PlayList()
{
    delete ui;
    self = NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////
//hidePlayList && showPlayList
//these two func is animation for playlist hide and show
/*!
 * @function
 * @brief des: this func mainly for animation of ui hide;
 */
void PlayList::hidePlayList()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QRect(704, 55, 320, 450));
    animation->setEndValue(QRect(1024, 55, 320, 450));
    animation->start();
    //hide operation
    connect(animation, SIGNAL(finished()), this, SLOT(hide()));
}
void PlayList::showPlayList()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->setStartValue(QRect(1024, 55, 320, 450));
    animation->setEndValue(QRect(704, 55, 320, 450));
    animation->start();
    //show operation
    connect(animation, SIGNAL(finished()), this, SLOT(show()));
}
//////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @function
 * @brief des
 * this function is used for add the music file to tablewidget after scan thread is complete
 */
void PlayList::onScanComplete(QList<QFileInfo>* fileInfoList)
{
    qDebug()<<"onScanComplete's thread ID is "<<QThread::currentThreadId();
    qDebug()<<"PlayList::onScanComplete";
    m_PlayList = *fileInfoList;
    if(fileInfoList == NULL){
        ui->tableWidget->setItem(0,0,new QTableWidgetItem("未找到音乐文件"));
    } else {
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(fileInfoList->count());
        qDebug()<<ui->tableWidget->rowCount();
        for(int i = 0; i < fileInfoList->count(); i++){

            QByteArray ba = fileInfoList->at(i).fileName().toLocal8Bit();
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            QString str = codec->toUnicode(ba);
            qDebug()<<"filename:"<<str;

            QPushButton* playButton = new QPushButton(this);
            QPushButton* delButton = new QPushButton(this);
            playButton->setStyleSheet("QPushButton{\
                                         background-color: rgba(255, 255, 255, 0);\
                                         border-image: url(:/images/playerIcons/playlistplayNormal.png);\
                                    }\
                                    QPushButton:pressed{\
                                        border-image: url(:/images/playerIcons/playlistplayPress.png);\
                                    }"
            );
            delButton->setStyleSheet("QPushButton{\
                                          background-color: rgba(255, 255, 255, 0);\
                                          border-image: url(:/images/playerIcons/playlistdelete.png);\
                                    }\
                                    QPushButton:pressed{\
                                        border-image: url(:/images/playerIcons/playlistdeletePress.png);\
                                    }"
                );
            playButton->setFocusPolicy(Qt::NoFocus);
            delButton->setFocusPolicy(Qt::NoFocus);

            connect(playButton,SIGNAL(clicked()),this,SLOT(onBtnClicked()));
            connect(delButton,SIGNAL(clicked()),this,SLOT(onBtnClicked()));

            ui->tableWidget->setRowHeight(i,35);
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(str));
            ui->tableWidget->setCellWidget(i,1,playButton);
            ui->tableWidget->setCellWidget(i,2,delButton);

            ui->statusLabel->setText(QString("状态：添加文件%1").arg(fileInfoList->at(i).fileName()));
        }//for
        QTimer::singleShot(1000, this, SLOT(updateStatusLabel()));
        emit stopMovie();
        emit notifyMusicPlayerListChanged(m_PlayList);
    }//if
}

void PlayList::onBtnClicked()
{
    QPushButton* pBtn = dynamic_cast<QPushButton*>(this->sender());
    if(pBtn == NULL){
        return;
    }
    int x = pBtn->frameGeometry().x();
    int y = pBtn->frameGeometry().y();

    QModelIndex index = ui->tableWidget->indexAt(QPoint(x,y));
    int row = index.row();
    int colum = index.column();

    qDebug()<<(QObject::tr("onBtnClicked ,row: %1, colum: %2").arg(row).arg(colum));

    if(colum == 1){
        qDebug()<<QObject::tr("this is play button");
        emit notifyMusicPlayerToPlay(m_PlayList,row);
    }
    if(colum == 2){
        qDebug()<<QObject::tr("this is remove button");

        ui->statusLabel->setText(QString("状态：删除文件%1").arg(m_PlayList.at(row).fileName()));
        m_PlayList.removeAt(row);
        ui->tableWidget->removeRow(row);
        QTimer::singleShot(1000, this, SLOT(updateStatusLabel()));

        emit notifyMusicPlayerListChanged(m_PlayList);
    }
}
void PlayList::updateStatusLabel()
{
    ui->statusLabel->setText(QString("状态：列表音乐数量--%1").arg(m_PlayList.count()));
}
void PlayList::scanMusicFileFromUdisk()
{
    ScanMusicFileThread* scanThread = new ScanMusicFileThread(this);
    connect(scanThread,SIGNAL(scanComplete(QList<QFileInfo>*)),this,SLOT(onScanComplete(QList<QFileInfo>*)));
    scanThread->start();
}
void PlayList::on_refreshButton_clicked()
{
    on_clearButton_clicked();
    ScanMusicFileThread* scanThread = new ScanMusicFileThread(this);
    connect(scanThread,SIGNAL(scanComplete(QList<QFileInfo>*)),this,SLOT(onScanComplete(QList<QFileInfo>*)));
    scanThread->start();
}

void PlayList::on_clearButton_clicked()
{
    ui->tableWidget->clear();
    m_PlayList.clear();
    QTimer::singleShot(1000, this, SLOT(updateStatusLabel()));
    emit notifyMusicPlayerListChanged(m_PlayList);
}
/*!
 * @function
 * @brief des
 * this function is designed for scan udisk music file
 */
void PlayList::on_UsbListButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->udiskMusicList);
    ui->statusLabel->setText("状态：扫描u盘文件");
    ui->titleLabel->setText("u盘音乐文件");
//    showWaittingDialog();// a bug
    scanMusicFileFromUdisk();
}
/*!
 * @function
 * @brief des
 * this function is designed for read a user's favorite music list from ini or txt etc.
 */
void PlayList::on_favoriteListButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->favoriteMusicList);
    ui->statusLabel->setText("状态：读取收藏文件");
    ui->titleLabel->setText("我喜欢的音乐");
}
void PlayList::showWaittingDialog()
{
    waittingWidgetThread* waittingThread = new waittingWidgetThread(this);
    waittingThread->start();
}
/////////////////////////////////////////////////////////////////////////////////////////////
/// \brief ScanMusicFileThread::ScanMusicFileThread
/// this thread is used for scan music file from udisk and other path
ScanMusicFileThread::ScanMusicFileThread(QObject *parent)
    : QThread(parent)
{
    qDebug() << "ScanMusicFileThread Thread : " << QThread::currentThreadId();
}
ScanMusicFileThread::~ScanMusicFileThread()
{
    qDebug()<<"~ScanMusicFileThread";
}

void ScanMusicFileThread::ScanMusicFileThread::run()
{
    qDebug() << "ScanMusicFileThread Run Thread : " << QThread::currentThreadId() ;

    QDir* dir = new QDir("/mnt/usb/sdb4/music/");
    QStringList filter;
    filter<<"*.mp3";
    dir->setNameFilters(filter);
    fileInfoList = new QList<QFileInfo>(dir->entryInfoList(filter));
    emit scanComplete(fileInfoList);
    return ;

}

////////////////////////////////////////////////////////////////////////////////////
/// \brief waittingWidgetThread::waittingWidgetThread
/// this is a loading ui for waitting scan music file and other
/// this ui is a model window

waittingWidgetThread::waittingWidgetThread(QObject *parent)
{
    qDebug() << "waittingWidgetThread Thread : " << QThread::currentThreadId();
}

waittingWidgetThread::~waittingWidgetThread()
{
    qDebug()<<"~waittingWidgetThread";
}

void waittingWidgetThread::waittingWidgetThread::run()
{
    qDebug()<<"showWaittingDialog's thread ID is "<<QThread::currentThreadId();
    m_pWaittingWidget = new WaittingWidget;
    connect(this, SIGNAL(stopMovie()),m_pWaittingWidget,SLOT(onStopMovie()));
    m_pWaittingWidget->user_setGeometry(QRect(704, 75, 320, 450));
#if 0
    QThread* waittingwidgetThread = new QThread;
    m_pWaittingWidget->moveToThread(waittingwidgetThread);
#endif
    m_pWaittingWidget->show();
    m_pWaittingWidget->startMovie();
}




