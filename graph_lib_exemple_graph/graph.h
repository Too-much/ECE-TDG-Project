#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont propos�es 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Ar�te ou Arc)
            Graph (=Graphe)

    Les ar�tes et les sommets et le graphe qu'ils constituent
    "travaillent" �troitement ensemble : pour cette raison les
    Vertex et Edge se d�clarent amis (friend) de Graph pour que
    ce dernier puisse librement acc�der aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent �tres compl�t�es. Il est �galement possible
    de les d�river mais il est malheureusement assez difficile
    de d�river le "triplet" des 3 classes en maintenant des relations
    coh�rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute pr�f�rable, si possible,
    de "customiser" ces classes de base directement, sans h�ritage.

    Le mod�le propos� permet de repr�senter un graphe orient� �ventuellement
    pond�r�, les arcs portent une ou des informations suppl�mentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse �tre assez facilement
    lue et �crite en fichier, et b�n�ficie d'une bonne lisibilit� en cas de bugs...

    Chaque arc poss�de 2 attributs principaux (en plus de son �ventuelle pond�ration)
        -> m_from (indice du sommet de d�part de l'arc )
        -> m_to (indice du sommet d'arriv�e de l'arc )

    Chaque sommet poss�de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : acc�s aux successeurs)

    Cependant le probl�me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entit� (un arc et/ou un sommet sont enlev�s du graphe) alors :

    - Soit il faut reprendre toute la num�rotation pour "boucher le trou"
      (par exemple on a supprim� le sommet n�4, le sommet n�5 devient le 4, 6 devient 5 etc...)
      ce qui pose des probl�mes de stabilit� et de coh�rence, et une difficult� � r�-introduire
      le(s) m�me(s) �l�ment supprim� (au m�me indice)

    - Soit on admet que la num�rotation des sommets et arcs n'est pas contigue, c�d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La num�rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet � des indices arbitraires (pas forc�ment contigus)

    C'est cette 2�me approche qui est propos�e ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra �tre attentif au fait que par rapport � un simple vecteur, le parcours des �l�ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours � it�rateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'int�r�t se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associ�e � chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non repr�sent�s � l'�cran
    Imaginons par exemple qu'on doive g�n�rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas repr�senter graphiquement ces 1000 graphes, et les
    interfaces p�sent lourd en ressource, avec cette organisation on est libre de r�server ou
    pas une interface de pr�sentation associ�e aux datas (d�couplage donn�es/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <time.h>
#include <stack>
#include <queue>

#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

    // La boite qui contient toute l'interface d'un sommet
    grman::WidgetBox m_top_box;

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetText m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label pr�c�dent
    grman::WidgetText m_box_label_idx;

    //Creation du bouton select de chaque sommet
    grman::WidgetCheckBox m_select;
    grman::WidgetCheckBox m_select2;

public :

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0,int growth=0, bool active=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

private :
    /// liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs
    std::vector<int> m_in;

    /// liste des indices des arcs partant du sommet : acc�s aux successeurs
    std::vector<int> m_out;

    /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
    double m_value;

    bool m_marque;

    ///ce que l'espece consomme (qt� de ressource environnementale ou d'animaux pr�dat�s)
    float m_consumption;

    ///le ryhtme de croissance de l'espece
    float m_growth;

    ///place que prend une espece dans un environnement (pour une capacite de portage de son environnement = � 100)
    int m_usedSpace;

    ///ce que l'espece "produit" : waste (+ ressources env pour les plantes)
    float m_waste;

    /// Nom du sommet
    std::string m_name;

    /// Nom de l'image associ�e
    std::string m_namePicture;

    /// Position en x et y
    int m_pos_x;
    int m_pos_y;

    /// Si on supprime le sommet ou non
    bool m_active = true;
    /// Bool pour faire fonctionner la sauvegarde des sommets supp
    bool m_saveSupp;

    /// Si influencer par l'EVENEMENT
    int m_event;

    /// Coef d'individu mort par tour
    float m_mortality;

    // determine si un sommet est un d�trivore ou non
    bool m_detrivore = false;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    //std::shared_ptr<VertexInterface> m_interface = nullptr;
    VertexInterface * m_interface = nullptr;


    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne pr�c�dente est en gros �quivalent � la ligne suivante :
    // VertexInterface * m_interface = nullptr;


public:

    /// Les constructeurs sont � compl�ter selon vos besoin...

    /// Constructor par defaut
    Vertex(std::ifstream& fc);

    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Vertex (double value=0, VertexInterface *interface=nullptr) :
        m_value(value), m_interface(interface)  {  }


    /// Vertex �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'impl�mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Edge;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

    // Le WidgetEdge qui "contient" toute l'interface d'un arc
    grman::WidgetEdge m_top_edge;

    // Une boite pour englober les widgets de r�glage associ�s
    grman::WidgetBox m_box_edge;

    // Un slider de visualisation/modification du poids valeur de l'arc
    grman::WidgetVSlider m_slider_weight;

    // Un label de visualisation du poids de l'arc
    grman::WidgetText m_label_weight;
    grman::WidgetText m_name;

    // Si il est select dans main et dans toolbox
    grman::WidgetCheckBox m_select;
    grman::WidgetCheckBox m_select2;

public :

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to, double weight=0);
};


class Edge
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class EdgeInterface;

private :
    /// indice du sommet de d�part de l'arc
    int m_from;

    /// indice du sommet d'arriv�e de l'arc
    int m_to;

    /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
    double m_weight;

    ///Si supression ou non
    bool m_active = true;
    bool m_active2 = true;
    bool m_active3 = false;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    //std::shared_ptr<EdgeInterface> m_interface = nullptr;
    EdgeInterface * m_interface = nullptr;




public:

    /// Les constructeurs sont � compl�ter selon vos besoin...

    /// Constructor par defaut
    Edge(std::ifstream& fc);
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (double weight=0, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_interface(interface)  {  }
    Edge(int from, int to, double weight) :
        m_from(from), m_to(to), m_weight(weight){}

    /// Edge �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'impl�mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

private :



    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de d�clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le param�trer ( voir l'impl�mentation du constructeur dans le .cpp )

    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Une image de "remplissage"
    grman::WidgetImage m_img;

    /// Dans cette boite seront ajout�s les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;

    /// Dans cette boite seront ajout�s des boutons de contr�le etc...
    grman::WidgetBox m_tool_box;

    // Boite de boutton de supression
    grman::WidgetButton m_buttonDel;
    grman::WidgetText m_buttonDel_label;

    // Boite de button d'ajout de sommet
    grman::WidgetButton m_buttonAdd;
    grman::WidgetText m_buttonAdd_label;

    // Creation de la boite de lancement de simulation
    grman::WidgetCheckBox m_simulation;
    grman::WidgetText m_textSimulation;

    //Bouton pour la forte connexit�
    grman::WidgetButton m_buttonFCC;
    grman::WidgetText m_textFCC;

    // Boutton de lancement EVENT
    grman::WidgetCheckBox m_event;
    grman::WidgetText m_nomEvent;

    // Boutton de lancement EVENT 2
    grman::WidgetCheckBox m_event2;
    grman::WidgetText m_nomEvent2;

    // Boutton pour la k-arete connexit�
    grman::WidgetButton m_button_k_plet;
    grman::WidgetButton m_button_robuste;
    grman::WidgetText m_text_robuste;
    grman::WidgetText m_text_k_plet;

    grman::WidgetVSlider m_slider_robuste;
    grman::WidgetText m_textSlider_robuste;

    grman::WidgetCheckBox m_k_sommet;
    grman::WidgetText m_text_k_sommet;
    grman::WidgetCheckBox m_k_arete;
    grman::WidgetText m_text_k_arete;

    // Reset Color
    grman::WidgetCheckBox m_reset;
    grman::WidgetText m_name_reset;

public :

    // Le constructeur met en place les �l�ments de l'interface
    // voir l'impl�mentation dans le .cpp
    GraphInterface(int x, int y, int w, int h, std::string photo, int ordre);
};


class Graph
{
private :

    /// La "liste" des ar�tes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;

    /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;

    ///tableau d'adjacense
    std::vector<std::vector<int>> m_adjacensePoids;

    ///tableau d'adjacense sym�trique (sans l'orientation des aretes)
    std::vector<std::vector<int>> m_adjacensePoidsSymetrique;

    ///ordre du graphe
    int m_ordre;

    ///nombre d'arrete du graphe
    int m_nb_arete;

    ///capacit� de portage (places dispo) de l'environnement
    int m_capacity;

    std::vector<int> tab;

    /// ensemble des composants fortements connexes
    std::vector<std::vector<int>> m_comp;

    ///couleur de fond du sommet
    int m_color;

    ///deuv vecteurs pour les kplets sommets
    std::vector<int> m_people;
    std::vector<int> m_combination;

    std::vector<std::vector<int>> m_kplet;


public:

    /// Les constructeurs sont � compl�ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Graph (GraphInterface *interface=nullptr) :
        m_interface(interface)  {  }

    Graph (std::string fichier, std::string photo)
    {
        load_graph(fichier);
        m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600, photo, m_ordre);

        // Creeer les widget des sommet et aretes
        for(std::map<int, Vertex>::iterator it(m_vertices.begin()); it!=m_vertices.end(); ++it)
            add_interfaced_vertex(it->first, it->second.m_value, it->second.m_pos_x, it->second.m_pos_y, it->second.m_namePicture,0,it->second.m_growth);
        for(std::map<int, Edge>::iterator it(m_edges.begin()); it!=m_edges.end(); ++it)
        {
            add_interfaced_edge(it->first, it->second.m_from, it->second.m_to, it->second.m_weight);
        }

        initTabAdja();

    }

    void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0, int growthcolor=0, bool active=0);

    void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

    /// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
    void update(int compteur_simulation);

    /// DELETE ET ADD VERTEX
    void deleteVertex(int i);
    void addVertex(int i);

    /// DELETE ET ADD EDGES
    void deleteEdges(int i);
    void addEdges(int i);

    /// Sp�cifie toutes les capacit�s de portage de chaque individus par rapport � son environnement
    void init_consumption_Vertices();

    /// Mise � jour des population de chaque sommet en fonction de leur influence
    void evolution();

    ///inititialisation du tableau d'adjacense
    void initTabAdja();

    ///metre � jour la tableau d'adjacense
    void majTabAdja();

    /// Lance les teste pour les connexit�
    void update_connexite();

    /// Lecture des fichiers du projet
    void load_graph(std::string nom_fichier);

    /// Enregistre les valeurs des Aretes dans le fichier
    void save_graph(std::string nom_fichier);

    ///algo pour trouver la forte connexit� V2
    void algo_forte_connexite();

    void CFC(int x,int& n, std::stack<int>& pile, std::vector<int>& pref, std::vector<int>& ret,  std::vector<bool> dansPile );

    ///fonction pour d�terminer la couleur des sommets
    int colorChoice(int nb_color);

    ///fonction pour d�terminer la k-arete-connexit�
    void k_arete_conexe();

    ///fonction pour d�terminer la k-sommet-connexit�
    void k_sommet_connexe();

    ///fonction pour voir si un graphe est simplement-connexe
    bool grapheConnexe(std::vector<std::vector<int>> tab,int z);

    ///deux proc�dures pour calculer les k-plets sommets
    void pretty_print(const std::vector<int>& v);
    void go(int offset, int k);
};


#endif // GRAPH_H_INCLUDED
