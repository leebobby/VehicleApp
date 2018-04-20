/*!
 * @file musicplayer.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "src/frmmessagebox.h"

#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QDateTime>
#include <QPropertyAnimation>



/*!
 * @pGlobalMainDesk
 * this global var is defined in maindesk, for widget return
 */
#include "src/maindesk.h"
extern MainDesk* pGlobalMainDesk;

MusicPlayer *MusicPlayer::self = NULL;
MusicPlayer *MusicPlayer::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            self = new MusicPlayer(parent);
        }
    }
    return self;
}

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayer),
    ap(NULL),
    m_totalDuration(0),
    m_curDuration(0),
    m_bPlayState(STOPPED),
    m_bMusicListOpen(0),
    m_pPlayList(NULL),
    m_curMusicFilePath(""),
    m_curMusicFileCount(-1)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);//the ~CameraSetting will be call when func close() run

    m_commonFunction = new CommonFunction ;


    m_pPlayList = PlayList::Instance(this);
    m_pPlayList->move(1024,50);
    connect(this, SIGNAL(hidePlayList()), m_pPlayList, SLOT(hidePlayList()));
    connect(this,SIGNAL(showPlayList()),m_pPlayList,SLOT(showPlayList()));


    connect(m_pPlayList,SIGNAL(notifyMusicPlayerToPlay(QList<QFileInfo>,int)),\
            this,SLOT(on_notifyMusicPlayerToPlay(QList<QFileInfo>,int)));
    connect(m_pPlayList,SIGNAL(notifyMusicPlayerListChanged(QList<QFileInfo>)),\
            this,SLOT(on_notifyMusicPlayerListChanged(QList<QFileInfo>)));

    progressSliderTimer = new QTimer(this);
    connect(progressSliderTimer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(onProgressSliderMoved(int)));
    connect(ui->progressSlider,SIGNAL(sliderReleased()),this,SLOT(onProgressSliderReleased()));
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
    delete m_commonFunction;
    m_commonFunction = NULL;
    self = NULL;
    if(ap != NULL){
        delete ap;
        ap = NULL;
    }
}

void MusicPlayer::initAutPlayer()
{
    if(m_curMusicFilePath.isEmpty())
    {
        qDebug()<<"file path is emtpy";
    }
    if(ap != NULL){
        delete ap;
        ap = NULL;
    }
    ap = new AUTPlayer();
    if(ap == NULL){
        qDebug()<<"new AutPlayer error";
        return ;
    }
    m_bPlayState = STOPPED;

    int ret = ap->setUrl((char*)m_curMusicFilePath.toLatin1().data());
    if(ret != 0){
        qDebug()<<"this video set url errror";
        return;
    }
    ap->setUserCb(autCb_func, this);

    m_totalDuration = ap->getDuration() / 1000;
    ui->progressSlider->setRange(0,m_totalDuration * 1000);

    qDebug()<<"----------------------the music totaltime is "<<m_totalDuration;
    QTime totalTime((m_totalDuration / 3600) % 60, (m_totalDuration / 60) % 60,\
                    m_totalDuration % 60, (m_totalDuration * 1000) % 1000);
    QTime curTime((m_curDuration / 3600) % 60, (m_curDuration / 60) % 60,\
                    m_curDuration % 60, (m_curDuration * 1000) % 1000);
    QString format = "mm:ss";
    if (m_totalDuration > 3600)
        format = "hh:mm:ss";
    ui->totalTimeLabel->setText(totalTime.toString(format));
    ui->curTimeLabel->setText(curTime.toString(format));
    qDebug()<<"----------------------the music format totaltime is"<<\
           totalTime.toString(format);

    ret = ap->play();
    if(ret != 0)
        qDebug()<<"play bug";

    ui->playButton->setIcon(QIcon(":/images/playerIcons/Pause.png"));
    ui->playButton->setIconSize(QSize(50,50));
    m_bPlayState = PLAYING;

    progressSliderTimer->start(1000);
}
void MusicPlayer::timerUpdate()
{
    if (ap == NULL)
        return ;
    m_curDuration = ap->getCurrentPosition();
    ui->progressSlider->setValue(m_curDuration);
    updateDurationInfo(m_curDuration / 1000);
}
void MusicPlayer::updateDurationInfo(qint64 currentInfo)
{
    qDebug()<<"update the time label";
    QString tStr;

    if (ap == NULL)
        return ;
    if(m_totalDuration == currentInfo)
    {
        currentInfo = 0;
        ui->progressSlider->setValue(0);
        m_bPlayState = STOPPED;
        ui->playButton->setIcon(QIcon(":/images/playerIcons/Play.png"));
        ui->playButton->setIconSize(QSize(50,50));
        return ;
    }

    if (currentInfo) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60, \
                          currentInfo % 60, (currentInfo * 1000) % 1000);

        QString format = "mm:ss";
        if (m_totalDuration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format);
    }
    ui->curTimeLabel->setText(tStr);
}
void MusicPlayer::onProgressSliderMoved(int value)
{
    qDebug()<<"slider is moved"<<value;
    progressSliderTimer->stop();
    if (!ui->progressSlider->isSliderDown()) {
        ui->progressSlider->setValue(value);
    }
    ui->progressSlider->setValue(value);
    updateDurationInfo(value / 1000);
    progressSliderTimer->start(1000);
}

void MusicPlayer::onProgressSliderReleased()
{
    qDebug()<<"slider is released";
    if(m_bPlayState == PAUSEED)
        on_playButton_clicked();
    progressSliderTimer->stop();
    m_bPlayState = SEEKING;
    if(ap != NULL){
        ap->seekto(ui->progressSlider->value() / 1000);
    }
    progressSliderTimer->start(1000);

    m_bPlayState = PLAYING;

}

void MusicPlayer::startPlay()
{
    bool isExist = m_commonFunction->fileExistOrNot(m_curMusicFilePath);
    if(!isExist){
        qDebug()<<"file exist";
        frmMessageBox *msg = new frmMessageBox(this);
        msg->SetMessage(QString("文件不存在！"),0);
        msg->exec();
        return;
    }
    if(!m_commonFunction->video_support_or_not(this,m_curMusicFilePath)){
        qDebug()<<"format not support";
        return ;
    }
    initAutPlayer();
    getMusicInfo(m_curMusicFilePath);
}


void MusicPlayer::on_notifyMusicPlayerToPlay(QList<QFileInfo> playList,int row)
{
    //hide the playlist
    emit hidePlayList();
    m_bMusicListOpen = 0;

    m_playList = playList;
    if(m_playList.count() < row){
        qDebug()<<"playlist is wrong";
        return;
    }
    m_curMusicFilePath = m_playList.at(row).absoluteFilePath();
    m_curMusicFileCount = row;

    startPlay();
}

void MusicPlayer::on_notifyMusicPlayerListChanged(QList<QFileInfo> playList)
{
    qDebug()<<"playlist has been update";
    m_playList = playList;
}
void MusicPlayer::on_playButton_clicked()
{
    qDebug()<<"MusicPlayer::on_playButton_clicked()";
    int ret=0;
    if(ap != NULL){
        ret = ap->pause();
        if(ret != 0)
            qDebug()<<"pause bug";
        if (m_bPlayState == PAUSEED){
          m_bPlayState = PLAYING;
          ui->playButton->setIcon(QIcon(":/images/playerIcons/Pause.png"));
        }else if(m_bPlayState == PLAYING){
          m_bPlayState = PAUSEED;
          ui->playButton->setIcon(QIcon(":/images/playerIcons/Play.png"));
        }
    }
}
void MusicPlayer::on_preMusicButton_clicked()
{
    qDebug()<<"-----------------on_nextMovieButton_clicked,here";
    if(m_curMusicFileCount -1 >= 0){
        m_curMusicFileCount--;
        m_curMusicFilePath = m_playList.at(m_curMusicFileCount).absoluteFilePath();
        qDebug()<<"----------------now is:"<<m_curMusicFileCount<<
                  "\n-----------------------filename is"<<m_curMusicFilePath;
        progressSliderTimer->stop();
        startPlay();
    }else{
        return ;
    }
}

void MusicPlayer::on_nextMusicButton_clicked()
{
    qDebug()<<"-----------------on_nextMovieButton_clicked,here";
    if(m_curMusicFileCount + 1 < m_playList.count()){
        m_curMusicFileCount++;
        m_curMusicFilePath = m_playList.at(m_curMusicFileCount).absoluteFilePath();
        qDebug()<<"----------------now is:"<<m_curMusicFileCount<<"\n-----------------------filename is"<<m_curMusicFilePath;
        progressSliderTimer->stop();
        startPlay();
    }else{
        return ;
    }
}

void MusicPlayer::on_searchButton_clicked()
{

}

void MusicPlayer::on_playListButton_clicked()
{
    qDebug()<<"on_musicButton_clicked";
#if 1
    if(m_pPlayList == NULL){
        qDebug()<<"new";
        m_pPlayList = PlayList::Instance(this);
        m_pPlayList->move(1024,50);
    }
    qDebug()<<"already exisit";
    if(!m_bMusicListOpen){
        qDebug()<<"show!!!!";
        emit showPlayList();
        m_bMusicListOpen = 1;
    } else {
        qDebug()<<"hide!!!";
        emit hidePlayList();
        m_bMusicListOpen = 0;
    }
#endif
}
//some operation when MusicPlayer get focus
//通过焦点实现的方法还需要进一步测试，存在bug
void MusicPlayer::focusInEvent(QFocusEvent*)
{
    qDebug()<<"Musicplayer get focus";
    if(m_bMusicListOpen == 1){
        emit hidePlayList();
        m_bMusicListOpen = 0;
    }
}
void MusicPlayer::on_playOrderButton_clicked()
{

}

void MusicPlayer::on_setttingButton_clicked()
{

}

void MusicPlayer::on_pushButton_clicked()
{
    pGlobalMainDesk->setHidden(false);
    this->close();
}
void MusicPlayer::getMusicInfo(QString path)
{
    MusicInfoThread* thread = new MusicInfoThread(path, this);
    connect(thread,SIGNAL(complete(QString,QString,QString,QPixmap)),\
            this,SLOT(setMusicInfo(QString ,QString ,QString ,QPixmap )));
    thread->start();
}
void MusicPlayer::setMusicInfo(QString albumName,QString musicName,QString singerName,QPixmap poster)
{
    m_albumName = albumName;
    m_musicName = musicName;
    m_singerName = singerName;
    m_poster = poster;
    if(!m_albumName.isEmpty()){
        ui->albumTextLabel->setText(m_albumName);
    }else{
        ui->albumTextLabel->setText("未知专辑");
    }
    if(!m_musicName.isEmpty()){
        ui->MusicTextLabel->setText(m_musicName);
    }else{
        ui->MusicTextLabel->setText("未知音乐名");
    }
    if(!m_singerName.isEmpty()){
        ui->artistTextLabel->setText(m_singerName);
    }else{
        ui->artistTextLabel->setText("未知艺术家");
    }
    if(!m_poster.isNull()){
        ui->posterLabel->setPixmap(m_poster);
    }else{
         ui->posterLabel->setText("没有图片");
    }
}
///////////////////////////////////////////////////////////////////////////////


MusicInfoThread::MusicInfoThread(QString path,QObject *parent)
    : m_filepath(path),
      QThread(parent)
{
    qDebug() << "MusicInfoThread Thread : " << QThread::currentThreadId();
    commonfunc = new CommonFunction;
    m_albumName = QString("");
    m_musicName = QString("");
    m_singerName = QString("");
    m_poster = QPixmap();
}
MusicInfoThread::~MusicInfoThread()
{
    qDebug()<<"~MusicInfoThread";
}

void MusicInfoThread::run()
{
    qDebug() << "MusicInfoThread run in  Thread : " << QThread::currentThreadId();
    qDebug()<<"the file's absoluteFilePath is"<<m_filepath;

    XRetriever *demoRetriver = XRetrieverCreate();
    if(NULL == demoRetriver){
        printf("XRetrieverCreate failed\n");
    }
    int ret = XRetrieverSetDataSource(demoRetriver, (char*)m_filepath.toLatin1().data(), NULL);
    if(ret < 0){
         printf("set datasource failed\n");
    }
    printf("XRetrieverSetDataSource end\n");
    CdxMediaInfoT mi;
    XRetrieverGetMetaData(demoRetriver, METADATA_MEDIAINFO, &mi);
    printf("the video information:titile[64]=[%.02x %.02x %.02x %.02x %.02x]\n",mi.title[0],mi.title[1],mi.title[2],mi.title[3],mi.title[4]);
    printf("the video information:album[64]=[%s] ,albumsz=%d,genre[64]=%s,genresz=%d,title[64]=%s,titlesz=%d,year[64]=%s,yearsz=%d,composer[64]=%s,date[64]=%s\n",
           mi.album,mi.albumsz,mi.genre,mi.genresz,mi.title,mi.titlesz,mi.year,mi.yearsz,mi.composer,mi.date);

    if(mi.albumsz!=0){
        m_albumName = commonfunc->print_info(mi.album,mi.albumsz);
        qDebug()<<"album is"<<m_albumName;
    }
    if(mi.titlesz!=0){
        m_musicName = commonfunc->print_info(mi.title,mi.titlesz);
        qDebug()<<"title is"<<m_musicName;
    }
    if(mi.authorsz!=0){
        m_singerName = commonfunc->print_info(mi.author,mi.authorsz);
        qDebug()<<"author is"<< m_singerName;
    }
    if(mi.nAlbumArtBufSize != 0){
        qDebug()<<"this music has poster";
        QImage img = QImage::fromData((const uchar*)mi.pAlbumArtBuf,mi.nAlbumArtBufSize);
        QImage imgScaled = img.scaled(320,260,Qt::KeepAspectRatio);
        m_poster.convertFromImage(imgScaled);
    }

    emit complete(m_albumName,m_musicName,m_singerName,m_poster);

    XRetrieverDestory(demoRetriver);
    demoRetriver = NULL;
    return ;
}
