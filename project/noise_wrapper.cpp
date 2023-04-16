#include "noise_wrapper.h"
#include <noise/noise.h>

using namespace noise;

extern "C" float noise2D(float x, float z) {
    return static_cast<float>(valley_noise(x, z));
}



double valley_noise(double x, double z) {
    noise::module::Perlin perlin_noise;

    // Customize the noise properties
    perlin_noise.SetOctaveCount(4);  // Increase octave count for more detail
    perlin_noise.SetFrequency(0.01); // Decrease frequency for larger features
    perlin_noise.SetPersistence(0.5); // Decrease persistence for smoother transitions

    // Combine multiple layers of noise
    double elevation = perlin_noise.GetValue(x, z, 0.0);

    // Create a valley-like shape by squaring the elevation
    elevation = elevation * elevation;

    return elevation;
}
