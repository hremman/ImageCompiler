#ifndef OLD_COLORFUNCTIONS_H
#define OLD_COLORFUNCTIONS_H
#include <QImage>
#include <random>

class ColorFunctions
{
public:

    static QImage CompositeAlpha(const QImage & up, const QImage & down);
    static QImage ChangeColor(const QImage & img, int hue); //TODO Р вЂќР С•Р В±Р В°Р Р†Р С‘РЎвЂљРЎРЉ РЎС“РЎвЂЎРЎвЂРЎвЂљ РЎРѓР В°РЎвЂљРЎС“РЎР‚Р В°РЎвЂ Р С‘Р С‘ Р С‘ Р В·Р Р…Р В°РЎвЂЎР ВµР Р…Р С‘РЎРЏ

};

#endif // OLD_COLORFUNCTIONS_H
