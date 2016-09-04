#ifndef FULLSCREENQUAD_H
#define FULLSCREENQUAD_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/gl.h>
#include "drawable.hpp"

class FullscreenQuad: public Drawable{
    public:
    FullscreenQuad(string vertexShader, string fragmentShader);
    virtual void init() override;
    virtual void draw(glm::mat4 projection_matrix) override;
    virtual void drawShadow(mat4 projection_matrix) override;

    virtual void update(float elapsedTimeMs) override;
    virtual void MouseClick(vec3 position) override;
    virtual vec3 Position3D() override;
    void recreateGeoemtry() override;

    protected:
    virtual std::string getVertexShader() override;
    virtual std::string getFragmentShader() override;

    Geometry* _geo = 0;
    string _vertexShader, _fragmentShader;

    private:
};


#endif // FULLSCREENQUAD_H
