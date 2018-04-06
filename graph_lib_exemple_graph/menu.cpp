#include "menu.h"


MenuInterface::MenuInterface(int x, int y, int w, int h, int choix)
{
    if (choix==1)
    {
            //affichage du fond pour le menu
    m_top_box.set_dim(1020,760);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_top_box.add_child(m_background);
    m_background.set_pic_name("background.jpg");

    //affichage du titre pour le menu
    m_top_box.add_child(m_titre);
    m_titre.set_pos(355,150);
    m_titre.set_pic_name("menu.jpg");

    //affichage du premier bouton
    m_top_box.add_child(m_button1);
    m_button1.set_dim(280,80);
    m_button1.set_pos(350,300);
    m_button1.set_bg_color(GRISSOMBRE);
    m_button1.add_child(m_texteButton1);
    m_texteButton1.set_pos(110,30);
    m_texteButton1.set_message(" Jouer ");


    //affichage du second bouton
    m_top_box.add_child(m_button2);
    m_button2.set_dim(280,80);
    m_button2.set_pos(350,400);
    m_button2.set_bg_color(GRISSOMBRE);
    m_button2.add_child(m_texteButton2);
    m_texteButton2.set_pos(110,30);
    m_texteButton2.set_message(" Regle");

    //affichage du troisième bouton
    m_top_box.add_child(m_button3);
    m_button3.set_dim(280,80);
    m_button3.set_pos(350,500);
    m_button3.set_bg_color(GRISSOMBRE);
    m_button3.add_child(m_texteButton3);
    m_texteButton3.set_pos(110,30);
    m_texteButton3.set_message(" Quitter ");

    }

    if (choix==2)
    {
           //affichage du fond pour le menu
    m_top_box.set_dim(1020,760);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_top_box.add_child(m_background);
    m_background.set_pic_name("background.jpg");

//    //affichage du titre pour le menu
//    m_top_box.add_child(m_titre);
//    m_titre.set_pos(355,150);
//    m_titre.set_pic_name("menu.jpg");

    //affichage du premier bouton
    m_top_box.add_child(m_button1);
    m_button1.set_dim(280,80);
    m_button1.set_pos(350,300);
    m_button1.set_bg_color(GRISSOMBRE);
    m_button1.add_child(m_texteButton1);
    m_texteButton1.set_pos(110,30);
    m_texteButton1.set_message(" Graphe numero 1 ");


    //affichage du second bouton
    m_top_box.add_child(m_button2);
    m_button2.set_dim(280,80);
    m_button2.set_pos(350,400);
    m_button2.set_bg_color(GRISSOMBRE);
    m_button2.add_child(m_texteButton2);
    m_texteButton2.set_pos(110,30);
    m_texteButton2.set_message(" Graphe numero 2");

    //affichage du troisième bouton
    m_top_box.add_child(m_button3);
    m_button3.set_dim(280,80);
    m_button3.set_pos(350,500);
    m_button3.set_bg_color(GRISSOMBRE);
    m_button3.add_child(m_texteButton3);
    m_texteButton3.set_pos(110,30);
    m_texteButton3.set_message(" graphe numero 3 ");
    }

}


int Menu::update()
{
    int tmp;

        m_interface->m_top_box.update();
        if(m_interface->m_button1.clicked())
            m_interface->m_choix=1;
        if(m_interface->m_button2.clicked())
            m_interface->m_choix=2;
        if(m_interface->m_button3.clicked())
            m_interface->m_choix=3;

        tmp=m_interface->m_choix;

        m_interface->m_choix=0;

        return tmp;
}
