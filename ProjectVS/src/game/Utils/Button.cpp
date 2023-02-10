#include "Button.h"
#include "../../utils/CheckML.h"

bool Button::MouseOver()
{
	int mx = ih.getMousePos().first;
	int my = ih.getMousePos().second;

	return (mx > x) && (mx < x + w) && (my > y) && (my < y + h);
}

bool Button::MouseEnter()
{
	return mouseOver && !mouseOverRemember;
}

bool Button::MouseExit()
{
	return !mouseOver && mouseOverRemember;
}

Button::Button(Texture* t, int x, int y, int size, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :tex(t), x(x), y(y), pointers(p), hasHoverSprite(hasHoverOver), hasPressedSprite(hasPressed)
{
	w = size;
	h = size * tex->height() / tex->width();

	if (hasHoverSprite && hasPressedSprite)
	{
		defTex = { 0, 0, tex->width(), tex->height() / 3 };
		hoverTex = { 0, tex->height() / 3, tex->width(), tex->height() / 3 };
		pressTex = { 0, tex->height() * 2 / 3, tex->width(), tex->height() / 3 };
	}
	else
	{
		if (hasHoverSprite || hasPressedSprite)
		{
			defTex = { 0, 0, tex->width(), tex->height() / 2 };
			hasHoverSprite ?
				hoverTex = { 0, tex->height() / 2, tex->width(), tex->height() / 2 } :
				pressTex = { 0, tex->height() / 2, tex->width(), tex->height() / 2 };
		}
		else
		{
			defTex = { 0, 0, tex->width(), tex->height() };
		}
	}
	currentSprite = defTex;
}

Button::Button(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :tex(t), x(x), y(y), w(w), h(h), pointers(p), hasHoverSprite(hasHoverOver), hasPressedSprite(hasPressed)
{
	if (hasHoverSprite && hasPressedSprite)
	{
		defTex = { 0, 0, tex->width(), tex->height() / 3 };
		hoverTex = { 0, tex->height() / 3, tex->width(), tex->height() / 3 };
		pressTex = { 0, tex->height() * 2 / 3, tex->width(), tex->height() / 3 };
	}
	else
	{
		if (hasHoverSprite || hasPressedSprite)
		{
			defTex = { 0, 0, tex->width(), tex->height() / 2 };
			hasHoverSprite ?
				hoverTex = { 0, tex->height() / 2, tex->width(), tex->height() / 2 } :
				pressTex = { 0, tex->height() / 2, tex->width(), tex->height() / 2 };
		}
		else
		{
			defTex = { 0, 0, tex->width(), tex->height() };
		}
	}
	currentSprite = defTex;
}

void Button::SetActive(bool f)
{
	active = f;
	SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
}

void Button::update()
{
	if (rendered && active)
	{
		if (onMouseClick != nullptr)
		{
			bool toAct = false;
			if (ih.getMouseButtonState(ih.LEFT))
			{
				isClicking = true;
			}
			if (isClicking)
			{
				if (ih.mouseButtonEvent() && !ih.getMouseButtonState(ih.LEFT))
				{
					isClicking = false;
					toAct = true;
				}
			}
			mouseOver = MouseOver();
			if (mouseOver)
			{
				if (isClicking)
				{
					pressed = true;
					if (hasPressedSprite)
					{
						currentSprite = pressTex;
					}
					else
					{
						Uint8 r;
						tex->GetTexMod(r, r, r);
						if (r != pressedTint)
						{
							tex->SetTexMod(pressedTint);
						}
					}
				}
				else if (toAct)
				{
					onMouseClick();
					pressed = false;
					if (hasBeenDeleted)
						return;
				}
			}
			if (MouseEnter())
			{
				SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
				if (hasHoverSprite)
				{
					currentSprite = hoverTex;
				}
				else
				{
					Uint8 r;
					tex->GetTexMod(r, r, r);
					if (r != hoverOverTint)
					{
						tex->SetTexMod(hoverOverTint);
					}
				}
			}
			else if (MouseExit())
			{
				SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
				pressed = false;
				currentSprite = defTex;
				Uint8 r;
				tex->GetTexMod(r, r, r);
				if (r != defaultTint)
				{
					tex->SetTexMod(defaultTint);
				}
			}
		}

		if (validPointers && (onPointerClick != nullptr || onMouseClick != nullptr))
		{
			for (auto i = 0; i < pointers.size(); i++)
			{
				SDL_Rect r = {x, y, w, h};
				SDL_Rect p = pointers[i]->getRect();
				if (pointers[i]->Click() && SDL_HasIntersection(&r, &p))
				{
					if (onMouseClick != nullptr)
						onMouseClick();
					if (hasBeenDeleted)
						return;
					if (onPointerClick != nullptr)
						onPointerClick(i);
					if (hasBeenDeleted)
						return;
				}
			}
		}
	}
	else if (!active)
	{
		currentSprite = defTex;
		Uint8 r;
		tex->GetTexMod(r, r, r);
		if (r != defaultTint)
		{
			tex->SetTexMod(defaultTint);
		}
	}
	rendered = false;
	mouseOverRemember = mouseOver;
}

void Button::render()
{
	rendered = true;
	tex->render(currentSprite, { x,y,w,h });
}

SDL_Rect Button::getRect()
{
	SDL_Rect r = { x, y, w, h };
	return r;
}

PlayButton::PlayButton(Texture* t, int x, int y, int size, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :
	Button(t, x, y, size, p, hasHoverOver, hasPressed)
{
	for (auto i = 0u; i < 22; i++) {
		std::string key = "C" + std::to_string(i);
		std::string file = "resources/images/rompe_cristal/" + std::to_string(i) + ".png";
		sdl->images().emplace(key, Texture(sdl->renderer(), file));
		txV.push_back(&sdl->images().at(key));
	}
}

PlayButton::PlayButton(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :
	Button(t, x, y, w, h, p, hasHoverOver, hasPressed)
{
	for (auto i = 0u; i < 22; i++) {
		std::string key = "C" + std::to_string(i);
		std::string file = "resources/images/rompe_cristal/" + std::to_string(i) + ".png";
		sdl->images().emplace(key, Texture(sdl->renderer(), file));
		txV.push_back(&sdl->images().at(key));
	}
}

void PlayButton::render()
{
	rendered = true;
	if (frame < 21) {
		if (cont + 5u < SDL_GetTicks()) {
			cont = SDL_GetTicks();
			frame++;
		}
	}
	txV[frame]->render({ x, y, w, h });

}

ToggleButton::ToggleButton(Texture* t, int x, int y, int size, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :
	Button(t, x, y, size, p, hasHoverOver, hasPressed)
{
	defTex.w /= 2;
	hoverTex.w /= 2;
	pressTex.w /= 2;
	currentSprite = defTex;
}

ToggleButton::ToggleButton(Texture* t, int x, int y, int w, int h, std::vector<PlayerPointer*>& p, bool hasHoverOver, bool hasPressed) :
	Button(t, x, y, w, h, p, hasHoverOver, hasPressed)
{
	defTex.w /= 2;
	hoverTex.w /= 2;
	pressTex.w /= 2;
	currentSprite = defTex;
}

void ToggleButton::render()
{
	rendered = true;
	if (enabled)
		currentSprite.x = 0;
	else
		currentSprite.x = currentSprite.w;
	tex->render(currentSprite, { x,y,w,h });
}

void ToggleButton::SetEnabled(bool a)
{
	enabled = a;
}
