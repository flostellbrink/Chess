#include <GL/glew.h>

#include "texManager.hpp"
#include "objects.hpp"
#include <iostream>

using std::string;

TexManager::TexManager()
{

}

bool TexManager::IsReflected(int object){
    if(object >= objects::whiteKing && object <= objects::blackPawn)
        return true;
    return false;
}

bool TexManager::IsTranslucent(int object){
    if(_theme == themes::Glass && object >= objects::whiteKing && object <= objects::blackPawn)
        return true;
    return false;
}

vector<TextureWrapper*> TexManager::Texture(int object){
    if(_textureCollections[_theme].count(object))
        return _textureCollections[_theme][object];
    return loadTexture(object);
}

float TexManager::reflectivity(int object){
    switch( object ) {
        case objects::ClockButton:
        case objects::ClockButtonBorder:
        case objects::ClockGlass:
        case objects::ClockGlassBorder:
        case objects::ClockFace:
            return 1;
    }
    if(_theme == themes::Glass && object >= objects::whiteKing && object <= objects::blackPawn)
        return .8;
    return .1;
}

float TexManager::shininess(int object){
    switch( object ) {
        case objects::ClockButton:
        case objects::ClockButtonBorder:
        case objects::ClockGlass:
        case objects::ClockGlassBorder:
            return 15;
    }
    if(_theme == themes::Glass && object >= objects::whiteKing && object <= objects::blackPawn)
        return 10;
    return 3;
}

vector<TextureWrapper*> TexManager::loadTexture(int object){
    switch( object ) {
        case objects::Table:
            return storeTexture(object, ":/res/images/Melamine-wood-005.png");
        case objects::Skybox:
            return storeTexture(object, ":/res/images/LancellottiChapel/cube", GL_TEXTURE_CUBE_MAP);
        case objects::ClockBase:
            return storeTexture(object, ":/res/images/Melamine-wood-004.png");
        case objects::ClockFace:
            return storeTexture(object, ":/res/images/clockFace.png", GL_TEXTURE_2D);
        case objects::ClockTimeCap:
            return storeTexture(object, ":/res/images/redMark.png");
        case objects::ClockMarks:
            return storeTexture(object, ":/res/images/Melamine-wood-006.png");
            case objects::ClockLeg:
        case objects::ClockGlassBorder:
        case objects::ClockButton:
        case objects::ClockButtonBorder:
        case objects::whiteSecondsHand:
        case objects::blackSecondsHand:
        case objects::whiteMinutesHand:
        case objects::blackMinutesHand:
            return storeTexture(object, ":/res/images/metal.png");
        case objects::FullscreenQuad:
            vector<TextureWrapper*> result;
            result.push_back( Texture(":/res/images/whiteWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
            result.push_back( Texture(":/res/images/blackWins.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
            result.push_back( Texture(":/res/images/draw.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
            return result;
    }
    switch( _theme ) {
        case themes::Default:
            switch( object ) {
                case objects::BoardBorderTop:
                case objects::BoardBorderBottom:
                    return storeTexture(object, ":/res/images/Melamine-wood-004.png");
            }
            if((object >= objects::Field00) && (object <= objects::Field77)){
                vector<TextureWrapper*> result;
                int row = (object - objects::Field00) / 8,
                    column = (object - objects::Field00) % 8;
                if(row % 2 == column % 2) // black
                    result.push_back( Texture(":/res/images/Melamine-wood-004.png") );
                else //white
                    result.push_back( Texture(":/res/images/Melamine-wood-001.png") );
                result.push_back( Texture(":/res/images/overlayMove.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayHit.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayMate1.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayMate2.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) ); //broken

                _textureCollections[_theme][object] = result;
                return result;
            }
            if((object >= objects::whiteKing && object <= objects::blackPawn)){
                if(!(object % 2))// white
                    return storeTexture(object, ":/res/images/Melamine-wood-001.png");
                else //black
                    return storeTexture(object, ":/res/images/Melamine-wood-004.png");
            }
        break;
        case themes::Glass:
            switch( object ) {
                case objects::BoardBorderTop:
                case objects::BoardBorderBottom:
                    return storeTexture(object, ":/res/images/Melamine-wood-004.png");
            }
            if((object >= objects::Field00) && (object <= objects::Field77)){
                vector<TextureWrapper*> result;
                int row = (object - objects::Field00) / 8,
                    column = (object - objects::Field00) % 8;
                if(row % 2 == column % 2) // black
                    result.push_back( Texture(":/res/images/Melamine-wood-004.png") );
                else //white
                    result.push_back( Texture(":/res/images/Melamine-wood-001.png") );
                result.push_back( Texture(":/res/images/overlayMove.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayHit.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayMate1.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) );
                result.push_back( Texture(":/res/images/overlayMate2.png", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE) ); //broken

                _textureCollections[_theme][object] = result;
                return result;
            }
            if((object >= objects::whiteKing && object <= objects::blackPawn)){
                if(!(object % 2))// white
                    return storeTexture(object, ":/res/images/whiteGlass.png");
                else //black
                    return storeTexture(object, ":/res/images/blackGlass.png");
            }
        break;
    }
    std::cerr << "ChessWarn: Using default texture for objectID: " << object << std::endl;
    return storeTexture(object, ":/res/images/Melamine-wood-001.png");
}

vector<TextureWrapper*> TexManager::storeTexture(int object, std::string filePath, GLenum target, GLenum textureRepeat){
    vector<TextureWrapper*> result = vector<TextureWrapper*> { Texture(filePath, target, textureRepeat) };
    _textureCollections[_theme][object] = result;
    return result;
}

TextureWrapper* TexManager::Texture(string path, GLenum target, GLenum textureRepeat){
    if(_textures[_theme].count(path))
        return _textures[_theme][path];
    return loadTexture(path, target, textureRepeat);
}

TextureWrapper* TexManager::loadTexture(string path, GLenum target, GLenum textureRepeat){
     TextureWrapper* result = new TextureWrapper(path, target, textureRepeat);
     _textures[_theme][path] = result;
     return result;
}

void TexManager::SetTheme(int theme){
    _theme = theme;
    /*if(!_textures.count(theme))
        _textures[theme] = map<string, TextureWrapper*>();
    if necessary intialize, also _textureCollections */
}
