/*!
 * @file src/commonfunction.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "src/commonfunction.h"
#include "frmmessagebox.h"
#include <QDebug>

int autCb_func(int32_t msgType, void *user,void* data,int len)
{
#if 1
    SubtitleItem* pItem = (SubtitleItem*)data;
    switch(msgType)
    {
        //case AUDIO_PCM_OUTPUT:
          //  break;

        case SUBCTRL_SUBTITLE_AVAILABLE:
        {
            if(pItem == NULL){
                logw("error pItem == NULL");
                return -1;
            }
            if(pItem->bText != 0){
                if(pItem->pText != NULL){
                    #if 0
                    QString str = QString(pItem->pText);
                    pStaticVideoPlayer->subtitle_label->setVisible(true);
                    pStaticVideoPlayer->subtitle_label->setAlignment(Qt::AlignCenter);

                    printf("-----now is text subtitle--------\n");
                    printf("subtitle show,text=%s\n",pItem->pText);
                    QString str_subtitle = QObject::trUtf8(pItem->pText);

                    pStaticVideoPlayer->subtitle_label->setText(str_subtitle);
                    qDebug()<<"subtitle show done";
                    #endif
                }
            }else{
                printf("-----now is bitmap subtitle--------\n");
                #if 0
                pStaticVideoPlayer->subtitle_label->setVisible(true);
                pStaticVideoPlayer->subtitle_label->setAlignment(Qt::AlignCenter);
                if(pItem->pBitmapData != NULL){
                    int w,h;
                    w = pItem->nBitmapWidth;
                    h = pItem->nBitmapHeight;
                    QImage* img = new QImage((const uchar*)pItem->pBitmapData,w,h,QImage::Format_RGBA8888);
                    QImage imgScaled = img->scaled(pStaticVideoPlayer->subtitle_label->width(),pStaticVideoPlayer->subtitle_label->height(),Qt::KeepAspectRatio);
                    QPixmap pix = QPixmap();
                    pix.convertFromImage(imgScaled);
                    pStaticVideoPlayer->subtitle_label->setPixmap(pix);
                } else {
                    qDebug()<<"-----bitmap subtitle data is NULL-------";
                }
                #endif
            }
            break;
        }
        case SUBCTRL_SUBTITLE_EXPIRED:
        {
            printf("subtitle disable\n");
            if(pItem == NULL){
                printf("error: pItem == NULL");
                return -1;
            }
            if(pItem->bText != 0){
                printf("-----now is text subtitle--------\n");
            #if 0
                pStaticVideoPlayer->subtitle_label->setVisible(true);
                pStaticVideoPlayer->subtitle_label->setText(" ");
            #endif
            } else {
                printf("-----now is bitmap subtitle--------\n");
            #if 0
                pStaticVideoPlayer->subtitle_label->setVisible(false);
            #endif
            }
            break;
        }
        case AWPLAYER_MEDIA_PREPARED:
        {
            qDebug()<<"----libo app the video is prepared";
            break;
        }

        case AWPLAYER_MEDIA_PLAYBACK_COMPLETE:
        {
            qDebug()<<"---libo app the video is complete";
            break;
        }

        case AWPLAYER_MEDIA_SEEK_COMPLETE:
        {
            qDebug()<<"call back seek to is complete ";
            break;
        }
        case AWPLAYER_MEDIA_STARTED:
        {
            qDebug()<<"----libo app the video is started";
            break;
        }
//        case AW_MEDIA_INFO_RENDERING_START:
//        {
//            qDebug()<<" ---libo app AW_MEDIA_INFO_RENDERING_START the video is started";
//            break;
//        }
        case AWPLAYER_MEDIA_LOG_RECORDER:
            break;
        case AWPLAYER_MEDIA_BUFFERING_UPDATE:
        {
             qDebug()<<" ---libo  app AWPLAYER_MEDIA_BUFFERING_UPDATE the video is started";
             break;
        }
        case AWPLAYER_MEDIA_ERROR:
        case AWPLAYER_MEDIA_INFO:
        case AWPLAYER_MEDIA_SET_VIDEO_SIZE:
            break;
        default:
        {
            //logw("message 0x%x not handled.", messageId);
            break;
        }
    }
#endif
    return 0;
}
CommonFunction::CommonFunction()
{

}
bool CommonFunction::fileExistOrNot(QString path)
{
    if(path.isEmpty()){
        qDebug()<<"path is empty, please check";
        return false;
    }
    if((access((char*)path.toLatin1().data(),F_OK)) != -1){
        qDebug()<<"file exist";
        return true;
    }else{
        qDebug()<<"this file is not exist";
        return false;
    }
}
bool CommonFunction::video_support_or_not(QWidget* parent_widget,QString path)
{
    AUTPlayer *ap = NULL;
    if(ap == NULL){
        qDebug()<<"video_support_or_not"<<path;
        ap = new AUTPlayer();
        int ret = ap->setUrl((char*)path.toLatin1().data());
        if (ret < 0){
            qDebug()<<"setUrl is error-------------------";
            frmMessageBox *msg = new frmMessageBox(parent_widget);
            msg->SetMessage(QString("媒体不支持！"),0);
            msg->exec();
            if(ap != NULL){
                delete ap;
                 ap = NULL;
            }
            return false;
        }else{
            if(ap != NULL){
                delete ap;
                 ap = NULL;
            }
            qDebug()<<"this video is support";
            return true;
        }
    }
    return true;
}

QString CommonFunction::print_info(unsigned char tmp[],int tmp2)
{
    //tmp
    QString str = "";
    for(int i = 0;i < tmp2 && i < 64;i++)
    {
        if(tmp[i] == 0)
            continue;
        str += tmp[i];
    }
    return QObject::trUtf8(str.toLatin1().data());
}
