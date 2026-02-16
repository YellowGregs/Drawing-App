#include "filemanager.h"
#include "layermanager.h"
#include "layer.h"
#include <QImageReader>
#include <QImageWriter>
#include <QFile>
#include <QDataStream>

bool FileManager::saveImage(const QString &filePath, const QImage &image)
{
    QImageWriter writer(filePath);
    return writer.write(image);
}

QImage FileManager::loadImage(const QString &filePath)
{
    QImageReader reader(filePath);
    return reader.read();
}

bool FileManager::saveProject(const QString &filePath, LayerManager *layerManager)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);
    
    out << QString("DRAWAPP");
    out << (quint32)1; // Version
    
    QSize docSize = layerManager->getDocumentSize();
    out << docSize;
    
    out << (quint32)layerManager->getLayerCount();
    
    for (int i = 0; i < layerManager->getLayerCount(); ++i) {
        Layer *layer = layerManager->getLayer(i);
        if (!layer) continue;
        
        out << layer->getName();
        out << layer->getOpacity();
        out << layer->isVisible();
        out << layer->isLocked();
        out << (quint32)layer->getBlendMode();
        out << *layer->getImage();
    }
    
    file.close();
    return true;
}

bool FileManager::loadProject(const QString &filePath, LayerManager *layerManager)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);
    
    QString header;
    quint32 version;
    in >> header >> version;
    
    if (header != "DRAWAPP" || version != 1) {
        return false;
    }
    
    QSize docSize;
    in >> docSize;
    
    // Read layer count
    quint32 layerCount;
    in >> layerCount;
    
    // TODO: Clear layers properly
    
    for (quint32 i = 0; i < layerCount; ++i) {
        QString name;
        qreal opacity;
        bool visible, locked;
        quint32 blendMode;
        QImage image;
        
        in >> name >> opacity >> visible >> locked >> blendMode >> image;
        
        Layer *layer = layerManager->addLayer(name);
        layer->setOpacity(opacity);
        layer->setVisible(visible);
        layer->setLocked(locked);
        layer->setBlendMode((Layer::BlendMode)blendMode);
        *layer->getImage() = image;
    }
    
    file.close();
    return true;
}

QStringList FileManager::getSupportedImageFormats()
{
    QStringList formats;
    formats << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif" << "*.tiff";
    return formats;
}

QString FileManager::getImageFilter()
{
    return "Images (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;All Files (*)";
}
