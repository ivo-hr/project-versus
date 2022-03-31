#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"

PlayingState::PlayingState(FightManager* game) : State(game) {

}


void PlayingState::update() {
	Uint32 startTime = fmngr->sdl->currRealTime();
	float step = 1.f / 60.f;
	fmngr->world.Step(step, 1, 1);

	fmngr->sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

	//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
	fmngr->background->render(fmngr->deathZone);
	fmngr->testura->render(fmngr->platformRect);
	fmngr->testura->render(fmngr->stageRect);
	//Dibujamos las cajas
	SDL_SetRenderDrawColor(fmngr->sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->stageRect);
	SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->platformRect);
	SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->deathZone);


	for (Particle* part : fmngr->particulas)
	{
		part->draw();
		part->update();
	}
	for (auto i = 0u; i < fmngr->entities.size(); i++)
	{
		fmngr->entities[i]->update();
	}
	for (Entity* ent : fmngr->entities)
	{
		ent->CheckHits();
	}
	for (int i = fmngr->entities.size() - 1; i >= 0; i--)
	{
		fmngr->entities[i]->draw();
	}

	while (fmngr->addedDelay > 0)
	{

		Uint32 startTime = fmngr->sdl->currRealTime();
		fmngr->addedDelay--;

		// clear screen
		fmngr->sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));
		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
		fmngr->background->render(fmngr->deathZone);
		fmngr->testura->render(fmngr->platformRect);
		fmngr->testura->render(fmngr->stageRect);
		//Dibujamos las cajas
		SDL_SetRenderDrawColor(fmngr->sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->stageRect);
		SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->platformRect);
		SDL_RenderDrawRect(fmngr->sdl->renderer(), &fmngr->deathZone);

		for (Particle* part : fmngr->particulas)
		{
			part->draw();
			part->update();
		}
		for (Entity* ent : fmngr->entities)
		{
			ent->updateParticles();
		}
		for (int i = fmngr->entities.size() - 1; i >= 0; i--)
		{
			fmngr->entities[i]->draw();
		}
		// present new frame
		fmngr->sdl->presentRenderer();

		double frameTime = fmngr->sdl->currRealTime() - startTime;

		if (frameTime < (step * 1000))
		{
			SDL_Delay((step * 1000));
		}
	}

	// present new frame
	fmngr->sdl->presentRenderer();

	double frameTime = fmngr->sdl->currRealTime() - startTime;

	if (frameTime < (step * 1000))
	{
		SDL_Delay((step * 1000));
	}

	fmngr->addedDelay = 0;
}

void PlayingState::draw() {

}

void PlayingState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new GameOverState(fmngr));
    delete this;
}
