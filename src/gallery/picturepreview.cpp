/*!
 * @file picturepreview.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "picturepreview.h"
#include "ui_picturepreview.h"
#include "gallery.h"

#include <QDebug>
PicturePreview::PicturePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicturePreview)
{
    ui->setupUi(this);
}

PicturePreview::~PicturePreview()
{
    delete ui;
}
void PicturePreview::receiveFileList(QList<QFileInfo>PicPlayList,QString picPath,int count)
{
    m_picList = PicPlayList;
    m_curFilePath = picPath;
    m_curFileCount = count;

    QImage pic;
    pic.load(picPath);
    if(pic.isNull()){

    }else{
        m_picRegion = new PictureRegion(ui->widget);
        m_picRegion->setPicture(pic);
        qDebug()<<"show_image:"<<ui->widget->size();
        m_picRegion->resize(ui->widget->width(),ui->widget->height());

        ui->titleLabel->setText(QString("%1 / %2").arg(m_curFileCount + 1).arg(m_picList.count()));
    }
}

void PicturePreview::on_returnButton_clicked()
{
    emit notifyGalleryUnHide();
    delete m_picRegion;
    m_picRegion = NULL;
    this->close();
}

void PicturePreview::on_largerButton_clicked()
{
    if(m_picRegion != NULL){
        m_picRegion->zoomIn();
    }
}

void PicturePreview::on_smallerButton_clicked()
{
    if(m_picRegion != NULL){
        m_picRegion->zoomOut();
    }
}

void PicturePreview::on_nextPicButton_clicked()
{
    if(m_picRegion != NULL){
        delete m_picRegion;
        m_picRegion = NULL;
    }
    m_picRegion = new PictureRegion(ui->widget);
    if(m_curFileCount + 1 < m_picList.count())
    {
        QImage pic;
        pic.load(m_picList.at(++m_curFileCount).absoluteFilePath());

        if(pic.isNull()){

        }else{
            m_picRegion->setPicture(pic);
            qDebug()<<"show_image:"<<ui->widget->size();
            m_picRegion->resize(ui->widget->width(),ui->widget->height());

            m_picRegion->show();
            ui->titleLabel->setText(QString("%1 / %2")\
                                    .arg(m_curFileCount + 1).arg(m_picList.count()));
        }
    }else{
        m_curFileCount = -1;
        qDebug()<<"here2"<<m_curFileCount;
        on_nextPicButton_clicked();
    }
}

void PicturePreview::on_prePicButton_clicked()
{
    if(m_picRegion != NULL){
        delete m_picRegion;
        m_picRegion = NULL;
    }
    m_picRegion = new PictureRegion(ui->widget);
    if(m_curFileCount - 1 >= 0)
    {
        QImage pic;

        if(m_picList.count() < m_curFileCount)
            return;
        pic.load(m_picList.at(--m_curFileCount).absoluteFilePath());
        if(pic.isNull()){

        }else{
            m_picRegion->setPicture(pic);
            qDebug()<<"show_image:"<<ui->widget->size();
            m_picRegion->resize(ui->widget->width(),ui->widget->height());

            m_picRegion->show();
            ui->titleLabel->setText(QString("%1 / %2")\
                                    .arg(m_curFileCount + 1).arg(m_picList.count()));
        }
    }else{
        m_curFileCount = m_picList.count();
        qDebug()<<"here2"<<m_curFileCount;
        on_prePicButton_clicked();
    }
}
