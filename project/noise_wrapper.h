#ifndef NOISE_WRAPPER_H
#define NOISE_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

float noise2D(float x, float z);
double valley_noise(double x, double z);

#ifdef __cplusplus
}
#endif

#endif // NOISE_WRAPPER_H