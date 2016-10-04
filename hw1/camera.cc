#include "camera.h"

using namespace Raytra;

Camera::Camera(float x, float y, float z,
               float vx, float vy, float vz,
               float d, float iw, float ih, float pw, float ph)
{
    eye = {.x = x, .y = y, .z = z};

    vector direction = {.x = vx, .y = vy, .z = vz};
    direction  = norm(direction);
    vector up = {.x = 0, .y = 1, .z = 0};

    w = -(direction);
    u = cross(direction, up);
    v = cross(u, direction);

    focal_length = d;
    image_plane = {.width = iw, .height = ih};
    pixels_size = {.width = pw, .height = ph};
}
