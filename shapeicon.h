#ifndef SHAPEICON_H
#define SHAPEICON_H

#include "enums.h"
#include <QIcon>


class ShapeIcon
{
public:
    ShapeIcon();

    QIcon getIcon(void);

    void setShape(shape_type_t shape);
    void setHollow(bool hollow);
    void setDeleted(bool deleted);
    void setChanged(bool changed);

    shape_type_t getShape(void);
    bool isHollow(void);
    bool isDeleted(void);
    bool isChanged(void);

private:
    shape_type_t _shape = UNSET;
    bool _hollow = false;
    bool _deleted = false;
    bool _changed = false;
    QPixmap *_iconPixmap;
};

#endif // SHAPEICON_H
