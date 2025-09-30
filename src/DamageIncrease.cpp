#include "DamageIncrease.h"

DamageIncrease::DamageIncrease(void)
{
	loadDefaultSettings();
}

DamageIncrease::~DamageIncrease(void)
{
}

void DamageIncrease::update()
{
	PowerUp::update();

	// Override PowerUp updates here:
}

void DamageIncrease::loadDefaultSettings()
{
	PowerUp::loadDefaultSettings();

	// Override PowerUp default settings here:
	this->setPowerID( PowerUp::TypeID::IncreasedDamage );

	this->setTexture( &resource.getPowerupTexture( 0 ) );
}

void DamageIncrease::render(Window &window)
{
	PowerUp::render(window);

	// Make additional renders here:
}