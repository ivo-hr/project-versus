// This file is part of the course TPV2@UCM - Samir Genaim

#include "Texture.h"
#include "../utils/CheckML.h"
Texture::Texture()
{
	texture_ = nullptr;
	renderer_ = nullptr;
	width_ = 0;
	height_ = 0;
}

Texture& Texture::operator=(Texture &&other) noexcept {
	this->~Texture();
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;

	return *this;
}

Texture::Texture(Texture &&other) noexcept {
	texture_ = other.texture_;
	other.texture_ = nullptr;
	renderer_ = other.renderer_;
	other.renderer_ = nullptr;
	width_ = other.width_;
	height_ = other.height_;
}

Texture::Texture(SDL_Renderer *renderer, const std::string &fileName) {
	assert(renderer != nullptr);

	SDL_Surface *surface = IMG_Load(fileName.c_str());
	if (surface == nullptr)
		throw "Couldn't load image: " + fileName;

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture_ == nullptr) {
		SDL_FreeSurface(surface);
		throw "Couldn't load image: " + fileName;
	}

	width_ = surface->w;
	height_ = surface->h;
	renderer_ = renderer;

	SDL_FreeSurface(surface);

}

Texture::Texture(SDL_Renderer* renderer, SDL_Surface* surface) {
	assert(renderer != nullptr);

	if (surface == nullptr)
		throw "Surface not valid";

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture_ == nullptr) {
		throw "Surface not valid";
	}

	width_ = surface->w;
	height_ = surface->h;
	renderer_ = renderer;
}

Texture::Texture(SDL_Renderer* renderer, SDL_Window* window)
{
	assert(renderer != nullptr);

	SDL_UpdateWindowSurface(window);
	auto surface = SDL_GetWindowSurface(window);
	SDL_LockSurface(surface);
	SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);

	if (surface == nullptr)
		throw "Surface not valid";

	texture_ = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture_ == nullptr) {
		SDL_FreeSurface(surface);
		throw "Surface not valid";
	}

	width_ = surface->w;
	height_ = surface->h;
	renderer_ = renderer;

	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
}

Texture::Texture(SDL_Renderer *renderer, const std::string &text,
		const Font &font, const SDL_Color &fgColor) {
	constructFromText(renderer, text, font, &fgColor);
}

Texture::Texture(SDL_Renderer *renderer, const std::string &text,
		const Font &font, const SDL_Color &fgColor, const SDL_Color &bgColor) {
	constructFromText(renderer, text, font, &fgColor, &bgColor);
}

void Texture::constructFromText(SDL_Renderer *renderer, const std::string &text,
		const Font &font, const SDL_Color *fgColor, const SDL_Color *bgColor) {
	assert(renderer != nullptr);

	SDL_Surface *textSurface =
			bgColor == nullptr ?
					font.renderText(text, *fgColor) :
					font.renderText(text, *fgColor, *bgColor);

	if (textSurface == nullptr)
		throw "Couldn't create text: " + text;

	texture_ = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (texture_ == nullptr) {
		SDL_FreeSurface(textSurface);
		throw "Couldn't create text: " + text;
	}

	width_ = textSurface->w;
	height_ = textSurface->h;
	renderer_ = renderer;

	SDL_FreeSurface(textSurface);

}
