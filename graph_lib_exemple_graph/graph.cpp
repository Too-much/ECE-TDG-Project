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
    m_interface=nullptr;
}

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx,int growth, bool active)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100); // Change la taille des box des SOMMETS
    m_top_box.set_bg_color(BLANC);
    m_top_box.set_moveable();


    // Le slider de réglage de valeur
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

    // Création d'un boutton sélectionner ou non en haut à gauche de chaque sommet
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

/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

    m_pos_x = m_interface->m_top_box.get_posx();
    m_pos_y = m_interface->m_top_box.get_posy();
    m_value = m_interface->m_slider_value.get_value();

}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

///Constructor par défaut de Edge
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

/// Le constructeur met en place les éléments de l'interface
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

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(32,70);
    m_box_edge.set_bg_color(BLANCBLEU);

    m_box_edge.add_child(m_select);
    m_select.set_dim(10,5);
    m_select.set_bg_color(BLANCBLEU);
    m_select.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Up);


    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 1.0);  // CHANGE LA RANGE DES SLIDERS DES ARETES
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Center);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Down);
}



/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;


    if(m_weight>=0.9&&m_weight<1)
        m_interface->m_top_edge.setEdgeColor(ROUGESOMBRE);
    if(m_weight>=0.80&&m_weight<0.90)
        m_interface->m_top_edge.setEdgeColor(ROUGE);
    if(m_weight>=0.70&&m_weight<0.80)
        m_interface->m_top_edge.setEdgeColor(ROUGECLAIR);
    if(m_weight>=0.60&&m_weight<0.70)
        m_interface->m_top_edge.setEdgeColor(ORANGESOMBRE);
    if(m_weight>=0.50&&m_weight<0.60)
        m_interface->m_top_edge.setEdgeColor(ORANGE);
    if(m_weight>=0.40&&m_weight<0.50)
        m_interface->m_top_edge.setEdgeColor(ORANGECLAIR);
    if(m_weight>=0.30&&m_weight<0.40)
        m_interface->m_top_edge.setEdgeColor(JAUNESOMBRE);
    if(m_weight>=0.20&&m_weight<0.30)
        m_interface->m_top_edge.setEdgeColor(JAUNE);
    if(m_weight>=0.10&&m_weight<0.20)
        m_interface->m_top_edge.setEdgeColor(JAUNECLAIR);
    if(m_weight>=0&&m_weight<0.10)
        m_interface->m_top_edge.setEdgeColor(VERT);

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value( m_weight);

    std::string sendNumber("");
    if(m_weight < 0.1)
        sendNumber = "0.0" + std::to_string((int) (m_weight*100) );
    else
        sendNumber = "0." + std::to_string((int) (m_weight*100) ) ;
    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message(sendNumber);
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH INIT
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1020,760);
    m_top_box.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(100,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    // CREATION DU SLIDER SELECT
    m_tool_box.add_child(m_slider);   //on crée un slider dans la dans la seconde boite qui est dans la toolbox
    m_slider.set_range(0.0, 100.0);
    m_slider.set_dim(16,40);
    m_slider.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Down);
    m_slider.set_bg_color(JAUNESOMBRE);
    m_tool_box.add_child(m_selectSlider);
    m_selectSlider.set_dim(30,30);
    m_selectSlider.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Down);

    // Label de visualisation de valeur de slider select
    m_slider.add_child( m_textSlider );
    m_textSlider.set_gravity_y(grman::GravityY::Down);
    m_textSlider.set_message(std::to_string(m_slider.get_value()));

    // BUTTON DE SUPPRESSION DE SOMMET
    m_tool_box.add_child(m_buttonDel);
    m_buttonDel.set_dim(35,30);
    m_buttonDel.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Center);
    m_buttonDel.set_bg_color(ROUGE);
    m_buttonDel.add_child(m_buttonDel_label);
    m_buttonDel_label.set_message("DEL");

    //BUTTON D'AJOUT DE SOMMET
    m_tool_box.add_child(m_buttonAdd);
    m_buttonAdd.set_dim(35,30);
    m_buttonAdd.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Center);
    m_buttonAdd.set_bg_color(VERT);
    m_buttonAdd.add_child(m_buttonAdd_label);
    m_buttonAdd_label.set_message("ADD");


    m_tool_box.add_child(m_simulation);
    m_simulation.set_dim(50,50);
    m_simulation.set_bg_color(GRISCLAIR);
    m_simulation.set_pos(25,400);
    m_tool_box.add_child(m_textSimulation);
    m_textSimulation.set_message("-> PLAY");
    m_textSimulation.set_pos(20,460);

    // MAIN BOX
    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLEU);
    m_main_box.add_child(m_img);
    m_img.set_pic_name("coupe_paysage.jpg");

    //bouton pour la forte connexité
    m_tool_box.add_child(m_buttonFCC);
    m_buttonFCC.set_dim(94,50);
    m_buttonFCC.set_bg_color(GRISCLAIR);
    m_buttonFCC.set_pos(3,520);
    m_buttonFCC.add_child(m_textFCC);
    m_textFCC.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Center);
    m_textFCC.set_message("Voir les FCC");
}



/********************************************************************************************

                            MISE A JOUR DONNEE (POIDS, CONSOMMATION, POPULATION

*********************************************************************************************/


/// Methode permettant de calculer en temps reel la quantité de consommation d'un individu par rapport à ses adjacents
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
            }
            it->second.m_consumption = k;

            if(it->second.m_in.size() == 0)
                it->second.m_consumption = 1000;
        }
    }
}


///mise à jour des pop de chaques sommets en fonction des influences respectives
void Graph::evolution()
{
    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        if(it->second.m_active)
        {
            int rapport_n_k = it->second.m_value/it->second.m_consumption;
            it->second.m_value=it->second.m_value*(1+it->second.m_growth*(1-rapport_n_k))-it->second.m_mortality;
            for(std::map<int, Edge>::iterator it2(m_edges.begin()); it2!=m_edges.end(); ++it2)
            {
                if(it2->second.m_active || it2->second.m_active2)
                    for(unsigned int i(0); i<it->second.m_out.size();++i)
                        if(it->first==it2->second.m_from && it->second.m_out[i]==it2->second.m_to && m_vertices[it->second.m_out[i]].m_active )
                        {
                            it->second.m_value = it->second.m_value - ( it2->second.m_weight*m_vertices[it->second.m_out[i]].m_value );
                            if( it->second.m_value < 0)
                             it->second.m_value=0;
                            if( it->second.m_value > 100)
                             it->second.m_value=100;

                            std::cout << it->first << " : " << it->second.m_value << std::endl;
                        }
            }
        }
    }
}



/********************************************************************************************

                            SOUS FONCTION ADD AND DELETE VERTEX ET EDGE

*********************************************************************************************/




void Graph::deleteVertex(int i)
{
    // Fonction permettant de : - DELETE un sommet
    //                          - Detruire l'interface du sommet sur la main BOX
    //                          - Ajoute le sommet dans la TOOLBOX afin de pouvoir le reADD plus tard
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

// delete l'arete selectionner ou si les 1 des 2 sommets n'existe plus
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

    // On enleve les aretes des sommets DELETED
    if( (!m_vertices[m_edges[i].m_from].m_active || !m_vertices[m_edges[i].m_to].m_active) && m_edges[i].m_active)
    {
        m_edges[i].m_active = false;
        m_interface->m_main_box.remove_child(m_edges[i].m_interface->m_top_edge);
    }
}

// Ajoute une arete apres select
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




/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
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

        // Permet d'init en permanence les slider de chaque sommet
        if(elt.second.m_interface->m_select.get_value() && m_interface->m_selectSlider.get_value())
            elt.second.m_value= m_interface->m_slider.get_value();

        elt.second.pre_update();
    }

    //si on veut voir la forte connexité
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


    for (auto &elt : m_edges)
    {
        addEdges(elt.first);
        deleteEdges(elt.first);

        if (elt.second.m_weight > 100)
            elt.second.m_weight = 100;

        elt.second.pre_update();
    }

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
    {
        if (elt.second.m_active)
        {
            m_interface->m_textSlider.set_message(std::to_string((int)m_interface->m_slider.get_value()));
            elt.second.post_update();
        }
    }

    for (auto &elt : m_edges)
    {
        elt.second.post_update();
    }
}





/********************************************************************************************

                            ADD INTERFACE + MAKE EXEMPLE

*********************************************************************************************/



/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx, int growthcolor, bool active)

{
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx, growthcolor);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    m_vertices[idx].m_interface = vi;

}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    // Création d'une interface d'arete
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2], weight);
    // Ajout de la top box de l'interface de l'arete
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx].m_interface = ei;
}

/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        add_interfaced_vertex(it->first, it->second.m_value, it->second.m_pos_x, it->second.m_pos_y, it->second.m_namePicture,0,it->second.m_growth);
    }

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...

    for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
    {
        add_interfaced_edge(it->first, it->second.m_from, it->second.m_to, it->second.m_weight);
    }
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

    //on donne les valeurs du fichier au tableau d'adjacense
    majTabAdja();

    //on affiche le tableau d'adjacense
    std::cout<<"Tableau d'adjacense : "<<std::endl;
    for(int w=0; w<m_ordre; w++)
    {
        std::cout<<std::endl;
        for(int z=0; z<m_ordre; z++)
        {
            std::cout<<m_adjacensePoids[w][z] <<" ";
        }
    }
    std::cout<<std::endl<<std::endl<<std::endl;

    // VERIFE
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        std::cout << "clef " << it->first << " Présentation : " << it->second.m_from << " " << it->second.m_to << " " << it->second.m_weight << std::endl;
    }

    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        std::cout << it->second.m_namePicture << " " << it->second.m_pos_x << " " << it->second.m_pos_y << " " << it->second.m_value << " // ";
    }


}

///mise a jour du tableau d'adjacense
void Graph::majTabAdja()
{
    //on donne les valeurs du fichier au tableau d'adjacense
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        m_adjacensePoids[it->second.m_from][it->second.m_to]=it->second.m_active;
    }

    //on donne les valeurs du fichier au tableau d'adjacense symétrique
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        m_adjacensePoidsSymetrique[it->second.m_from][it->second.m_to]=it->second.m_active2;
        m_adjacensePoidsSymetrique[it->second.m_to][it->second.m_from]=it->second.m_active2;
    }
}





/********************************************************************************************

                            TELECHARGEMENT + SAUVEGARDE

*********************************************************************************************/

// Methode de telechargement des données depuis un fichier
void Graph::load_graph(std::string nom_fichier)
{
    // Modife du nom de fichier pour fonction stream
    nom_fichier = nom_fichier + ".txt";
    std::ifstream fc(nom_fichier.c_str(), std::ios::in); // Ouverture

    // Condition en cas d'échec
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

    // Condition en cas d'échec
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
            if(!it->second.m_active2 && it->second.m_active)
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


///algo pour trouver la forte connexité V2
void Graph::algo_forte_connexite()
{

    //initialisation
    int n=0;
    std::stack<int> pile;
    std::vector<int> pref;      //pour savoir si le sommet est marqué
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


    //affichage des composants fortements connexes
    std::cout<<std::endl<<"Tableau des composantes fortement connexes : "<<std::endl;
    for(unsigned int w=0; w<m_comp.size(); w++)
    {
        for(unsigned int z=0; z<m_comp[w].size(); z++)
        {
            std::cout<<m_comp[w][z] <<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl<<std::endl;
}

void Graph::CFC(int x,int& n, std::stack<int>& pile, std::vector<int>& pref, std::vector<int>& ret, std::vector<bool> dansPile )
{
    //initialisation des variables
    int y;
    int m;
    //std::cout<<"X : "<<x<<std::endl;

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
                CFC(y,n,pile,pref,ret,dansPile);     //parcours en profondeur récursif

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
    if(m==pref[x])     //si x est une racine, alors on calcul la composante fortement connexe associée
    {
        std::vector<int> unecompconnexe;
        do
        {
            y=pile.top();
            pile.pop();
            dansPile[y]=false;
            unecompconnexe.push_back(y);
            // std::cout<<"Y : "<<y<<std::endl;
        }
        while(x!=y);
        m_comp.push_back(unecompconnexe);
    }

}


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
    std::vector<Edge> arbre;   //cree un vecteur local qui permettra de stocker l'arbre fabriqué au fur a mesure par l'algo de prim
    int indiceA =0;
    std::vector <int> marques;  //tableau pour savoir si un sommet est marqué
    int s,x,y,ymin,xmin;    // variable utile
    int mini;



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

    //marque le sommet choisis aleatoirement à 1
    marques[s]=1;

    //tant que les aretes de l'arbre ne sont pas toutes traités
    while(indiceA<m_ordre-1)
    {
        //on initialise la longueur minimale a l'infini
        mini=10000;

        //pour tous les sommets x marques
        //chercher le sommet de longueur minimale "ymin" adjacent a x et non marqué
        for(x=0; x<m_ordre; x++)
        {
            if (marques[x]==1)
            {
                for(y=0; y<m_ordre; y++)
                {
                    if((m_adjacensePoidsSymetrique[x][y]!=0)&&(m_adjacensePoidsSymetrique[x][y]!=100)&&(marques[y]!=1)&&(m_adjacensePoidsSymetrique[x][y]<mini))
                    {
                        mini=m_adjacensePoidsSymetrique[x][y];
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

    ///on affiche le resultat
    std::cout<<"Voici le resultat de l'algorithme de Prim (arbre recouvrant de plus petit poids) a partir du sommet "<<s<<std::endl<<std::endl;
    for (auto elem: arbre)
    {
        std::cout<<"Arete "<<elem.m_from<<" - "<<elem.m_to<<"        de poids      "<<elem.m_weight<<std::endl;
    }

    std::cout<<std::endl<<"Le graphe est "<<arbre.size()<<"-arete-connexe."<<std::endl;

}

///******************************************************************************************************
///************************************ ALGO k-SOMMET-CONNEXE *******************************************
///******************************************************************************************************

void Graph::k_sommet_connexe()
{
    std::vector<std::vector<int>> tab;

    for(unsigned int k=0; k<m_adjacensePoidsSymetrique.size(); k++)
    {
        std::cout<<"On supprime le sommet : "<<k<< " ";
        tab = m_adjacensePoidsSymetrique;       //on fait une copie du tableau d'adjacense pour pouvoir le manipuler sans le modifier
        for(unsigned int i=0; i< m_adjacensePoidsSymetrique[k].size(); i++)
            tab[i].erase(tab[i].begin()+k);

        tab.erase(tab.begin()+k);       //on supprime le sommet que l'on test
        grapheConnexe(tab,k);     //on appel la fonction qui détermine si ce nouveau graphe (sans k) est connexe
    }
}


///fonction pour tester la connexité d'un graphe à partir de sa matrice d'adjacense
bool Graph::grapheConnexe(std::vector<std::vector<int>> tab, int s)
{
    if (s!=0)
        s=0;
    else
        s=1;
    bool connexe=true; //booléen retourné à la fin de la fonction
    std::vector<int> marques;   //vecteur pour savoir si un sommet est marqués
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

        for (int t=0; t<m_ordre-1; ++t)       //parcours tous les sommets adjacents
        {
            if (!marques[t]&&tab[s][t])        //si le sommet adjacent n'est pas marqués
            {
                file.push(t);       //on enfile t
                marques[t]=true;      //et on le marque
            }
        }
    }

    for (auto elem : marques)
    {
        if (elem==false)
            connexe=false;      //si tous les sommets n'ont pas été marqués -> le graphe n'est pas connexe
    }


    for (auto elem : marques)
    {
        std::cout<<elem<<" - ";
    }
    if (!connexe)
        std::cout<<"Le graphe n'est pas connexe"<<std::endl;
    else
        std::cout<<"Le graphe est connexe"<<std::endl;
    return connexe;     //retourne si le graphe est connexe (true) ou s'il ne l'est pas (false)
}
