/**
 * @file   colorTable.h
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   02/11/2021
 */

#pragma once

#include <string>
#include <array>
#include <unordered_map>

// constexpr int TableSize = 139; 

struct Color{
    int b;
    int g; 
    int r; 
};

class ColorTable{

public:
    // CONSTRUCTOR
    ColorTable() = default;

    // MEMBER FUNCTIONS
    void GenerateColorTable(int gridSize);
    std::string LookUpColor(int r, int g, int b);
    void GetRGB(std::string color_name, int &r, int &g, int &b);

private:
    // This is a color table with the format
    std::unordered_map<std::string, Color> table{
        {"maroon", Color{128,0,0}},  // 800000
        {"dark red", Color{139,0,0}},  // 8B0000
        {"brown", Color{165,42,42}},  // A52A2A
        {"firebrick", Color{178,34,34}},  // B22222
        {"crimson", Color{220,20,60}},  // DC143C
        {"red", Color{255,0,0}},  // FF0000
        {"tomato", Color{255,99,71}},  // FF6347
        {"coral", Color{255,127,80}},  // FF7F50
        {"indian red", Color{205,92,92}},  // CD5C5C
        {"light coral", Color{240,128,128}},  // F08080
        {"dark salmon", Color{233,150,122}},  // E9967A
        {"salmon", Color{250,128,114}},  // FA8072
        {"light salmon", Color{255,160,122}},  // FFA07A
        {"orange red", Color{255,69,0}},  // FF4500
        {"dark orange", Color{255,140,0}},  // FF8C00
        {"orange", Color{255,165,0}},  // FFA500
        {"gold", Color{255,215,0}},  // FFD700
        {"dark golden rod", Color{184,134,11}},  // B8860B
        {"golden rod", Color{218,165,32}},  // DAA520
        {"pale golden rod", Color{238,232,170}},  // EEE8AA
        {"dark khaki", Color{189,183,107}},  // BDB76B
        {"khaki", Color{240,230,140}},  // F0E68C
        {"olive", Color{128,128,0}},  // 808000
        {"yellow", Color{255,255,0}},  // FFFF00
        {"yellow green", Color{154,205,50}},  // 9ACD32
        {"dark olive green", Color{85,107,47}},  // 556B2F
        {"olive drab", Color{107,142,35}},  // 6B8E23
        {"lawn green", Color{124,252,0}},  // 7CFC00
        {"chart reuse", Color{127,255,0}},  // 7FFF00
        {"green yellow", Color{173,255,47}},  // ADFF2F
        {"dark green", Color{0,100,0}},  // 006400
        {"green", Color{0,128,0}},  // 008000
        {"forest green", Color{34,139,34}},  // 228B22
        {"lime", Color{0,255,0}},  // 00FF00
        {"lime green", Color{50,205,50}},  // 32CD32
        {"light green", Color{144,238,144}},  // 90EE90
        {"pale green", Color{152,251,152}},  // 98FB98
        {"dark sea green", Color{143,188,143}},  // 8FBC8F
        {"medium spring green", Color{0,250,154}},  // 00FA9A
        {"spring green", Color{0,255,127}},  // 00FF7F
        {"sea green", Color{46,139,87}},  // 2E8B57
        {"medium aqua marine", Color{102,205,170}},  // 66CDAA
        {"medium sea green", Color{60,179,113}},  // 3CB371
        {"light sea green", Color{32,178,170}},  // 20B2AA
        {"dark slate gray", Color{47,79,79}},  // 2F4F4F
        {"teal", Color{0,128,128}},  // 008080
        {"dark cyan", Color{0,139,139}},  // 008B8B
        {"aqua", Color{0,255,255}},  // 00FFFF
        {"cyan", Color{0,255,255}},  // 00FFFF
        {"light cyan", Color{224,255,255}},  // E0FFFF
        {"dark turquoise", Color{0,206,209}},  // 00CED1
        {"turquoise", Color{64,224,208}},  // 40E0D0
        {"medium turquoise", Color{72,209,204}},  // 48D1CC
        {"pale turquoise", Color{175,238,238}},  // AFEEEE
        {"aqua marine", Color{127,255,212}},  // 7FFFD4
        {"powder blue", Color{176,224,230}},  // B0E0E6
        {"cadet blue", Color{95,158,160}},  // 5F9EA0
        {"steel blue", Color{70,130,180}},  // 4682B4
        {"corn flower blue", Color{100,149,237}},  // 6495ED
        {"deep sky blue", Color{0,191,255}},  // 00BFFF
        {"dodger blue", Color{30,144,255}},  // 1E90FF
        {"light blue", Color{173,216,230}},  // ADD8E6
        {"sky blue", Color{135,206,235}},  // 87CEEB
        {"light sky blue", Color{135,206,250}},  // 87CEFA
        {"midnight blue", Color{25,25,112}},  // 191970
        {"navy", Color{0,0,128}},  // 000080
        {"dark blue", Color{0,0,139}},  // 00008B
        {"medium blue", Color{0,0,205}},  // 0000CD
        {"blue", Color{0,0,255}},  // 0000FF
        {"royal blue", Color{65,105,225}},  // 4169E1
        {"blue violet", Color{138,43,226}},  // 8A2BE2
        {"indigo", Color{75,0,130}},  // 4B0082
        {"dark slate blue", Color{72,61,139}},  // 483D8B
        {"slate blue", Color{106,90,205}},  // 6A5ACD
        {"medium slate blue", Color{123,104,238}},  // 7B68EE
        {"medium purple", Color{147,112,219}},  // 9370DB
        {"dark magenta", Color{139,0,139}},  // 8B008B
        {"dark violet", Color{148,0,211}},  // 9400D3
        {"dark orchid", Color{153,50,204}},  // 9932CC
        {"medium orchid", Color{186,85,211}},  // BA55D3
        {"purple", Color{128,0,128}},  // 800080
        {"thistle", Color{216,191,216}},  // D8BFD8
        {"plum", Color{221,160,221}},  // DDA0DD
        {"violet", Color{238,130,238}},  // EE82EE
        {"magenta / fuchsia", Color{255,0,255}},  // FF00FF
        {"orchid", Color{218,112,214}},  // DA70D6
        {"medium violet red", Color{199,21,133}},  // C71585
        {"pale violet red", Color{219,112,147}},  // DB7093
        {"deep pink", Color{255,20,147}},  // FF1493
        {"hot pink", Color{255,105,180}},  // FF69B4
        {"light pink", Color{255,182,193}},  // FFB6C1
        {"pink", Color{255,192,203}},  // FFC0CB
        {"antique white", Color{250,235,215}},  // FAEBD7
        {"beige", Color{245,245,220}},  // F5F5DC
        {"bisque", Color{255,228,196}},  // FFE4C4
        {"blanched almond", Color{255,235,205}},  // FFEBCD
        {"wheat", Color{245,222,179}},  // F5DEB3
        {"corn silk", Color{255,248,220}},  // FFF8DC
        {"lemon chiffon", Color{255,250,205}},  // FFFACD
        {"light golden rod yellow", Color{250,250,210}},  // FAFAD2
        {"light yellow", Color{255,255,224}},  // FFFFE0
        {"saddle brown", Color{139,69,19}},  // 8B4513
        {"sienna", Color{160,82,45}},  // A0522D
        {"chocolate", Color{210,105,30}},  // D2691E
        {"peru", Color{205,133,63}},  // CD853F
        {"sandy brown", Color{244,164,96}},  // F4A460
        {"burly wood", Color{222,184,135}},  // DEB887
        {"tan", Color{210,180,140}},  // D2B48C
        {"rosy brown", Color{188,143,143}},  // BC8F8F
        {"moccasin", Color{255,228,181}},  // FFE4B5
        {"navajo white", Color{255,222,173}},  // FFDEAD
        {"peach puff", Color{255,218,185}},  // FFDAB9
        {"misty rose", Color{255,228,225}},  // FFE4E1
        {"lavender blush", Color{255,240,245}},  // FFF0F5
        {"linen", Color{250,240,230}},  // FAF0E6
        {"old lace", Color{253,245,230}},  // FDF5E6
        {"papaya whip", Color{255,239,213}},  // FFEFD5
        {"sea shell", Color{255,245,238}},  // FFF5EE
        {"mint cream", Color{245,255,250}},  // F5FFFA
        {"slate gray", Color{112,128,144}},  // 708090
        {"light slate gray", Color{119,136,153}},  // 778899
        {"light steel blue", Color{176,196,222}},  // B0C4DE
        {"lavender", Color{230,230,250}},  // E6E6FA
        {"floral white", Color{255,250,240}},  // FFFAF0
        {"alice blue", Color{240,248,255}},  // F0F8FF
        {"ghost white", Color{248,248,255}},  // F8F8FF
        {"honeydew", Color{240,255,240}},  // F0FFF0
        {"ivory", Color{255,255,240}},  // FFFFF0
        {"azure", Color{240,255,255}},  // F0FFFF
        {"snow", Color{255,250,250}},  // FFFAFA
        {"dim grey", Color{105,105,105}},  // 696969
        {"grey", Color{128,128,128}},  // 808080
        {"dark grey", Color{169,169,169}},  // A9A9A9
        {"silver", Color{192,192,192}},  // C0C0C0
        {"light grey", Color{211,211,211}},  // D3D3D3
        {"gainsboro", Color{220,220,220}},  // DCDCDC
        {"white smoke", Color{245,245,245}},  // F5F5F5
        {"white", Color{255,255,255}},  // FFFFFF
        {"black", Color{0,0,0}}  // 000000
    };
};