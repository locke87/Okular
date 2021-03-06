/****************************************************************************
**
** Copyright (C) 2014 John Layt <jlayt@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qpagesize.h"

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsize.h>
#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

// Define the Windows DMPAPER sizes for use in the look-up table
// See http://msdn.microsoft.com/en-us/library/windows/desktop/dd319099.aspx

enum WindowsDmPaper {
    DMPAPER_NONE                            =   0,  // Not a DMPAPER, use for sizes without a DMPAPER value
    DMPAPER_LETTER                          =   1,
    DMPAPER_LETTERSMALL                     =   2,
    DMPAPER_TABLOID                         =   3,
    DMPAPER_LEDGER                          =   4,
    DMPAPER_LEGAL                           =   5,
    DMPAPER_STATEMENT                       =   6,
    DMPAPER_EXECUTIVE                       =   7,
    DMPAPER_A3                              =   8,
    DMPAPER_A4                              =   9,
    DMPAPER_A4SMALL                         =  10,
    DMPAPER_A5                              =  11,
    DMPAPER_B4                              =  12,
    DMPAPER_B5                              =  13,
    DMPAPER_FOLIO                           =  14,
    DMPAPER_QUARTO                          =  15,
    DMPAPER_10X14                           =  16,
    DMPAPER_11X17                           =  17,
    DMPAPER_NOTE                            =  18,
    DMPAPER_ENV_9                           =  19,
    DMPAPER_ENV_10                          =  20,
    DMPAPER_ENV_11                          =  21,
    DMPAPER_ENV_12                          =  22,
    DMPAPER_ENV_14                          =  23,
    DMPAPER_CSHEET                          =  24,
    DMPAPER_DSHEET                          =  25,
    DMPAPER_ESHEET                          =  26,
    DMPAPER_ENV_DL                          =  27,
    DMPAPER_ENV_C5                          =  28,
    DMPAPER_ENV_C3                          =  29,
    DMPAPER_ENV_C4                          =  30,
    DMPAPER_ENV_C6                          =  31,
    DMPAPER_ENV_C65                         =  32,
    DMPAPER_ENV_B4                          =  33,
    DMPAPER_ENV_B5                          =  34,
    DMPAPER_ENV_B6                          =  35,
    DMPAPER_ENV_ITALY                       =  36,
    DMPAPER_ENV_MONARCH                     =  37,
    DMPAPER_ENV_PERSONAL                    =  38,
    DMPAPER_FANFOLD_US                      =  39,
    DMPAPER_FANFOLD_STD_GERMAN              =  40,
    DMPAPER_FANFOLD_LGL_GERMAN              =  41,
    DMPAPER_ISO_B4                          =  42,
    DMPAPER_JAPANESE_POSTCARD               =  43,
    DMPAPER_9X11                            =  44,
    DMPAPER_10X11                           =  45,
    DMPAPER_15X11                           =  46,
    DMPAPER_ENV_INVITE                      =  47,
    DMPAPER_RESERVED_48                     =  48,
    DMPAPER_RESERVED_49                     =  49,
    DMPAPER_LETTER_EXTRA                    =  50,
    DMPAPER_LEGAL_EXTRA                     =  51,
    DMPAPER_TABLOID_EXTRA                   =  52,
    DMPAPER_A4_EXTRA                        =  53,
    DMPAPER_LETTER_TRANSVERSE               =  54,
    DMPAPER_A4_TRANSVERSE                   =  55,
    DMPAPER_LETTER_EXTRA_TRANSVERSE         =  56,
    DMPAPER_A_PLUS                          =  57,
    DMPAPER_B_PLUS                          =  58,
    DMPAPER_LETTER_PLUS                     =  59,
    DMPAPER_A4_PLUS                         =  60,
    DMPAPER_A5_TRANSVERSE                   =  61,
    DMPAPER_B5_TRANSVERSE                   =  62,
    DMPAPER_A3_EXTRA                        =  63,
    DMPAPER_A5_EXTRA                        =  64,
    DMPAPER_B5_EXTRA                        =  65,
    DMPAPER_A2                              =  66,
    DMPAPER_A3_TRANSVERSE                   =  67,
    DMPAPER_A3_EXTRA_TRANSVERSE             =  68,
    DMPAPER_DBL_JAPANESE_POSTCARD           =  69,
    DMPAPER_A6                              =  70,
    DMPAPER_JENV_KAKU2                      =  71,
    DMPAPER_JENV_KAKU3                      =  72,
    DMPAPER_JENV_CHOU3                      =  73,
    DMPAPER_JENV_CHOU4                      =  74,
    DMPAPER_LETTER_ROTATED                  =  75,
    DMPAPER_A3_ROTATED                      =  76,
    DMPAPER_A4_ROTATED                      =  77,
    DMPAPER_A5_ROTATED                      =  78,
    DMPAPER_B4_JIS_ROTATED                  =  79,
    DMPAPER_B5_JIS_ROTATED                  =  80,
    DMPAPER_JAPANESE_POSTCARD_ROTATED       =  81,
    DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED   =  82,
    DMPAPER_A6_ROTATED                      =  83,
    DMPAPER_JENV_KAKU2_ROTATED              =  84,
    DMPAPER_JENV_KAKU3_ROTATED              =  85,
    DMPAPER_JENV_CHOU3_ROTATED              =  86,
    DMPAPER_JENV_CHOU4_ROTATED              =  87,
    DMPAPER_B6_JIS                          =  88,
    DMPAPER_B6_JIS_ROTATED                  =  89,
    DMPAPER_12X11                           =  90,
    DMPAPER_JENV_YOU4                       =  91,
    DMPAPER_JENV_YOU4_ROTATED               =  92,
    DMPAPER_P16K                            =  93,
    DMPAPER_P32K                            =  94,
    DMPAPER_P32KBIG                         =  95,
    DMPAPER_PENV_1                          =  96,
    DMPAPER_PENV_2                          =  97,
    DMPAPER_PENV_3                          =  98,
    DMPAPER_PENV_4                          =  99,
    DMPAPER_PENV_5                          = 100,
    DMPAPER_PENV_6                          = 101,
    DMPAPER_PENV_7                          = 102,
    DMPAPER_PENV_8                          = 103,
    DMPAPER_PENV_9                          = 104,
    DMPAPER_PENV_10                         = 105,
    DMPAPER_P16K_ROTATED                    = 106,
    DMPAPER_P32K_ROTATED                    = 107,
    DMPAPER_P32KBIG_ROTATED                 = 108,
    DMPAPER_PENV_1_ROTATED                  = 109,
    DMPAPER_PENV_2_ROTATED                  = 110,
    DMPAPER_PENV_3_ROTATED                  = 111,
    DMPAPER_PENV_4_ROTATED                  = 112,
    DMPAPER_PENV_5_ROTATED                  = 113,
    DMPAPER_PENV_6_ROTATED                  = 114,
    DMPAPER_PENV_7_ROTATED                  = 115,
    DMPAPER_PENV_8_ROTATED                  = 116,
    DMPAPER_PENV_9_ROTATED                  = 117,
    DMPAPER_PENV_10_ROTATED                 = 118,
    DMPAPER_LAST                            = DMPAPER_PENV_10_ROTATED,
    DMPAPER_USER                            = 256
};

// Conversion table for historic page size values that we don't support.
// These are deprecated in PPD and strongly discouraged from being used,
// so convert them to usable page sizes to support older print devices.
// The paper source orientation will be handled in the QPrintMedia class,
// we're only concerned about the standard size in QPageSize.
// _ROTATED = 90 degrees or QPageLayout::Landscape
// _TRANSVERSE = 180 degrees or QPageLayout::ReversePortrait

static const int qt_windowsConversion[][2] = {
    {DMPAPER_11X17,                         DMPAPER_TABLOID},  // = DMPAPER_LEDGER rotated
    {DMPAPER_A3_EXTRA_TRANSVERSE,           DMPAPER_A3_EXTRA},
    {DMPAPER_A3_ROTATED,                    DMPAPER_A3},
    {DMPAPER_A3_TRANSVERSE,                 DMPAPER_A3},
    {DMPAPER_A4_ROTATED,                    DMPAPER_A4},
    {DMPAPER_A4_TRANSVERSE,                 DMPAPER_A4},
    {DMPAPER_A5_ROTATED,                    DMPAPER_A5},
    {DMPAPER_A5_TRANSVERSE,                 DMPAPER_A5},
    {DMPAPER_A6_ROTATED,                    DMPAPER_A6},
    {DMPAPER_B4_JIS_ROTATED,                DMPAPER_B4},
    {DMPAPER_B5_JIS_ROTATED,                DMPAPER_B5},
    {DMPAPER_B5_TRANSVERSE,                 DMPAPER_B5},
    {DMPAPER_B6_JIS_ROTATED,                DMPAPER_B6_JIS},
    {DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED, DMPAPER_DBL_JAPANESE_POSTCARD},
    {DMPAPER_JAPANESE_POSTCARD_ROTATED,     DMPAPER_JAPANESE_POSTCARD},
    {DMPAPER_JENV_CHOU3_ROTATED,            DMPAPER_JENV_CHOU3},
    {DMPAPER_JENV_CHOU4_ROTATED,            DMPAPER_JENV_CHOU4},
    {DMPAPER_JENV_KAKU2_ROTATED,            DMPAPER_JENV_KAKU2},
    {DMPAPER_JENV_KAKU3_ROTATED,            DMPAPER_JENV_KAKU3},
    {DMPAPER_JENV_YOU4_ROTATED,             DMPAPER_JENV_YOU4},
    {DMPAPER_LETTER_EXTRA_TRANSVERSE,       DMPAPER_LETTER_EXTRA},
    {DMPAPER_LETTER_ROTATED,                DMPAPER_LETTER},
    {DMPAPER_LETTER_TRANSVERSE,             DMPAPER_LETTER},
    {DMPAPER_P16K_ROTATED,                  DMPAPER_P16K},
    {DMPAPER_P32K_ROTATED,                  DMPAPER_P32K},
    {DMPAPER_P32KBIG_ROTATED,               DMPAPER_P32KBIG},
    {DMPAPER_PENV_1_ROTATED,                DMPAPER_PENV_1},
    {DMPAPER_PENV_2_ROTATED,                DMPAPER_PENV_2},
    {DMPAPER_PENV_3_ROTATED,                DMPAPER_PENV_3},
    {DMPAPER_PENV_4_ROTATED,                DMPAPER_PENV_4},
    {DMPAPER_PENV_5_ROTATED,                DMPAPER_PENV_5},
    {DMPAPER_PENV_6_ROTATED,                DMPAPER_PENV_6},
    {DMPAPER_PENV_7_ROTATED,                DMPAPER_PENV_7},
    {DMPAPER_PENV_8_ROTATED,                DMPAPER_PENV_8},
    {DMPAPER_PENV_9_ROTATED,                DMPAPER_PENV_9},
    {DMPAPER_PENV_10_ROTATED,               DMPAPER_PENV_10}  // Is = DMPAPER_LAST, use as loop terminator
};

static const int windowsConversionCount = int(sizeof(qt_windowsConversion) / sizeof(qt_windowsConversion[0]));

// Standard sizes data
struct StandardPageSize {
    QPageSize::PageSizeId id;
    int windowsId;                  // Windows DMPAPER value
    QPageSize::Unit definitionUnits;    // Standard definition size, e.g. ISO uses mm, ANSI uses inches
    int widthPoints;
    int heightPoints;
    qreal widthMillimeters;
    qreal heightMillimeters;
    qreal widthInches;
    qreal heightInches;
    const char *mediaOption;  // PPD standard mediaOption ID
};

// Standard page sizes taken from the Postscript PPD Standard v4.3
// See http://partners.adobe.com/public/developer/en/ps/5003.PPD_Spec_v4.3.pdf
// Excludes all Transverse and Rotated sizes
// NB! This table needs to be in sync with QPageSize::PageSizeId
static const StandardPageSize qt_pageSizes[] = {

    // Existing Qt sizes including ISO, US, ANSI and other standards
    {QPageSize::A4                ,   DMPAPER_A4                   ,   QPageSize::Millimeter,    595,  842,      210  ,  297  ,       8.27,  11.69,    "A4"},
    {QPageSize::B5                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    499,  709,      176  ,  250  ,       6.9 ,   9.8 ,    "ISOB5"},
    {QPageSize::Letter            ,   DMPAPER_LETTER               ,   QPageSize::Inch      ,    612,  792,      215.9,  279.4,       8.5 ,  11   ,    "Letter"},
    {QPageSize::Legal             ,   DMPAPER_LEGAL                ,   QPageSize::Inch      ,    612, 1008,      215.9,  355.6,       8.5 ,  14   ,    "Legal"},
    {QPageSize::Executive         ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    540,  720,      190.5,  254  ,       7.5 ,  10   ,    "Executive.7.5x10in"}, // Qt size differs from Postscript / Windows
    {QPageSize::A0                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2384, 3370,      841  , 1189  ,      33.11,  46.81,    "A0"},
    {QPageSize::A1                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1684, 2384,      594  ,  841  ,      23.39,  33.11,    "A1"},
    {QPageSize::A2                ,   DMPAPER_A2                   ,   QPageSize::Millimeter,   1191, 1684,      420  ,  594  ,      16.54,  23.39,    "A2"},
    {QPageSize::A3                ,   DMPAPER_A3                   ,   QPageSize::Millimeter,    842, 1191,      297  ,  420  ,      11.69,  16.54,    "A3"},
    {QPageSize::A5                ,   DMPAPER_A5                   ,   QPageSize::Millimeter,    420,  595,      148  ,  210  ,       5.83,   8.27,    "A5"},
    {QPageSize::A6                ,   DMPAPER_A6                   ,   QPageSize::Millimeter,    297,  420,      105  ,  148  ,       4.13,   5.83,    "A6"},
    {QPageSize::A7                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    210,  297,       74  ,  105  ,       2.91,   4.13,    "A7"},
    {QPageSize::A8                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    148,  210,       52  ,   74  ,       2.05,   2.91,    "A8"},
    {QPageSize::A9                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    105,  148,       37  ,   52  ,       1.46,   2.05,    "A9"},
    {QPageSize::B0                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2835, 4008,     1000  , 1414  ,      39.37,  55.67,    "ISOB0"},
    {QPageSize::B1                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2004, 2835,      707  , 1000  ,      27.83,  39.37,    "ISOB1"},
    {QPageSize::B10               ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,     88,  125,       31  ,   44  ,       1.22,   1.73,    "ISOB10"},
    {QPageSize::B2                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1417, 2004,      500  ,  707  ,      19.68,  27.83,    "ISOB2"},
    {QPageSize::B3                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1001, 1417,      353  ,  500  ,      13.9 ,  19.68,    "ISOB3"},
    {QPageSize::B4                ,   DMPAPER_ISO_B4               ,   QPageSize::Millimeter,    709, 1001,      250  ,  353  ,       9.84,  13.9 ,    "ISOB4"},
    {QPageSize::B6                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    354,  499,      125  ,  176  ,       4.92,   6.93,    "ISOB6"},
    {QPageSize::B7                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    249,  354,       88  ,  125  ,       3.46,   4.92,    "ISOB7"},
    {QPageSize::B8                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    176,  249,       62  ,   88  ,       2.44,   3.46,    "ISOB8"},
    {QPageSize::B9                ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    125,  176,       44  ,   62  ,       1.73,   2.44,    "ISOB9"},
    {QPageSize::C5E               ,   DMPAPER_ENV_C5               ,   QPageSize::Millimeter,    459,  649,      162  ,  229  ,       6.38,   9.02,    "EnvC5"},
    {QPageSize::Comm10E           ,   DMPAPER_ENV_10               ,   QPageSize::Inch      ,    297,  684,      104.8,  241.3,       4.12,   9.5 ,    "Env10"},
    {QPageSize::DLE               ,   DMPAPER_ENV_DL               ,   QPageSize::Millimeter,    312,  624,      110  ,  220  ,       4.33,   8.66,    "EnvDL"},
    {QPageSize::Folio             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    595,  935,      210  ,  330  ,       8.27,  13   ,    "Folio"},
    {QPageSize::Ledger            ,   DMPAPER_LEDGER               ,   QPageSize::Inch      ,   1224,  792,      431.8,  279.4,      17   ,  11   ,    "Ledger"},
    {QPageSize::Tabloid           ,   DMPAPER_TABLOID              ,   QPageSize::Inch      ,    792, 1224,      279.4,  431.8,      11   ,  17   ,    "Tabloid"},
    {QPageSize::Custom            ,   DMPAPER_USER                 ,   QPageSize::Millimeter,     -1,   -1,       -1. ,   -1  ,      -1   ,  -1   ,    "Custom"}, // Special case to keep in sync with QPageSize::PageSizeId

    // ISO Standard Sizes
    {QPageSize::A10               ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,     73,  105,       26  ,  37   ,       1.02,   1.46,    "A10"},
    {QPageSize::A3Extra           ,   DMPAPER_A3_EXTRA             ,   QPageSize::Millimeter,    913, 1262,      322  ,  445  ,      12.67,  17.52,    "A3Extra"},
    {QPageSize::A4Extra           ,   DMPAPER_A4_EXTRA             ,   QPageSize::Millimeter,    667,  914,      235.5,  322.3,       9.27,  12.69,    "A4Extra"},
    {QPageSize::A4Plus            ,   DMPAPER_A4_PLUS              ,   QPageSize::Millimeter,    595,  936,      210  ,  330  ,       8.27,  13   ,    "A4Plus"},
    {QPageSize::A4Small           ,   DMPAPER_A4SMALL              ,   QPageSize::Millimeter,    595,  842,      210  ,  297  ,       8.27,  11.69,    "A4Small"},
    {QPageSize::A5Extra           ,   DMPAPER_A5_EXTRA             ,   QPageSize::Millimeter,    492,  668,      174  ,  235  ,       6.85,   9.25,    "A5Extra"},
    {QPageSize::B5Extra           ,   DMPAPER_B5_EXTRA             ,   QPageSize::Millimeter,    570,  782,      201  ,  276  ,       7.9 ,  10.8 ,    "ISOB5Extra"},

    // JIS Standard Sizes
    {QPageSize::JisB0             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2920, 4127,     1030  , 1456  ,      40.55,  57.32,    "B0"},
    {QPageSize::JisB1             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2064, 2920,      728  , 1030  ,      28.66,  40.55,    "B1"},
    {QPageSize::JisB2             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1460, 2064,      515  ,  728  ,      20.28,  28.66,    "B2"},
    {QPageSize::JisB3             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1032, 1460,      364  ,  515  ,      14.33,  20.28,    "B3"},
    {QPageSize::JisB4             ,   DMPAPER_B4                   ,   QPageSize::Millimeter,    729, 1032,      257  ,  364  ,      10.12,  14.33,    "B4"},
    {QPageSize::JisB5             ,   DMPAPER_B5                   ,   QPageSize::Millimeter,    516,  729,      182  ,  257  ,       7.17,  10.12,    "B5"},
    {QPageSize::JisB6             ,   DMPAPER_B6_JIS               ,   QPageSize::Millimeter,    363,  516,      128  ,  182  ,       5.04,   7.17,    "B6"},
    {QPageSize::JisB7             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    258,  363,       91  ,  128  ,       3.58,   5.04,    "B7"},
    {QPageSize::JisB8             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    181,  258,       64  ,   91  ,       2.52,   3.58,    "B8"},
    {QPageSize::JisB9             ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    127,  181,       45  ,   64  ,       1.77,   2.52,    "B9"},
    {QPageSize::JisB10            ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,     91,  127,       32  ,   45  ,       1.26,   1.77,    "B10"},

    // ANSI / US Standard sizes
    {QPageSize::AnsiC             ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,   1224, 1584,      431.8,  558.8,      17   ,  22   ,    "AnsiC"},
    {QPageSize::AnsiD             ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,   1584, 2448,      558.8,  863.6,      22   ,  34   ,    "AnsiD"},
    {QPageSize::AnsiE             ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,   2448, 3168,      863.6, 1118  ,      34   ,  44   ,    "AnsiE"},
    {QPageSize::LegalExtra        ,   DMPAPER_LEGAL_EXTRA          ,   QPageSize::Inch      ,    684, 1080,      241.3,  381  ,       9.5 ,  15   ,    "LegalExtra"},
    {QPageSize::LetterExtra       ,   DMPAPER_LETTER_EXTRA         ,   QPageSize::Inch      ,    684,  864,      241.3,  304.8,       9.5 ,  12   ,    "LetterExtra"},
    {QPageSize::LetterPlus        ,   DMPAPER_LETTER_PLUS          ,   QPageSize::Inch      ,    612,  914,      215.9,  322.3,       8.5 ,  12.69,    "LetterPlus"},
    {QPageSize::LetterSmall       ,   DMPAPER_LETTERSMALL          ,   QPageSize::Inch      ,    612,  792,      215.9,  279.4,       8.5 ,  11   ,    "LetterSmall"},
    {QPageSize::TabloidExtra      ,   DMPAPER_TABLOID_EXTRA        ,   QPageSize::Inch      ,    864, 1296,      304.8,  457.2,      12   ,  18   ,    "TabloidExtra"},

    // Architectural sizes
    {QPageSize::ArchA             ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    648,  864,      228.6,  304.8,       9   ,  12   ,    "ARCHA"},
    {QPageSize::ArchB             ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    864, 1296,      304.8,  457.2,      12   ,  18   ,    "ARCHB"},
    {QPageSize::ArchC             ,   DMPAPER_CSHEET               ,   QPageSize::Inch      ,   1296, 1728,      457.2,  609.6,      18   ,  24   ,    "ARCHC"},
    {QPageSize::ArchD             ,   DMPAPER_DSHEET               ,   QPageSize::Inch      ,   1728, 2592,      609.6,  914.4,      24   ,  36   ,    "ARCHD"},
    {QPageSize::ArchE             ,   DMPAPER_ESHEET               ,   QPageSize::Inch      ,   2592, 3456,      914.4, 1219  ,      36   ,  48   ,    "ARCHE"},

    // Inch-based Sizes
    {QPageSize::Imperial7x9       ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    504,  648,      177.8,  228.6,       7   ,   9   ,    "7x9"},
    {QPageSize::Imperial8x10      ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    576,  720,      203.2,  254  ,       8   ,  10   ,    "8x10"},
    {QPageSize::Imperial9x11      ,   DMPAPER_9X11                 ,   QPageSize::Inch      ,    648,  792,      228.6,  279.4,       9   ,  11   ,    "9x11"},
    {QPageSize::Imperial9x12      ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    648,  864,      228.6,  304.8,       9   ,  12   ,    "9x12"},
    {QPageSize::Imperial10x11     ,   DMPAPER_10X11                ,   QPageSize::Inch      ,    720,  792,      254  ,  279.4,      10   ,  11   ,    "10x11"},
    {QPageSize::Imperial10x13     ,   DMPAPER_NONE                 ,   QPageSize::Inch      ,    720,  936,      254  ,  330.2,      10   ,  13   ,    "10x13"},
    {QPageSize::Imperial10x14     ,   DMPAPER_10X14                ,   QPageSize::Inch      ,    720, 1008,      254  ,  355.6,      10   ,  14   ,    "10x14"},
    {QPageSize::Imperial12x11     ,   DMPAPER_12X11                ,   QPageSize::Inch      ,    864,  792,      304.8,  279.4,      12   ,  11   ,    "12x11"},
    {QPageSize::Imperial15x11     ,   DMPAPER_15X11                ,   QPageSize::Inch      ,   1080,  792,      381  ,  279.4,      15   ,  11   ,    "15x11"},

    // Other Page Sizes
    {QPageSize::ExecutiveStandard ,   DMPAPER_EXECUTIVE            ,   QPageSize::Inch      ,    522,  756,      184.2,  266.7,       7.25,  10.5 ,    "Executive"},   // Qt size differs from Postscript / Windows
    {QPageSize::Note              ,   DMPAPER_NOTE                 ,   QPageSize::Inch      ,    612,  792,      215.9,  279.4,       8.5 ,  11   ,    "Note"},
    {QPageSize::Quarto            ,   DMPAPER_QUARTO               ,   QPageSize::Inch      ,    610,  780,      215.9,  275.1,       8.5 ,  10.83,    "Quarto"},
    {QPageSize::Statement         ,   DMPAPER_STATEMENT            ,   QPageSize::Inch      ,    396,  612,      139.7,  215.9,       5.5 ,   8.5 ,    "Statement"},
    {QPageSize::SuperA            ,   DMPAPER_A_PLUS               ,   QPageSize::Millimeter,    643, 1009,      227  ,  356  ,       8.94,  14   ,    "SuperA"},
    {QPageSize::SuperB            ,   DMPAPER_B_PLUS               ,   QPageSize::Millimeter,    864, 1380,      305  ,  487  ,      12   ,  19.17,    "SuperB"},
    {QPageSize::Postcard          ,   DMPAPER_JAPANESE_POSTCARD    ,   QPageSize::Millimeter,    284,  419,      100  ,  148  ,       3.94,   5.83,    "Postcard"},
    {QPageSize::DoublePostcard    ,   DMPAPER_DBL_JAPANESE_POSTCARD,   QPageSize::Millimeter,    567,  419,      200  ,  148  ,       7.87,   5.83,    "DoublePostcard"},
    {QPageSize::Prc16K            ,   DMPAPER_P16K                 ,   QPageSize::Millimeter,    414,  610,      146  ,  215  ,       5.75,   8.5 ,    "PRC16K"},
    {QPageSize::Prc32K            ,   DMPAPER_P32K                 ,   QPageSize::Millimeter,    275,  428,       97  ,  151  ,       3.82,   5.95,    "PRC32K"},
    {QPageSize::Prc32KBig         ,   DMPAPER_P32KBIG              ,   QPageSize::Millimeter,    275,  428,       97  ,  151  ,       3.82,   5.95,    "PRC32KBig"},

    // Fan Fold Sizes
    {QPageSize::FanFoldUS         ,   DMPAPER_FANFOLD_US           ,   QPageSize::Inch      ,   1071,  792,      377.8,  279.4,      14.875, 11   ,    "FanFoldUS"},
    {QPageSize::FanFoldGerman     ,   DMPAPER_FANFOLD_STD_GERMAN   ,   QPageSize::Inch      ,    612,  864,      215.9,  304.8,       8.5 ,  12   ,    "FanFoldGerman"},
    {QPageSize::FanFoldGermanLegal,   DMPAPER_FANFOLD_LGL_GERMAN   ,   QPageSize::Inch      ,    612,  936,      215.9,  330  ,       8.5 ,  13   ,    "FanFoldGermanLegal"},

    // ISO Envelopes
    {QPageSize::EnvelopeB4        ,   DMPAPER_ENV_B4               ,   QPageSize::Millimeter,    708, 1001,      250  ,  353  ,       9.84,  13.9 ,    "EnvISOB4"},
    {QPageSize::EnvelopeB5        ,   DMPAPER_ENV_B5               ,   QPageSize::Millimeter,    499,  709,      176  ,  250  ,       6.9 ,   9.8 ,    "EnvISOB5"},
    {QPageSize::EnvelopeB6        ,   DMPAPER_ENV_B6               ,   QPageSize::Millimeter,    499,  354,      176  ,  125  ,       6.9 ,   4.9 ,    "EnvISOB6"},
    {QPageSize::EnvelopeC0        ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   2599, 3676,      917  ,  1297 ,      36.1 ,  51.06,    "EnvC0"},
    {QPageSize::EnvelopeC1        ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1837, 2599,      648  ,  917  ,      25.51,  36.1 ,    "EnvC1"},
    {QPageSize::EnvelopeC2        ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,   1298, 1837,      458  ,  648  ,      18.03,  25.51,    "EnvC2"},
    {QPageSize::EnvelopeC3        ,   DMPAPER_ENV_C3               ,   QPageSize::Millimeter,    918, 1296,      324  ,  458  ,      12.75,  18.03,    "EnvC3"},
    {QPageSize::EnvelopeC4        ,   DMPAPER_ENV_C4               ,   QPageSize::Millimeter,    649,  918,      229  ,  324  ,       9.02,  12.75,    "EnvC4"},
    {QPageSize::EnvelopeC6        ,   DMPAPER_ENV_C6               ,   QPageSize::Millimeter,    323,  459,      114  ,  162  ,       4.49,   6.38,    "EnvC6"},
    {QPageSize::EnvelopeC65       ,   DMPAPER_ENV_C65              ,   QPageSize::Millimeter,    324,  648,      114  ,  229  ,       4.5 ,   9   ,    "EnvC65"},
    {QPageSize::EnvelopeC7        ,   DMPAPER_NONE                 ,   QPageSize::Millimeter,    230,  323,       81  ,  114  ,       3.19,   4.49,    "EnvC7"},

    // US Envelopes
    {QPageSize::Envelope9         ,   DMPAPER_ENV_9                ,   QPageSize::Inch      ,    279,  639,       98.4,  225.4,       3.875,  8.875,   "Env9"},
    {QPageSize::Envelope11        ,   DMPAPER_ENV_11               ,   QPageSize::Inch      ,    324,  747,      114.3,  263.5,       4.5 ,  10.375,   "Env11"},
    {QPageSize::Envelope12        ,   DMPAPER_ENV_12               ,   QPageSize::Inch      ,    342,  792,      120.7,  279.4,       4.75,  11   ,    "Env12"},
    {QPageSize::Envelope14        ,   DMPAPER_ENV_14               ,   QPageSize::Inch      ,    360,  828,      127  ,  292.1,       5   ,  11.5 ,    "Env14"},
    {QPageSize::EnvelopeMonarch   ,   DMPAPER_ENV_MONARCH          ,   QPageSize::Inch      ,    279,  540,       98.43, 190.5,       3.875,  7.5 ,    "EnvMonarch"},
    {QPageSize::EnvelopePersonal  ,   DMPAPER_ENV_PERSONAL         ,   QPageSize::Inch      ,    261,  468,       92.08, 165.1,       3.625,  6.5 ,    "EnvPersonal"},

    // Other Envelopes
    {QPageSize::EnvelopeChou3     ,   DMPAPER_JENV_CHOU3           ,   QPageSize::Millimeter,    340,  666,      120  ,  235  ,       4.72,   9.25,    "EnvChou3"},
    {QPageSize::EnvelopeChou4     ,   DMPAPER_JENV_CHOU4           ,   QPageSize::Millimeter,    255,  581,       90  ,  205  ,       3.54,   8   ,    "EnvChou4"},
    {QPageSize::EnvelopeInvite    ,   DMPAPER_ENV_INVITE           ,   QPageSize::Millimeter,    624,  624,      220  ,  220  ,       8.66,   8.66,    "EnvInvite"},
    {QPageSize::EnvelopeItalian   ,   DMPAPER_ENV_ITALY            ,   QPageSize::Millimeter,    312,  652,      110  ,  230  ,       4.33,   9   ,    "EnvItalian"},
    {QPageSize::EnvelopeKaku2     ,   DMPAPER_JENV_KAKU2           ,   QPageSize::Millimeter,    680,  941,      240  ,  332  ,       9.45,  13   ,    "EnvKaku2"},
    {QPageSize::EnvelopeKaku3     ,   DMPAPER_JENV_KAKU3           ,   QPageSize::Millimeter,    612,  785,      216  ,  277  ,       8.5 ,  10.9 ,    "EnvKaku3"},
    {QPageSize::EnvelopePrc1      ,   DMPAPER_PENV_1               ,   QPageSize::Millimeter,    289,  468,      102  ,  165  ,       4   ,   6.5 ,    "EnvPRC1"},
    {QPageSize::EnvelopePrc2      ,   DMPAPER_PENV_2               ,   QPageSize::Millimeter,    289,  499,      102  ,  176  ,       4   ,   6.9 ,    "EnvPRC2"},
    {QPageSize::EnvelopePrc3      ,   DMPAPER_PENV_3               ,   QPageSize::Millimeter,    354,  499,      125  ,  176  ,       4.9 ,   6.9 ,    "EnvPRC3"},
    {QPageSize::EnvelopePrc4      ,   DMPAPER_PENV_4               ,   QPageSize::Millimeter,    312,  590,      110  ,  208  ,       4.33,   8.2 ,    "EnvPRC4"},
    {QPageSize::EnvelopePrc5      ,   DMPAPER_PENV_5               ,   QPageSize::Millimeter,    312,  624,      110  ,  220  ,       4.33,   8.66,    "EnvPRC5"},
    {QPageSize::EnvelopePrc6      ,   DMPAPER_PENV_6               ,   QPageSize::Millimeter,    340,  652,      120  ,  230  ,       4.7 ,   9   ,    "EnvPRC6"},
    {QPageSize::EnvelopePrc7      ,   DMPAPER_PENV_7               ,   QPageSize::Millimeter,    454,  652,      160  ,  230  ,       6.3 ,   9   ,    "EnvPRC7"},
    {QPageSize::EnvelopePrc8      ,   DMPAPER_PENV_8               ,   QPageSize::Millimeter,    340,  876,      120  ,  309  ,       4.7 ,  12.2 ,    "EnvPRC8"},
    {QPageSize::EnvelopePrc9      ,   DMPAPER_PENV_9               ,   QPageSize::Millimeter,    649,  918,      229  ,  324  ,       9   ,  12.75,    "EnvPRC9"},
    {QPageSize::EnvelopePrc10     ,   DMPAPER_PENV_10              ,   QPageSize::Millimeter,    918, 1298,      324  ,  458  ,      12.75,  18   ,    "EnvPRC10"},
    {QPageSize::EnvelopeYou4      ,   DMPAPER_JENV_YOU4            ,   QPageSize::Millimeter,    298,  666,      105  ,  235  ,       4.13,   9.25,    "EnvYou4"}
};

static const int pageSizesCount = int(sizeof(qt_pageSizes) / sizeof(qt_pageSizes[0]));

// Return key name for PageSize
static QString qt_keyForPageSizeId(QPageSize::PageSizeId id)
{
    return QString::fromLatin1(qt_pageSizes[id].mediaOption);
}

// Return key name for custom size
static QString qt_keyForCustomSize(const QSizeF &size, QPageSize::Unit units)
{
    // PPD custom format
    QString key = QString("Custom.%1x%2%3");
    QString abbrev;
    switch (units) {
    case QPageSize::Millimeter:
        abbrev = QString("mm");
        break;
    case QPageSize::Point:
        break;
    case QPageSize::Inch:
        abbrev = QString("in");
        break;
    case QPageSize::Pica:
        abbrev = QString("pc");
        break;
    case QPageSize::Didot:
        abbrev = QString("DD");
        break;
    case QPageSize::Cicero:
        abbrev = QString("CC");
        break;
    }
    // Assumes size is already max 2 decimal places
    return key.arg(size.width()).arg(size.height()).arg(abbrev);
}

// Multiplier for converting units to points.
static qreal qt_pointMultiplier(QPageSize::Unit unit)
{
    switch (unit) {
    case QPageSize::Millimeter:
        return 2.83464566929;
    case QPageSize::Point:
        return 1.0;
    case QPageSize::Inch:
        return 72.0;
    case QPageSize::Pica:
        return 12;
    case QPageSize::Didot:
        return 1.065826771;
    case QPageSize::Cicero:
        return 12.789921252;
    }
    return 1.0;
}

// Multiplier for converting pixels to points.
Q_GUI_EXPORT qreal qt_pixelMultiplier(int resolution)
{
    return resolution <= 0 ? 1.0 : 72.0 / resolution;
}

static QSizeF qt_definitionSize(QPageSize::PageSizeId pageSizeId)
{
    QPageSize::Unit units = qt_pageSizes[pageSizeId].definitionUnits;
    if (units == QPageSize::Millimeter)
        return QSizeF(qt_pageSizes[pageSizeId].widthMillimeters, qt_pageSizes[pageSizeId].heightMillimeters);
    Q_ASSERT(units == QPageSize::Inch);  // We currently only support definitions in mm or inches
    return QSizeF(qt_pageSizes[pageSizeId].widthInches, qt_pageSizes[pageSizeId].heightInches);
}

static QSizeF qt_convertUnits(const QSizeF &size, QPageSize::Unit fromUnits, QPageSize::Unit toUnits)
{
    if (!size.isValid())
        return QSizeF();

    // If the units are the same or the size is 0, then don't need to convert
    if (fromUnits == toUnits || (qFuzzyIsNull(size.width()) && qFuzzyIsNull(size.height())))
        return size;

    QSizeF newSize = size;
    // First convert to points
    if (fromUnits != QPageSize::Point) {
        const qreal multiplier = qt_pointMultiplier(fromUnits);
        newSize = newSize * multiplier;
    }
    // Then convert from points to required units
    const qreal multiplier = qt_pointMultiplier(toUnits);
    // Try force to 2 decimal places for consistency
    const int width = qRound(newSize.width() * 100 / multiplier);
    const int height = qRound(newSize.height() * 100 / multiplier);
    return QSizeF(width / 100.0, height / 100.0);
}

static QSize qt_convertUnitsToPoints(const QSizeF &size, QPageSize::Unit units)
{
    if (!size.isValid())
        return QSize();
    return QSizeF(size * qt_pointMultiplier(units)).toSize();
}

static QSize qt_convertPointsToPixels(const QSize &size, int resolution)
{
    if (!size.isValid() || resolution <= 0)
        return QSize();
    const qreal multiplier = qt_pixelMultiplier(resolution);
    return QSize(qRound(size.width() / multiplier), qRound(size.height() / multiplier));
}

static QSizeF qt_convertPointsToUnits(const QSize &size, QPageSize::Unit units)
{
    if (!size.isValid())
        return QSizeF();
    const qreal multiplier = qt_pointMultiplier(units);
    // Try force to 2 decimal places for consistency
    const int width = qRound(size.width() * 100 / multiplier);
    const int height = qRound(size.height() * 100 / multiplier);
    return QSizeF(width / 100.0, height / 100.0);
}

static QSizeF qt_unitSize(QPageSize::PageSizeId pageSizeId, QPageSize::Unit units)
{
    switch (units) {
    case QPageSize::Millimeter:
        return QSizeF(qt_pageSizes[pageSizeId].widthMillimeters, qt_pageSizes[pageSizeId].heightMillimeters);
    case QPageSize::Point:
        return QSizeF(qt_pageSizes[pageSizeId].widthPoints, qt_pageSizes[pageSizeId].heightPoints);
    case QPageSize::Inch:
        return QSizeF(qt_pageSizes[pageSizeId].widthInches, qt_pageSizes[pageSizeId].heightInches);
    case QPageSize::Pica:
    case QPageSize::Didot:
    case QPageSize::Cicero:
        return qt_convertPointsToUnits(QSize(qt_pageSizes[pageSizeId].widthPoints,
                                             qt_pageSizes[pageSizeId].heightPoints), units);
    }
    return QSizeF();
}

class QPageSizePrivate : public QSharedData
{
public:
    explicit QPageSizePrivate(QPageSize::PageSizeId pageSizeId);
    ~QPageSizePrivate();

    bool operator==(const QPageSizePrivate &other) const;
    bool isEquivalentTo(const QPageSizePrivate &other) const;

    bool isValid() const;

    QSizeF size(QPageSize::Unit units) const;
    QSize sizePixels(int resolution) const;

private:
    friend class QPageSize;

    void init(QPageSize::PageSizeId id);
    void init(const QSize &size);
    void init(const QSizeF &size, QPageSize::Unit units);

    QString m_key;
    QPageSize::PageSizeId m_id;
    QSize m_pointSize;
    int m_windowsId;
    QSizeF m_size;
    QPageSize::Unit m_units;
};

QPageSizePrivate::QPageSizePrivate(QPageSize::PageSizeId pageSizeId)
    : m_id(QPageSize::Custom),
      m_windowsId(0),
      m_units(QPageSize::Point)
{
    if (pageSizeId >= QPageSize::PageSizeId(0) && pageSizeId <= QPageSize::LastPageSize)
        init(pageSizeId);
}

QPageSizePrivate::~QPageSizePrivate()
{
}

// Init a standard PageSizeId
void QPageSizePrivate::init(QPageSize::PageSizeId id)
{
    m_id = id;
    m_size = qt_definitionSize(id);
    m_units = qt_pageSizes[id].definitionUnits;
    m_key = qt_keyForPageSizeId(id);
    m_windowsId = qt_pageSizes[id].windowsId;
    m_pointSize = QSize(qt_pageSizes[id].widthPoints, qt_pageSizes[id].heightPoints);
}

// Init a point size
void QPageSizePrivate::init(const QSize &size)
{
    m_id = QPageSize::Custom;
    m_size = size;
    m_units = QPageSize::Point;
    m_key = qt_keyForCustomSize(m_size, m_units);
    m_windowsId = 0;
    m_pointSize = size;
}

// Init a unit size
void QPageSizePrivate::init(const QSizeF &size, QPageSize::Unit units)
{
    m_id = QPageSize::Custom;
    m_size = size;
    m_units = units;
    m_key = qt_keyForCustomSize(m_size, m_units);
    m_windowsId = 0;
    m_pointSize = qt_convertUnitsToPoints(m_size, m_units);
}

bool QPageSizePrivate::operator==(const QPageSizePrivate &other) const
{
    return m_size == other.m_size
           && m_units == other.m_units
           && m_key == other.m_key;
}

bool QPageSizePrivate::isEquivalentTo(const QPageSizePrivate &other) const
{
    return m_pointSize == other.m_pointSize;
}

bool QPageSizePrivate::isValid() const
{
    return m_pointSize.isValid() && !m_key.isEmpty();
}

QSizeF QPageSizePrivate::size(QPageSize::Unit units) const
{
    // If want units we've stored in, we already have them
    if (units == m_units)
        return m_size;

    // If want points we already have them
    if (units == QPageSize::Point)
        return QSizeF(m_pointSize.width(), m_pointSize.height());

    // If a custom size do a conversion
    if (m_id == QPageSize::Custom)
        return qt_convertUnits(m_size, m_units, units);

    // Otherwise use the standard sizes
    return qt_unitSize(m_id, units);
}

QSize QPageSizePrivate::sizePixels(int resolution) const
{
    return qt_convertPointsToPixels(m_pointSize, resolution);;
}


/*!
    \class QPageSize
    \inmodule QtGui
    \since 5.3
    \brief The QPageSize class describes the size and name of a defined page size

    This class implements support for the set of standard page sizes as defined
    in the Adobe Postscript PPD Standard v4.3. It defines the standard set of
    page sizes in points, millimeters and inches and ensures these sizes are
    consistently used. Other size units can be used but will be calculated
    results and so may not always be consistent. The defined point sizes are
    always a integer, all other sizes can be fractions of a unit.

    The defined size is always in width x height order with no implied page
    orientation. Note that it is possible for page sizes to be defined where the
    width is greater than the height, such as QPageSize::Ledger, so you cannot
    rely on comparing the width and height values to determine page orientation.

    For example, A4 is defined by the standard as 210mm x 297mm, 8.27in x 11.69in,
    or 595pt x 842pt.

    You can also define custom page sizes with custom names in any units you want
    and this unit size will be preserved and used as the base for all other unit
    size calculations.

    When creating a QPageSize using a custom QSize you can choose if you want
    QPageSize to try match the size to a standard page size. By default
    QPaperSize uses a FuzzyMatch mode where it will match a given page size to
    a standard page size if it falls within 3 postscript points of a defined
    standard size. You can override this to request only an exact match but this
    is not recommended as conversions between units can easily lose 3 points and
    result in incorrect page sizes.

    A QPageSize instance may also be obtained by querying the supported page sizes
    for a print device. In this case the localized name returned is that defined
    by the printer itself. Note that the print device may not support the current
    default locale language.

    The class also provides convenience methods for converting page size IDs to and from
    various unit sizes.

    \sa QPagedPaintDevice, QPdfWriter
*/

/*!
    \enum QPageSize::PageSizeId

    This enum type lists the available page sizes as defined in the Postscript
    PPD standard. These values are duplicated in QPagedPaintDevice and QPrinter.

    The defined sizes are:

    \value A0 841 x 1189 mm
    \value A1 594 x 841 mm
    \value A2 420 x 594 mm
    \value A3 297 x 420 mm
    \value A4 210 x 297 mm, 8.26 x 11.69 inches
    \value A5 148 x 210 mm
    \value A6 105 x 148 mm
    \value A7 74 x 105 mm
    \value A8 52 x 74 mm
    \value A9 37 x 52 mm
    \value B0 1000 x 1414 mm
    \value B1 707 x 1000 mm
    \value B2 500 x 707 mm
    \value B3 353 x 500 mm
    \value B4 250 x 353 mm
    \value B5 176 x 250 mm, 6.93 x 9.84 inches
    \value B6 125 x 176 mm
    \value B7 88 x 125 mm
    \value B8 62 x 88 mm
    \value B9 44 x 62 mm
    \value B10 31 x 44 mm
    \value C5E 163 x 229 mm
    \value Comm10E 105 x 241 mm, U.S. Common 10 Envelope
    \value DLE 110 x 220 mm
    \value Executive 7.5 x 10 inches, 190.5 x 254 mm
    \value Folio 210 x 330 mm
    \value Ledger 431.8 x 279.4 mm
    \value Legal 8.5 x 14 inches, 215.9 x 355.6 mm
    \value Letter 8.5 x 11 inches, 215.9 x 279.4 mm
    \value Tabloid 279.4 x 431.8 mm
    \value Custom Unknown, or a user defined size.
    \value A10
    \value A3Extra
    \value A4Extra
    \value A4Plus
    \value A4Small
    \value A5Extra
    \value B5Extra
    \value JisB0
    \value JisB1
    \value JisB2
    \value JisB3
    \value JisB4
    \value JisB5
    \value JisB6,
    \value JisB7
    \value JisB8
    \value JisB9
    \value JisB10
    \value AnsiA = Letter
    \value AnsiB = Ledger
    \value AnsiC
    \value AnsiD
    \value AnsiE
    \value LegalExtra
    \value LetterExtra
    \value LetterPlus
    \value LetterSmall
    \value TabloidExtra
    \value ArchA
    \value ArchB
    \value ArchC
    \value ArchD
    \value ArchE
    \value Imperial7x9
    \value Imperial8x10
    \value Imperial9x11
    \value Imperial9x12
    \value Imperial10x11
    \value Imperial10x13
    \value Imperial10x14
    \value Imperial12x11
    \value Imperial15x11
    \value ExecutiveStandard
    \value Note
    \value Quarto
    \value Statement
    \value SuperA
    \value SuperB
    \value Postcard
    \value DoublePostcard
    \value Prc16K
    \value Prc32K
    \value Prc32KBig
    \value FanFoldUS
    \value FanFoldGerman
    \value FanFoldGermanLegal
    \value EnvelopeB4
    \value EnvelopeB5
    \value EnvelopeB6
    \value EnvelopeC0
    \value EnvelopeC1
    \value EnvelopeC2
    \value EnvelopeC3
    \value EnvelopeC4
    \value EnvelopeC5 = C5E
    \value EnvelopeC6
    \value EnvelopeC65
    \value EnvelopeC7
    \value EnvelopeDL = DLE
    \value Envelope9
    \value Envelope10 = Comm10E
    \value Envelope11
    \value Envelope12
    \value Envelope14
    \value EnvelopeMonarch
    \value EnvelopePersonal
    \value EnvelopeChou3
    \value EnvelopeChou4
    \value EnvelopeInvite
    \value EnvelopeItalian
    \value EnvelopeKaku2
    \value EnvelopeKaku3
    \value EnvelopePrc1
    \value EnvelopePrc2
    \value EnvelopePrc3
    \value EnvelopePrc4
    \value EnvelopePrc5
    \value EnvelopePrc6
    \value EnvelopePrc7
    \value EnvelopePrc8
    \value EnvelopePrc9
    \value EnvelopePrc10
    \value EnvelopeYou4
    \value LastPageSize = EnvelopeYou4
    \omitvalue NPageSize
    \omitvalue NPaperSize

    Due to historic reasons QPageSize::Executive is not the same as the standard
    Postscript and Windows Executive size, use QPageSize::ExecutiveStandard instead.

    The Postscript standard size QPageSize::Folio is different to the Windows
    DMPAPER_FOLIO size, use the Postscript standard size QPageSize::FanFoldGermanLegal
    if needed.
*/

/*!
    \enum QPageSize::Unit

    This enum type is used to specify the measurement unit for page sizes.

    \value Millimeter
    \value Point  1/72th of an inch
    \value Inch
    \value Pica  1/72th of a foot, 1/6th of an inch, 12 Points
    \value Didot  1/72th of a French inch, 0.375 mm
    \value Cicero  1/6th of a French inch, 12 Didot, 4.5mm
*/

/*!
    \enum QPageSize::SizeMatchPolicy

    \value FuzzyMatch  Match to a standard page size if within the margin of tolerance.
    \value FuzzyOrientationMatch  Match to a standard page size if within the margin of tolerance regardless of orientation.
    \value ExactMatch  Only match to a standard page size if the sizes match exactly.
*/

/*!
    Creates a QPageSize of the standard \a pageSize.

    If \a pageSize is QPageSize::Custom then the resulting QPageSize will not
    be valid. Use the custom size constructor instead.
*/

QPageSize::QPageSize(PageSizeId pageSize)
    : d(new QPageSizePrivate(pageSize))
{
}

/*!
    Destroys the page.
*/

QPageSize::~QPageSize()
{
}

/*!
    \fn void QPageSize::swap(QPageSize &other)

    Swaps this QPageSize with \a other. This function is very fast and
    never fails.
*/

/*!
    \fn QPageSize &QPageSize::operator=(QPageSize &&other)

    Move-assigns \a other to this QPageSize instance, transferring the
    ownership of the managed pointer to this instance.
*/

/*!
    Returns \c true if this page size is valid.

    The page size may be invalid if created with an invalid PageSizeId, or a
    negative or invalid QSize or QSizeF, or the null constructor.
*/

bool QPageSize::isValid() const
{
    return d && d->isValid();
}

/*!
    Returns the size of the page in the required \a units.

    If the QPageSize is invalid then the QSizeF will be invalid.
*/

QSizeF QPageSize::size(Unit units) const
{
    return isValid() ? d->size(units) : QSize();
}

QT_END_NAMESPACE
