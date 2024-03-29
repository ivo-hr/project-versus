// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cassert>
#include <iostream>

#include "Font.h"

struct RGB
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

class Texture {
public:

	Texture();

	// cannot copy textures
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// can be moved
	Texture& operator=(Texture &&other) noexcept;
	Texture(Texture &&other) noexcept;

	// Construct from image
	Texture(SDL_Renderer *renderer, const std::string &fileName);

	// Construct from surface
	Texture(SDL_Renderer* renderer, SDL_Surface* surface);

	// Construct from window (a screenshot)
	Texture(SDL_Renderer* renderer, SDL_Window* window);

	// Construct from text
	Texture(SDL_Renderer *renderer, const std::string &text, const Font &font,
			const SDL_Color &fgColor);

	// Construct from text with background
	Texture(SDL_Renderer *renderer, const std::string &text, const Font &font,
			const SDL_Color &fgColor, const SDL_Color &bgColor);


	virtual ~Texture() {
		if (texture_ != nullptr)
			SDL_DestroyTexture(texture_); // delete the SDL texture
	}

	inline int width() const {
		return width_;
	}

	inline int height() const {
		return height_;
	}

	// This rendering method corresponds to method SDL_RenderCopyEx.
	//
	// Renders part of the texture (src) to a destination rectangle (dest)
	// with a rotation (angle) around point p (of dest), and flips
	// according to the value of flip. If 'p'is nullptr, the rotation is done
	// wrt. the center
	//
	inline void render(const SDL_Rect &src, const SDL_Rect &dest, double angle,
			const SDL_Point *p = nullptr,
			SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) const {
		assert(texture_ != nullptr);
		SDL_RenderCopyEx(renderer_, texture_, &src, &dest, angle, p, flip);
	}

	// This rendering method corresponds to method SDL_RenderCopy.
	//
	// Renders part of the texture (src) to a destination rectangle (dest).
	// It can be implemented by calling the previous render method as well,
	// but we use SDL_RenderCopy directly since it does less checks so it
	// saves some checks ...
	inline void render(const SDL_Rect &src, const SDL_Rect &dest) const {
		assert(texture_ != nullptr);
		SDL_RenderCopy(renderer_, texture_, &src, &dest);
	}

	// render the complete texture at position (x,y).
	inline void render(int x, int y) const {
		SDL_Rect dest = { x, y, width_, height_ };
		render(dest);
	}

	// renders the complete texture at a destination rectangle (dest)
	inline void render(const SDL_Rect &dest) const {
		SDL_Rect src = { 0, 0, width_, height_ };
		render(src, dest);
	}

	// renders the complete texture at a destination rectangle (dest),
	// with rotation
	inline void render(const SDL_Rect &dest, float rotation) const {
		SDL_Rect src = { 0, 0, width_, height_ };
		render(src, dest, rotation);
	}

	// renders the complete texture at a destination rectangle (dest),
	// with rotation
	inline SDL_Texture* GetSDLTex() const {
		return texture_;
	}

	inline void GetTexMod(Uint8& r, Uint8& g, Uint8& b) const {
		SDL_GetTextureColorMod(texture_, &r, &g, &b);
	}

	inline void SetTexMod(Uint8 r, Uint8 g, Uint8 b) const {
		SDL_SetTextureColorMod(texture_, r, g, b);
	}

	inline void SetTexMod(Uint8 rgb) const {
		SDL_SetTextureColorMod(texture_, rgb, rgb, rgb);
	}

private:

	// Construct from text
	void constructFromText(SDL_Renderer *renderer, const std::string &text,
			const Font &font, const SDL_Color *fgColor,
			const SDL_Color *bgColor = nullptr);

	SDL_Texture *texture_;
	SDL_Renderer *renderer_;
	int width_;
	int height_;
};
