/*
 * Copyright (C) 2013, 2014
 * Martin Lambers <marlam@marlam.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/* Provide basic navigation, similar to common model viewers. */

class Navigator
{
private:
    // Current viewport.
    glm::ivec4 _viewport;
    // Scene parameters.
    glm::vec3 _center;
    float _radius;
    glm::quat _bias;
    // 3D Navigation.
    glm::vec3 _pos; // relative to center
    glm::quat _rot;
    // 2D Navigation.
    glm::vec2 _trans_2d;
    float _scale_2d;
    // States
    enum {
        state_inactive,
        state_rot,
        state_shift,
        state_zoom,
        state_shift_2d,
        state_zoom_2d
    } _state;
    // State for navigation
    glm::ivec2 _last_pos;
    glm::vec3 _last_ballpos;
    float _last_dist;

    // Helper functions
    bool check_pos(const glm::ivec2& pos);
    glm::vec3 ballmap(const glm::ivec2& pos);

public:
    Navigator();

    // The navigator must always know the current viewport.
    void set_viewport(const glm::ivec4& vp);

    // Set your scene. You must specify center and radius.
    // Optionally you can set a default view with the eye and up parameters.
    // Note that eye is interpreted relative to center, i.e. you need to
    // substract the center from your absolute eye position.
    // If your scene is 2D, just use center=0 and radius=1
    void set_scene(const glm::vec3& center, float radius,
            const glm::vec3& eye = glm::vec3(0.0f, 0.0f, 1.0f),
            const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    /* 3D Navigation */

    // Suggest values for near and far plane based on the scene.
    float suggest_near() const;
    float suggest_far() const;

    // Return the viewer position.
    glm::vec3 get_viewer_pos() const;

    // Return the viewer rotation.
    glm::quat get_viewer_rot() const;

    // Return the view matrix.
    glm::mat4 get_viewmatrix() const;

    // Reset to the default view.
    void reset();

    // Rotate based on mouse coordinates.
    void start_rot(const glm::ivec2& pos);
    void rot(const glm::ivec2& pos);

    // Shift based on mouse coordinates.
    void start_shift(const glm::ivec2& pos);
    void shift(const glm::ivec2& pos);

    // Zoom based on mouse coordinates.
    void start_zoom(const glm::ivec2& pos);
    void zoom(const glm::ivec2& pos);
    // Zoom based on mouse wheel.
    void zoom(float wheel_rot /* in radians, pos or neg */);

    /* 2D Navigation */

    // Return the 2D translation (in the xy plane).
    glm::vec2 get_translation_2d() const;

    // Return the 2D scale.
    float get_scale_2d() const;

    // Return the 2D view matrix.
    glm::mat4 get_viewmatrix_2d() const;

    // Shift based on mouse coordinates.
    void start_shift_2d(const glm::ivec2& pos);
    void shift_2d(const glm::ivec2& pos);

    // Zoom based on mouse coordinates.
    void start_zoom_2d(const glm::ivec2& pos);
    void zoom_2d(const glm::ivec2& pos);
    // Zoom based on mouse wheel.
    void zoom_2d(float wheel_rot /* in radians, pos or neg */);
};

#endif
