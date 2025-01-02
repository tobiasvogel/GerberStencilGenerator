#include "shapeicon.h"

ShapeIcon::ShapeIcon() {
   _iconPixmap = new QPixmap();
}

QIcon ShapeIcon::getIcon() {
   QIcon icon;

   switch ( _shape ) {
      case CIRCLE:
         if ( _hollow ) {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/hollow-circle-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/hollow-circle-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/hollow-circle" ) );
            }

         } else {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/circle-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/circle-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/circle" ) );
            }
         }

         break;

      case OBROUND:
         if ( _hollow ) {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/hollow-obround-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/hollow-obround-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/hollow-obround" ) );
            }

         } else {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/obround-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/obround-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/obround" ) );
            }
         }

         break;

      case RECTANGLE:
         if ( _hollow ) {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/hollow-rectangle-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/hollow-rectangle-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/hollow-rectangle" ) );
            }

         } else {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/rectangle-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/rectangle-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/rectangle" ) );
            }
         }

         break;

      case POLYGON:
         if ( _hollow ) {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/hollow-polygon-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/hollow-polygon-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/hollow-polygon" ) );
            }

         } else {
            if ( _deleted ) {
               icon.addPixmap( QPixmap( ":/res/polygon-deleted" ) );

            } else if ( _changed ) {
               icon.addPixmap( QPixmap( ":/res/polygon-changed" ) );

            } else {
               icon.addPixmap( QPixmap( ":/res/polygon" ) );
            }
         }

         break;

      case MACRO:
         if ( _deleted ) {
            icon.addPixmap( QPixmap( ":/res/macro-deleted" ) );

         } else if ( _changed ) {
            icon.addPixmap( QPixmap( ":/res/macro-changed" ) );

         } else {
            icon.addPixmap( QPixmap( ":/res/macro" ) );
         }

         break;

      case UNSET:
         if ( _deleted ) {
            icon.addPixmap( QPixmap( ":/res/unknown-deleted" ) );

         } else if ( _changed ) {
            icon.addPixmap( QPixmap( ":/res/unknown-changed" ) );

         } else {
            icon.addPixmap( QPixmap( ":/res/unknown" ) );
         }

         break;
   }

   return icon;
}

void ShapeIcon::setShape( shape_type_t shape ) {
   _shape = shape;
}

void ShapeIcon::setHollow( bool hollow ) {
   _hollow = hollow;
}

void ShapeIcon::setDeleted( bool deleted ) {
   _deleted = deleted;
}

void ShapeIcon::setChanged( bool changed ) {
   _changed = changed;
}

shape_type_t ShapeIcon::getShape() {
   return _shape;
}

bool ShapeIcon::isHollow() {
   return _hollow;
}

bool ShapeIcon::isDeleted() {
   return _deleted;
}

bool ShapeIcon::isChanged() {
   return _changed;
}
