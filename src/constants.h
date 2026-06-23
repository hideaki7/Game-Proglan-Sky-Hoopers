#ifndef CONSTANTS_H
#define CONSTANTS_H

enum class GravityMode {
    Vertical,
    Horizontal
};

const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 720;

const int PLAYER_SIZE = 55;
const float PLAYER_MOVE_SPEED = 400.f;
const float PLAYER_JUMP_FORCE = -800.f;
const float PLAYER_ACCELERATION = 10.f;
const float PLAYER_DECELERATION = 15.f;

const float ROCKET_BOOST = 1000.f;

const float GRAVITY = 1200.f;
const float TERMINAL_VELOCITY = 1000.f;

const int PLATFORM_COUNT   = 15;
const float PLATFORM_WIDTH  = 100.f;
const float PLATFORM_HEIGHT = 20.f;
const float PLATFORM_SPAWN_Y_MIN = 50.f;
const float PLATFORM_SPAWN_Y_MAX = WINDOW_HEIGHT;

const float BUFF_DURATION = 3.0f;

#endif