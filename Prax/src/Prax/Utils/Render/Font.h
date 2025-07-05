#pragma once

#include <map>
#include <string>
#include <imgui.h>
#include <Prax/Utils/Render/Resource.h>


LOAD_RESOURCE(fonts_inter_Inter_Medium_ttf)
LOAD_RESOURCE(fonts_inter_Inter_Bold_ttf)
LOAD_RESOURCE(fonts_inter_Inter_ExtraLight_ttf)
LOAD_RESOURCE(fonts_inter_Inter_Light_ttf)
LOAD_RESOURCE(fonts_inter_Inter_Regular_ttf)
LOAD_RESOURCE(fonts_inter_Inter_SemiBold_ttf)
LOAD_RESOURCE(fonts_inter_Sarabun_Light_ttf)
LOAD_RESOURCE(fonts_jetbrains_mono_JetBrainsMono_Regular_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_Bold_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_DemiBold_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraBoldOblique_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraLightOblique_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_ExtraLight_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_Light_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_LightOblique_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_Medium_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_Regular_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_Thin_ttf)
LOAD_RESOURCE(fonts_greycliff_cf_GreycliffCF_ThinOblique_ttf)
LOAD_RESOURCE(fonts_icons_icons_v1_ttf)
enum IconFont {
    MagnifyingGlass = 'a',
    MusicNote = 'b',
    LocationPing = 'c',
    BellOn = 'd',
    BellOff = 'e',
    HTMLMarkup = 'f',
    Like = 'g',
    Dislike = 'h',
    ClosedCircle = 'i',
    Checkmark = 'j',
    Cross = 'k',
    Save = 'l',
    EyeClosed = 'm',
    EyeOpen = 'n',
    EditPencil = 'o',
    Person = 'p',
    Crosshair = 'q',
    Memory = 'r',
    PersonCircle = 's',
    Wheelchair = 't',
    SingleGear = 'u',
    Wrench = 'v',
    ManyGears = 'w',
    Config = 'x',
    QuestionMark = 'y',
    Flag = 'z',
    Trashcan = '0',
    FolderClosedFill = '1',
    FolderOpenFill = '2',
    FolderClosed = '3',
    FolderOpen = '4',
    Signal = '5',
    Earth = '6',
    Server = '7',
    ChevronLeft = '8',
    ChevronRight = '9',
    ChevronUp = 'A',
    ChevronUpThick = 'B',
    ChevronRightThick = 'C',
    ChevronLeftThick = 'D',
    ChevronDownThick = 'E',
    ArrowRight = 'F',
    ArrowLeft = 'G',
    ArrowUp = 'H',
    ArrowDown = 'I',
    Spinner = 'J',
    InfoI = 'K',
    GridSmall = 'L',
    GridLarge = 'M',
    Terminal = 'N',
    BanHammer = 'O',
    PaintBrush = 'P',
    Cloud = 'Q',
    LinkPaperclip = 'R',
    Signal2 = 'S',
    BulletedList = 'T',
    Column = 'U',
    GUI = 'V',
    Keyboard = 'W',
};

class Font {
public:
    static std::map<std::string, ImFont*> Fonts;

    static const char* IconFontToString(IconFont icon) {
        switch (icon) {
            case IconFont::MagnifyingGlass:
                return "a";
            case IconFont::MusicNote:
                return "b";
            case IconFont::LocationPing:
                return "c";
            case IconFont::BellOn:
                return "d";
            case IconFont::BellOff:
                return "e";
            case IconFont::HTMLMarkup:
                return "f";
            case IconFont::Like:
                return "g";
            case IconFont::Dislike:
                return "h";
            case IconFont::ClosedCircle:
                return "i";
            case IconFont::Checkmark:
                return "j";
            case IconFont::Cross:
                return "k";
            case IconFont::Save:
                return "l";
            case IconFont::EyeClosed:
                return "m";
            case IconFont::EyeOpen:
                return "n";
            case IconFont::EditPencil:
                return "o";
            case IconFont::Person:
                return "p";
            case IconFont::Crosshair:
                return "q";
            case IconFont::Memory:
                return "r";
            case IconFont::PersonCircle:
                return "s";
            case IconFont::Wheelchair:
                return "t";
            case IconFont::SingleGear:
                return "u";
            case IconFont::Wrench:
                return "v";
            case IconFont::ManyGears:
                return "w";
            case IconFont::Config:
                return "x";
            case IconFont::QuestionMark:
                return "y";
            case IconFont::Flag:
                return "z";
            case IconFont::Trashcan:
                return "0";
            case IconFont::FolderClosedFill:
                return "1";
            case IconFont::FolderOpenFill:
                return "2";
            case IconFont::FolderClosed:
                return "3";
            case IconFont::FolderOpen:
                return "4";
            case IconFont::Signal:
                return "5";
            case IconFont::Earth:
                return "6";
            case IconFont::Server:
                return "7";
            case IconFont::ChevronLeft:
                return "8";
            case IconFont::ChevronRight:
                return "9";
            case IconFont::ChevronUp:
                return "A";
            case IconFont::ChevronUpThick:
                return "B";
            case IconFont::ChevronRightThick:
                return "C";
            case IconFont::ChevronLeftThick:
                return "D";
            case IconFont::ChevronDownThick:
                return "E";
            case IconFont::ArrowRight:
                return "F";
            case IconFont::ArrowLeft:
                return "G";
            case IconFont::ArrowUp:
                return "H";
            case IconFont::ArrowDown:
                return "I";
            case IconFont::Spinner:
                return "J";
            case IconFont::InfoI:
                return "K";
            case IconFont::GridSmall:
                return "L";
            case IconFont::GridLarge:
                return "M";
            case IconFont::Terminal:
                return "N";
            case IconFont::BanHammer:
                return "O";
            case IconFont::PaintBrush:
                return "P";
            case IconFont::Cloud:
                return "Q";
            case IconFont::LinkPaperclip:
                return "R";
            case IconFont::Signal2:
                return "S";
        }

        // Handle the default case, or return an error string if needed.
        return "Invalid IconFont";
    }
};