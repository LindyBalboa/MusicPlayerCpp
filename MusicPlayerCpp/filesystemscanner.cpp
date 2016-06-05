#include "filesystemscanner.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QStandardPaths>
#include <QTreeView>

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <taglib/commentsframe.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/flacfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/mpegfile.h>
#include <taglib/taglib.h>
#include <taglib/tpropertymap.h>
#include <taglib/xiphcomment.h>
#include <taglib/xiphcomment.h>

FileSystemScanner::FileSystemScanner(QSqlDatabase &db, QWidget *parent) : QWidget(parent)
{
    libraryDb = db;
    setWindowTitle("Scan filesystem for new songs");
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    QTreeView *fileSystemView = new QTreeView(this);
        model = new CheckableFileSystemModel(this);
        fileSystemView->setModel(model);
        fileSystemView->setExpanded(model->index(model->rootPath()), true); //Expand to default music location
        fileSystemView->scrollTo(model->index(model->rootPath()));
        for (int i = 1; i < model->columnCount(); i++)
            fileSystemView->hideColumn(i);
        mainLayout->addWidget(fileSystemView);
        QPushButton *okayButton = new QPushButton("Okay");
            mainLayout->addWidget(okayButton);
            connect(okayButton, &QPushButton::released, this, &FileSystemScanner::scanFolders);
    show();
}

void FileSystemScanner::scanFolders()
{
    QStringList dirs;
    foreach(QModelIndex index, model->checkedList){
        dirs << model->filePath(index);
    }
    QThread *workerThread = new QThread();
    Worker *worker = new Worker(dirs, libraryDb);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, worker, &Worker::doWork);
    connect(worker, &Worker::finished, workerThread, &QThread::quit);
    connect(worker, &Worker::currentFileCountSignal, this, &FileSystemScanner::currentFileCountSignal);
    connect(worker, &Worker::totalFileCountSignal, this, &FileSystemScanner::totalFileCountSignal);
    connect(workerThread, &QThread::finished, worker, &Worker::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    connect(workerThread, &QThread::finished, this, &FileSystemScanner::finished);
    workerThread->start();
    this->hide();
}

CheckableFileSystemModel::CheckableFileSystemModel(QObject *parent) : QFileSystemModel(parent)
{
    QString defaultPath = QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0];
    setRootPath(defaultPath);
}
CheckableFileSystemModel::~CheckableFileSystemModel(){}
QVariant CheckableFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole){
       return checkedList.contains(index);
    }
    return QFileSystemModel::data(index, role);
}
Qt::ItemFlags CheckableFileSystemModel::flags(const QModelIndex &index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}
bool CheckableFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole){
        if (checkedList.contains(index)){
            checkedList.removeAll(index);
        }else{
            checkedList.append(index);
        }
    }
    emit this->dataChanged(index,index);
    return true;
}

Worker::Worker(QStringList _dirs, QSqlDatabase &db)
{
    libraryDb = db;
    dirs = _dirs;
}
Worker::~Worker(){}
void Worker::doWork()
{
    QMap<QString, TagLib::String> mp3;
    mp3["Album"] = "ALBUM"; // "TALB";
    mp3["Album_Artist"] = "ALBUMARTIST"; // "TPE2";
    mp3["Artist"] = "ARTIST"; // "TPE1";
    mp3["BPM"] = "BPM"; // "TBPM";
    mp3["Comment"] = "COMMENT"; // "COMM::XXX";
    mp3["Composer"] = "COMPOSER"; // "TCOM";
    mp3["Custom_1"] = "COMMENT:SONGS-DB_CUSTOM1"; // "COMM:Songs-DB_Custom1:XXX";
    mp3["Custom_2"] = "COMMENT:SONGS-DB_CUSTOM2"; // "COMM:Songs-DB_Custom2:XXX";
    mp3["Custom_3"] = "COMMENT:SONGS-DB_CUSTOM3"; // "COMM:Songs-DB_Custom3:XXX";
    mp3["Custom_4"] = "COMMENT:SONGS-DB_CUSTOM4"; // "COMM:Songs-DB_Custom4:XXX";
    mp3["Custom_5"] = "COMMENT:SONGS-DB_CUSTOM5"; // "COMM:Songs-DB_Custom5:XXX";
    mp3["Date"] = "DATE"; // "X";
    mp3["Disc_Number"] = "DISCNUMBER"; // "TPOS";
    mp3["Genre"] = "GENRE"; // "TCON";
    mp3["Grouping"] = "CONTENTGROUP"; // "TIT1";
    mp3["Mood"] = "COMMENT:MUSICMATCH_MOOD"; // "COMM:MusicMatch_Mood:XXX";
    mp3["Occasion"] = "COMMENT:MUSICMATCH_SITUATION"; // "COMM:MusicMatch_Situation:XXX";
    mp3["Original_Date"] = "ORIGINALDATE"; // "TDOR";
    mp3["Quality"] = "COMMENT:MUSICMATCH_PREFERENCE";// "COMM:Songs-DB_Preference:XXX";
    //mp3["Rating"] = ""; // "POPM"; Not a supported TagLib property, handled separately
    mp3["Tempo"] = "COMMENT:MUSICMATCH_TEMPO"; // "COMM:MusicMatch_Tempo:XXX";
    mp3["Title"] = "TITLE"; // "TIT2";
    mp3["Track_Number"] = "TRACKNUMBER"; // "TRCK";
    mp3["Track_total"] = "";// "_";

    QMap<QString,TagLib::String> mp4;
    mp4["Album"] = TagLib::String("\xa9""alb");
    mp4["Album_Artist"] = "aART";
    mp4["Artist"] = TagLib::String("\xa9""ART");
    mp4["BPM"] = "tmpo";
    mp4["Comment"] = TagLib::String("\xa9""cmt");
    mp4["Composer"] = TagLib::String("\xa9""wrt");
    mp4["Custom_1"] = "----:com.apple.iTunes:CUSTOM1";
    mp4["Custom_2"] = "----:com.apple.iTunes:CUSTOM2";
    mp4["Custom_3"] = "----:com.apple.iTunes:CUSTOM3";
    mp4["Custom_4"] = "----:com.apple.iTunes:CUSTOM4";
    mp4["Custom_5"] = "----:com.apple.iTunes:CUSTOM5";
    mp4["Date"] = TagLib::String("\xa9""day");
    mp4["Disc_Number"] = "disk";
    mp4["Genre"] = TagLib::String("\xa9""gen");
    mp4["Grouping"] = TagLib::String("\xa9""grp");
    mp4["Mood"] = "----:com.apple.iTunes:MOOD";
    mp4["Occasion"] = "----:com.apple.iTunes:OCCASION";
    mp4["Original_Date"] = "----:com.apple.iTunes:ORIGINAL DATE";
    mp4["Quality"] = "----:com.apple.iTunes:QUALITY";
    mp4["Rating"] = "rate";
    mp4["Tempo"] = "----:com.apple.iTunes:TEMPO";
    mp4["Title"] = TagLib::String("\xa9""nam");
    mp4["Track_Number"] = "trkn";
    mp4["Track_Total"] = "_";

    QMap<QString, TagLib::String> flac;
    flac["Album"] = "ALBUM";
    flac["Album_Artist"] = "ALBUMARTIST";
    flac["Artist"] = "ARTIST";
    flac["BPM"] = "BPM";
    flac["Comment"] = "COMMENT";
    flac["Composer"] = "COMPOSER";
    flac["Custom_1"] = "CUSTOM1";
    flac["Custom_2"] = "CUSTOM2";
    flac["Custom_3"] = "CUSTOM3";
    flac["Custom_4"] = "CUSTOM4";
    flac["Custom_5"] = "CUSTOM5";
    flac["Date"] = "DATE";
    flac["Disc_Number"] = "DISCNUMBER";
    flac["Genre"] = "GENRE";
    flac["Grouping"] = "GROUPING";
    flac["Mood"] = "MOOD";
    flac["Occasion"] = "OCCASION";
    flac["Original_Date"] = "ORIGINAL DATE";
    flac["Quality"] = "QUALITY";
    flac["Rating"] = "RATING";
    flac["Tempo"] = "TEMPO";
    flac["Title"] = "TITLE";
    flac["Track_Number"] = "TRACKNUMBER";
    flac["Track_Total"] = "TRACKTOTAL";

    QSqlQuery query(libraryDb);
    double totalFileCount = 0;
    foreach(QString dir, dirs){
        QDirIterator countIter(dir, QDirIterator::Subdirectories);
        while (countIter.hasNext() ){
           countIter.next();
           if (!countIter.fileInfo().isFile()){
               continue;
           }
           totalFileCount+=1.0;
        }
        emit(totalFileCountSignal(totalFileCount));
    }

    QStringList existingPaths;
    query.exec("SELECT Path FROM Songs");
    while(query.next()){
        existingPaths.append(query.value(0).toString());
    }
    int currentFileCount = 0;
    QStringList skipList;
    skipList << "log" << "jpg" << "peg" << "cue" << "txt" << "m3u" << "md5" << "png" << "gif" << "pdf" << "rip" << "st5" << "tc2" << "nfo" << "sfv" << "ffp" << "ini";

    foreach(QString dir, dirs){
        QDirIterator iter(dir, QDirIterator::Subdirectories);
        while (iter.hasNext()){
           iter.next();
           if (!iter.fileInfo().isFile()){
               continue;
           }
           QString filepath = iter.filePath();
          //qDebug() << filepath;
           if (skipList.contains(filepath.right(3))) {
               currentFileCount++;
               continue;
           }
           if (existingPaths.contains(filepath)){
               currentFileCount++;
               continue;  //Skip to next track
           }
           //std::cout << QFile::encodeName(filepath).constData() << endl;
           if (!filepath.right(3).compare("mp3", Qt::CaseInsensitive) ){
               TagLib::MPEG::File f(QFile::encodeName(filepath).constData());
               if (f.isValid()){
                   TagLib::ID3v2::Tag *id3v2tag = f.ID3v2Tag();

                   QString columnsStatement = "insert into Songs (";
                   QString valuesStatement = " values (";

                   TagLib::PropertyMap pm = f.ID3v2Tag()->properties();
                   QMapIterator <QString, TagLib::String> mp3i(mp3);
                   while (mp3i.hasNext() ){
                       mp3i.next();
                       //qDebug() << QString::fromStdString(mp3i.key().toStdString()) << ": " << QVariant(pm[mp3i.value().toCString()].toString().toCString());
                       columnsStatement.append(mp3i.key() + ",");
                       if(pm[mp3i.value().toCString()].toString().size()==0){
                           valuesStatement.append("'',");
                       }else{
                               valuesStatement.append("'" + QString(pm[mp3i.value().toCString()].toString().toCString()).replace("'","''") + "'" + ",");
                           }
                   }
                   TagLib::ID3v2::FrameListMap flm = f.ID3v2Tag()->frameListMap();
                   //QString("POPM"), QVariant(QString(flm["POPM"].front()->toString().toCString())) );
                   //QString("TIPL"), QVariant(QString(flm["TIPL"].front()->toString().toCString())) );
                   columnsStatement.append("Length,");
                   valuesStatement.append("'" + QString::number(f.audioProperties()->lengthInSeconds()).replace("'","''") + "'" + ",");
                   columnsStatement.append("Path,");
                   valuesStatement.append("'" + filepath.replace("'","''") + "'" + ",");
                   columnsStatement.append("Filename,");
                   valuesStatement.append("'" + iter.fileName().replace("'","''") + "'" + ",");
                   columnsStatement.append("Bitrate,");
                   valuesStatement.append("'" + QString::number(f.audioProperties()->bitrate()).replace("'","''") + "'" + ",");
                   columnsStatement.append("Channels,");
                   valuesStatement.append("'" + QString::number(f.audioProperties()->channels()).replace("'","''") + "'" + ",");
                   columnsStatement.append("Sample_Rate,");
                   valuesStatement.append("'" + QString::number(f.audioProperties()->sampleRate()).replace("'","''") + "'" + ",");

                   QString insertStatement = columnsStatement.replace(columnsStatement.size()-1,1,")") + valuesStatement.replace(valuesStatement.size()-1,1,")");
                   if (!query.exec(insertStatement)){
                      //qDebug() << insertStatement;
                       std::cout << QFile::encodeName(filepath).constData() << endl;
                      //qDebug() << query.lastError();
                   }
               }else{
                   //std::cout << QFile::encodeName(filepath).constData() << endl;
               }
           }
           else if (!filepath.right(3).compare("m4a", Qt::CaseInsensitive) | !filepath.right(3).compare("mp4", Qt::CaseInsensitive) | !filepath.right(3).compare("aac", Qt::CaseInsensitive)){
                    TagLib::MP4::File f(QFile::encodeName(filepath).constData());
                   //qDebug() << "LOADED";
                    if(f.isValid()){
                       //qDebug() << "VALID";
                        TagLib::MP4::Tag *mp4tag = f.tag();
                        TagLib::MP4::ItemMap mp4ItemMap= mp4tag->itemMap();
                        QString columnsStatement = "insert into Songs (";
                        QString valuesStatement = " values (";
                        QMapIterator <QString, TagLib::String> mp4i(mp4);
                        while (mp4i.hasNext()){
                           //qDebug() << "WHILE";
                            mp4i.next();
                            TagLib::MP4::Item item = mp4ItemMap[mp4i.value()];
                            columnsStatement.append(mp4i.key() + ",");
                            if (strlen(item.toStringList().toString().toCString()) != 0){
                                valuesStatement.append("'" + QString(item.toStringList().toString().toCString()).replace("'","''") + "'" + ",");
                               //qDebug() << "INSIDE IF";
                            }else if (item.isValid()){
                                valuesStatement.append("'" + QString::number(item.toInt()).replace("'","''") + "'" + ",");
                               //qDebug() << mp4i.key().toStdString().c_str() << ": " << item.toInt() << endl;
                            }else{
                                valuesStatement.append("'',");
                            }
                        }
                        columnsStatement.append("Length,");
                        valuesStatement.append("'" + QString::number(f.audioProperties()->lengthInSeconds()).replace("'","''") + "'" + ",");
                        columnsStatement.append("Path,");
                        valuesStatement.append("'" + filepath.replace("'","''") + "'" + ",");
                        columnsStatement.append("Filename,");
                        valuesStatement.append("'" + iter.fileName().replace("'","''") + "'" + ",");
                        columnsStatement.append("Bitrate,");
                        valuesStatement.append("'" + QString::number(f.audioProperties()->bitrate()).replace("'","''") + "'" + ",");
                        columnsStatement.append("Channels,");
                        valuesStatement.append("'" + QString::number(f.audioProperties()->channels()).replace("'","''") + "'" + ",");
                        columnsStatement.append("Sample_Rate,");
                        valuesStatement.append("'" + QString::number(f.audioProperties()->sampleRate()).replace("'","''") + "'" + ",");

                        QString insertStatement = columnsStatement.replace(columnsStatement.size()-1,1,")") + valuesStatement.replace(valuesStatement.size()-1,1,")");
                       //qDebug() << insertStatement;
                        if (!query.exec(insertStatement)){
                            //std::cout << QFile::encodeName(filepath).constData() << endl;
                            //qDebug() << query.lastError();
                        }
                       //qDebug() << query.lastQuery();
                    }else{
                        //std::cout << QFile::encodeName(filepath).constData() << endl;
                    }
                }else if(!filepath.right(4).compare("flac", Qt::CaseInsensitive)){
                         TagLib::FLAC::File f(QFile::encodeName(filepath).constData());
                         if (f.isValid()){
                             QString columnsStatement = "insert into Songs (";
                             QString valuesStatement = " values (";
                             TagLib::Ogg::XiphComment *flactag = f.xiphComment();
                             TagLib::Ogg::FieldListMap flacmap = flactag->fieldListMap();
                             QMapIterator <QString, TagLib::String> flaci(flac);
                             while(flaci.hasNext()){
                                 flaci.next();
                                 //cout << flaci.key().toStdString() << ": " << flacmap[flaci.value()].toString() << endl;
                                 columnsStatement.append(flaci.key() + ",");
                                 if (strlen(flacmap[flaci.value()].toString().toCString()) != 0){
                                     valuesStatement.append("'" + QString(flacmap[flaci.value()].toString().toCString()).replace("'","''") + "'" + ",");
                                     //cout << mp4i.key().toStdString().c_str() << ": " << item.toStringList().toString().toCString() << endl;
                                 }else{
                                     valuesStatement.append("'',");
                                 }
                             }
                             columnsStatement.append("Length,");
                             valuesStatement.append("'" + QString::number(f.audioProperties()->lengthInSeconds()).replace("'","''") + "'" + ",");
                             columnsStatement.append("Path,");
                             valuesStatement.append("'" + filepath.replace("'","''") + "'" + ",");
                             columnsStatement.append("Filename,");
                             valuesStatement.append("'" + iter.fileName().replace("'","''") + "'" + ",");
                             columnsStatement.append("Bitrate,");
                             valuesStatement.append("'" + QString::number(f.audioProperties()->bitrate()).replace("'","''") + "'" + ",");
                             columnsStatement.append("Channels,");
                             valuesStatement.append("'" + QString::number(f.audioProperties()->channels()).replace("'","''") + "'" + ",");
                             columnsStatement.append("Sample_Rate,");
                             valuesStatement.append("'" + QString::number(f.audioProperties()->sampleRate()).replace("'","''") + "'" + ",");

                             QString insertStatement = columnsStatement.replace(columnsStatement.size()-1,1,")") + valuesStatement.replace(valuesStatement.size()-1,1,")");
                            //qDebug() << insertStatement;
                             if (!query.exec(insertStatement)){
                                 std::cout << QFile::encodeName(filepath).constData() << endl;
                                //qDebug() << query.lastError();
                             }
                         }else{
                             //std::cout << QFile::encodeName(filepath).constData() << endl;
                         }
                     }else{
                         //std::cout << QFile::encodeName(filepath).constData() << endl;
                     }
        currentFileCount++;
        emit(currentFileCountSignal(currentFileCount));
        }
        libraryDb.commit();
    }
    emit finished();
}

void FileSystemScanner::closeEvent(QCloseEvent *event)
{
    emit finished();
}
