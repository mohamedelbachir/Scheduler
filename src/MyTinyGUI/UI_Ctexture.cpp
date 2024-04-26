#include"include/UI_Ctexture.h"
//#include"SDL2_gfxPrimitives.h"
#include<sstream>
int UI_Ctexture::instance=0;

UI_Ctexture::UI_Ctexture(SDL_Renderer *rendu,int x,int y,int w,int h):Texture(x,y,w,h,rendu,"ctexture")
{

}

UI_Ctexture::~UI_Ctexture()
{
    std::cout<<"Suppression de C_Texture"<<std::endl;
    for(size_t i=0; i<widget_contener.size(); i++)
    {
        if(widget_contener[i]!=nullptr){
            delete widget_contener[i];
        }
    }
    widget_contener.clear();
    clean();
}
void UI_Ctexture::startContentUpdate(SDL_Event *e){
    for(size_t i=0; i<widget_contener.size(); i++)
    {
        //widget_contener[i]->getVector()->setY(widget_contener[i]->getVector()->getY()+theInputManager::instance()->getMouseWhellVelocity());
        widget_contener[i]->update(e);
    }
    static int lvl=1;

    Texture::startContentUpdate(e);
        /*if(theInputManager::instance()->isKeyDown(SDL_SCANCODE_LCTRL)&&theInputManager::instance()->getMouseWhellVelocity()!=0){
            lvl+=theInputManager::instance()->getMouseWhellVelocity();
            if(lvl<=0)lvl=1;
        }
        SDL_RenderSetScale(refRender,lvl,lvl);*/
        for(size_t i=0; i<widget_contener.size(); i++)
        {
            widget_contener[i]->draw(refRender);
        }
}
void UI_Ctexture::update(SDL_Event *e)
{
    viewPort.w=m_width;
    viewPort.h=m_height;
    Texture::update(e);
    SDL_Point m_gmouse;
    m_gmouse.x=e->motion.x;
    m_gmouse.y=e->motion.y;
    SDL_Point m_mouse={m_gmouse.x-viewPort.x,m_gmouse.y-viewPort.y};
}
void UI_Ctexture::addWidget(Widget *t){
    t->setPositionMouseRelative(this->m_posMouseRef);
    widget_contener.push_back(t);
}
void UI_Ctexture::draw(SDL_Renderer *pRenderer)
{
    Texture::draw(pRenderer);
}

void UI_Ctexture::clean()
{
    TextureManager::Instance()->clearFromTextureMap(ID);
}
