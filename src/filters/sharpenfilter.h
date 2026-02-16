#ifndef SHARPENFILTER_H
#define SHARPENFILTER_H

#include "filterbase.h"

class SharpenFilter : public FilterBase
{
public:
    SharpenFilter(qreal amount = 1.0);
    
    QString getName() const override { return "Sharpen"; }
    QImage apply(const QImage &source) override;
    
    void setAmount(qreal amount) { m_amount = amount; }
    qreal getAmount() const { return m_amount; }

private:
    qreal m_amount;
};

#endif // SHARPENFILTER_H
