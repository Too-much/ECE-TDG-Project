#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

int main()
{
    // A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    // Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    //variable utile pour le menu
    int choix=0;
    int choix2=0;
    bool menu=true;

    //inititialisation des menus
    Menu m(1);
    Menu m2(2);
    Menu m3(3);



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
            int compteur_simulation(0);

            //si le premier bouton est press�, on rentre dans le deuxieme menu pour choisir le graphe
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
                        Graph g("countryside", "coupe_paysage.jpg");

                        //boucle de jeu du premier graphe
                        while(!key[KEY_N])
                        {
                            ++compteur_simulation;

                            // Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                            g.update(compteur_simulation);
                            g.majTabAdja();

                            g.save_graph("countryside");

                            // Mise � jour g�n�rale (clavier/souris/buffer etc...)
                            grman::mettre_a_jour();

                            if(key[KEY_N])      //on revient au menu g�n�ral
                                menu=true;

                            if(key[KEY_ESC])    //si echape, on quitte directement le jeu
                            {
                                grman::fermer_allegro();
                                return 0;
                            }

                            if(compteur_simulation%100 == 0)
                                compteur_simulation=0;
                        }

                    }
                    if(choix2==2)
                    {
                        Graph g2("dinosaure", "dinosaure.jpg");
                        //boucle de jeu du premier graphe
                        while(!key[KEY_N])
                        {
                            ++compteur_simulation;

                            // Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                            g2.update(compteur_simulation);
                            g2.majTabAdja();
                            g2.save_graph("dinosaure");

                            // Mise � jour g�n�rale (clavier/souris/buffer etc...)
                            grman::mettre_a_jour();

                            if(key[KEY_N])      //on revient au menu g�n�ral
                                menu=true;

                            if(key[KEY_ESC])    //si echape, on quitte directement le jeu
                            {
                                grman::fermer_allegro();
                                return 0;
                            }

                            if(compteur_simulation%100 == 0)
                                compteur_simulation=0;
                        }
                    }

                    if(choix2==3)
                    {
                        Graph g3("ocean", "banquise.jpg");

                        //boucle de jeu du premier graphe
                        while(!key[KEY_N])
                        {
                            ++compteur_simulation;

                            // Il faut appeler les m�thodes d'update des objets qui comportent des widgets
                            g3.update(compteur_simulation);
                            g3.majTabAdja();

                            g3.save_graph("ocean");

                            // Mise � jour g�n�rale (clavier/souris/buffer etc...)
                            grman::mettre_a_jour();

                            if(key[KEY_N])      //on revient au menu g�n�ral
                                menu=true;

                            if(key[KEY_ESC])    //si echape, on quitte directement le jeu
                            {
                                grman::fermer_allegro();
                                return 0;
                            }

                            if(compteur_simulation%100 == 0)
                                compteur_simulation=0;
                        }
                    }

                }

            }

            //si le deuxieme bouton est press�, on affiche les r�gles
            if(choix==2)
            {
                 while(!key[KEY_R])
                 {
                    m3.update();
                    grman::mettre_a_jour();
                 }
            }

            //si le troisi�me bouton est press�, on quitte l'application
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


