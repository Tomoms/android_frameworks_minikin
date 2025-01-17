/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINIKIN_FONT_STYLE_H
#define MINIKIN_FONT_STYLE_H

#include <minikin/Buffer.h>

#include <ostream>

namespace minikin {

// FontStyle represents style information.
class FontStyle {
public:
    enum class Weight : uint16_t {
        THIN = 100,
        EXTRA_LIGHT = 200,
        LIGHT = 300,
        NORMAL = 400,
        MEDIUM = 500,
        SEMI_BOLD = 600,
        BOLD = 700,
        EXTRA_BOLD = 800,
        BLACK = 900,
        EXTRA_BLACK = 1000,
    };

    enum class Slant : bool {
        ITALIC = true,
        UPRIGHT = false,
    };

    constexpr FontStyle() : FontStyle(Weight::NORMAL, Slant::UPRIGHT) {}
    constexpr explicit FontStyle(Weight weight) : FontStyle(weight, Slant::UPRIGHT) {}
    constexpr explicit FontStyle(Slant slant) : FontStyle(Weight::NORMAL, slant) {}
    constexpr FontStyle(Weight weight, Slant slant)
            : FontStyle(static_cast<uint16_t>(weight), slant) {}
    constexpr FontStyle(uint16_t weight, Slant slant) : mWeight(weight), mSlant(slant) {}
    explicit FontStyle(BufferReader* reader) {
        mWeight = reader->read<uint16_t>();
        mSlant = static_cast<Slant>(reader->read<uint8_t>());
    }

    void writeTo(BufferWriter* writer) const {
        writer->write<uint16_t>(mWeight);
        writer->write<uint8_t>(static_cast<uint8_t>(mSlant));
    }

    constexpr uint16_t weight() const { return mWeight; }
    constexpr Slant slant() const { return mSlant; }

    constexpr uint32_t identifier() const {
        return (static_cast<uint32_t>(weight()) << 16) | static_cast<uint32_t>(slant());
    }

private:
    uint16_t mWeight;
    Slant mSlant;
};

inline std::ostream& operator<<(std::ostream& os, const FontStyle::Slant& slant) {
    switch (slant) {
        case FontStyle::Slant::ITALIC:
            return os << "italic";
        case FontStyle::Slant::UPRIGHT:
            return os << "upright";
        default:
            return os << "[UNKNOWN]";
    }
}

inline std::ostream& operator<<(std::ostream& os, const FontStyle& style) {
    return os << "{weight=" << style.weight() << ", slant=" << style.slant() << "}";
}

constexpr bool operator==(const FontStyle& l, const FontStyle& r) {
    return l.weight() == r.weight() && l.slant() == r.slant();
}

constexpr bool operator!=(const FontStyle& l, const FontStyle& r) {
    return !(l == r);
}

}  // namespace minikin

#endif  // MINIKIN_FONT_STYLE_H
