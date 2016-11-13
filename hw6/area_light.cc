#include "area_light.h"

AreaLight::AreaLight (
        float x, float y, float z,      // position
        float nx, float ny, float nz,   // normal
        float ux, float uy, float uz,   // u-axis
        float len,                       // len of each side
        float r, float g, float b       // exitance
)
{
    c = {.red = r, .blue = b, .green = g};
    side_length = len;
    position = {x, y, z};
    normal = {nx, ny, nz};
    U = {ux, uy, uz};
    V = norm(cross(U, normal));
}

/**
 *
 * @param i -  x coordinate of strata
 * @param j -  y coordinate of strata
 * @param strata_count - number of strata the light is divided into
 * @return
 */
point AreaLight::get_point (
    float i,
    float j,
    unsigned int strata_count
)
{
    float x_disp = (get_rand() + i) / strata_count;
    float y_disp = (get_rand() + j) / strata_count;

    return (x_disp - 0.5f) * side_length * U +
           (y_disp - 0.5f) * side_length * V + position;
}

bool AreaLight::is_occluded_by (
   const Raytra::point &point_on_light,
   const Raytra::point &intersection_point,
   const std::vector<Surface *> &surfaces,
   const BVHTree *tree
)
{
    /* to avoid shadow rounding errors */
    const float surface_delta = 0.001;

    /* generate ray from point to light */
    vec light_dir = norm(point_on_light - intersection_point);
    Ray light_ray(intersection_point, light_dir);

    /* the t at which the light is located */
    float t_light = light_ray.offset(point_on_light);

    std::vector<unsigned int> surface_indices;
    tree->compute_intersections(light_ray, surface_indices);

    for (auto i: surface_indices) {
        float t = surfaces[i]->get_intersection_point(light_ray);
        if (t > surface_delta && t < t_light && fabsf(t) > surface_delta)
                return true;
    }

    return false;
}