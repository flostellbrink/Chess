#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include "drawable.hpp"
#include "board.hpp"
#include "camera.hpp"
#include <QMouseEvent>
#include "texture/texManager.hpp"
#include "skybox.hpp"
#include "geometry/geoManager.hpp"
#include "animation/aniManager.hpp"
#include "basicObject.hpp"
#include "collision/collManager.hpp"

using std::vector;
using glm::mat4;
using glm::vec3;

class Board;
class Clock;

class ObjectManager{
    public:
    ObjectManager();
    void NewGame();
    void Update(float elapsedTime, mat4 projection, vec2 mouse);
    void Draw(mat4 projection);
    void AddObject(Drawable* object);
    void AddPost(Drawable* object);

    void MouseDown(QMouseEvent *event);
    void MouseUp(QMouseEvent *event);
    void MouseMove(QMouseEvent *event);
    void MouseWheel(QWheelEvent *event);

    // Stores the state of the game. represents current game
    Board* GameBoard = 0;
    Clock* _clock;

    void SetTheme(int theme);

    static ObjectManager Instance;
    static TexManager Textures;
    static GeoManager Geos;
    static AniManager Animations;
    static CollManager Collisions;

    protected:
    void UpdateFramebuffer(GLuint& framebuffer, GLuint& texture, GLuint& depth, int width, int height);
    vector<Drawable*> _objects;
    vector<Drawable*> _postProcessors;
    Camera _camera;
    Skybox _skybox;
    // Need a flag when a click happened. In the next update cycle it will be evaluated
    bool _clickHappened;
    vec2 _lastMouse;
    int _theme;

    private:
    vec3 checkDepth(vec2 mousePos, mat4 viewProjection);
    mat4 mirrorMat(vec3 normal, float distance);
    GLuint _mirrorFrameBuffer = 0, _mirrorTexture = 0, _mirrorDepth = 0,
        _postFrameBuffer = 0, _postTexture = 0, _postDepth = 0,
        _shadowFrameBuffer = 0, _shadowTexture = 0, _shadowDepth = 0;
    int _resWidth = 0, _resHeight = 0, _shadowRes = 0;
};


#endif // OBJECTMANAGER_H
