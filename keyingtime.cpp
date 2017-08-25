/*
 * Copyright 2017 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This program estimates the minimum time for inputting the given alphabet
 * string. While the estimation algorithm used in this program is different,
 * the basic idea of this sort of program is presented as key2time.cpp in
 * the ○ layout (4-698):
 *   http://www.geocities.jp/rage2050a/GeneKana/
 *
 * The keying time file to be specified with a command line argument can be
 * generated using html2txt.py from the keying time record pages for US
 * QWERTY keyboards:
 *   https://esrille.github.io/keyboard-layout-comparison/keytime.html
 *
 * Each line of a keying time file should be in the following format:
 *   [first character][second character] [time]
 * like,
 *   aa 145
 *   ab 32
 *   aL 58
 *   aR 40
 * L and R represent the space bar pressed by the left thumb and the right
 * thumb, respectively.
 *
 * An example keying time data is presented at:
 *   https://esrille.github.io/keyboard-layout-comparison/keytime.notepc.html
 */

#include <array>
#include <climits>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

class KeyingTime
{
    static const size_t MaxFingers = 10;
    static const std::array<const char*, MaxFingers> fingering;

    std::array<size_t, CHAR_MAX + 1> fingerMap;

    std::array<std::array<int32_t, CHAR_MAX + 1>, CHAR_MAX + 1> setup;   // Time to move a finger from c1 to c2.
    std::array<std::array<int32_t, CHAR_MAX + 1>, CHAR_MAX + 1> press;   // Time to press c2 after c1.

    // Previous characters and times typed by each finger:
    std::array<char, MaxFingers> previousCharacters;
    std::array<int32_t, MaxFingers> previousTimes;

    int32_t time;   // Current time
    char previous;  // The previous character; assume a white space typed by the left thumb

    // Returns the finger number for typing the character c.
    inline size_t fingerNumber(char c) {
        return fingerMap[c];
    }

public:
    KeyingTime() {
        for (auto &f : fingerMap) {
            f = 4;
        }
        for (size_t i = 0; i < MaxFingers; ++i) {
            const char* p{fingering[i]};
            while (*p) {
                fingerMap[*p++] = i;
            }
        }
    }

    bool initialize(const char* filename) {
        previousCharacters = { 'a', 's', 'd', 'f', 'L', 'R', 'j', 'k', 'l', ';' };
        previousTimes = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        time = 0;
        previous = 'l';

        for (auto &r : press) {
            for (auto &c : r) {
                c = 0;
            }
        }
        std::ifstream stream(filename);
        if (!stream) {
            std::cerr << "Error: Could not open '" << filename << "'." << std::endl;
            return false;
        }
        while (stream) {
            char c1, c2;
            int32_t time;
            stream >> c1 >> c2 >> time;
            press[c1][c2] = time;
        }

        for (auto &r : setup) {
            for (auto &c : r) {
                c = 0;
            }
        }
        for (const auto& s : fingering) {
            int n = strlen(s);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i == j) {
                        continue;
                    }
                    char c1{s[i]};
                    char c2{s[j]};
                    setup[c1][c2] = press[c1][c2] - press[c1][c1];
                    // std::cout << c1 << c2 << ": " << setup[c1][c2] << ' ' << press[c1][c2] << ' ' << press[c1][c1] << std::endl;
                    if (setup[c1][c2] <= 0) {
                        std::cerr << "Warning: The time for " << c1 << c2 << " is faster than " << c1 << c1 << ", which appears to be wrong." << std::endl;
                        setup[c1][c2] = 1;
                    }
                }
            }
        }
        return true;
    }

    KeyingTime& operator<<(char c) {
        if (c == ' ') { // Assume a white space is typed by the left thumb especially for the New Stickney Kana layout
            c = 'L';
        }
        if (press[previous][c] == 0) {
            return *this;   // Ignore c
        }
        size_t f{fingerNumber(c)};
        if (previous != previousCharacters[f]) {
            int32_t s{setup[previousCharacters[f]][c]};
            if (time < previousTimes[f] + s) {
                // std::cout << time << ": " << previous << c << "<-" << previousCharacters[f] << ' ' << (previousTimes[f] + s) << ' ' << press[previous][c] << std::endl;
                time = previousTimes[f] + s;
            }
        }
        time += press[previous][c];
        previousTimes[f] = time;
        previousCharacters[f] = c;
        previous = c;
        return *this;
    }

    KeyingTime& operator<<(const char* s) {
        while (*s) {
            (*this) << (*s++);
        }
        return *this;
    }

    int32_t currentTime() const {
        return time;
    }
};

const std::array<const char*, KeyingTime::MaxFingers> KeyingTime::fingering =
{
    "qaz",
    "wsx",
    "edc",
    "rfvtgb",
    "L",        // space pressed by the left thumb
    "R",        // space pressed by the right thumb
    "yhnujm",
    "ik,",
    "ol.",
    "p;/['"
};

void usage()
{
    std::cout <<
        "Usage: keyingtime keying_time_file [text ...]\n"
        "\n"
        "Estimate the minimum time for inputting the given text(s).\n"
        "With no text, read the standard input.\n"
        "Example: keyingtime keytime.notepc.txt \"hello, world\"" << std::endl;
}

int main(int argc, char* argv[])
{
    KeyingTime keyingTime;

    if (argc < 2 || !keyingTime.initialize(argv[1])) {
        usage();
        exit(1);
    }

    if (argc <= 2) {
        char c;
        while (std::cin.get(c)) {
            keyingTime << c;
        }
    } else {
        keyingTime << argv[2];
        for (int i = 3; i < argc; ++i) {
            keyingTime << ' ' << argv[i];
        }
    }
    std::cout << keyingTime.currentTime() << std::endl;
}
