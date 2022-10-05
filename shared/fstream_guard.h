//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_FSTREAM_GUARD_H
#define OPENGL_FSTREAM_GUARD_H

#include <fstream>

struct fstream_guard {
    std::fstream fstr;

    std::string operator()(const char *file_path) {
        fstr.open(file_path, std::ios::in);

        std::string tmp;
        std::string token;
        while (std::getline(fstr, token)) {
            tmp.append(token + '\n');
        }

        return tmp;
    }

    ~fstream_guard() {
        fstr.close();
    }
};

#endif //OPENGL_FSTREAM_GUARD_H
