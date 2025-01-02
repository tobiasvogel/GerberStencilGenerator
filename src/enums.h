#ifndef ENUMS_H
#define ENUMS_H

#include <QGraphicsItem>

enum unit_type_t { UNKNOWN, METRIC, IMPERIAL };
enum shape_type_t { UNSET, CIRCLE, RECTANGLE, OBROUND, POLYGON, MACRO };
enum change_type_t { UNCHANGED, DRAFT, ACCEPTED, REVERTED, DISCARDED, DELETED };

enum sides_type_t { UNDETERMINED, WIDTH, HEIGHT, EQUAL };

enum { HOLLOWGRAPHICSELLIPSEITEM = QGraphicsItem::UserType + 1 };
enum { HOLLOWGRAPHICSOBROUNDITEM = QGraphicsItem::UserType + 2 };
enum { HOLLOWGRAPHICSPOLYGONITEM = QGraphicsItem::UserType + 3 };
enum { HOLLOWGRAPHICSRECTITEM = QGraphicsItem::UserType + 4 };
enum { HOLLOWROUNDEDGRAPHICSPOLYGONITEM = QGraphicsItem::UserType + 5 };
enum { HOLLOWROUNDEDGRAPHICSRECTITEM = QGraphicsItem::UserType + 6 };

typedef enum unit_type_t unit_type;
typedef enum shape_type_t shape_type;
typedef enum change_type_t change_type;

typedef enum sides_type_t sides_type;


#endif // ENUMS_H
