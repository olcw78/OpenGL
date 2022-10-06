//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_FSTREAM_GUARD_H
#define OPENGL_FSTREAM_GUARD_H

#include <fstream>
#include <algorithm>
#include <array>

struct fstream_guard {
    std::fstream fstr;
    std::array<const char *, 3> comment_prefixes{
            "//",
            "/*",
            "/**"
    };

    std::string operator()(const char *file_path) {
        fstr.clear();
        fstr.open(file_path, std::ios::in);

        std::string tmp;
        std::string token;
        while (std::getline(fstr, token)) {
            if (check_comment_line(token)) {
                continue;
            }

            tmp.append(token + '\n');
        }

        return tmp;
    }

    std::string operator()(std::string_view file_path) {
        fstr.clear();
        fstr.open(file_path, std::ios::in);

        std::string tmp;
        std::string token;
        while (std::getline(fstr, token)) {
            if (check_comment_line(token)) {
                continue;
            }

            tmp.append(token + '\n');
        }

        return tmp;
    }

    ~fstream_guard() {
        fstr.close();
    }

private:
    bool check_comment_line(std::string const &from) {
        return std::any_of(std::begin(comment_prefixes), std::end(comment_prefixes), [&from](const char *c) {
            return from.find(c) != std::string::npos;
        });
    }
};

#endif //OPENGL_FSTREAM_GUARD_H
