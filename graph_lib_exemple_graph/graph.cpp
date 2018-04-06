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
    fc >> m_hunger;
    fc >> m_active;
    fc >> m_saveSupp;
    m_interface=nullptr;
    m_growth = 6;
}

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx,int growth, bool active)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100); // Change la taille des box des SOMMETS
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

    if(growth==0)
        m_top_box.set_bg_color(ROUGESOMBRE);
    if(growth==1)
        m_top_box.set_bg_color(ROUGE);
    if(growth==2)
        m_top_box.set_bg_color(ROUGECLAIR);
    if(growth==3)
        m_top_box.set_bg_color(VERTSOMBRE);
    if(growth==4)
        m_top_box.set_bg_color(VERT);
    if(growth==5)
        m_top_box.set_bg_color(VERTCLAIR);
    if(growth>=6)
        m_top_box.set_bg_color(BLANC);
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
    m_box_edge.set_dim(32,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    m_box_edge.add_child(m_select);
    m_select.set_dim(5,5);
    m_select.set_bg_color(BLANCBLEU);
    m_select.set_gravity_xy(grman::GravityX::Left,grman::GravityY::Down);


    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // CHANGE LA RANGE DES SLIDERS DES ARETES
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_xy(grman::GravityX::Right,grman::GravityY::Down);

}



/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    if(m_weight>=90&&m_weight<100)
        m_interface->m_top_edge.setEdgeColor(ROUGESOMBRE);
    if(m_weight>=80&&m_weight<90)
        m_interface->m_top_edge.setEdgeColor(ROUGE);
    if(m_weight>=70&&m_weight<80)
        m_interface->m_top_edge.setEdgeColor(ROUGECLAIR);
    if(m_weight>=60&&m_weight<70)
        m_interface->m_top_edge.setEdgeColor(ORANGESOMBRE);
    if(m_weight>=50&&m_weight<60)
        m_interface->m_top_edge.setEdgeColor(ORANGE);
    if(m_weight>=40&&m_weight<50)
        m_interface->m_top_edge.setEdgeColor(ORANGECLAIR);
    if(m_weight>=30&&m_weight<40)
        m_interface->m_top_edge.setEdgeColor(JAUNESOMBRE);
    if(m_weight>=20&&m_weight<30)
        m_interface->m_top_edge.setEdgeColor(JAUNE);
    if(m_weight>=10&&m_weight<20)
        m_interface->m_top_edge.setEdgeColor(JAUNECLAIR);
    if(m_weight>=0&&m_weight<10)
        m_interface->m_top_edge.setEdgeColor(VERT);

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );


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

    // MAIN BOX
    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.add_child(m_img);
    m_img.set_pic_name("coupe_paysage.jpg");
}



/********************************************************************************************

                            MISE A JOUR DONNEE (POIDS, CONSOMMATION, POPULATION

*********************************************************************************************/




/// Methode permettant de calculer en temps reel la quantité de consommation d'un individu par rapport à ses adjacents
void Graph::init_consumption_Vertices()
{
    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        //le k du mangeur
        float k_sommet(0);

            for(unsigned int i(0); i<it->second.m_out.size(); ++i)
            {
                for(std::map<int, Edge>::iterator it2(m_edges.begin()); it2!=m_edges.end(); ++it2)
                {
                    //si on retrouve le sommet it2 dans les successeurs de it + arete corespondante
                    if(it->first == it2->second.m_from)
                    {
                        //k_sommet = k_sommet + ( (it->second.m_hunger/it->second.m_out.size()) * it2->second.m_value);
                        k_sommet = k_sommet + (it2->second.m_weight*m_vertices[it2->second.m_to].m_value);
                    }
                }
            }
            it->second.m_consumption = k_sommet;
            //std::cout<<"le k du sommet : "<<it->first<<" est "<<it->second.m_consumption<<std::endl<<std::endl;

    }
}

///mise à jour des pop de chaques sommets en fonction des influences respectives
/*void Graph::evolution()
{
    ///pour chaque sommet
    for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
    {
        ///le quotient population/capacité de portage
        int rapport_n_k = it->second.m_value/it->second.m_consumption;

        std::cout << it->first << " K = " << it->second.m_consumption << std::endl;

        ///N=n*[1+r*n*(1-n/K)] avec N = nbre à t+1 et n = nbre à t
        it->second.m_value=it->second.m_value*(1+it->second.m_growth*(1-rapport_n_k));

        for(unsigned int i=0; i<it->second.m_out.size(); i++)
        {
            for(std::map<int, Vertex>::iterator it2(m_vertices.begin()); it2!=m_vertices.end(); ++it2)
            {
                if(it2->first == it->second.m_out[i])
                {
                    ///regulation de la population en fonction des prédateurs
                    it->second.m_value-=m_vertices[it->second.m_out[i]].m_value*it2->second.m_value;
                    std::cout << it->first << "Pop = " << it->second.m_value << std::endl;
                }
            }
        }
    }

}*/





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
        for (unsigned int j(0); j<tab.size();++j)
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
    if( (m_edges[i].m_interface->m_select2.get_value() && !m_edges[i].m_active2 && m_interface->m_buttonAdd.clicked() ) || !m_edges[i].m_active2 && !m_edges[i].m_active )
    {
        if(m_vertices[m_edges[i].m_from].m_active && m_vertices[m_edges[i].m_to].m_active)
        {
            for (int j(0); j<tab.size();++j)
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
void Graph::update()
{
    if (!m_interface)
        return;

    init_consumption_Vertices();

    //evolution();

    for (auto &elt : m_vertices)
    {
        deleteVertex(elt.first);
        addVertex(elt.first);

        // Permet d'init en permanence les slider de chaque sommet
        if(elt.second.m_interface->m_select.get_value() && m_interface->m_selectSlider.get_value())
            elt.second.m_value= m_interface->m_slider.get_value();

        elt.second.pre_update();
    }


    for (auto &elt : m_edges)
    {
        addEdges(elt.first);
        deleteEdges(elt.first);

        // MAJ des poids des aretes
        elt.second.m_weight = m_vertices[elt.second.m_to].m_hunger * m_vertices[elt.second.m_to].m_value;
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




void Graph::initTabAdja()
{
    ///on initialise le tableau d'adjacense
    for(int w=0; w<m_ordre; w++)
    {
        m_adjacensePoids.push_back( std::vector<int> () );
        for(int z=0; z<m_ordre; z++)
        {
            m_adjacensePoids[w].push_back(0);
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
    for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); ++it)
    {
        m_adjacensePoids[it->second.m_from][it->second.m_to]=it->second.m_weight;
        m_adjacensePoids[it->second.m_to][it->second.m_from]=it->second.m_weight;
    }

    ///on affiche le tableau d'adjacense
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

    for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
    {
        it->second.m_weight=m_vertices[it->second.m_to].m_value*m_vertices[it->second.m_to].m_hunger;
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
            fc << it->second.m_hunger << " ";
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
