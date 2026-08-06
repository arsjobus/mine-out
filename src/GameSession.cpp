#include "GameSession.h"

GameSession& GameSession::instance() {
    static GameSession session;
    return session;
}

size_t GameSession::getLivesRemaining() const {
    return livesRemaining;
}

void GameSession::setLivesRemaining(size_t newLivesRemaining) {
    livesRemaining = newLivesRemaining;
}

void GameSession::reset() {
    livesRemaining = 3;
}
