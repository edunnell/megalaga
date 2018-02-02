#include <SDL2/SDL.h>

#define MAXX 640
#define MAXY 480

typedef struct {
  float x;
  float y;
} Vertex;

typedef struct {
  int r;
  int g;
  int b;
} Color;

typedef struct {
  Vertex top_left;
  Vertex bottom_right;
  Color color;
} Sector;

typedef struct {
  Sector sectors[4];
  int numSectors;
} Hero;

void drawSectors(SDL_Renderer * renderer, Sector sectors[], int numSectors) {
  int sector_i, x_i, y_i;
  int x, y;
  for(sector_i = 0; sector_i < numSectors; ++sector_i) {
    const Sector * currentSector = &sectors[sector_i];
    SDL_SetRenderDrawColor(renderer, currentSector->color.r, currentSector->color.g, currentSector->color.b, SDL_ALPHA_OPAQUE);
    for(y_i = currentSector->top_left.y; y_i >= currentSector->bottom_right.y; --y_i) {
      for(x_i = currentSector->top_left.x; x_i <= currentSector->bottom_right.x; ++x_i) {
        x = x_i + 320;
        y = 240 - y_i;
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }
  }
}

void drawHero(SDL_Renderer * renderer, Hero * hero) {
  drawSectors(renderer, hero->sectors, hero->numSectors);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window * window = SDL_CreateWindow("Megalaga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAXX, MAXY, SDL_WINDOW_OPENGL);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  Hero hero = {
               {
                {{-2, -174}, {2, -180}, {255, 255, 255}},
                {{-16, -180}, {16, -204}, {255, 255, 255}},
                {{-34, -186}, {-16, -204}, {255, 0, 0}},
                {{16, -186}, {34, -204}, {255, 0, 0}}
               },
               4
  };

  SDL_bool playing = SDL_TRUE;
  while(playing) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 40, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    drawHero(renderer, &hero);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
        playing = SDL_FALSE;
        break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
        case SDLK_ESCAPE:
          playing = SDL_FALSE;
          break;
        }
        break;
      }
    }
  }
  
  if(renderer)
    SDL_DestroyRenderer(renderer);
  if(window)
    SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
