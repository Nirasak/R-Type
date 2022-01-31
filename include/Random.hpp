#pragma once

namespace tools {
    /**
     *  Generate a random number between 0 and max
     *
     *  @param max limit of the generated number
     *  @return the generated random number
     */
    inline int random(int max) {
        return rand() % max;
    }
}