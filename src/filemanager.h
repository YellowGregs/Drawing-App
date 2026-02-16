#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QImage>

class LayerManager;

class FileManager
{
public:
    static bool saveImage(const QString &filePath, const QImage &image);
    static QImage loadImage(const QString &filePath);
    
    static bool saveProject(const QString &filePath, LayerManager *layerManager);
    static bool loadProject(const QString &filePath, LayerManager *layerManager);
    
    static QStringList getSupportedImageFormats();
    static QString getImageFilter();

private:
    FileManager() = delete;
};

#endif // FILEMANAGER_H
