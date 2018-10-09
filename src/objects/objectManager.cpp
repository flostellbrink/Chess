#include <GL/glew.h>
#include <vector>
#include <iostream>
#include "objectManager.hpp"
#include "src/objects.hpp"
#include "drawable.hpp"
#include "board.hpp"
#include "skybox.hpp"
#include "piece.hpp"
#include "basicObject.hpp"
#include "src/config.h"
#include "fullscreenQuad.hpp"
#include <algorithm>
#include <src/config.h>
#include <src/objects.hpp>

using std::vector;
using glm::vec4;
using glm::mat4;

GeoManager ObjectManager::Geos;
TexManager ObjectManager::Textures;
AniManager ObjectManager::Animations;
ObjectManager ObjectManager::Instance;
CollManager ObjectManager::Collisions;

const GLuint defaultFrambuffer = 1;

ObjectManager::ObjectManager(): _skybox(){
}

void ObjectManager::NewGame(){
    _objects.clear();
    _postProcessors.clear();
    delete GameBoard;
    AddObject(&_skybox);
    AddObject(new BasicObject(objects::Table, vec3(0,-1,0), 0, "basic"));
    // Creates all game related objects itself
    GameBoard = new Board(&_camera);
    _clock = new Clock();

    // Post processing
    AddPost(new FullscreenQuad("res/shader/blurVer.vs.glsl", "res/shader/blur.fs.glsl"));
    AddPost(new FullscreenQuad("res/shader/blurHor.vs.glsl", "res/shader/blur.fs.glsl"));
    AddPost(new FullscreenQuad("res/shader/overlay.vs.glsl", "res/shader/overlay.fs.glsl"));
}

void ObjectManager::UpdateFramebuffer(GLuint& framebuffer, GLuint& texture, GLuint& depth, int width, int height){
    // Get rid of old objects
    if(framebuffer)
        glDeleteFramebuffers(1, &framebuffer);
    if(texture)
        glDeleteTextures(1, &texture);
    if(depth)
        glDeleteRenderbuffers(1, &depth);

    // Generate Framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Generate Texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Generate Depth Buffer
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Setup Framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void ObjectManager::Update(float elapsedTime, mat4 projection){
    // Ignore parameter unused
    (void)projection;

    if(Config::changed){
        Config::changed = false;
        Geos.Regenerate();
    }
    if(Config::changeTheme){
        Config::changeTheme = false;
        SetTheme((_theme == themes::Default) ? themes::Glass : themes::Default);
    }
    if(Config::undoTurn){
        Config::undoTurn = false;
        GameBoard->UndoMove(false);
    }
    if(Config::ai != GameBoard->getAI()){
        GameBoard->EnableAI(Config::ai);
    }
    if(Config::newGame){
        std::cerr << "Spiel wird neugestartet!" << std::endl;
        Config::newGame = false;
        GameBoard->ResetGame();
        //NewGame();
    }
    if(Config::demo){
        Config::demo = false;
        GameBoard->RunDemo();
    }

    if (_clock->timeout()) {
        GameBoard->setState(_clock->timeout());
    }

    if(elapsedTime > 200) {
        std::cerr << "ChessWarn: Too much time passed since last update: " << elapsedTime << "ms" << std::endl;
        //This prevents skipping of animations
        elapsedTime = 20;
    }

    for (Drawable* obj : _objects)
        obj->update(elapsedTime);
    GameBoard->Update();
    _camera.Update();
    Animations.Update(elapsedTime);

    _clock->Update(elapsedTime, GameBoard->isWhitesTurn());
}

/**
 * @brief The DepthSort struct compares objects depth, using a given projection
 */
struct DepthSort {
    explicit DepthSort(mat4 projection) { this->_projection = projection; }
    bool operator () (Drawable* obj1, Drawable* obj2) {
        vec4 projected1 = _projection * vec4(obj1->Position3D(), 1),
             projected2 = _projection * vec4(obj2->Position3D(), 1);
        return projected1.z > projected2.z;
    }

    mat4 _projection;
};

void ObjectManager::Draw(mat4 projection){
    // Setup framebuffer
    if(!_mirrorFrameBuffer || !_postFrameBuffer || !_shadowFrameBuffer ||
        Config::viewportWidth != _resWidth || Config::viewportHeight != _resHeight)
    {
        _resWidth = Config::viewportWidth;
        _resHeight = Config::viewportHeight;
        UpdateFramebuffer(_postFrameBuffer, _postTexture, _postDepth, _resWidth, _resHeight);
        UpdateFramebuffer(_mirrorFrameBuffer, _mirrorTexture, _mirrorDepth, _resWidth, _resHeight);
        Drawable::ReflectionTexture = _mirrorTexture;
    }
    if(!_shadowFrameBuffer || _shadowRes != Config::shadowResolution){
        _shadowRes = Config::shadowResolution;
        UpdateFramebuffer(_shadowFrameBuffer, _shadowTexture, _shadowDepth, _shadowRes, _shadowRes);
        Drawable::ShadowTexture = _shadowTexture;
    }
    // Get Projection Matrices
    Drawable::_camPos = _camera.Position();
    mat4 viewProjection = projection * _camera.ViewMat(),
        viewProjectionSkybox = projection * _camera.ViewMatAtCamera(),
        mirror = mirrorMat(vec3(0,1,0), 0),
        viewProjectionMirrored = viewProjection * mirror,
        viewProjectionSkyboxMirrored = viewProjectionSkybox * mirror,
        viewProjectionShadow = _camera.ViewProjectionShadow();
    Drawable::ShadowViewProjection = viewProjectionShadow;

    // Sort objects by depth
    std::sort(_objects.begin(), _objects.end(), DepthSort(viewProjection));

    // Render Shadow depth map
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowFrameBuffer);
    glViewport(0, 0, _shadowRes, _shadowRes);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    for (Drawable* obj: _objects)
        obj->drawShadow(viewProjectionShadow);

    // Reset Viewport, assuming all other steps use default resolution
    glViewport(0, 0, _resWidth, _resHeight);

    // Render Mirrored
    glBindFramebuffer(GL_FRAMEBUFFER, _mirrorFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _skybox.drawSkybox(viewProjectionSkyboxMirrored);
    for (Drawable* obj: _objects)
        obj->drawReflection(viewProjectionMirrored);

    // Render Background
    glBindFramebuffer(GL_FRAMEBUFFER, _postFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _skybox.drawSkybox(viewProjectionSkybox);

    // Render Main Scene
    for (Drawable* obj : _objects)
        obj->drawOpaque(viewProjection);
    for (Drawable* obj : _objects){
        // Draw depth
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        obj->drawTranslucent(viewProjection);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        // Draw surface color
        glDepthFunc(GL_EQUAL);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        obj->drawTranslucent(viewProjection);
        glDisable (GL_BLEND);
        glDepthFunc(GL_LESS);
    }

    if(_clickHappened){
        _clickHappened = false;
        vec3 mousePos = checkDepth(_lastMouse, projection * _camera.ViewMat());
        for (Drawable* obj : _objects)
            obj->MouseClick(mousePos);
    }

    // Do Post and render to screen
    if(_postProcessors.empty()) return;
    glDisable(GL_DEPTH_TEST);
    bool renderToPost = false;
    glBindFramebuffer(GL_FRAMEBUFFER, _mirrorFrameBuffer);
    Drawable::PostTexture = _postTexture;

    for (int i = 0; i < _postProcessors.size() - 1; ++i) {
        _postProcessors[i]->draw(mat4());
        // Switch Framebuffer
        renderToPost = !renderToPost;
        glBindFramebuffer(GL_FRAMEBUFFER, renderToPost ? _postFrameBuffer : _mirrorFrameBuffer);
        Drawable::PostTexture = renderToPost ? _mirrorTexture : _postTexture;
    }

    // Render final pass to screen
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFrambuffer);
    _postProcessors[_postProcessors.size()-1]->draw(mat4());
    glEnable(GL_DEPTH_TEST);

    /*
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _shadowFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFrambuffer);
    glBlitFramebuffer(0, 0, _shadowRes, _shadowRes, 0, 0, Config::viewportWidth, Config::viewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    /**/
}

void ObjectManager::AddObject(Drawable *object){
    _objects.push_back(object);
    object->init();
}

void ObjectManager::AddPost(Drawable *object){
    _postProcessors.push_back(object);
    object->init();
}

void ObjectManager::SetTheme(int theme){
    _theme = theme;
    Geos.SetTheme(theme);
    Textures.SetTheme(theme);
    for (Drawable* obj : _objects)
        obj->recreateGeoemtry();
}

void ObjectManager::MouseButton(int button, int action){
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            _clickHappened = true;
        } else {
            _camera.MouseDown();
        }
    } else {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            // Ignored
        } else {
            _camera.MouseUp();
        }
    }
}

void ObjectManager::MouseMove(double xPos, double yPos){
    _lastMouse = vec2(xPos, yPos);
    _camera.MouseMove(_lastMouse);
}

void ObjectManager::MouseWheel(double xOffset, double yOffset){
    _camera.MouseWheel((float)yOffset / -360.0f);
}

/**
 * @brief ObjectManager::checkDepth Unprojects a point from window coordinates to world woordinates
 * @param mousePos The mouse position in window coordinates
 * @param viewProjection The world to window projection
 * @return The mosue position in world coordinates
 */
vec3 ObjectManager::checkDepth(vec2 mousePos, mat4 viewProjection){
    int width = Config::viewportWidth, height = Config::viewportHeight;
    if(mousePos.x < 0 || mousePos.x >= width || mousePos.y < 0 || mousePos.y >= height)
        return vec3();
    mousePos.y = height - mousePos.y;
    float pixel;
    glReadPixels(static_cast<GLint>(mousePos.x), static_cast<GLint>(mousePos.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixel);

    vec4 windowCoords((float)mousePos.x / (float)width * 2.f - 1.f,
                      (float)mousePos.y /(float)height * 2.f - 1.f,
                      pixel * 2.f - 1.f,
                      1);
    vec4 result = glm::inverse(viewProjection) * windowCoords;
    if(result.w == 0)
        return vec3();
    result /= result.w;
    return vec3(result);
}

mat4 ObjectManager::mirrorMat(glm::vec3 normal, float distance){
    return mat4(vec4(1 - 2 * normal.x, -2 * normal.x * normal.y, -2 * normal.x * normal.z, 0),
                vec4(-2 * normal.x * normal.y, 1 - 2 * normal.y, -2 * normal.y * normal.z, 0),
                vec4(-2 * normal.x * normal.z, -2 * normal.y * normal.z, 1 - 2 * normal.z, 0),
                vec4(-2 * normal.x * distance, -2 * normal.y * distance, -2 * normal.z * distance, 1));
}