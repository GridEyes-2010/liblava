// file      : liblava/asset/scope_image.cpp
// copyright : Copyright (c) 2018-present, Lava Block OÜ
// license   : MIT; see accompanying LICENSE file

#include <stb_image.h>
#include <liblava/asset/scope_image.hpp>

namespace lava {

    scope_image::scope_image(string_ref filename)
    : image_file(str(filename)), file_data(image_file.get_size(), false) {
        if (image_file.opened()) {
            if (!file_data.allocate())
                return;

            if (file_error(image_file.read(file_data.ptr)))
                return;
        }

        i32 tex_width, tex_height, tex_channels = 0;

        if (image_file.opened())
            data = as_ptr(stbi_load_from_memory((stbi_uc const*) file_data.ptr, to_i32(file_data.size),
                                                &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha));
        else
            data = as_ptr(stbi_load(str(filename), &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha));

        if (!data)
            return;

        size = { tex_width, tex_height };
        channels = tex_channels;
        ready = true;
    }

    scope_image::~scope_image() {
        if (data)
            stbi_image_free(data);
    }

} // namespace lava
