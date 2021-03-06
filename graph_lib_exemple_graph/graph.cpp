#include "graph.h"

#define toolX 15
#define toolY 5

/***************************************************
                    VERTEX
****************************************************/

/// Constructor par defaut de VERTEX
Vertex::Vertex(std::ifstream& fc)
{
    getline(fc, m_namePicture, '#');
    fc >> m_pos_x;
    fc >> m_pos_y;
    fc >> m_value;
    fc >> m_growth;
    fc >> m_mortality;
    fc >> m_active;
    fc >> m_saveSupp;
    fc >> m_event;
    fc >> m_detrivore;
    m_interface=nullptr;
}

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx,int growth, bool active)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100); // Change la taille des box des SOMMETS
    m_top_box.set_bg_color(BLANC);
    m_top_box.set_moveable();


    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // CHANGE LA RANGE DES SLIDERS DES SOMMETS
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }
    // Cr�ation d'un boutton s�lectionner ou non en haut � gauche de chaque sommet
    m_img.add_child(m_select);
    m_select.set_dim(15,15);
    m_select.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_select.set_value(active);

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}

/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

    m_pos_x = m_interface->m_top_box.get_posx();
    m_pos_y = m_interface->m_top_box.get_posy();
    m_value = m_interface->m_slider_value.get_value();

}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

///Constructor par d�faut de Edge
Edge::Edge(std::ifstream& fc)
{
    fc >> m_from;
    fc >> m_to;
    fc >> m_weight;
    fc >> m_active;
    fc >> m_active2;
    fc >> m_active3;
    m_interface=nullptr;
}

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to, double weight)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(32,70);
    m_box_edge.set_bg_color(BLANCBLEU);

    m_box_edge.add_child(m_select);
    m_select.set_dim(10,5);
    m_select.set_bg_color(BLANCBLEU);
    m_select.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Up);


    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 1.0);  // CHANGE LA RANGE DES SLIDERS DES ARETES
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Center);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Down);
}



/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    if(m_weight>=0.9&&m_weight<1)
        m_interface->m_top_edge.set_thickness(10);
    if(m_weight>=0.80&&m_weight<0.90)
        m_interface->m_top_edge.set_thickness(9);
    if(m_weight>=0.70&&m_weight<0.80)
        m_interface->m_top_edge.set_thickness(8);
    if(m_weight>=0.60&&m_weight<0.70)
        m_interface->m_top_edge.set_thickness(7);
    if(m_weight>=0.50&&m_weight<0.60)
        m_interface->m_top_edge.set_thickness(6);
    if(m_weight>=0.40&&m_weight<0.50)
        m_interface->m_top_edge.set_thickness(5);
    if(m_weight>=0.30&&m_weight<0.40)
        m_interface->m_top_edge.set_thickness(4);
    if(m_weight>=0.20&&m_weight<0.30)
        m_interface->m_top_edge.set_thickness(3);
    if(m_weight>=0.10&&m_weight<0.20)
        m_interface->m_top_edge.set_thickness(2);
    if(m_weight>=0&&m_weight<0.10)
        m_interface->m_top_edge.set_thickness(1);

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value( m_weight);

    std::string sendNumber("");
    if(m_weight < 0.1)
        sendNumber = "0.0" + std::to_string((int) (m_weight*100) );
    else
        sendNumber = "0." + std::to_string((int) (m_weight*100) ) ;
    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message(sendNumber);
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH INIT
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h, std::string photo, int ordre)
{
    m_top_box.set_dim(1020,760);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(100,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    /// BUTTON DE SUPPRESSION DE SOMMET
    m_tool_box.add_child(m_buttonDel);
    m_buttonDel.set_dim(35,30);
    m_buttonDel.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Center);
    m_buttonDel.set_bg_color(ROUGE);
    m_buttonDel.add_child(m_buttonDel_label);
    m_buttonDel_label.set_message("DEL");

    ///BUTTON D'AJOUT DE SOMMET
    m_tool_box.add_child(m_buttonAdd);
    m_buttonAdd.set_dim(35,30);
    m_buttonAdd.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Center);
    m_buttonAdd.set_bg_color(VERT);
    m_buttonAdd.add_child(m_buttonAdd_label);
    m_buttonAdd_label.set_message("ADD");

    /// BOUTTON LANCEMENT SIMULATION
    m_tool_box.add_child(m_simulation);
    m_simulation.set_dim(50,50);
    m_simulation.set_bg_color(GRISCLAIR);
    m_simulation.set_pos(25,400);
    m_tool_box.add_child(m_textSimulation);
    m_textSimulation.set_message("PLAY");
    m_textSimulation.set_pos(35,460);

    ///BOUTTON LANCEMENT EVENT
    if(photo=="banquise.jpg")
    {
        m_tool_box.add_child(m_event);
        m_event.set_dim(30,30);
        m_event.set_bg_color(GRISCLAIR);
        m_event.set_pos(5,270);
        m_tool_box.add_child(m_nomEvent);
        m_nomEvent.set_message("Melt");
        m_nomEvent.set_pos(7,320);

        m_tool_box.add_child(m_event2);
        m_event2.set_dim(30,30);
        m_event2.set_bg_color(GRISCLAIR);
        m_event2.set_pos(60,270);
        m_tool_box.add_child(m_nomEvent2);
        m_nomEvent2.set_message("Rise");
        m_nomEvent2.set_pos(62,320);
    }

    ///BOUTTON LANCEMENT EVENT
    if(photo=="dinosaure.jpg")
    {
        m_tool_box.add_child(m_event);
        m_event.set_dim(40,40);
        m_event.set_bg_color(GRISCLAIR);
        m_event.set_pos(30,270);
        m_tool_box.add_child(m_nomEvent);
        m_nomEvent.set_message("METEOR");
        m_nomEvent.set_pos(32,320);
    }

    /// MAIN BOX
    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.add_child(m_img);
    m_img.set_pic_name(photo);

    ///bouton pour la forte connexit�
    m_tool_box.add_child(m_buttonFCC);
    m_buttonFCC.set_dim(94,50);
    m_buttonFCC.set_bg_color(GRISCLAIR);
    m_buttonFCC.set_pos(3,480);
    m_buttonFCC.add_child(m_textFCC);
    m_textFCC.set_message("Strongly CC");

    /// CHECK pour reset
    m_tool_box.add_child(m_reset);
    m_reset.set_bg_color(GRISCLAIR);
    m_reset.set_dim(30,30);
    m_reset.set_pos(35,550);
    m_tool_box.add_child(m_name_reset);
    m_name_reset.set_pos(35,590);
    m_name_reset.set_message("Reset");

    /// K-PLET SOMMET // ARETE
    m_tool_box.add_child(m_button_k_plet);
    m_button_k_plet.set_pos(2,670);
    m_button_k_plet.set_dim(45,30);
    m_button_k_plet.set_bg_color(GRISCLAIR);
    m_button_k_plet.add_child(m_text_k_plet);
    m_text_k_plet.set_message("k-v");

    m_tool_box.add_child(m_button_robuste);
    m_button_robuste.set_pos(52,670);
    m_button_robuste.set_dim(45,30);
    m_button_robuste.set_bg_color(GRISCLAIR);
    m_button_robuste.add_child(m_text_robuste);
    m_text_robuste.set_message("k-e");

    m_tool_box.add_child(m_slider_robuste);
    m_slider_robuste.set_dim(10,40);
    m_slider_robuste.set_pos(83,610);
    m_slider_robuste.set_range(0,ordre-1);
    m_tool_box.add_child(m_textSlider_robuste);
    m_textSlider_robuste.set_pos(85,660);
    m_textSlider_robuste.set_message(std::to_string( (int) m_slider_robuste.get_value()));

    m_tool_box.add_child(m_k_sommet);
    m_k_sommet.set_dim(10,10);
    m_k_sommet.set_pos(3,630);
    m_tool_box.add_child(m_text_k_sommet);
    m_text_k_sommet.set_pos(18,630);
    m_text_k_sommet.set_message("k-vertex");

    m_tool_box.add_child(m_k_arete);
    m_k_arete.set_dim(10,10);
    m_k_arete.set_pos(3,645);
    m_tool_box.add_child(m_text_k_arete);
    m_text_k_arete.set_pos(18,645);
    m_text_k_arete.set_message("k-edge");

}



/********************************************************************************************

                            MISE A JOUR DONNEE (POIDS, CONSOMMATION, POPULATION

*********************************************************************************************/


/// Methode permettant de calculer en temps reel la quantit� de consommation d'un individu par rapport � ses adjacents
void Graph::init_consumption_Vertices()
{
    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        if(it->second.m_active)
        {
            float k(0.0);
            for(std::map<int, Edge>::iterator it2(m_edges.begin()); it2!=m_edges.end(); ++it2)
            {
                if( it2->second.m_active || it2->second.m_active2)
                    for(unsigned int i(0); i<it->second.m_in.size();++i)
                        if(m_vertices[it->second.m_in[i]].m_active)
                            k = k + ( it2->second.m_weight * m_vertices[it->second.m_in[i]].m_value );
                        if(it->second.m_in.size() == 0)
                            k = 50;
            }
            it->second.m_consumption = k;

            if(it->second.m_in.size() == 0 && it->second.m_event == 0)
                it->second.m_consumption = 1000;
        }
    }
}


///mise � jour des pop de chaques sommets en fonction des influences respectives
void Graph::evolution()
{
    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        if(it->second.m_active)
        {
            /// d�termination de la value du somet selon l'event
            int rapport_n_k = it->second.m_value/it->second.m_consumption;

            if(m_interface->m_event2.get_value() && it->second.m_event==2)
                it->second.m_growth = 0.4;

            if(m_interface->m_event.get_value() && it->second.m_event==1)
                it->second.m_value=it->second.m_value*(1+it->second.m_growth*(1-rapport_n_k))-(it->second.m_mortality+20);

            else
                it->second.m_value=it->second.m_value*(1+it->second.m_growth*(1-rapport_n_k))-it->second.m_mortality;

            for(std::map<int, Edge>::iterator it2(m_edges.begin()); it2!=m_edges.end(); ++it2)
            {
                if(it2->second.m_active || it2->second.m_active2)
                    for(unsigned int i(0); i<it->second.m_out.size();++i)
                        if(it->first==it2->second.m_from && it->second.m_out[i]==it2->second.m_to && m_vertices[it->second.m_out[i]].m_active )
                        {
                            // Calcule de la nouvelle population de l'indidivus selon ce que lui prend les autres
                            if(!m_vertices[it->second.m_out[i]].m_detrivore)
                                it->second.m_value = it->second.m_value - ( it2->second.m_weight*m_vertices[it->second.m_out[i]].m_value );

                            // Check les depecement de valeur
                            if( it->second.m_value < 0)
                             it->second.m_value=0;
                            if( it->second.m_value > 100)
                             it->second.m_value=100;
                        }
            }
        }
    }
}



/********************************************************************************************

                            SOUS FONCTION ADD AND DELETE VERTEX ET EDGE

*********************************************************************************************/



/// Delete le sommet cocher et le place dans la tool box
void Graph::deleteVertex(int i)
{
    /// Fonction permettant de : - DELETE un sommet
    ///                          - Detruire l'interface du sommet sur la main BOX
    ///                          - Ajoute le sommet dans la TOOLBOX afin de pouvoir le reADD plus tard
    if( (m_vertices[i].m_interface->m_select.get_value() && m_interface->m_buttonDel.clicked()) || m_vertices[i].m_saveSupp)
    {
        tab.push_back(i);

        m_vertices[i].m_active = false;
        m_vertices[i].m_saveSupp = false; // evite de tourner dans ce if pour les sommets DELETED
        m_vertices[i].m_interface->m_select.set_value(false);

        m_interface->m_main_box.remove_child(m_vertices[i].m_interface->m_top_box); // Remove

        m_interface->m_tool_box.add_child(m_vertices[i].m_interface->m_label_idx); // recre dans la toolbox
        m_vertices[i].m_interface->m_label_idx.set_message(m_vertices[i].m_namePicture);
        m_vertices[i].m_interface->m_label_idx.set_pos(toolX,(toolY+10)*tab.size());

        m_interface->m_tool_box.add_child(m_vertices[i].m_interface->m_select2);
        m_vertices[i].m_interface->m_select2.set_dim(10,10);
        m_vertices[i].m_interface->m_select2.set_pos(toolX-15,(toolY+10)*tab.size());
        m_vertices[i].m_interface->m_select2.set_value(false);
    }
}

/// Ajoute le sommet coch� dans la toolbox � sa place d'origine
void Graph::addVertex(int i)
{
    // Fonction permettant de : - Detruit l'interface du sommet dans la TOOLBOX
    //                          - Cree l'interface du sommet sur la main BOX
    if(m_vertices[i].m_interface->m_select2.get_value() && m_interface->m_buttonAdd.clicked())
    {
        for (unsigned int j(0); j<tab.size(); ++j)
            if(tab[j]==i)
                tab.erase(tab.begin()+j);

        for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
        {
            if(it->second.m_from == i || it->second.m_to == i)
            {
                it->second.m_active = false;
                it->second.m_active2 = false;
            }
        }

        m_vertices[i].m_active = true;
        m_vertices[i].m_interface->m_select2.set_value(false);
        m_interface->m_tool_box.remove_child(m_vertices[i].m_interface->m_label_idx);
        m_interface->m_tool_box.remove_child(m_vertices[i].m_interface->m_select2);
        add_interfaced_vertex(i,m_vertices[i].m_value,m_vertices[i].m_pos_x,m_vertices[i].m_pos_y,m_vertices[i].m_namePicture,0,m_vertices[i].m_growth);
    }
}

/// delete l'arete selectionner ou si les 1 des 2 sommets n'existe plus
void Graph::deleteEdges(int i)
{
    if( (m_edges[i].m_interface->m_select.get_value() && m_interface->m_buttonDel.clicked() && m_edges[i].m_active2) || m_edges[i].m_active3 )
    {
        m_edges[i].m_active2 = false;
        m_edges[i].m_active3 = false;
        m_edges[i].m_interface->m_select.set_value(false);
        tab.push_back(i);

        std::string name("");
        name = std::to_string((int)m_edges[i].m_from) + " -> " + std::to_string((int)m_edges[i].m_to);
        m_interface->m_main_box.remove_child(m_edges[i].m_interface->m_top_edge); // Remove

        m_interface->m_tool_box.add_child(m_edges[i].m_interface->m_select2);// Recree dans la toolbox
        m_edges[i].m_interface->m_select2.set_value(false);
        m_edges[i].m_interface->m_select2.set_dim(10,10);
        m_edges[i].m_interface->m_select2.set_pos(toolX-15,(toolY+10)*tab.size());

        m_interface->m_tool_box.add_child(m_edges[i].m_interface->m_name);
        m_edges[i].m_interface->m_name.set_message(name);
        m_edges[i].m_interface->m_name.set_pos(toolX, (toolY+10)*tab.size());
    }

    /// On enleve les aretes des sommets DELETED
    if( (!m_vertices[m_edges[i].m_from].m_active || !m_vertices[m_edges[i].m_to].m_active) && m_edges[i].m_active)
    {
        m_edges[i].m_active = false;
        m_interface->m_main_box.remove_child(m_edges[i].m_interface->m_top_edge);
    }
}

/// Ajoute une arete apres select
void Graph::addEdges(int i)
{
    if( (m_edges[i].m_interface->m_select2.get_value() && !m_edges[i].m_active2 && m_interface->m_buttonAdd.clicked() ) || (!m_edges[i].m_active2 && !m_edges[i].m_active ) )
    {
        if(m_vertices[m_edges[i].m_from].m_active && m_vertices[m_edges[i].m_to].m_active)
        {
            for (unsigned int j(0); j<tab.size(); ++j)
                if(tab[j]==i)
                    tab.erase(tab.begin()+j);
            m_edges[i].m_active2 = true;
            m_edges[i].m_active = true;
            m_edges[i].m_interface->m_select2.set_value(false);
            m_interface->m_tool_box.remove_child(m_edges[i].m_interface->m_name);
            m_interface->m_tool_box.remove_child(m_edges[i].m_interface->m_select2);
            add_interfaced_edge(i,m_edges[i].m_from,m_edges[i].m_to,m_edges[i].m_weight);
        }
    }

    if( (m_vertices[m_edges[i].m_from].m_active && m_vertices[m_edges[i].m_to].m_active) && !m_edges[i].m_active)
    {
        m_edges[i].m_active = true;
        add_interfaced_edge(i,m_edges[i].m_from, m_edges[i].m_to,m_edges[i].m_weight);
    }
}



/********************************************************************************************

                            GRAPHE UPDATE

*********************************************************************************************/




/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(int compteur_simulation)
{
    if (!m_interface)
        return;

    if(m_interface->m_simulation.get_value() && compteur_simulation%100 == 0)
    {
        init_consumption_Vertices();
        evolution();
    }


    for (auto &elt : m_vertices)
    {
        deleteVertex(elt.first);
        addVertex(elt.first);

        elt.second.pre_update();
    }

    for (auto &elt : m_edges)
    {
        addEdges(elt.first);
        deleteEdges(elt.first);

        if (elt.second.m_weight > 100)
            elt.second.m_weight = 100;

        elt.second.pre_update();
    }

    m_interface->m_top_box.update();
    update_connexite();

    for (auto &elt : m_vertices)
    {
        if (elt.second.m_active)
        {
            elt.second.post_update();
        }
    }

    for (auto &elt : m_edges)
    {
        elt.second.post_update();
    }
}

void Graph::update_connexite()
{
    //si on veut voir la forte connexit�
    if(m_interface->m_buttonFCC.clicked())
    {
        algo_forte_connexite();
        for(unsigned int i=0; i<m_comp.size(); ++i)
        {
            for(unsigned int j=0; j<m_comp[i].size(); j++)
            {
                for (auto &elt2 : m_vertices)
                {
                    if(elt2.first==m_comp[i][j])
                    {
                        elt2.second.m_interface->m_top_box.set_bg_color(colorChoice(i));        //on change les couleurs
                    }
                }
            }
        }
    }

    if( m_interface->m_button_robuste.clicked() && m_interface->m_k_arete.get_value())
        k_arete_conexe();

    if( m_interface->m_button_k_plet.clicked() && m_interface->m_k_sommet.get_value())
    {
        k_sommet_connexe();
    }

    //Reset des couleurs
    if(m_interface->m_reset.get_value())
    {
        if(!m_interface->m_k_sommet.get_value())
            for(std::map<int, Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
                it->second.m_interface->m_top_box.set_bg_color(BLANC);
    }

    /// Quand le boutton k_arete_connexit� n'est pas cocher
    if(!m_interface->m_k_arete.get_value())
        for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
            it->second.m_interface->m_top_edge.setEdgeColor(VERT);

}





/********************************************************************************************

                            ADD INTERFACE + MAKE EXEMPLE

*********************************************************************************************/



/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx, int growthcolor, bool active)
{
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx, growthcolor);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    m_vertices[idx].m_interface = vi;
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    // Cr�ation d'une interface d'arete
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2], weight);
    // Ajout de la top box de l'interface de l'arete
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx].m_interface = ei;
}



/********************************************************************************************

                            TABLEAU D'ADJACENCE

*********************************************************************************************/


///inititialisation du tableau d'adjacense
void Graph::initTabAdja()
{
    //on initialise le tableau d'adjacense et le tableau d'adjacense symetrique
    for(int w=0; w<m_ordre; w++)
    {
        m_adjacensePoids.push_back( std::vector<int> () );
        m_adjacensePoidsSymetrique.push_back(std::vector<int> ());
        for(int z=0; z<m_ordre; z++)
        {
            m_adjacensePoids[w].push_back(0);
            m_adjacensePoidsSymetrique[w].push_back(0);
        }
    }

    for(std::map<int, Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
    {
        for(std::map<int, Edge>::iterator it2 = m_edges.begin(); it2 != m_edges.end(); ++it2)
        {
            if(it2->second.m_from == it->first)
            {
                it->second.m_out.push_back(it2->second.m_to);
            }
            if(it2->second.m_to== it->first)
            {
                it->second.m_in.push_back(it2->second.m_from);
            }
        }
    }
    ///on donne les valeurs du fichier au tableau d'adjacense
    majTabAdja();
}

///mise a jour du tableau d'adjacense
void Graph::majTabAdja()
{
    //on donne les valeurs du fichier au tableau d'adjacense
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        m_adjacensePoids[it->second.m_from][it->second.m_to]=it->second.m_active;
    }

    //on donne les valeurs du fichier au tableau d'adjacense sym�trique
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        m_adjacensePoidsSymetrique[it->second.m_from][it->second.m_to]=it->second.m_active2;
        m_adjacensePoidsSymetrique[it->second.m_to][it->second.m_from]=it->second.m_active2;
    }
}





/********************************************************************************************

                            TELECHARGEMENT + SAUVEGARDE

*********************************************************************************************/

// Methode de telechargement des donn�es depuis un fichier
void Graph::load_graph(std::string nom_fichier)
{
    // Modife du nom de fichier pour fonction stream
    nom_fichier = nom_fichier + ".txt";
    std::ifstream fc(nom_fichier.c_str(), std::ios::in); // Ouverture

    // Condition en cas d'�chec
    if(!fc.fail())
    {
        // Recup nb sommet et nb arete
        fc >> m_ordre;
        fc >> m_nb_arete;

        // Rempli la map d'arete
        for(int i(0); i<m_nb_arete; ++i)
        {
            int recup(0);
            fc >> recup;

            m_edges.insert(std::pair<int,Edge>(recup,Edge(fc)));
        }

        // Rempli la map de sommet
        for(int i(0); i<m_ordre; ++i)
        {
            int recup(0);
            fc >> recup;
            m_vertices.insert(std::pair<int,Vertex>(recup,Vertex(fc)));
        }
    }

    else
    {
        std::cout << "Error lors du chargement du fichier !" << std::endl;
    }
}

void Graph::save_graph(std::string nom_fichier)
{
    // Modife du nom de fichier pour fonction stream
    nom_fichier = nom_fichier + ".txt";
    std::ofstream fc(nom_fichier.c_str(), std::ios::out); // Ouverture

    // Condition en cas d'�chec
    if(!fc.fail())
    {
        // Ecrit le nb sommet et nb arete
        fc << m_ordre << " ";
        fc << m_nb_arete << " ";

        // Ecrit la map EDGE dans le fichier
        for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
        {
            fc << it->first << " ";
            fc << it->second.m_from << " ";
            fc << it->second.m_to << " ";
            fc << it->second.m_weight << " ";
            fc << it->second.m_active << " ";
            fc << it->second.m_active2 << " ";
            if(!it->second.m_active2 || !it->second.m_active)
                fc << true << " ";
            else
                fc << it->second.m_active3 << " ";
        }

        // Ecrit la map VERTEX dans le fichier
        for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
        {
            fc << it->first;
            fc << it->second.m_namePicture << "#";
            fc << it->second.m_pos_x << " ";
            fc << it->second.m_pos_y << " ";
            fc << it->second.m_value << " ";
            fc << it->second.m_growth << " ";
            fc << it->second.m_mortality << " ";
            fc << it->second.m_active << " ";
            if(!it->second.m_active)
                fc << true << " ";
            else
                fc << it->second.m_saveSupp << " ";
            fc << it->second.m_event << " ";
            fc << it->second.m_detrivore << " ";
        }
    }

    else
    {
        std::cout << "Error lors du chargement du fichier !" << std::endl;
    }
}

///*******************************************************************************************************
///************************************** FORTE CONNEXITE ************************************************
///*******************************************************************************************************


///algo pour trouver la forte connexit� V2
void Graph::algo_forte_connexite()
{

    //initialisation
    int n=0;
    std::stack<int> pile;
    std::vector<int> pref;      //pour savoir si le sommet est marqu�
    std::vector<int> ret;
    std::vector<bool> dansPile;     //pour savoir si le sommet est dans la pile

    m_comp.erase(m_comp.begin(),m_comp.end());

    //initialisation des vecteurs
    for(int s=0; s<m_ordre; ++s)
    {
        pref.push_back(0);
        ret.push_back(0);
        dansPile.push_back(false);
    }

    //calcul de toutes les CFC
    for(int s=0; s<m_ordre; ++s)
    {
        if(pref[s]==0)
        {
            CFC(s,n,pile,pref,ret,dansPile);
        }
    }
}

void Graph::CFC(int x,int& n, std::stack<int>& pile, std::vector<int>& pref, std::vector<int>& ret, std::vector<bool> dansPile )
{
    //initialisation des variables
    int y;
    int m;

    pref[x]=n;      //on marque le sommet x;
    m=pref[x];
    n=n+1;
    pile.push(x);
    dansPile[x]=true;

    for (y=0; y<m_ordre; y++)
    {
        if(m_adjacensePoids[x][y])
        {
            if(pref[y]==0)
            {
                CFC(y,n,pile,pref,ret,dansPile);     //parcours en profondeur r�cursif

                if(pref[x]<ret[y])
                    m=pref[x];
                else
                    m=ret[y];
            }
            else if(dansPile[y])
            {
                if(m<pref[y])
                    m=m;
                else
                    m=pref[y];
            }
        }
    }

    ret[x]=m;
    if(m==pref[x])     //si x est une racine, alors on calcul la composante fortement connexe associ�e
    {
        std::vector<int> unecompconnexe;
        do
        {
            y=pile.top();
            pile.pop();
            dansPile[y]=false;
            unecompconnexe.push_back(y);
        }
        while(x!=y);
        m_comp.push_back(unecompconnexe);
    }

}

// Colorie les sommets en fonction de leur forte connexit�
int Graph::colorChoice(int nb_color)
{
    if(nb_color==0)
        return BLEU;
    else if(nb_color==1)
        return JAUNE;
    else if(nb_color==2)
        return ROUGE;
    else if(nb_color==3)
        return VIOLET;
    else if(nb_color==4)
        return MARRON;
    else if(nb_color==5)
        return VERT;
    else if(nb_color==6)
        return BLEUCLAIR;
    else if(nb_color==7)
        return JAUNECLAIR;
    else if(nb_color==8)
        return ROUGECLAIR;
    else if(nb_color==9)
        return VIOLETCLAIR;
    else if(nb_color==10)
        return MARRONCLAIR;
    else if(nb_color==11)
        return VERTCLAIR;
    else
        return GRIS;
}


///*************************************************************************************************
///************************************* ALGO K-ARETE-CONNEXE **************************************
///*************************************************************************************************


void Graph::k_arete_conexe()
{
    srand(time(NULL));  //initialise aleatoire
    std::vector<Edge> arbre;   //cree un vecteur local qui permettra de stocker l'arbre fabriqu� au fur a mesure par l'algo de prim
    int indiceA =0;
    std::vector <int> marques;  //tableau pour savoir si un sommet est marqu�
    int s(0);
    int x(0);
    int y(0);
    int ymin(0);
    int xmin(0);    // variable utile
    double mini(0);

    for(int i=0; i<m_ordre-1; i++)       //initialisation du vecteur arbre
    {
        arbre.push_back(Edge());
    }
    for(int i=0; i<m_ordre; i++)     //initialisation du vecteur marques
    {
        marques.push_back(0);
    }

    //on choisit aleatoirement le sommet de depart
    s=rand()%m_ordre;

    //marque le sommet choisis aleatoirement � 1
    marques[s]=1;

    //tant que les aretes de l'arbre ne sont pas toutes trait�s
    while(indiceA<m_ordre-1)
    {
        //on initialise la longueur minimale a l'infini
        mini=10000;

        //pour tous les sommets x marques
        //chercher le sommet de longueur minimale "ymin" adjacent a x et non marqu�
        for(x=0; x<m_ordre; x++)
        {
            if (marques[x]==1)
            {
                for(y=0; y<m_ordre; y++)
                {
                    if((m_adjacensePoidsSymetrique[x][y]!=0)&&(m_adjacensePoidsSymetrique[x][y]!=100)&&(marques[y]!=1)&&(m_adjacensePoidsSymetrique[x][y]<mini))
                    {
                        for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
                            if(y == it->second.m_to && x == it->second.m_from)
                                mini=it->second.m_weight;
                        ymin=y;
                        xmin=x;
                    }
                }
            }

        }
        marques[ymin]=1;
        arbre[indiceA].m_weight=mini;
        arbre[indiceA].m_from=xmin;
        arbre[indiceA].m_to=ymin;
        indiceA++;
    }
    for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
    {
        bool ok(false);
        for (auto elem: arbre)
            if(elem.m_from == it->second.m_from && elem.m_to == it->second.m_to)
                ok=true;
        if(!ok)
            it->second.m_interface->m_top_edge.setEdgeColor(JAUNE);
    }

    m_interface->m_slider_robuste.set_value(m_nb_arete - arbre.size());
    m_interface->m_textSlider_robuste.set_message(std::to_string(m_nb_arete - arbre.size()));
}

///******************************************************************************************************
///************************************ ALGO k-SOMMET-CONNEXE *******************************************
///******************************************************************************************************

void Graph::k_sommet_connexe()
{
    std::vector<std::vector<int>> tab;

    ///variables pour r�gler la k-sommet-connexit�
    int k = 2;
    int n = m_ordre-k;

    //calcul et affichage des k-plets sommets
    for (int i = 0; i < n; ++i)
    {
        m_people.push_back(i+1);
    }
    go(0, k);

    std::cout<<std::endl<<std::endl;
    for(unsigned int z=0; z<m_kplet.size(); ++z)    //pour chaque k-plet
    {
        tab = m_adjacensePoidsSymetrique;       //on fait une copie du tableau d'adjacense pour pouvoir le manipuler sans le modifier
        for(unsigned int h=0; h<m_kplet[z].size(); ++h)     //on supprime tous les sommets du k-plet
        {
            //suppresion colone + ligne numero k
            for(unsigned int i=0; i< m_adjacensePoidsSymetrique.size()-h; ++i) /// modif j'ai enlev� le [i] de m_adjacensePoidsSymetrique -> aucun sens
            {
                tab[i].erase(tab[i].begin()+m_kplet[z][h]);
            }
            tab.erase(tab.begin()+m_kplet[z][h]);       //on supprime le sommet que l'on test
        }
        grapheConnexe(tab,z);     //on appel la fonction qui d�termine si ce nouveau graphe (sans k) est connexe

    }

    m_kplet.erase(m_kplet.begin(),m_kplet.end());
    m_people.erase(m_people.begin(),m_people.end());
    m_interface->m_slider_robuste.set_value(k);
    m_interface->m_textSlider_robuste.set_message(std::to_string(k));
}


///fonction pour tester la connexit� d'un graphe � partir de sa matrice d'adjacense
bool Graph::grapheConnexe(std::vector<std::vector<int>> tab, int z)
{
    int s=0;
    bool connexe=true; //bool�en retourn� � la fin de la fonction
    std::vector<int> marques;   //vecteur pour savoir si un sommet est marqu�s
    for(unsigned int i=0; i< tab.size(); ++i)   //initialisation du vecteur marques
        marques.push_back(0);

    ///Parcours en profnodeur (BFS)
    std::queue<int> file;
    file.push(s);   //on enfile s
    marques[s]=1;   //on marques s

    while (!file.empty())
    {
        s= file.front();
        file.pop();

        for (unsigned int t=0; t<tab.size(); ++t)       //parcours tous les sommets adjacents
        {
            if (!marques[t]&&tab[s][t])        //si le sommet adjacent n'est pas marqu�s
            {
                file.push(t);       //on enfile t
                marques[t]=true;      //et on le marque
            }
        }
    }

    for (auto elem : marques)
    {
        if (elem==false)
            connexe=false;      //si tous les sommets n'ont pas �t� marqu�s -> le graphe n'est pas connexe
    }

    if (!connexe)
    {
        std::cout<<"Le K-plet suivant brise la connexite du graphe : ";
        for (unsigned int i(0); i<m_kplet[z].size(); ++i)
        {
            std::cout << m_kplet[z][i] << " ";
            m_vertices[ m_kplet[z][i] ].m_interface->m_top_box.set_bg_color(ORANGE);
        }
        std::cout << std::endl;
    }
    return connexe;     //retourne si le graphe est connexe (true) ou s'il ne l'est pas (false)
}


///code pris sur ce site : https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c
///*************************************************************************************************************************
///Il permet de calculer les K-plets de sommets


void Graph::pretty_print(const std::vector<int>& v)
{
    static int count = 0;
    std::cout << "combination no " << (++count) << ": [ ";
    for (unsigned int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << "] " << std::endl;
}

void Graph::go(int offset, int k)
{
    //vecteur tempon
    std::vector<int> temp;
//    bool test;
    if (k == 0)
    {
        //pour afficher les k_plets :
        pretty_print(m_combination);
        return;
    }

    for (unsigned int i = offset; i <= m_people.size() - k ; ++i)
    {
//        test=true;
        m_combination.push_back(m_people[i]);
        go(i+1, k-1);
        temp=m_combination;
        std::reverse(temp.begin(),temp.end());
//        for(unsigned int j(0); j<temp.size(); ++j)
//        {
//            if(!m_vertices[temp[j]].m_active)
//            {
//                test=false;
//            }
//        }
//
//        if(test)
//        {
            m_kplet.push_back(temp);
//            test=false;
//        }
        m_combination.pop_back();
    }
}
