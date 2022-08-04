#pragma once

#include <image.h>
#include <camera_options.h>
#include <render_options.h>
#include <string>
#include <scene.h>
#include <geometry.h>

Image RenderDepth(const std::string& filename, const CameraOptions& camera_options) {
    const auto scene = ReadScene(filename);
    double max_value = -1.0;
    std::vector<std::vector<double>> prepixels{
        static_cast<size_t>(camera_options.screen_width),
        std::vector<double>(static_cast<size_t>(camera_options.screen_height))};
    Image output(camera_options.screen_width, camera_options.screen_height);
    Camera camera(&camera_options);
    for (int i = 0; i < output.Width(); ++i) {
        for (int j = 0; j < output.Height(); ++j) {
            Ray ray{camera_options.look_from, camera.GetDirection(i, j)};
            for (auto& obj : scene.GetObjects()) {
                auto inter = GetIntersection(ray, obj.polygon);
                if (inter) {
                    auto value = inter.value().GetDistance();
                    prepixels[i][j] =
                        (prepixels[i][j] == 0) ? value : std::min(prepixels[i][j], value);
                }
            }
            for (auto& sphere : scene.GetSphereObjects()) {
                auto inter = GetIntersection(ray, sphere.sphere);
                if (inter) {
                    auto value = inter.value().GetDistance();
                    prepixels[i][j] =
                        (prepixels[i][j] == 0) ? value : std::min(prepixels[i][j], value);
                }
            }
            max_value = std::max(max_value, prepixels[i][j]);
        }
    }

    for (int i = 0; i < output.Width(); ++i) {
        for (int j = 0; j < output.Height(); ++j) {
            RGB rgb;
            if (prepixels[i][j] == 0) {
                rgb.r = 255;
                rgb.g = 255;
                rgb.b = 255;
            } else {
                auto tmp = round(255 * prepixels[i][j] / max_value);
                rgb.r = tmp;
                rgb.g = tmp;
                rgb.b = tmp;
            }
            output.SetPixel(rgb, j, i);
        }
    }
    return output;
}

inline Vector ComputeNormal(const Object& obj, const Vector& point, const Vector& default_normal) {
    Vector output;
    auto coordinates = GetBarycentricCoords(obj.polygon, point);
    for (size_t i = 0; i < 3; ++i) {
        output = (obj.GetNormal(i)) ? (output + *obj.GetNormal(i) * coordinates[i])
                                    : (output + default_normal * coordinates[i]);
    }
    return output;
}

inline void RgbNormal(Vector& normal, RGB& rgb) {
    normal = normal * 0.5 + 0.5;
    rgb.r = 255 * normal[0];
    rgb.g = 255 * normal[1];
    rgb.b = 255 * normal[2];
}

Image RenderNormal(const std::string& filename, const CameraOptions& camera_options) {
    const auto scene = ReadScene(filename);
    Image output(camera_options.screen_width, camera_options.screen_height);
    Camera camera(&camera_options);
    for (int i = 0; i < output.Width(); ++i) {
        for (int j = 0; j < output.Height(); ++j) {
            Ray ray{camera_options.look_from, camera.GetDirection(i, j)};
            RGB rgb{0, 0, 0};
            double min_dist = std::numeric_limits<double>::max();
            for (auto& obj : scene.GetObjects()) {
                auto inter = GetIntersection(ray, obj.polygon);
                if (inter && inter.value().GetDistance() < min_dist) {
                    min_dist = inter.value().GetDistance();
                    auto normal =
                        ComputeNormal(obj, inter.value().GetPosition(), inter.value().GetNormal());
                    RgbNormal(normal, rgb);
                }
            }
            for (auto& sphere : scene.GetSphereObjects()) {
                auto inter = GetIntersection(ray, sphere.sphere);
                if (inter && inter.value().GetDistance() < min_dist) {
                    min_dist = inter.value().GetDistance();
                    auto normal = inter.value().GetNormal();
                    RgbNormal(normal, rgb);
                }
            }
            output.SetPixel(rgb, j, i);
        }
    }
    return output;
}

bool IsVisible(const Light& light, const Vector& position, const Scene& scene) {
    bool fl = true;
    auto dir = light.position - position;
    auto length = Length(dir);
    dir.Normalize();
    Ray ray{position, dir};
    for (const auto& obj : scene.GetObjects()) {
        auto inter = GetIntersection(ray, obj.polygon);
        if (inter && inter.value().GetDistance() < length) {
            fl = false;
            return fl;
        }
    }
    for (const auto& sphere : scene.GetSphereObjects()) {
        auto inter = GetIntersection(ray, sphere.sphere);
        if (inter && inter.value().GetDistance() < length) {
            fl = false;
            return fl;
        }
    }
    return fl;
}

Vector ComputeLightedColor(const Material* material, const Intersection& inter, const Ray& ray,
                           const Scene& scene) {
    Vector output;
    for (const auto& light : scene.GetLights()) {
        if (IsVisible(light, inter.GetPosition(), scene)) {
            auto vl = light.position - inter.GetPosition();
            vl.Normalize();
            auto normal_x_vl = DotProduct(inter.GetNormal(), vl);
            output =
                output + material->diffuse_color * light.intensity * std::max(0.0, normal_x_vl);
            output = output +
                     material->specular_color * light.intensity *
                         pow(std::max(0.0, DotProduct(-1 * ray.GetDirection(),
                                                      2 * normal_x_vl * inter.GetNormal() - vl)),
                             material->specular_exponent);
        }
    }
    return output;
}

std::optional<std::pair<Intersection, const Material*>> FindIntersection(const Ray& ray,
                                                                         const Scene& scene) {
    double min_dist = std::numeric_limits<double>::max();
    std::optional<std::pair<Intersection, const Material*>> best;
    for (auto& obj : scene.GetObjects()) {
        auto inter = GetIntersection(ray, obj.polygon);
        if (inter and inter.value().GetDistance() < min_dist) {
            min_dist = inter.value().GetDistance();
            Intersection new_inter{
                inter.value().GetPosition(),
                ComputeNormal(obj, inter.value().GetPosition(), inter.value().GetNormal()),
                inter.value().GetDistance()};
            best.emplace(new_inter, obj.material);
        }
    }
    for (auto& sphere : scene.GetSphereObjects()) {
        auto inter = GetIntersection(ray, sphere.sphere);
        if (inter and inter.value().GetDistance() < min_dist) {
            min_dist = inter.value().GetDistance();
            best.emplace(inter.value(), sphere.material);
        }
    }
    return best;
}

Vector ComputeColor(const Material* material, const Ray& ray, const Intersection& inter, int depth,
                    const Scene& scene, bool inside) {
    Vector output = (material->ambient_color + material->intensity) +
                    (material->albedo[0] * ComputeLightedColor(material, inter, ray, scene));
    if (material->albedo[1] > 0 && depth > 1 && !inside) {
        Ray reflect_ray{inter.GetPosition(), Reflect(ray.GetDirection(), inter.GetNormal())};
        auto reflect_inter = FindIntersection(reflect_ray, scene);
        output =
            (reflect_inter)
                ? (output + material->albedo[1] *
                                ComputeColor(reflect_inter.value().second, reflect_ray,
                                             reflect_inter.value().first, depth - 1, scene, inside))
                : output;
    }
    if (material->albedo[2] > 0 && depth > 1) {
        double eta;
        eta = (!inside) ? (1 / (material->refraction_index)) : (material->refraction_index);
        auto refract = Refract(ray.GetDirection(), inter.GetNormal(), eta);
        if (refract) {
            Ray refract_ray{inter.GetPosition() + (-1.0) * 10e-5 * inter.GetNormal(),
                            refract.value()};
            auto refract_inter = FindIntersection(refract_ray, scene);
            if (refract_inter) {
                double alb = material->albedo[2];
                alb = (inside) ? 1 : alb;
                output = output + alb * ComputeColor(refract_inter.value().second, refract_ray,
                                                     refract_inter.value().first, depth - 1, scene,
                                                     !inside);
            }
        }
    }
    return output;
}

inline void ToneMapping(Image& image, const std::vector<std::vector<Vector>>& prepixels) {
    double max = -1;
    for (int i = 0; i < image.Width(); ++i) {
        for (int j = 0; j < image.Height(); ++j) {
            auto& pixel = prepixels[i][j];
            for (size_t iter = 0; iter < 3; ++iter) {
                max = std::max(max, pixel[iter]);
            }
        }
    }
    for (int i = 0; i < image.Width(); ++i) {
        for (int j = 0; j < image.Height(); ++j) {
            RGB rgb;
            std::array<double, 3> rgb_tmp{{0.0, 0.0, 0.0}};
            auto& pixel = prepixels[i][j];
            for (size_t iter = 0; iter < 3; ++iter) {
                rgb_tmp[iter] = pixel[iter] * (1 + pixel[iter] / max / max) / (1 + pixel[iter]);
                rgb_tmp[iter] = pow(rgb_tmp[iter], 1 / 2.2);
            }
            rgb.r = round(255 * rgb_tmp[0]);
            rgb.g = round(255 * rgb_tmp[1]);
            rgb.b = round(255 * rgb_tmp[2]);
            image.SetPixel(rgb, j, i);
        }
    }
}

Image RenderFull(const std::string& filename, const CameraOptions& camera_options, int depth) {
    const auto scene = ReadScene(filename);
    std::vector<std::vector<Vector>> prepixels;
    Image output{camera_options.screen_width, camera_options.screen_height};
    Camera camera{&camera_options};

    for (int i = 0; i < output.Width(); ++i) {
        prepixels.push_back(std::vector<Vector>(output.Height()));
        for (int j = 0; j < output.Height(); ++j) {
            Ray ray{camera_options.look_from, camera.GetDirection(i, j)};
            auto inter = FindIntersection(ray, scene);
            prepixels[i][j] = (inter) ? ComputeColor(inter.value().second, ray, inter.value().first,
                                                     depth, scene, false)
                                      : prepixels[i][j];
        }
    }
    ToneMapping(output, prepixels);
    return output;
}

Image Render(const std::string& filename, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    if (render_options.mode == RenderMode::kDepth) {
        return RenderDepth(filename, camera_options);
    } else if (render_options.mode == RenderMode::kNormal) {
        return RenderNormal(filename, camera_options);
    }
    return RenderFull(filename, camera_options, render_options.depth);
}
