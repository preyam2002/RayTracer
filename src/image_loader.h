//
// Created by Preyam Rao on 02/10/24.
//

#ifndef RAYTRACER_IMAGE_LOADER_H
#define RAYTRACER_IMAGE_LOADER_H

#include "rayTracer.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../external/stb_image.h"
class image_loader{
public:
    explicit image_loader() {}
    explicit image_loader(const std::string& image_filename){
        auto image_dir= getenv("IMAGE_DIR");
        bool loaded=false;
        if(image_dir == nullptr){
            loaded = load("../images/" + image_filename);
        }
        if(!loaded){
            loaded =load(std::string(image_dir) + "/" + image_filename);
        }
        if(!loaded){
            std::cerr<<"ERROR: Could not load file "<<image_filename<<"\n";
        }
    }
    bool load(const std::string& image_filename){
        auto n = bytes_per_pixel; // Dummy out parameter: original components per pixel
        fdata = stbi_loadf(image_filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if(fdata == nullptr){
            return false;
        }
        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();
        return true;
    }
    [[nodiscard]] const unsigned char* pixel_data(int x, int y) const {
        // Return the address of the three RGB bytes of the pixel at x,y. If there is no image
        // data, returns magenta.
        static unsigned char magenta[] = { 255, 0, 255 };
        if (bdata == nullptr) return magenta;

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bdata + y*bytes_per_scanline + x*bytes_per_pixel;
    }

    int get_height() const{
        return image_height;
    }

    int get_width() const{
        return image_width;
    }

private:
    int image_width;
    int image_height;
    int bytes_per_pixel=3;
    float *fdata = nullptr;
    unsigned char *bdata = nullptr;
    int bytes_per_scanline;

    void convert_to_bytes(){
        int total_bytes = image_width * image_height * bytes_per_pixel;
        bdata = new unsigned char[total_bytes];
        auto *bptr = bdata;
        auto *fptr = fdata;
        for (auto i=0; i < total_bytes; i++, fptr++, bptr++)
            *bptr = float_to_byte(*fptr);
    }

    static int clamp(int x, int low, int high) {
        // Return the value clamped to the range [low, high).
        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
    }

    static unsigned char float_to_byte(float value) {
        if (value <= 0.0)
            return 0;
        if (1.0 <= value)
            return 255;
        return static_cast<unsigned char>(256.0 * value);
    }
};
#endif //RAYTRACER_IMAGE_LOADER_H
