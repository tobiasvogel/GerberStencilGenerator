#!/bin/bash

for header in `find . -iwholename */QtColorWidgets/color_dialog.hpp`; do
	sed -i '55s/f = 0/f = {0}/' ${header}
done

for source in `find . -iwholename */QtColorWidgets/color_dialog.cpp`; do
	sed -i '28s/#include <QDesktopWidget>/#include <QWindow>/' ${source}
done

for source in `find . -iwholename */QtColorWidgets/gradient_editor.cpp`; do
	sed -i '461s/QApplication::globalStrut()/QWidget::minimumSize()/' ${source}
done

for source in `find . -iwholename */QtColorWidgets/swatch.cpp`; do
	sed -i '563s/event->delta()/event->angleDelta().y()/' ${source}
done

for source in `find . -iwholename */QtColorWidgets/color_wheel.cpp`; do
	sed -i '/#include <QDragEnterEvent>/a#include <QPainterPath>' ${source}
done

for header in `find . -iwholename */QtColorWidgets/color_utils.hpp`; do
	sed -i '72s/qreal/double/g' ${header}
done

for source in `find . -iwholename */QtColorWidgets/color_utils.cpp`; do
	sed -i '54s/qreal/double/g' ${source}
done

for header in `find . -iwholename */QtColorWidgets/color_wheel_private.hpp`; do
	sed -i '53s/qreal/double/g' ${header}
done
