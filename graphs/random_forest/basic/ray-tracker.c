#include <stdio.h>
#include <math.h>

// Structure for a 3D vector
typedef struct {
    float x, y, z;
} Vec3;

// Utility functions for vector operations
Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 multiply(Vec3 a, float scalar) {
    return (Vec3){a.x * scalar, a.y * scalar, a.z * scalar};
}

float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 normalize(Vec3 a) {
    float length = sqrt(dot(a, a));
    return multiply(a, 1.0f / length);
}

// Structure for a sphere
typedef struct {
    Vec3 center;
    float radius;
    Vec3 color;
} Sphere;

// Checks if a ray intersects with a sphere
int intersect(Sphere sphere, Vec3 origin, Vec3 direction, float *t) {
    Vec3 oc = subtract(origin, sphere.center);
    float b = 2.0f * dot(oc, direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * c;

    if (discriminant < 0) {
        return 0; // No intersection
    } else {
        *t = (-b - sqrt(discriminant)) / 2.0f;
        return 1;
    }
}

// Main ray tracing function
void render(int width, int height) {
    Vec3 camera = {0, 0, 0}; // Camera position
    Sphere spheres[] = {
        {{0, 0, -5}, 1, {1, 0, 0}}, // Red sphere
        {{2, 0, -6}, 1, {0, 1, 0}}, // Green sphere
        {{-2, 0, -4}, 1, {0, 0, 1}} // Blue sphere
    };

    int numSpheres = sizeof(spheres) / sizeof(Sphere);

    printf("P3\n%d %d\n255\n", width, height); // PPM image format header

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Convert pixel coordinate to normalized device coordinate
            float u = (float)x / (width - 1) * 2.0f - 1.0f;
            float v = (float)y / (height - 1) * 2.0f - 1.0f;

            Vec3 direction = normalize((Vec3){u, v, -1});
            Vec3 pixelColor = {0, 0, 0};

            for (int i = 0; i < numSpheres; i++) {
                float t;
                if (intersect(spheres[i], camera, direction, &t)) {
                    pixelColor = spheres[i].color; // Use sphere's color
                }
            }

            // Convert color to 0-255 range
            int r = (int)(pixelColor.x * 255);
            int g = (int)(pixelColor.y * 255);
            int b = (int)(pixelColor.z * 255);

            printf("%d %d %d ", r, g, b);
        }
        printf("\n");
    }
}

int main() {
    int width = 800;
    int height = 600;

    render(width, height);
    return 0;
}
