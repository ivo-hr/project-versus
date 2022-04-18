#include "Button.h"

void Button::render()
{
	tex->render({ x,y,w,h });
}

bool Button::mouseClick()
{
   int mx = ih.getMousePos().first;
   int my = ih.getMousePos().second;

   if ((mx > x) && (mx < x + w) && (my > y) && (my < y + h))
   {
	   if (ih.getMouseButtonState(ih.LEFT)==1) {
		   pressed = true;
		   return true;
		   
		}
	    pressed = false;
	   
   }
	pressed = false;
	return false;
}

SDL_Rect Button::getRect()
{
	SDL_Rect r = build_sdlrect(x, y, w, h);
	return r;
}
