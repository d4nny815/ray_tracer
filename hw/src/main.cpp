#include <sycl/sycl.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <stdint.h>

#include "primatives.h"

using namespace sycl;

const size_t width = 1280, height = 720;

void render(queue& q, std::vector<uint8_t>& pixels) {
    buffer<uint8_t, 1> buf(pixels.data(), range<1>(pixels.size()));
    q.submit([&](handler& h) {
        auto acc = buf.get_access<access::mode::write>(h);
        h.parallel_for(range<1>(width * height), [=](id<1> i) {
            int index = i[0];

            RandLCG rng(index);

            float r = rng.next_float(0, 255);
            float g = rng.next_float(0, 255);
            float b = rng.next_float(0, 255);

            size_t base = index * 3;
            acc[base + 0] = static_cast<uint8_t>(r * 255.0f);
            acc[base + 1] = static_cast<uint8_t>(g * 255.0f);
            acc[base + 2] = static_cast<uint8_t>(b * 255.0f);
        });
    }).wait();
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL2\n";
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SYCL Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);

    std::vector<uint8_t> pixels(width * height * 3);

    queue q;
    try {
        q = queue{gpu_selector_v};
    } catch (const sycl::exception& e) {
        std::cout << "GPU not available. Falling back to CPU.\n";
        q = queue{cpu_selector_v};
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        render(q, pixels);

        SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 3);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
