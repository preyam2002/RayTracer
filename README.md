# RayTracer

A C++17 ray tracing engine for generating photorealistic images with realistic lighting, materials, and camera effects.

## Overview

This is a complete implementation of a ray tracing engine based on the "Ray Tracing" book series by Peter Shirley. It renders 3D scenes with realistic lighting, various materials, and advanced camera effects like depth of field.

## Features

### Core Rendering
- **3D Vector Mathematics**: Complete vec3 implementation with operations
- **Ray-Object Intersection**: Accurate geometric calculations for spheres and quads
- **Recursive Ray Tracing**: Configurable depth for reflections and refractions
- **Anti-aliasing**: Multi-sampling per pixel for smooth edges

### Material System
- **Lambertian (Diffuse)**: Matte surfaces with cosine-weighted scattering
- **Metal**: Reflective surfaces with configurable fuzziness
- **Dielectric**: Glass-like materials with accurate refraction (Snell's law)
- **Diffuse Light**: Emissive materials for area lights

### Advanced Features
- **BVH Acceleration**: Bounding Volume Hierarchy for O(log n) ray-object intersection
- **Textures**: Image textures (via stb_image) and procedural textures
- **Perlin Noise**: Turbulence-based procedural textures
- **Quads**: Support for rectangular primitives (planes)
- **Constant Medium**: Fog and volumetric effects with ray marching
- **Transformations**: Translation and rotation of objects
- **Motion Blur**: Moving spheres with time-based interpolation
- **Defocus Blur**: Depth of field with camera aperture simulation

### Scene Types
The engine includes 11 built-in scene configurations:
1. **Simple**: Basic spheres demonstrating materials
2. **Spheres**: Random sphere scene (484 spheres)
3. **Checkered Spheres**: Procedural checker texture
4. **Earth**: Image texture mapping
5. **Perlin Spheres**: Noise-based procedural texture
6. **Quads**: Five aligned quads forming a box
7. **Simple Light**: Emissive materials demo
8. **Cornell Box**: Classic Cornell box with area lights
9. **Cornell Smoke**: Cornell box with volumetric fog
10. **Final Scene**: Complex scene (boxes, spheres, smoke, moving objects)

## Tech Stack

- **Language**: C++17
- **Output Format**: PPM (Portable Pixmap)
- **Image Loading**: stb_image (for textures)
- **Math**: Custom vec3, ray, and matrix implementations
- **Acceleration**: BVH (Bounding Volume Hierarchy)

## Project Structure

```
RayTracer/
├── src/
│   ├── main.cpp              # Entry point with scene selection
│   ├── vec3.h                # 3D vector math
│   ├── ray.h                 # Ray representation
│   ├── colour.h              # Color utilities
│   ├── interval.h            # Interval arithmetic
│   ├── camera.h              # Camera with DOF
│   ├── hittable.h            # Abstract hit interface
│   ├── hittables.h           # List of hittable objects
│   ├── sphere.h              # Sphere primitive
│   ├── quad.h                # Quad/plane primitive
│   ├── bvh_node.h            # BVH acceleration structure
│   ├── aabb.h                # Axis-aligned bounding boxes
│   ├── material.h            # Material types
│   ├── texture.h             # Texture system
│   ├── perlin.h              # Perlin noise generation
│   ├── image_loader.h        # Image texture loading
│   └── constant_medium.h     # Volumetric effects
├── worldmap.jpg              # Earth texture
├── CMakeLists.txt            # CMake build configuration
└── README.md
```

## Building

### Prerequisites
- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.10+

### Build Instructions

```bash
# Clone or navigate to repository
cd RayTracer

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make

# Or with multiple cores
make -j4
```

## Usage

The ray tracer outputs PPM image data to stdout. To save to a file:

```bash
# Build first
cd build && make

# Render to file
./RayTracer > render.ppm

# Or view directly
./RayTracer | display -
```

### Viewing PPM Files

```bash
# Convert to PNG with ImageMagick
convert render.ppm render.png

# Or with ImageMagick v7+
magick render.ppm render.png

# Open with GIMP, Photoshop, or any image viewer supporting PPM
```

## Scene Selection

Edit `src/main.cpp` and change the switch case (line 380) to select scenes:

```cpp
switch(1) {  // Change this number (1-11)
    case 1:  simple(); break;                    // Basic demo
    case 2:  spheres(false); break;              // Random spheres
    case 3:  checkered_spheres(); break;         // Checkered texture
    case 4:  earth(); break;                     // Earth texture
    case 5:  perlin_spheres(); break;            // Perlin noise
    case 6:  quads(); break;                     // Quad demo
    case 7:  simple_light(); break;              // Lights
    case 8:  cornell_box(); break;               // Cornell box
    case 9:  cornell_smoke(); break;             // Volumetrics
    case 10: final_scene(400, 250, 4); break;    // Complex (fast)
    case 11: final_scene(800, 10000, 40); break; // Complex (quality)
}
```

### Scene Parameters (final_scene)
```cpp
final_scene(image_width, samples_per_pixel, max_depth)
```

## Configuration

### Camera Settings

Each scene configures a camera with these parameters:

```cpp
camera cam;
cam.aspect_ratio      = 16.0 / 9.0;   // Image aspect ratio
cam.image_width       = 400;           // Output width in pixels
cam.samples_per_pixel = 100;           // Anti-aliasing samples
cam.max_depth         = 50;            // Ray bounce limit
cam.background        = colour(0.7, 0.8, 1.0);  // Sky color
cam.vfov              = 20;            // Vertical field of view (degrees)
cam.look_from         = point3(13, 2, 3);       // Camera position
cam.look_at           = point3(0, 0, 0);        // Look-at point
cam.vup               = vec3(0, 1, 0);          // Up vector
cam.defocus_angle     = 0.6;           // Aperture size (DOF)
cam.focus_dist        = 10.0;          // Focus distance
```

### Performance Tuning

Rendering time depends on:
- **Image resolution** (width × height)
- **Samples per pixel** (100 = 100 rays per pixel)
- **Max depth** (ray recursion limit)
- **Scene complexity** (number of objects)
- **BVH acceleration** (enabled by default)

For quick previews:
```cpp
cam.image_width       = 400;
cam.samples_per_pixel = 50;
cam.max_depth         = 10;
```

For production quality:
```cpp
cam.image_width       = 1920;
cam.samples_per_pixel = 500;
cam.max_depth         = 50;
```

## Example Renders

### Simple Scene
Basic demonstration of materials (diffuse, metal, glass).

### Random Spheres
484 spheres with randomized materials on a ground plane.

### Cornell Box
Classic reference scene with area lighting and accurate diffuse interreflection.

### Final Scene
Complex scene featuring:
- 400 moving boxes
- Glass spheres
- Metal spheres
- Image-textured sphere
- Noise-textured sphere
- Volumetric fog
- 1000 smaller spheres in motion

## Learning Resources

This implementation is based on the excellent "Ray Tracing" book series:

1. [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
   - Vectors, rays, spheres
   - Materials (diffuse, metal, dielectric)
   - Camera and positioning

2. [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
   - Motion blur
   - BVH acceleration
   - Textures (solid, checker, noise, image)
   - Quads and lights
   - Instances and volumes

3. [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
   - Monte Carlo integration
   - Importance sampling
   - PDFs for rendering

## Roadmap

- [ ] Multi-threading support (OpenMP/C++ threads)
- [ ] GPU acceleration (CUDA/Metal/OptiX)
- [ ] OBJ/FBX model loading
- [ ] Area lights with proper sampling
- [ ] Motion blur for all objects
- [ ] Subsurface scattering
- [ ] Spectral rendering
- [ ] HDR environment maps
- [ ] Denoising (OIDN)

## Performance Benchmarks

Approximate render times on modern hardware:

| Scene | Resolution | Samples | Time (Single-threaded) |
|-------|-----------|---------|----------------------|
| Simple | 400x225 | 100 | ~5 seconds |
| Spheres | 400x225 | 50 | ~30 seconds |
| Cornell | 1000x1000 | 400 | ~5 minutes |
| Final (fast) | 400x400 | 250 | ~2 minutes |
| Final (quality) | 800x800 | 10000 | ~2 hours |

*Times measured on M1 MacBook Pro*

## Author

**Preyam** - [GitHub](https://github.com/preyam2002)

## License

MIT

## Acknowledgments

- Peter Shirley for the "Ray Tracing" book series
- Sean Barrett for stb_image
- The graphics programming community
