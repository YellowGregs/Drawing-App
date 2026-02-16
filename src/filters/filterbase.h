#ifndef FILTERBASE_H
#define FILTERBASE_H

#include <QImage>
#include <QString>

class FilterBase
{
public:
    virtual ~FilterBase() {}
    
    virtual QString getName() const = 0;
    virtual QImage apply(const QImage &source) = 0;
    
protected:
    QImage clampImage(const QImage &image);
};

#endif // FILTERBASE_H
