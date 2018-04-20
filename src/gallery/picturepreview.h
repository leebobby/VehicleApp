/*!
 * @file picturepreview.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef PICTUREPREVIEW_H
#define PICTUREPREVIEW_H

#include <QWidget>
#include <QFileInfo>

#include "pictureregion.h"

namespace Ui {
class PicturePreview;
}

class PicturePreview : public QWidget
{
    Q_OBJECT

public:
    explicit PicturePreview(QWidget *parent = 0);
    ~PicturePreview();
    void initImage();
public slots:
    void receiveFileList(QList<QFileInfo>,QString,int);

private slots:
    void on_returnButton_clicked();

    void on_largerButton_clicked();

    void on_smallerButton_clicked();

    void on_nextPicButton_clicked();

    void on_prePicButton_clicked();

signals:
    void notifyGalleryUnHide();

private:
    Ui::PicturePreview *ui;
    QImage m_image;
    PictureRegion* m_picRegion;
    QList<QFileInfo> m_picList;
    QString m_curFilePath;
    int m_curFileCount;
};

#endif // PICTUREPREVIEW_H
