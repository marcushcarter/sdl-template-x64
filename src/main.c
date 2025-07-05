#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define TEXT_LENGTH 512

char title[TEXT_LENGTH] = "kanoodle deckbuilding roguelike";
char version[TEXT_LENGTH] = "demo v0.0.0";

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Cursor* cursor;
bool running;

#define NUM_KEYS 256
Uint8 currKeyState[NUM_KEYS];
Uint8 prevKeyState[NUM_KEYS];
Uint32 currMouseState;
Uint32 prevMouseState;
float mousex, mousey;

#define VIRTUAL_WIDTH 1920
#define VIRTUAL_HEIGHT 1280

int windowW = VIRTUAL_WIDTH, windowH = VIRTUAL_HEIGHT;
float offsetX, offsetY, viewportW, viewportH;
float scale = 1.0f;

// CLASSES ====================================================================================================

// COMMON FUNCTIONS ====================================================================================================

#define point_box_collision(px, py, bx, by, bw, bh) (px >= bx && px <= bx + bw && py >= by && py <= by + bh)

float* floatarr(int num, ...) {
	va_list args;
	float* combined_array = (float*)malloc(num * sizeof(float));
	int count = 0;
	va_start(args, num);
	for (int i = 0; i < num; i++) combined_array[count++] = va_arg(args, double);
	va_end(args);

	return combined_array;
}

int* intarr(int num, ...) {
	va_list args;
	int* combined_array = (int*)malloc(num * sizeof(int));
	int count = 0;
	va_start(args, num);
	for (int i = 0; i < num; i++) combined_array[count++] = va_arg(args, int);
	va_end(args);

	return combined_array;
}

char* stringf(const char* format, ...) { //converts a data type to a string ("%d", x)
    static char output[128];
    va_list args;
    va_start(args, format);
    vsprintf(output, format, args);
    va_end(args);
    return output;
}

float clampf(float value, float min, float max) {
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

float clampi(int value, int min, int max) {
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

float sine_decay(float time, float frequency, float amplitude, float decay_rate) {
    return amplitude * sinf(2 * M_PI * frequency * time) * expf(-decay_rate * time);
}

float cosine_decay(float time, float frequency, float amplitude, float decay_rate) {
    return amplitude * cosf(2 * M_PI * frequency * time) * expf(-decay_rate * time);
}

// DELTA TIME FUNCTIONS ====================================================================================================

clock_t previous_time = 0;
float dt;

int frame_count = 0;
float fps_timer = 0.0f;
float fps = 0.0f;

float get_delta_time() {
    clock_t current_time = clock();
    float delta_time = (float)(current_time - previous_time) / CLOCKS_PER_SEC;
    previous_time = current_time;

    frame_count++;
    fps_timer += delta_time;

    if (fps_timer >= 1.0f) {
        fps = frame_count / fps_timer;
        frame_count = 0;
        fps_timer = 0.0f;
        // printf("FPS: %.2f\n", fps);
    }

    return delta_time;
}

void control_fps(float target_fps, bool limited) {
	float frame_duration = 1.0f / target_fps;
	clock_t now = clock();
	float elapsed = (float)(now - previous_time) / CLOCKS_PER_SEC;
	float remaining_time = frame_duration - elapsed;
	if (remaining_time > 0 && limited) {
		usleep((useconds_t)(remaining_time * 1000000.0f));
	}
}

// SETUP FUNCTIONS ====================================================================================================

typedef struct Textures {
    // SDL_Texture* texture;
} Textures; Textures texture;

typedef struct Fonts {
    TTF_Font* Default;
} Fonts; Fonts font;

bool load_textures() {

    // texture.event_marker = IMG_LoadTexture(renderer, "resources/textures/card-event.png");
    // SDL_SetTextureScaleMode(texture.event_marker, SDL_SCALEMODE_NEAREST);

    // font.Default= TTF_OpenFont("resources/fonts/balatro.ttf", 24);
    
	return true;
}

void setup() {

    const Uint8* sdlKeys = (Uint8*)SDL_GetKeyboardState(NULL);
    SDL_memcpy(currKeyState, sdlKeys, NUM_KEYS);
    SDL_memcpy(prevKeyState, sdlKeys, NUM_KEYS);

}

// UPDATE FUNCTIONS ====================================================================================================

void update_window_scale() {
    float aspactWindow = (float)windowW / (float)windowH;
    float aspectVirtual = (float)VIRTUAL_WIDTH / (float)VIRTUAL_HEIGHT;

    float scaleX = (float)windowW / VIRTUAL_WIDTH;
    float scaleY = (float)windowH / VIRTUAL_HEIGHT;
    scale = fminf(scaleX, scaleY);

    viewportW  = (int)(VIRTUAL_WIDTH * scale);
    viewportH = (int)(VIRTUAL_HEIGHT * scale);
    offsetX = (windowW - viewportW) / 2;
    offsetY = (windowH - viewportH) / 2;
}

void inputs() {
    
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_EVENT_QUIT) { 
            running = false; 
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            windowW = event.window.data1;
            windowH = event.window.data2;
            printf("%d x %d\n", windowW, windowH);
            update_window_scale();
        }
    }

    const Uint8* sdlKeys = (Uint8*)SDL_GetKeyboardState(NULL);
    SDL_memcpy(currKeyState, sdlKeys, NUM_KEYS);
    prevMouseState = currMouseState;
    currMouseState = SDL_GetMouseState(&mousex, &mousey);
    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);

    if (currKeyState[SDL_SCANCODE_ESCAPE] && !prevKeyState[SDL_SCANCODE_ESCAPE]) SDL_SetWindowFullscreen(window, 0);
    if (currKeyState[SDL_SCANCODE_TAB] && !prevKeyState[SDL_SCANCODE_TAB]) SDL_SetWindowFullscreen(window, 1);

}

void dev_tools() {

    if (currKeyState[SDL_SCANCODE_1] && !prevKeyState[SDL_SCANCODE_1]);

}

void update() {}

// PRIMITIVE RENDER FUNCTIONS ===================================================================================================

// LOOP RENDER FUNCTIONS ===================================================================================================

void render() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_FRect viewport = {
        offsetX,
        offsetY,
        viewportW,
        viewportH,
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &viewport);
    
    SDL_SetCursor(cursor);
}

// MAIN FUNCTION ====================================================================================================

int main() {
    srand(0);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow(stringf("%s %s", title, version), (VIRTUAL_WIDTH/2), (VIRTUAL_HEIGHT/2), SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    renderer = SDL_CreateRenderer(window, NULL);
	SDL_SetRenderScale(renderer, 1, 1);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    update_window_scale();
    setup();
    load_textures();

    running = 1;
    while (running) {

        dt = get_delta_time();

        inputs();
        dev_tools();
        update();

        render();
        SDL_RenderPresent(renderer);

        SDL_memcpy(prevKeyState, currKeyState, NUM_KEYS);
        control_fps(120.0f, false);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
