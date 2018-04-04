#include "graph.h"

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
    m_interface=nullptr;
    m_growth = 6;
}

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx,int growth)
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
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // CHANGE LA RANGE DES SLIDERS DES ARETES
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}



/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    if(m_weight>=90&&m_weight<100)
        m_interface->m_top_edge.setEdgeColor(NOIR);
    if(m_weight>=80&&m_weight<90)
        m_interface->m_top_edge.setEdgeColor(GRISSOMBRE);
    if(m_weight>=70&&m_weight<80)
        m_interface->m_top_edge.setEdgeColor(GRIS);
    if(m_weight>=60&&m_weight<70)
        m_interface->m_top_edge.setEdgeColor(GRISCLAIR);
    if(m_weight>=50&&m_weight<60)
        m_interface->m_top_edge.setEdgeColor(ROUGESOMBRE);
    if(m_weight>=40&&m_weight<50)
        m_interface->m_top_edge.setEdgeColor(ROUGE);
    if(m_weight>=30&&m_weight<40)
        m_interface->m_top_edge.setEdgeColor(ROUGECLAIR);
    if(m_weight>=20&&m_weight<30)
        m_interface->m_top_edge.setEdgeColor(VERTSOMBRE);
    if(m_weight>=10&&m_weight<20)
        m_interface->m_top_edge.setEdgeColor(VERT);
    if(m_weight>=0&&m_weight<10)
        m_interface->m_top_edge.setEdgeColor(VERTCLAIR);

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
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_img.set_pic_name("coupe_paysage.jpg");
    m_main_box.add_child(m_img);
    // m_main_box.set_bg_color(BLANCJAUNE);
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
    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...

    for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
    {
        add_interfaced_edge(it->first, it->second.m_from, it->second.m_to, it->second.m_weight);
    }
}

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
        }
    }

    else
    {
        std::cout << "Error lors du chargement du fichier !" << std::endl;
    }
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
    {
        elt.second.m_weight = m_vertices[elt.second.m_to].m_hunger * m_vertices[elt.second.m_to].m_value;
        if (elt.second.m_weight > 100)
            elt.second.m_weight = 100;
        elt.second.pre_update();
    }

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx, int growthcolor)

{
//    if ( m_vertices.find(idx)!=m_vertices.end() )
//    {
//        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
//        throw "Error adding vertex";
//    }

    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx, growthcolor);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    m_vertices[idx].m_interface = vi;

    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    ///m_vertices[idx] = Vertex(value,vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
//    if ( m_edges.find(idx)!=m_edges.end() )
//    {
//        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
//        throw "Error adding edge";
//    }
//
//    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
//    {
//        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
//        throw "Error adding edge";
//    }

    // Création d'une interface d'arete
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2], weight);
    // Ajout de la top box de l'interface de l'arete
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx].m_interface = ei;
    //m_edges[idx] = Edge(weight, ei);
}

