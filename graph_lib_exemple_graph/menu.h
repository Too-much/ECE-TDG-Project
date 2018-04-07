#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "graph.h"


class MenuInterface
{
    friend class Menu;
private:
    int m_choix;
    grman::WidgetBox m_top_box;
    grman::WidgetImage m_background;
    grman::WidgetImage m_titre;
    grman::WidgetButton m_button1;
    grman::WidgetButton m_button2;
    grman::WidgetButton m_button3;
    grman::WidgetText m_texteButton1;
    grman::WidgetText m_texteButton2;
    grman::WidgetText m_texteButton3;
    grman::WidgetImage m_imageButton1;
    grman::WidgetImage m_imageButton2;
    grman::WidgetImage m_imageButton3;


public:

    MenuInterface(int x, int y, int w, int h, int choix);
};

class Menu
{
private:
    std::shared_ptr<MenuInterface> m_interface = nullptr;
    std::shared_ptr<MenuInterface> m_interface2 = nullptr;

public:
    Menu (int choix)  { m_interface = std::make_shared<MenuInterface>(50, 0, 750, 600, choix); }
//    Menu2() { m_interface2 = std::make_shared<MenuInterface> (50, 0, 750, 600, 2); }

    int update();


};


#endif // MENU_H_INCLUDED
