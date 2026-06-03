#pragma once
#include "src/field.h"
#include <random>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

/*
  Доступные паттерны:
    0 — Коловрат (солнечное колесо, 8 изогнутых лучей)
    1 — Лицо человека (овальная голова, глаза, нос, рот)
    2 — Академгородок (здания среди сосен)
    3 — НГУ (буквы Н Г У)
    4 — Хищники сверху / Фотосинтезаторы снизу (разреженно)
 */

enum class PatternType {
    KOLOVRAT = 0,
    FACE,
    AKADEMGORODOK,
    NSU,
    PREDATOR_PHOTO_SPLIT,
    COUNT
};

inline const char* PatternName(PatternType p) {
    switch (p) {
        case PatternType::KOLOVRAT:             return "Коловрат";
        case PatternType::FACE:                 return "Лицо человека";
        case PatternType::AKADEMGORODOK:        return "Академгородок";
        case PatternType::NSU:                  return "НГУ";
        case PatternType::PREDATOR_PHOTO_SPLIT: return "Хищники / Фото";
        default: return "???";
    }
}

inline void addBac(Field& f, int x, int y, std::mt19937& rng,
                   BacteriumType type = BacteriumType::PHOTOSYNTHESIZER) {
    if (x < 0 || x >= f.get_size_W() || y < 0 || y >= f.get_size_H()) return;
    if (!f.get_cell(x, y).IsEmpty()) return;
    f.add_bac(x, y, f.getNextId(), type);
}

inline void addBacRandom(Field& f, int x, int y, std::mt19937& rng) {
    std::uniform_int_distribution<int> dist(0, 99);
    BacteriumType type = (dist(rng) < 15) ? BacteriumType::PREDATOR
                                          : BacteriumType::PHOTOSYNTHESIZER;
    addBac(f, x, y, rng, type);
}

inline void generateKolovrat(Field& f, std::mt19937& rng) {
    int W = f.get_size_W();
    int H = f.get_size_H();
    float cx = W / 2.0f;
    float cy = H / 2.0f;
    float maxR = std::min(W, H) * 0.42f;

    float innerR = maxR * 0.15f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            float dx = x - cx;
            float dy = y - cy;
            float r = std::sqrt(dx * dx + dy * dy);
            if (r <= innerR) {
                addBacRandom(f, x, y, rng);
            }
        }
    }

    float ringInner = maxR * 0.88f;
    float ringOuter = maxR * 0.95f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            float dx = x - cx;
            float dy = y - cy;
            float r = std::sqrt(dx * dx + dy * dy);
            if (r >= ringInner && r <= ringOuter) {
                addBacRandom(f, x, y, rng);
            }
        }
    }

    int numArms = 8;
    float armWidth = 5.0f;
    for (int arm = 0; arm < numArms; ++arm) {
        float baseAngle = arm * (2.0f * 3.14159265f / numArms);
        for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
            float r = innerR + t * (ringInner - innerR);
            float curvature = 0.8f; 
            float angle = baseAngle + curvature * t;

            float px = cx + r * std::cos(angle);
            float py = cy + r * std::sin(angle);

            for (int ox = -2; ox <= 2; ++ox) {
                for (int oy = -2; oy <= 2; ++oy) {
                    if (ox * ox + oy * oy <= 6) {
                        int ix = static_cast<int>(px + ox);
                        int iy = static_cast<int>(py + oy);
                        addBacRandom(f, ix, iy, rng);
                    }
                }
            }
        }
    }
}

inline void generateFace(Field& f, std::mt19937& rng) {
    int W = f.get_size_W();
    int H = f.get_size_H();
    float cx = W / 2.0f;
    float cy = H / 2.0f;

    float headRx = W * 0.3f;
    float headRy = H * 0.4f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            float dx = (x - cx) / headRx;
            float dy = (y - cy) / headRy;
            float d = dx * dx + dy * dy;
            if (d >= 0.92f && d <= 1.0f) {
                addBacRandom(f, x, y, rng);
            }
        }
    }

    float eyeLx = cx - headRx * 0.35f;
    float eyeY  = cy - headRy * 0.25f;
    float eyeR  = 12.0f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            float dx = x - eyeLx;
            float dy = y - eyeY;
            float r = std::sqrt(dx * dx + dy * dy);
            if (r <= eyeR) {
                addBacRandom(f, x, y, rng);
            }
        }
    }

    float eyeRx = cx + headRx * 0.35f;
    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            float dx = x - eyeRx;
            float dy = y - eyeY;
            float r = std::sqrt(dx * dx + dy * dy);
            if (r <= eyeR) {
                addBacRandom(f, x, y, rng);
            }
        }
    }

    for (int y = static_cast<int>(cy - headRy * 0.05f);
         y <= static_cast<int>(cy + headRy * 0.15f); ++y) {
        for (int ox = -2; ox <= 2; ++ox) {
            addBacRandom(f, static_cast<int>(cx) + ox, y, rng);
        }
    }

    float mouthY = cy + headRy * 0.35f;
    float mouthW = headRx * 0.45f;
    for (float t = -1.0f; t <= 1.0f; t += 0.005f) {
        float mx = cx + t * mouthW;
        float my = mouthY + t * t * 15.0f; 
        for (int ox = -2; ox <= 2; ++ox) {
            for (int oy = -1; oy <= 1; ++oy) {
                addBacRandom(f, static_cast<int>(mx) + ox,
                             static_cast<int>(my) + oy, rng);
            }
        }
    }

    float browY = eyeY - 18.0f;
    float browW = 18.0f;
    for (float t = -1.0f; t <= 1.0f; t += 0.02f) {
        float bx = eyeLx + t * browW;
        float by = browY - std::abs(t) * 4.0f;
        for (int ox = -1; ox <= 1; ++ox) {
            for (int oy = -1; oy <= 1; ++oy) {
                addBacRandom(f, static_cast<int>(bx) + ox,
                             static_cast<int>(by) + oy, rng);
            }
        }
    }
    for (float t = -1.0f; t <= 1.0f; t += 0.02f) {
        float bx = eyeRx + t * browW;
        float by = browY - std::abs(t) * 4.0f;
        for (int ox = -1; ox <= 1; ++ox) {
            for (int oy = -1; oy <= 1; ++oy) {
                addBacRandom(f, static_cast<int>(bx) + ox,
                             static_cast<int>(by) + oy, rng);
            }
        }
    }
}

inline void drawRect(Field& f, int x0, int y0, int w, int h, std::mt19937& rng,
                     BacteriumType type = BacteriumType::PHOTOSYNTHESIZER) {
    for (int x = x0; x < x0 + w; ++x) {
        for (int y = y0; y < y0 + h; ++y) {
            addBac(f, x, y, rng, type);
        }
    }
}

inline void drawTriangle(Field& f, int cx, int topY, int baseHalfW, int height,
                         std::mt19937& rng, BacteriumType type) {
    for (int row = 0; row < height; ++row) {
        float frac = static_cast<float>(row) / height;
        int halfW = static_cast<int>(frac * baseHalfW);
        int y = topY + row;
        for (int dx = -halfW; dx <= halfW; ++dx) {
            addBac(f, cx + dx, y, rng, type);
        }
    }
}

inline void generateAkademgorodok(Field& f, std::mt19937& rng) {
    int W = f.get_size_W();
    int H = f.get_size_H();

    for (int x = 0; x < W; ++x) {
        for (int y = H - 15; y < H; ++y) {
            if ((x + y) % 3 == 0)
                addBac(f, x, y, rng, BacteriumType::PHOTOSYNTHESIZER);
        }
    }

    drawRect(f, 100, 120, 56, 75, rng, BacteriumType::PREDATOR);
    drawTriangle(f, 128, 95, 35, 25, rng, BacteriumType::PREDATOR);
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 5; ++col) {
            int wx = 106 + col * 10;
            int wy = 130 + row * 13;
            for (int dx = 0; dx < 5; dx++) {
                for (int dy = 0; dy < 7; dy++) {
                    f.delete_bac(wx + dx, wy + dy);
                }
            }
        }
    }

    drawRect(f, 30, 150, 30, 45, rng, BacteriumType::PREDATOR);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int wx = 34 + col * 9;
            int wy = 155 + row * 13;
            for (int dx = 0; dx < 5; dx++)
                for (int dy = 0; dy < 7; dy++)
                    f.delete_bac(wx + dx, wy + dy);
        }
    }

    drawRect(f, 190, 140, 35, 55, rng, BacteriumType::PREDATOR);
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 3; ++col) {
            int wx = 195 + col * 10;
            int wy = 145 + row * 12;
            for (int dx = 0; dx < 5; dx++)
                for (int dy = 0; dy < 7; dy++)
                    f.delete_bac(wx + dx, wy + dy);
        }
    }

    drawTriangle(f,  15, 140, 10, 30, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 13, 170, 4, 12, rng, BacteriumType::PHOTOSYNTHESIZER); // ствол

    drawTriangle(f,  75, 130, 12, 35, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 73, 165, 4, 14, rng, BacteriumType::PHOTOSYNTHESIZER);

    drawTriangle(f,  85, 145, 8, 25, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 83, 170, 4, 10, rng, BacteriumType::PHOTOSYNTHESIZER);

    drawTriangle(f, 170, 135, 10, 30, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 168, 165, 4, 12, rng, BacteriumType::PHOTOSYNTHESIZER);

    drawTriangle(f, 240, 125, 12, 40, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 238, 165, 4, 15, rng, BacteriumType::PHOTOSYNTHESIZER);

    drawTriangle(f, 235, 150, 8, 20, rng, BacteriumType::PHOTOSYNTHESIZER);
    drawRect(f, 233, 170, 4, 10, rng, BacteriumType::PHOTOSYNTHESIZER);

    for (int x = 20; x < W - 20; ++x) {
        for (int y = H - 25; y < H - 15; ++y) {
            if ((x + y) % 4 == 0) continue; 
        }
    }
}


inline void drawBitmapChar(Field& f, int startX, int startY, int scale,
                           const uint8_t bitmap[7], std::mt19937& rng) {
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 5; ++col) {
            if (bitmap[row] & (1 << (4 - col))) {
                for (int dx = 0; dx < scale; ++dx) {
                    for (int dy = 0; dy < scale; ++dy) {
                        addBacRandom(f,
                            startX + col * scale + dx,
                            startY + row * scale + dy, rng);
                    }
                }
            }
        }
    }
}

inline void generateNSU(Field& f, std::mt19937& rng) {
    const uint8_t letterN[7] = {
        0b10001,
        0b10001,
        0b10001,
        0b11111,
        0b10001,
        0b10001,
        0b10001
    };

    const uint8_t letterG[7] = {
        0b11111,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000,
        0b10000
    };

    const uint8_t letterU[7] = {
        0b10001,
        0b10001,
        0b01010,
        0b00100,
        0b00100,
        0b01000,
        0b10000
    };

    int scale = 6;
    int charW = 5 * scale;
    int gap = 2 * scale;
    int totalW = 3 * charW + 2 * gap;
    int startX = (f.get_size_W() - totalW) / 2;
    int startY = (f.get_size_H() - 7 * scale) / 2;

    drawBitmapChar(f, startX, startY, scale, letterN, rng);
    drawBitmapChar(f, startX + charW + gap, startY, scale, letterG, rng);
    drawBitmapChar(f, startX + 2 * (charW + gap), startY, scale, letterU, rng);
}

inline void generatePredatorPhotoSplit(Field& f, std::mt19937& rng) {
    int W = f.get_size_W();
    int H = f.get_size_H();
    int midY = H / 2;

    for (int x = 0; x < W; x += 4) {
        for (int y = 0; y < midY; y += 4) {
            addBac(f, x, y, rng, BacteriumType::PREDATOR);
        }
    }

    for (int x = 2; x < W; x += 4) {
        for (int y = midY; y < H; y += 4) {
            addBac(f, x, y, rng, BacteriumType::PHOTOSYNTHESIZER);
        }
    }

    for (int x = 0; x < W; x += 2) {
        addBac(f, x, midY, rng, BacteriumType::PREDATOR);
        addBac(f, x + 1, midY, rng, BacteriumType::PHOTOSYNTHESIZER);
    }
}

inline void clearField(Field& f) {
    for (int x = 0; x < f.get_size_W(); ++x)
        for (int y = 0; y < f.get_size_H(); ++y)
            f.delete_bac(x, y);
}

inline void generatePattern(Field& f, PatternType pattern, std::mt19937& rng) {
    clearField(f);
    switch (pattern) {
        case PatternType::KOLOVRAT:
            generateKolovrat(f, rng);
            break;
        case PatternType::FACE:
            generateFace(f, rng);
            break;
        case PatternType::AKADEMGORODOK:
            generateAkademgorodok(f, rng);
            break;
        case PatternType::NSU:
            generateNSU(f, rng);
            break;
        case PatternType::PREDATOR_PHOTO_SPLIT:
            generatePredatorPhotoSplit(f, rng);
            break;
        default:
            break;
    }
}
