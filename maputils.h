#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <QColor>
#include <QWidget>
#include <QString>
#include <QLabel>

class MapUtils {
public:
    // Obtiene el color para un distrito específico
    static QColor getDistrictColor(int index);
};

#endif // MAPUTILS_H
