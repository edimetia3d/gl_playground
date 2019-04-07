//
// Created by edimetia3d on 2019-03-26.
//

#ifndef GL_PLAYGROUND_MOVEONLYBASE_H
#define GL_PLAYGROUND_MOVEONLYBASE_H

class UnCopyable {
public:
    UnCopyable() = default;

    UnCopyable(const UnCopyable &) = delete;

    UnCopyable &operator=(const UnCopyable &) = delete;

    UnCopyable(UnCopyable &&) = default;

    UnCopyable &operator=(UnCopyable &&) = default;
};

#endif //GL_PLAYGROUND_MOVEONLYBASE_H
