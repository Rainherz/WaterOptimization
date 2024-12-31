#include "maputils.h"
#include <QPixmap>
#include <QMessageBox>
#include <QLabel>

QColor MapUtils::getDistrictColor(int index) {
    QColor colors[] = {
        QColor(255, 99, 71),   // Tomato
        QColor(65, 105, 225),  // Royal Blue
        QColor(60, 179, 113),  // Medium Sea Green
        QColor(238, 130, 238), // Violet
        QColor(255, 165, 0),   // Orange
        QColor(106, 90, 205),  // Slate Blue
        QColor(255, 215, 0),   // Gold
        QColor(70, 130, 180),  // Steel Blue
        QColor(218, 112, 214), // Orchid
        QColor(0, 128, 0),     // Green
        QColor(255, 192, 203), // Pink
        QColor(0, 139, 139),   // Dark Cyan
        QColor(219, 112, 147)  // Pale Violet Red
    };

    return colors[index % 13];
}
