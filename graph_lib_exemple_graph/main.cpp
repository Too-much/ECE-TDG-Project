#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

int main()
{
    // A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    // Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    //variable utile pour le menu
    int choix=0;
    int choix2=0;
    bool menu=true;

    //inititialisation des menus
    Menu m(1);
    Menu m2(2);



    /// BOUCLE DE JEU PRINCIPAL
    while ( !key[KEY_ESC] )
    {
        choix=0;
        choix2=0;
        ///boucle du menu principak
        while ( menu==true && choix!=1 && choix!=2 && choix!=3 )
        {
            choix=m.update();
            grman::mettre_a_jour();

            //si le premier bouton est pressé, on rentre dans le deuxieme menu pour choisir le graphe
            if(choix==1)
            {
                //deuxieme boucle de menu pour choisir le graphe
                while(!key[KEY_R] && choix2!=1 && choix2!=2 && choix2!=3)
                {
                    choix2=m2.update();
                    grman::mettre_a_jour();

                    //si choix 1, on lance le premier graphe
                    if(choix2==1)
                    {
                        //on initialise le graphe
                        Graph g;
                        g.load_graph("countryside");
                        g.initTabAdja();
                        g.algo_forte_connexite();
                        g.make_example();
                        g.k_arete_conexe();
                        g.k_sommet_connexe();
                        //boucle de jeu du premier graphe
                        while(!key[KEY_N])
                        {
                            // Il faut appeler les méthodes d'update des objets qui comportent des widgets
                            g.update();
                            g.majTabAdja();

                            g.save_graph("countryside");

                            // Mise à jour générale (clavier/souris/buffer etc...)
                            grman::mettre_a_jour();

                            if(key[KEY_N])      //on revient au menu général
                                menu=true;

                            if(key[KEY_ESC])    //si echape, on quitte directement le jeu
                            {
                                grman::fermer_allegro();
                                return 0;
                            }
                        }

                    }
                    if(choix2==2)
                    {

                    }
                    if(choix==3)
                    {

                    }

                }

            }

            //si le deuxieme bouton est pressé, on affiche les règles
            if(choix==2)
            {

            }

            //si le troisième bouton est pressé, on quitte l'application
            if(choix==3)
            {
                grman::fermer_allegro();
                return 0;
            }
        }
    }

    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();


