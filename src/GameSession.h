#pragma once

#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <cstddef>

class GameSession {
public:
    static GameSession& instance();

    size_t getLivesRemaining() const;
    void setLivesRemaining(size_t newLivesRemaining);
    void reset();

private:
    GameSession() = default;

    size_t livesRemaining = 3;
};

#endif
