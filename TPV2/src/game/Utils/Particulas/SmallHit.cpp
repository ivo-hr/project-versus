#include "SmallHit.h"

SmallHit::SmallHit(Entity* ent, Vector2D* position): Particle(ent, position, 0, 75, 75)
{
	texture = &ent->GetManager()->GetSDLU()->images().at("sHitParticle");

	numSpritesinX = 2;
	numSpritesinY = 3;
	numSprites = 5;
	duration = 15;

	CalcularResto();
}
