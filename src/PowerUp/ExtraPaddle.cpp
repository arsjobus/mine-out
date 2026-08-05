#include "ExtraPaddle.h"

ExtraPaddle::ExtraPaddle(void) {
    loadDefaultSettings();
}

ExtraPaddle::~ExtraPaddle(void) { }

void ExtraPaddle::update(sf::Time dt) {
    PowerUp::update(dt);
}

void ExtraPaddle::loadDefaultSettings() {
    PowerUp::loadDefaultSettings();
    this->setLabel("power-up");
    this->setPowerID( PowerUp::TypeID::ExtraPaddle );
    this->setTexture( &resource.getPowerupTexture( 2 ) );
}

void ExtraPaddle::render(Window &window) {
    PowerUp::render(window);
}
