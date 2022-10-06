//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_MAXSHADERATTRIBUTESCHECKER_H
#define OPENGL_MAXSHADERATTRIBUTESCHECKER_H


class opengl_status_checker {
public:
    opengl_status_checker() = delete;

    ~opengl_status_checker() = delete;

    static void check_max_shader_attributes();
};


#endif //OPENGL_MAXSHADERATTRIBUTESCHECKER_H
