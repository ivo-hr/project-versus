#include "PlayerSelectRect.h"
#include "../../utils/CheckML.h"

PlayerSelectRect::PlayerSelectRect(Texture* b) :back(b), front(nullptr), gotInput(false)
{
	m = &sdl->images().at("Mando");
	k1= &sdl->images().at("k1");
	k2 = &sdl->images().at("k2");
	noInputBackg= &sdl->images().at("P0");
}

void PlayerSelectRect::render(int x, int y, int w, int h)
{
	if (gotInput)
		back->render({ x,y,w,h });
	else
		noInputBackg->render({ x,y,w,h });
	if (!gotInput) {
		if (tick + 1500 < SDL_GetTicks()) {	
			tick = SDL_GetTicks();
			if (cont == 0) {
				cont++;
			}
			else if (cont == 1)
			{
				cont++;
			}
			else if (cont == 2)
			{
				cont = 0;
			}
		}

		if (cont == 0) {
			m->render({ x + w / 4,y + h / 4,w / 2,w / 2 });
		}
		else if (cont == 1)
		{
			k1->render({ x + w / 4,y + h / 4,w / 2,w / 2 });
		}
		else if (cont == 2)
		{
			k2->render({ x + w / 4,y + h / 4,w / 2,w / 2 });
		}
		
	}
	else {
		if(front!=nullptr)front->render({ x + w / 4,y + h / 4,w / 2,w / 2 });
	}

}
