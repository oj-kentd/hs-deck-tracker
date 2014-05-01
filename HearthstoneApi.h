#ifndef HEARTHSTONEAPI_H
#define HEARTHSTONEAPI_H

#include <QString>

namespace HearthstoneApi
{
enum HeroClass{
    NeutralClass = 0,
    WarriorClass = 1,
    PaladinClass = 2,
    HunterClass = 3,
    RogueClass = 4,
    PriestClass = 5,
    ShamanClass = 7,
    MageClass = 8,
    WarlockClass = 9,
    DruidClass = 11
};

enum Quality{
    LegendaryQuality = 5,
    EpicQuality = 4,
    RareQuality = 3,
    CommonQuality = 1,
    BasicQuality = 0
};

enum CardType{
    Minion = 4,
    Spell = 5,
    Weapon = 7
};

}

static const QString NeutralColor = QString("#919191");
static const QString WarriorColor = QString("#cc9966");
static const QString PaladinColor = QString("#ff9999");
static const QString HunterColor = QString("#aad35a");
static const QString RogueColor = QString("#ffff33");
static const QString PriestColor = QString("#fff");
static const QString ShamanColor = QString("#2459f1");
static const QString MageColor = QString("#5eccef");
static const QString WarlockColor = QString("#9382c9");
static const QString DruidColor = QString("#ff6600");

static const QString HeroColors[12] = {
    NeutralColor,   // 0
    WarriorColor,   // 1
    PaladinColor,   // 2
    HunterColor,    // 3
    RogueColor,     // 4
    PriestColor,    // 5
    NeutralColor,   // 6
    ShamanColor,    // 7
    MageColor,      // 8
    WarlockColor,   // 9
    NeutralColor,   // 10
    DruidColor     // 11
};


#endif // HEARTHSTONEAPI_H
