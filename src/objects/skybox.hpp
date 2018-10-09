#ifndef SKYBOX_H
#define SKYBOX_H

#include "texture/texturewrapper.hpp"
#include "objects/drawable.hpp"

class Skybox : public Drawable
{
public:
    Skybox();

    /**
     * @see Drawable::init()
     */
    virtual void init() override;

    /**
     * @see Drawable::draw(glm::mat4)
     */
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;

    void recreateGeoemtry() override;

    virtual vec3 Position3D() override;

    /**
     * @brief same as draw, only with a view mat for the skybox
     */
    virtual void drawSkybox(glm::mat4 projection_matrix);

    /**
     * @see Drawable::update(float, glm::mat4)
     */
    virtual void update(float elapsedTimeMs) override;
    virtual void MouseClick(vec3 position) override;


protected:

    /**
     * @see Drawable::getVertexShader()
     */
    virtual std::string getVertexShader() override;

    /**
     * @see Drawable::getFragmentShader()
     */
    virtual std::string getFragmentShader() override;
private:
    Geometry* _geo;
};

#endif // SKYBOX_H
