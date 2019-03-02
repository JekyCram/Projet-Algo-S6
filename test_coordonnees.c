/* Mini-projet ROB3 Algorithmique 2012-13 */

/* pour compiler : gcc projet.c -o projet */
/* pour lancer le programme après compilation : ./projet nom_du_fichier_tsp_sans_extension */
/* exemple : ./projet berlin52 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>


#define C 100
#define L 10
#define K 2

/***********************************/
/* les types Graphe et Coordonnees */
/***********************************/

/* type struct tas */
typedef struct tas
{
	int capacite_max;		//nombre maximum d'éléments dans le tas
	int n; 					//nombre d'élément max déjà dans le tas
	int *tab;				//tableau des éléments du tas (de tab[1] à tab[n])
	double *priorite;		//tableau des cles des éléments du tas
	int *pos; 				//tableau des positions des éléments du tas
	int *origine;           //tableau des sommets d'origine qui sont associés à la priorité
}Tas;

/* type struct graphe */
struct graphe {
    int n;              /* nombre de sommets */
    int m;              /* nombre d'aretes */
    struct voisins {
        int d;          /* degre */
        int nbcell;        /* nombre de cellules dans le tableau */
        char est_trie; /* vrai si la liste est deja triee */
        int list[1];    /* liste des voisins, sous forme d'un tableau dynamique */
    } *alist[1];
};

/*type graphe non-oriente */
typedef struct graphe *Graphe;

/* type struct coordonnees */
struct coordonnees {
	int n;		      /* nombre de points */
	char nom[256];	      /* nom de l'instance */
	struct coord {
		float coordx; /* coordonnee sur l'axe x */
		float coordy; /* coordonnee sur l'axe y */
	} *clist[1];
};

/*type coordonnees */
typedef struct coordonnees *Coordonnees;

/*************************************/
/* 	!!!!!!!	ATTENTION !!!!!!     */
/* 	!!!!!!!	ACHTUNG !!!!!!!!     */
/*	!!!!!!!	BE CAREFUL !!!!!     */
/* les sommets sont indices de 0     */
/* n-1 dans Coordonnees et graph     */
/* MAIS ils sont indicés de 1 a n    */
/* dans les differents fichiers	     */
/*************************************/

/**********************************************************************************/
/* les signatures des fonctions et procedures liees aux graphes et aux affichages */
/**********************************************************************************/

/* cree un nouveau graphe avec n sommets numerotes de 0 a n-1 et aucune arete */
Graphe creer_graphe(int n);

/* libere la memoire utilisee par le graphe g*/
void detruire_graphe(Graphe g);

/* ajoute une arete (s1,s2) a un graphe existant */
/* le faire plus d'une fois peut conduire a des resultats imprevisibles */
void graphe_ajouter_arete(Graphe g, int s1, int s2);

/* retourne le nombre de sommets/aretes d'un graphe g*/
int graphe_nb_sommets(Graphe g);
int graphe_nb_aretes(Graphe g);

/* retourne le degre d'un sommet s dans g*/
int graphe_degre(Graphe g, int s);

/* retourne 1 si l'arete (s1,s2) existe dans g, 0 sinon */
int graphe_arete_existe(Graphe g, int s1, int s2);

/* conversion d'un fichier tour nom_tour.tour en un graphe */
Graphe conversion_tour_graphe(char *nom_tour);

/* cree un tableau de n coordonnees numerotees de 0 a n-1 et toutes a (0,0) */
Coordonnees creer_coordonnees(int n,char *nom);

/* libere la memoire utilisee par les coordonnees c*/
void detruire_coordonnees(Coordonnees c);

/* lit une instance dans le fichier nom.tsp, la charge en memoire dans Coordonnees */
/* et cree un fichier nom.gnu pour l'affichage */
Coordonnees lecture_instance(char* nom);

/* affiche une instance apres lecture de l'instance nom.tsp */
/* a partir du fichier nom.gnu qui a ete genere par lecture_instance */
void afficher_instance(char* nom);

/* affiche une tournee nom_tour.tour */
void afficher_tour(Coordonnees c, char* nom_tour);

/* affiche le graphe g */
void afficher_graphe(Coordonnees c, Graphe g);


/************************************************************/
/* les signatures des fonctions et procedures liees aux tas */
/************************************************************/

/* création d'un nouveau tas */
Tas new_tas(int capa_max, int dep);

/* libérer l'espace mémoire d'un tas */
void free_tas(Tas t);

/* affiche le tas */
void affiche_tas( Tas t );

/* insertion d'un nouveau sommet dans le tas */
void insert(Tas *T, int x, double l);

/* retourne le sommet de priorite minimum */
int tache_prioritaire(Tas t);

/* retourne 1 si le sommet d'indice i est une feuille du tas, 0 sinon */
int estFeuille(Tas t,int i);

/* retourne 1 si le tas est vide, 0 sinon */
int est_vide(Tas t);

/* retourne 1 si le sommet d'indice i a un fils à droite */
int aUnFilsD(Tas t, int i);

/* retourne l'indice du fils gauche du sommet d'indice i */
int feuilleG(Tas t, int i);

/* retourne l'indice du fils droit du sommet d'indice i */
int feuilleD(Tas t, int i);

/* retourne l'indice du pere du sommet d'indice i en supposant qu'il existe */
int pere(Tas t, int i);

/* supprime un élément (x,l) du tas */
void supp(Tas* T, int x);

/* renvoie le sommet de priorite min et le supprime du tas */
int recup_min(Tas *t);

/******************************/
/* Les fonctions a programmer */
/******************************/

/* fonction qui donne la distance (la priorite) entre deux sommets */
//double getDist(double m[L][K], int l1, int l2);
double getDist(Coordonnees c, int l1, int l2);

/* algorithme de Prim */
//void prim(double m[L][K], int dep);
void prim(Coordonnees c, int dep);


/***************************************************************************************/
/* les implementations des fonctions et procedures liees aux graphes et aux affichages */
/***************************************************************************************/

/* cree un nouveau graphe avec n sommets numerotes de 0 a n-1 et aucune arete */
Graphe creer_graphe(int n)
{
    Graphe g;
    int i;

    g = malloc(sizeof(struct graphe) + sizeof(struct voisins *) * (n-1));
    assert(g);

    g->n = n;
    g->m = 0;

    for(i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct voisins));
        assert(g->alist[i]);

        g->alist[i]->d = 0;
        g->alist[i]->nbcell = 1;
        g->alist[i]->est_trie= 1;
    }

    return g;
}

/* libere la memoire utilisee par le graphe g */
void detruire_graphe(Graphe g)
{
    int i;

    for(i = 0; i < g->n; i++) free(g->alist[i]);
    free(g);
}

/* ajoute une arete a un graphe existant */
/* le faire plus d'une fois peut conduire a des resultats imprevisibles */
void graphe_ajouter_arete(Graphe g, int s1, int s2)
{
    assert(s1 >= 0);
    assert(s1 < g->n);
    assert(s2 >= 0);
    assert(s2 < g->n);

    /* est-il necessaire d'agrandir la liste de s1 ? */
    while(g->alist[s1]->d >= g->alist[s1]->nbcell) {
        g->alist[s1]->nbcell *= 2;
        g->alist[s1] =
            realloc(g->alist[s1],
                sizeof(struct voisins) + sizeof(int) * (g->alist[s1]->nbcell - 1));
    }

    /* est-il necessaire d'agrandir la liste de s2 ? */
    while(g->alist[s2]->d >= g->alist[s2]->nbcell) {
        g->alist[s2]->nbcell *= 2;
        g->alist[s2] =
            realloc(g->alist[s2],
                sizeof(struct voisins) + sizeof(int) * (g->alist[s2]->nbcell - 1));
    }

    /* ajoute le sommet s2 dans la liste d'adjacence de s1 */
    g->alist[s1]->list[g->alist[s1]->d++] = s2;
    g->alist[s1]->est_trie = 0;

    /* ajoute le sommet s1 dans la liste d'adjacence de s2 */
    g->alist[s2]->list[g->alist[s2]->d++] = s1;
    g->alist[s2]->est_trie = 0;

    /* incremente le compteur d'aretes */
    g->m++;
}

/* retourne le nombre de sommets d'un graphe */
int graphe_nb_sommets(Graphe g)
{
    return g->n;
}

/* retourne le nombre d'aretes d'un graphe */
int graphe_nb_aretes(Graphe g)
{
    return g->m;
}

/* retourne le degre d'un sommet s */
int graphe_degre(Graphe g, int s)
{
    assert(s >= 0);
    assert(s < g->n);

    return g->alist[s]->d;
}

/* a partir d'une liste de longueur 10 */
/* on appelle la recherche dichotomique */
#define BSEARCH_THRESHOLD (10)

static int intcmp(const void *a, const void *b)
{
    return *((const int *) a) - *((const int *) b);
}

/* retourne 1 si l'arete (s1,s2) existe, 0 sinon */
int graphe_arete_existe(Graphe g, int s1, int s2)
{
    int i;

    assert(s1 >= 0);
    assert(s1 < g->n);
    assert(s2 >= 0);
    assert(s2 < g->n);

    if(graphe_degre(g,s1) >= BSEARCH_THRESHOLD) {
        /* on s'assure que c'est trie */
        if(! g->alist[s1]->est_trie) {
            qsort(g->alist[s1]->list,g->alist[s1]->d,sizeof(int),intcmp);
        }

        /* on appelle la recherche dichotomique */
        return
            bsearch(&s2,g->alist[s1]->list,g->alist[s1]->d,sizeof(int),intcmp)!= 0;
    } else {
        /* on realise une simple recherche sequentielle */
        for(i = 0; i < g->alist[s1]->d; i++) {
            if(g->alist[s1]->list[i] == s2) return 1;
        }
        /* sinon */
        return 0;
    }
}

/* conversion d'un fichier tour en un graphe */
Graphe conversion_tour_graphe(char *nom_tour)
{
	FILE* fichier_tour = NULL;
	char temp[256];
	int i,j,dimension,sommet1,sommet2;
	Graphe g;

	char nom_complet_tour[256];

	/* ouverture en lecture du fichier nom_tour.tour */
	/* exemple : berlin52.opt.tour */
	sprintf(nom_complet_tour,"%s.tour",nom_tour);
	fichier_tour = fopen(nom_complet_tour, "r");

	/* on recupere la dimension dans nom_tour.tour */
	/* exemple : on recupere la dimension dans berlin52.opt.tour */
	do
	{
		fgets(temp,256,fichier_tour);
	} while (temp[0] != 'D');

	i=0;
	while (temp[i] != ':') i++;
	i=i+2;
	char dim[256];
	j=0;
	while (temp[i] != '\0')
	{
		dim[j]=temp[i];
		i++; j++;
	}
	dim[j]='\0';
	dimension=atoi(dim);

	/* creation d'un graphe vide */
	g = creer_graphe(dimension);

	/* on se place sur la ligne comportant le sommet 1 dans nom_tour.tour */
	/* exemple : ligne comportant le sommet 1 dans berlin52.opt.tour */
	fgets(temp,256,fichier_tour);
	while(temp[0] != '1')
	{
		fgets(temp,256,fichier_tour);
	}

	/* on parcourt le fichier nom_tour.tour pour generer le graphe g */
	/* exemple : on parcourt berlin52.opt.tour pour generer le graphe g */
	sommet1=1;
	fscanf(fichier_tour,"%d",&sommet2);
	while (sommet2 != -1)
	{
		graphe_ajouter_arete(g,sommet1-1,sommet2-1);
		sommet1=sommet2;
		fscanf(fichier_tour,"%d",&sommet2);
	}

	/* on genere la derniere arete de la tournee */
	graphe_ajouter_arete(g,sommet1-1,0);

	/* fermeture du fichier nom_tour.tour */
	fclose(fichier_tour);

	return g;
}

/* cree un tableau de n coordonnees numerotees de 0 a n-1 et toutes a (0,0) */
/* le parametre "nom" est le nom que l'on veut donner a l'instance */
Coordonnees creer_coordonnees(int n,char *nom)
{
    Coordonnees c;
    int i;

    c = malloc(sizeof(struct coordonnees) + sizeof(struct coord *) * (n-1));
    assert(c);

    c->n = n;
    sprintf(c->nom,"%s",nom);

    for(i = 0; i < n; i++) {
        c->clist[i] = malloc(sizeof(struct coord));
        assert(c->clist[i]);

        c->clist[i]->coordx = 0;
        c->clist[i]->coordy = 0;
    }

    return c;
}

/* libere la memoire utilisee par les coordonnees c */
void detruire_coordonnees(Coordonnees c)
{
    int i;

    for(i = 0; i < c->n; i++) free(c->clist[i]);
    free(c);
}

/* lit une instance dans le fichier nom.tsp, charge les coordonnees en memoire dans c */
/* et cree un fichier nom.gnu pour l'affichage */
Coordonnees lecture_instance(char* nom)
{
	int i,j,point,dimension;
	float coordx,coordy;
	char temp[256];
	char* plot = "plot";
 	FILE* fichier_tsp = NULL;
	FILE* fichier_dat = NULL;
	FILE* fichier_gnu = NULL;
	Coordonnees c;

	/* ouverture en lecture du fichier nom.tsp */
	/* exemple : berlin52.tsp */
	char nom_tsp[256];
       	sprintf(nom_tsp,"%s.tsp",nom);
	fichier_tsp = fopen(nom_tsp, "r");

	/* ouverture en ecriture du fichier nom.dat */
	/* le fichier stocke les differents points */
	/* sous un format interpretable par gnuplot */
	/* exemple : berlin52.dat */
	char nom_dat[256];
	sprintf(nom_dat,"%s.dat",nom);
	fichier_dat = fopen(nom_dat, "w");

	/* ouverture en ecriture du fichier nom.gnu */
	/* le fichier nom.gnu fait appel a nom.dat */
	/* exemple : berlin52.gnu */
	char nom_gnu[256];
	sprintf(nom_gnu,"%s.gnu",nom);
	fichier_gnu = fopen(nom_gnu, "w");

	if ((fichier_tsp != NULL)&&(fichier_dat != NULL)&&(fichier_gnu != NULL))
	{
        	/* on recupere la dimension dans nom.tsp */
		/* exemple : on recupere la dimension dans berlin52.tsp */
		for (i=0;i<3;i++) fgets(temp,256,fichier_tsp);
		fgets(temp,256,fichier_tsp);
		i=0;
		while (temp[i] != ':') i++;
		i=i+2;
		char dim[256];
		j=0;
		while (temp[i] != '\0')
		{
			dim[j]=temp[i];
			i++; j++;
		}
		dim[j]='\0';
		dimension=atoi(dim);

		/* creation du tableau de coordonnees */
		c = creer_coordonnees(dimension,nom);

		/* on charge les coordonnees en memoire a partir du fichier nom.tsp */
		/* exemple : a partir de berlin52.tsp */
		for (i=0;i<2;i++) fgets(temp,256,fichier_tsp);
		for (i=0;i<c->n;i++)
		{
			fscanf(fichier_tsp,"%d %f %f",&point,&coordx,&coordy);
			c->clist[point-1]->coordx = coordx;
			c->clist[point-1]->coordy = coordy;
			fprintf(fichier_dat,"%d %f %f \n",point,coordx,coordy);
		}
	}
    	else
	{
	        /* On affiche un message d'erreur */
	        printf("Impossible d'ouvrir le fichier");
	}

	/* on genere les dernieres lignes de berlin52.gnu */
	fprintf(fichier_gnu,"%s",plot);
	fprintf(fichier_gnu,"%c",' ');
        fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s",nom_dat);
 	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s"," u 2:3 title ");
	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%s",nom);
 	fprintf(fichier_gnu,"%c",putchar(39));
	fprintf(fichier_gnu,"%c",'\n');
	fprintf(fichier_gnu,"%s","pause -1");
	fprintf(fichier_gnu,"%c",'\n');

	/* on ferme les differents fichiers */
	fclose(fichier_tsp);
	fclose(fichier_dat);
	fclose(fichier_gnu);

	/* retourne les coordonnees */
	return c;
}

/* affiche une instance apres lecture de l'instance nom.tsp */
/* a partir du fichier nom.gnu qui a ete genere */
void afficher_instance(char* nom)
{
	char commande[256];
       	sprintf(commande,"gnuplot %s.gnu",nom);
	system(commande);
}

/* affiche une tournee nom_tour.tour */
/* prerequis : avoir lance lecture_instance au prealable */
/* exemple : affiche berlin52.opt.tour apres avoir lu berlin52.tsp */
void afficher_tour(Coordonnees c, char* nom_tour)
{
	FILE* fichier_gnu = NULL;
	FILE* fichier_tour_gnu = NULL;
	FILE* fichier_tour = NULL;
	int i,sommet1,sommet2;

	char nom_gnu[256];
	char nom_tour_gnu[256];
	char nom_complet_tour[256];

	/* ouverture en lecture du fichier (c->nom).gnu */
	/* exemple : berlin52.gnu */
	sprintf(nom_gnu,"%s.gnu",c->nom);
	fichier_gnu = fopen(nom_gnu, "r");

	/* ouverture en ecriture du fichier nom_tour.gnu */
	/* exemple : berlin52.opt.gnu */
	sprintf(nom_tour_gnu,"%s.gnu",nom_tour);
	fichier_tour_gnu = fopen(nom_tour_gnu, "w");

	/* ouverture en lecture du fichier nom_tour.tour */
	/* exemple : berlin52.opt.tour */
	sprintf(nom_complet_tour,"%s.tour",nom_tour);
	fichier_tour = fopen(nom_complet_tour, "r");

	/* on se place sur la ligne comportant le sommet 1 dans nom_tour.tour */
	/* exemple : ligne comportant le sommet 1 dans berlin52.opt.tour */
	char temp[256];
	fgets(temp,256,fichier_tour);
	while(temp[0] != '1')
	{
		fgets(temp,256,fichier_tour);
	}

	/* on copie la premiere ligne de nom.gnu dans nom_tour.gnu */
	/* exemple : on copie de berlin52.gnu vers berlin52.opt.gnu */
	char ligne[256];
	fgets(ligne,sizeof(ligne),fichier_gnu);
	fprintf(fichier_tour_gnu,"%s",ligne);

	/* on parcourt le fichier nom_tour.tour pour generer nom_tour.gnu */
	/* exemple : on parcourt berlin52.opt.tour pour generer berlin52.opt.gnu */
	sommet1=1;
	fscanf(fichier_tour,"%d",&sommet2);
	while (sommet2 != -1)
	{
		fprintf(fichier_tour_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1-1]->coordx,c->clist[sommet1-1]->coordy,c->clist[sommet2-1]->coordx,c->clist[sommet2-1]->coordy);
		sommet1=sommet2;
		fscanf(fichier_tour,"%d",&sommet2);
	}

	/* on genere les dernieres lignes de nom_tour.gnu */
	/* exemple : on genere les dernieres lignes de berlin52.opt.gnu */
	fprintf(fichier_tour_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1-1]->coordx,c->clist[sommet1-1]->coordy,c->clist[0]->coordx,c->clist[0]->coordy);
	fprintf(fichier_tour_gnu,"%s","replot\n");
	fprintf(fichier_tour_gnu,"%s","pause -1\n");

	/* fermeture des differents fichiers */
	fclose(fichier_gnu);
	fclose(fichier_tour_gnu);
	fclose(fichier_tour);

	/* affichage obtenu par 'gnuplot nom_tour.gnu' */
	/* exemple : affichage obtenu par 'gnuplot berlin52.opt.gnu' */
	char commande[256];
	sprintf(commande,"gnuplot %s.gnu",nom_tour);
	system(commande);
}

/* affiche le graphe g */
/* prerequis : avoir lance lecture_instance au prealable */
void afficher_graphe(Coordonnees c, Graphe g)
{
	FILE* fichier_gnu = NULL;
	FILE* fichier_graphe_gnu = NULL;
	int i,sommet1,sommet2;
	char nom_gnu[256];

	/* ouverture en lecture de nom.gnu */
	/* exemple : berlin52.gnu */
	/* ouverture en ecriture de "graphe.gnu" */
	sprintf(nom_gnu,"%s.gnu",c->nom);
	fichier_gnu = fopen(nom_gnu, "r");
	fichier_graphe_gnu = fopen("graphe.gnu", "w");

	/* generation de "graphe.gnu" a partir de nom.gnu et de g */
	char ligne[256];
	fgets(ligne,sizeof(ligne),fichier_gnu);
	fprintf(fichier_graphe_gnu,"%s",ligne);
	for(sommet1 = 0; sommet1 < g->n; sommet1++) {
		for (i = 0; i < g->alist[sommet1]->d; i++) {
			sommet2 = g->alist[sommet1]->list[i];
			if (sommet2 > sommet1) fprintf(fichier_graphe_gnu,"set arrow from %f,%f to %f,%f nohead\n",c->clist[sommet1]->coordx,c->clist[sommet1]->coordy,c->clist[sommet2]->coordx,c->clist[sommet2]->coordy);
		}
	}
	fprintf(fichier_graphe_gnu,"%s","replot\n");
	fprintf(fichier_graphe_gnu,"%s","pause -1\n");

	/*fermeture des differents fichiers */
	fclose(fichier_gnu);
	fclose(fichier_graphe_gnu);

	/* affichage */
	system("gnuplot graphe.gnu");
}


/* ------------------------------------- GESTION DES TAS --------------------------------------*/




/* fonction qui créer un tas avec dep comme sommet qui initialise les priotités */
Tas new_tas(int capa_max, int dep)
{
	Tas t;
	t.capacite_max = capa_max;
	t.n=0;
	t.tab = (int*) malloc ( t.capacite_max * sizeof(int) );
	t.priorite = (double*) malloc ( t.capacite_max * sizeof(double) );
	t.pos = (int*) malloc ( t.capacite_max * sizeof(int) );
	t.origine = (int*) malloc (t.capacite_max * sizeof(int) );

	int i;
	t.tab[0] = 0;
	t.pos[0] = -1;
	for( i=1 ; i<t.capacite_max ; i++ )
	{
		t.tab[i] = -1;
		t.pos[i] = -1;
		//*(t.tab+i) = -1;
		//*(t.pos+i) = -1;
		t.origine[i] = dep;
	}

	return t;
}


/* libère l'espace mémoire d'un tas */
void free_tas(Tas t)
{
	free(t.tab);
	free(t.priorite);
	free(t.pos);
	free(t.origine);
}

/* affiche le tas et ses composantes */
void affiche_tas( Tas t )
{
	printf("\nTas:\nIdentifiant:\t");
	int i;
	for( i=1 ; i<=t.tab[0] ; i++ )
	{
		printf("%d\t",*(t.tab+i));
	}
	printf("\nPriorité:\t");
	for( i=1 ; i<=t.tab[0] ; i++ )
	{
		printf("%3.2f\t",*(t.priorite+i));
	}
	printf("\nPosition:\t");
	for( i=1 ; i<=t.n ; i++ )
	{
		printf("%d\t",*(t.pos+i));
	}
	printf("\nOrigine:\t");
	for( i=1 ; i<=t.n ; i++ )
	{
		printf("%d\t",t.origine[i]);
	}
	printf("\n");
}


// EXO 1.4
/* insert un couple (x,l) d'identifiant x et de priorité l dans le tas T */
/*void insert(Tas *T, int x, double l)
{
	int *lg = &(T->tab[0]);
	if( *(lg)+1 > T->capacite_max ) printf("\n-----\nplus d'élément ajoutable...\n-----\n"); //T->n+1
	else
	{
		T->tab[*(lg)+1] = x; // *(T->tab+(T->n)+1) = x;
		T->priorite[*(lg)+1] = l; // *(T->priorite+(T->n)+1) = l;
		*(T->pos+(T->n)+1) = (T->n)+1; //   T->pos[*(lg)+1] = *(lg)+1;
		int a = *(lg)+1; // (T->n)+1;
		*(lg) += 1;
		//if( *(lg) > T->n ) T->n = *(lg);
		T->n += 1 ;
		while( *(T->priorite+a) < *(T->priorite+(a/2)) )
		{
			int temp = *(T->tab+(a/2));
			*(T->tab+(a/2)) = *(T->tab+a);
			*(T->tab+a) = temp;
			double tempd = *(T->priorite+(a/2));
			*(T->priorite+(a/2)) = *(T->priorite+a);
			*(T->priorite+a) = tempd;
			T->pos[T->tab[a]] = a;
			T->pos[T->tab[a/2]] = a/2;
			a = a/2;
		}
	}
}*/

/* fonction d'insertion bis */
void insert2(Tas* t, int x, double l, int o)
{
	//on souhaite insérer un élément en fin de liste donc à la postion du nombre d'élément +1
	int new = (t->tab[0])+1;
	if( new >= t->capacite_max ) printf("\n!!!!!!!!!!!!!!!!!!!!!!!!\nPLus de place\n!!!!!!!!!!!!!!!!!!!!!!!!\n");
	else
	{
		//on ajoute le nouvel élément en fin de liste
		t->tab[new] = x;
		t->priorite[new] = l;
		//printf("\nindice d'insertion: %d", new);
		t->pos[x] = new;
		t->origine[new] = o;
		
		//on incrémente le nombre d'élément présent dans le tas
		t->tab[0]++;
		if( x > t->n ) t->n++;
		
		//il faut faire remonter l'élément à sa place pour avoir un arbre parfait tournoi
		while( t->priorite[new] < t->priorite[new/2] && new != 1 )
		{
			//échange des identifiants
			int temp = t->tab[new];
			t->tab[new] = t->tab[new/2];
			t->tab[new/2] = temp;
			
			//échange des origines
			temp = t->origine[new];
			t->origine[new] = t->origine[new/2];
			t->origine[new/2] = temp;
			
			//échange des priorités
			double tempd = t->priorite[new];
			t->priorite[new] = t->priorite[new/2];
			t->priorite[new/2] = tempd;
			
			//update des positions
			t->pos[t->tab[new]] = new;
			t->pos[t->tab[new/2]] = new/2;
			
			new = new/2;
		}
		
	}
}
	

//EXO 1.5
/* retourne l'identifiant de la tache prioritaire */
int tache_prioritaire(Tas t)
{
	return t.tab[1];
}


//EXO 2.2
/* retourne 1 si le tas est vide, 0 sinon */
int est_vide(Tas t)
{
	return t.tab[0] == 0 ;
}

/* retourne 1 si l'élément en position i est une feuille, 0 sinon */
int estFeuille(Tas t,int i)
{
    //printf("\n---\nfonction estFeuille\ntache fils gauche: %d\tposition: %d\n---\n",t.tab[2*i],t.pos[t.tab[2*i]]);
	return t.pos[t.tab[2*i]] == -1 || t.tab[2*i] == -1 || 2*i>t.capacite_max;
}

/* retourne 1 si l'élément à la position i a un fils à droite, 0 sinon */
int aUnFilsD(Tas t, int i)
{
	return !(estFeuille(t,i)) && ((t.pos[t.tab[2*i+1]] != -1) || t.tab[2*i]== -1);
}

/* retourne l'indice de position du fils de gauche de l'élément en position i s'il existe */
int feuilleG(Tas t, int i)
{
	if( !(estFeuille(t,i)) )
	{
		return 2*i;
	}
	else return 0;
}

/* retourne l'indice de position du fils de droite de l'élément en position i s'il existe */
int feuilleD(Tas t, int i)
{
	if( aUnFilsD(t,i) )
	{
		return 2*i+1;
	}
	else return 0;
}

/* retourne l'indice de position du père de l'élément en position i s'il existe */
int pere(Tas t, int i)
{
	if( i > 1 )
	{
		return i/2;
	}
	else return 0;
}


void supp(Tas* T, int x)
{
	int *lg = &(T->tab[0]);
	int ind = T->pos[x];
	//printf("\nind: %d",ind);

	if( ind == *(lg) )
	{
		*(lg) -= 1;
		T->pos[x] = -1;
		//affiche_tas(*T);
	}
	else
	{

		//printf("\ninversion dans tab\ntab[ind]: %d\ttab[n]: %d\n",T->tab[ind],T->tab[*(lg)]);
		int temp = T->tab[ind];
		T->tab[ind] = T->tab[*(lg)];
		T->tab[*(lg)] = temp;

		temp = T->origine[ind];
		T->origine[ind] = T->origine[*lg];
		T->origine[*lg] = temp;

		//printf("\ninversion dans priorite\nprio[ind]: %lf\tprio[n]: %lf\n",T->priorite[ind],T->priorite[*(lg)]);
		double tempd = T->priorite[ind];
		T->priorite[ind] = T->priorite[*(lg)];
		T->priorite[*(lg)] = tempd;

		T->pos[T->tab[ind]] = ind;
		//T->pos[T->tab[ind/2]] = ind/2;
		T->pos[x] = -1;

		*(lg) -= 1;

		//printf("\ntableau avant réinsertion sans la tache prio:");
		affiche_tas(*T);

		while( !(estFeuille(*T,ind) ) )
		{
			//printf("tab[ind] (%d) n'est pas une feuille\n", T->tab[ind]);
			if( aUnFilsD(*T,ind) && T->priorite[feuilleD(*T,ind)] < T->priorite[ind] )
			{
				//printf("tab[ind] (%d) a un fils a droite: %d de priorite: %lf\n", T->tab[ind], T->tab[feuilleD(*T,ind)], T->priorite[feuilleD(*T,ind)]);
				int temp = T->tab[ind];
				T->tab[ind] = T->tab[feuilleD(*T,ind)];
				T->tab[feuilleD(*T,ind)] = temp;

				temp = T->origine[ind];
                T->origine[ind] = T->origine[feuilleD(*T,ind)];
                T->origine[feuilleD(*T,ind)] = temp;

				double tempd = T->priorite[ind];
				T->priorite[ind] = T->priorite[feuilleD(*T,ind)];
				T->priorite[feuilleD(*T,ind)] = tempd;

				T->pos[T->tab[ind]] = ind;
				T->pos[T->tab[feuilleD(*T,ind)]] = feuilleD(*T,ind);

				//printf("Inversion done!");
				affiche_tas(*T);

				ind = feuilleD(*T,ind);
				//printf("nouveau ind: %d\ttache: %d\n", ind, T->tab[ind]);
			}
			else if( T->priorite[feuilleG(*T,ind)] < T->priorite[ind] )
			{
				//printf("tab[ind] (%d) a un fils a gauche: %d de priorite: %lf\n", T->tab[ind], T->tab[feuilleG(*T,ind)], T->priorite[feuilleG(*T,ind)]);
				int temp = T->tab[ind];
				T->tab[ind] = T->tab[feuilleG(*T,ind)];
				T->tab[feuilleG(*T,ind)] = temp;

				temp = T->origine[ind];
                T->origine[ind] = T->origine[feuilleG(*T,ind)];
                T->origine[feuilleG(*T,ind)] = temp;

				double tempd = T->priorite[ind];
				T->priorite[ind] = T->priorite[feuilleG(*T,ind)];
				T->priorite[feuilleG(*T,ind)] = tempd;

				T->pos[T->tab[ind]] = ind;
				T->pos[T->tab[feuilleG(*T,ind)]] = feuilleG(*T,ind);

				//printf("Inversion done!");
				affiche_tas(*T);

				ind = feuilleG(*T,ind);
				//printf("nouveau ind: %d\ttache: %d\n", ind, T->tab[ind]);
			}
			else
			{
			    //printf("\npas de fils de priorite inf\n");
                break;
            }

		//affiche_tas(*T);

        }
        while( T->priorite[ind] < T->priorite[ind/2] && ind != 1) //*(T->priorite+a) < *(T->priorite+(a/2)) )
		{
			int temp = *(T->tab+(ind/2));
			*(T->tab+(ind/2)) = *(T->tab+ind);
			*(T->tab+ind) = temp;

			double tempd = *(T->priorite+(ind/2));
			*(T->priorite+(ind/2)) = *(T->priorite+ind);
			*(T->priorite+ind) = tempd;

			//printf("\nn=%d\ta=%d\ta/2=%d\n%d\t%d\t%d\n",T->n,a,a/2,*(T->pos+(a/2)),*(T->pos+*(T->pos+(a/2))),*(T->pos+T->n));
			/*temp = *(T->pos+*(T->pos+(a/2))); T->pos[pos[a/2]]
			*(T->pos+*(T->pos+(a/2))) = *(T->pos+T->n);
			*(T->pos+T->n) = temp;*/

			T->pos[T->tab[ind]] = ind;
			T->pos[T->tab[ind/2]] = ind/2;

			ind = ind/2;
		}
    }
}


/* supprime du tas l'élément d'identifiant x en supposant qu'il est présent dans le tas */
void supp2(Tas* t, int x)
{
	int lg = t->tab[0];
	int ind = t->pos[x];
	printf("\n--Suppression de l'élément %d\n",t->tab[ind]);
	
	//on vérifie si l'indice est le dernier élément du tas
	if( ind == lg )
	{
		t->tab[0]--;
		t->pos[x] = -1;
		printf("L'élément a supprimé était le dernier");
		affiche_tas(*t);
	}
	//sinon on inverse les positions de l'élément a supprimer et le dernier élément de la liste
	else
	{
		//échange des identifiants
		int temp = t->tab[ind];
		t->tab[ind] = t->tab[lg];
		t->tab[lg] = temp;
		
		//échange des origines
		temp = t->origine[ind];
		t->origine[ind] = t->origine[lg];
		t->origine[lg] = temp;
		
		//échange des priorités
		double tempd = t->priorite[ind];
		t->priorite[ind] = t->priorite[lg];
		t->priorite[lg] = tempd;
		
		//update des positions
		t->pos[t->tab[ind]] = ind;
		t->pos[x] = -1;				//-1 signifie que l'élément n'est plus dans le tas
		
		//on peut réduire la taille du tas
		t->tab[0]--;
		printf("Tas après sa suppression:");
		affiche_tas(*t);
		
		//On cherche maintenant a réinserer correctement l'élément mis à l'indice ind
		//On cherche à le descendre tant qu'il n'est pas une feuille ou qu'il a des fils de priorité inf
		printf("\nt.pos[t.tab[2*i]] = %d\t t.tab[2*i] = %d\n",t->pos[t->tab[2*ind]], t->tab[2*ind]);
		printf("(t.pos[t.tab[2*i]] == -1) = %d\t (t.tab[2*i] == -1) = %d\t\tcapa_max: %d\n",t->pos[t->tab[2*ind]] == -1 , t->tab[2*ind] == -1, t->capacite_max);
		while( !(estFeuille(*t,ind)) )
		{
			printf("tab[ind] (%d) n'est pas une feuille\n", t->tab[ind]);
			
			//On vérifie si l'élément a un fils à droite de prio inf à lui et à son fils de gauche
			if( aUnFilsD(*t,ind) && t->priorite[feuilleD(*t,ind)] < t->priorite[ind] && t->priorite[feuilleD(*t,ind)] <= t->priorite[feuilleG(*t,ind)] )
			{
				printf("tab[ind] (%d) a un fils a droite: %d de priorite: %lf minimum\n", t->tab[ind], t->tab[feuilleD(*t,ind)], t->priorite[feuilleD(*t,ind)]);
				
				//Dans ce cas on inverse les positions avec le fils de droite
				//échange des identifiants
				int temp = t->tab[ind];
				t->tab[ind] = t->tab[feuilleD(*t,ind)];
				t->tab[feuilleD(*t,ind)] = temp;
				
				//échange des origines
				temp = t->origine[ind];
				t->origine[ind] = t->origine[feuilleD(*t,ind)];
				t->origine[feuilleD(*t,ind)] = temp;
				
				//échange des priorités
				double tempd = t->priorite[ind];
				t->priorite[ind] = t->priorite[feuilleD(*t,ind)];
				t->priorite[feuilleD(*t,ind)] = tempd;
				
				//update des positions
				t->pos[t->tab[ind]] = ind;
				t->pos[t->tab[feuilleD(*t,ind)]] = feuilleD(*t,ind);
				
				printf("Inversion done!");
				affiche_tas(*t);

				//maj de l'indice pour le tour suivant -> ind prend l'indice du fils a droite
				ind = feuilleD(*t,ind);
				printf("nouveau ind: %d\ttache: %d\n", ind, t->tab[ind]);
			}
			
			//sinon on vérifie si le fils à gauche est de prio inf
			else if( t->priorite[feuilleG(*t,ind)] < t->priorite[ind] )
			{
				printf("tab[ind] (%d) a un fils a gauche: %d de priorite: %lf\n", t->tab[ind], t->tab[feuilleG(*t,ind)], t->priorite[feuilleG(*t,ind)]);
				
				//Dans ce cas on inverse les positions avec le fils de gauche
				//échange des identifiants
				int temp = t->tab[ind];
				t->tab[ind] = t->tab[feuilleG(*t,ind)];
				t->tab[feuilleG(*t,ind)] = temp;
				
				//échange des origines
				temp = t->origine[ind];
				t->origine[ind] = t->origine[feuilleG(*t,ind)];
				t->origine[feuilleG(*t,ind)] = temp;
				
				//échange des priorités
				double tempd = t->priorite[ind];
				t->priorite[ind] = t->priorite[feuilleG(*t,ind)];
				t->priorite[feuilleG(*t,ind)] = tempd;
				
				//update des positions
				t->pos[t->tab[ind]] = ind;
				t->pos[t->tab[feuilleG(*t,ind)]] = feuilleG(*t,ind);
				
				printf("Inversion done!");
				affiche_tas(*t);

				//maj de l'indice pour le tour suivant -> ind prend l'indice du fils a droite
				ind = feuilleG(*t,ind);
				printf("nouveau ind: %d\ttache: %d\n", ind, t->tab[ind]);
			}
			
			//Si ancune des conditions précédentes ne sont remplies alors on sort du while
			else
			{
			    printf("pas de fils de priorite inf\n");
                break;
            }
		}
		
		//On va faire remonter l'élément le plus haut possible avec ses pères d'indice ind/2
		while( t->priorite[ind] < t->priorite[ind/2] && ind != 1 )
		{
			//échange des identifiants
			int temp = t->tab[ind];
			t->tab[ind] = t->tab[ind/2];
			t->tab[ind/2] = temp;
			
			//échange des origines
			temp = t->origine[ind];
			t->origine[ind] = t->origine[ind/2];
			t->origine[ind/2] = temp;
			
			//échange des priorités
			double tempd = t->priorite[ind];
			t->priorite[ind] = t->priorite[ind/2];
			t->priorite[ind/2] = tempd;
			
			//update des positions
			t->pos[t->tab[ind]] = ind;
			t->pos[t->tab[ind/2]] = ind/2;
			
			//maj de l'indice pour le tour suivant -> il prend l'indice du père
			ind = ind/2;
		}
		printf("tab[ind] (%d) est une feuille ou est bien positionné\n",t->tab[ind]);
	}
}


/* supprime l'élément ( x, l) de valeur l minimum dans K et qui retourne le sommet x */
int recup_min(Tas *t)
{
	printf("\ntache prioritaire: %d\tpriorite: %3.2f\n",tache_prioritaire(*t),t->priorite[1]);
	supp2(t,t->tab[1]);
	return tache_prioritaire(*t);
}

//-------------------------PRIM------------------------------------

/* retourne la distance (priorite) entre les sommets s1 et s2 */
/*double getDist(double m[L][K], int l1, int l2)
{
    int a = m[l2][0] - m[l1][0];
    int b = m[l2][1] - m[l1][1];
    return sqrt( (double) (a*a + b*b) );
    //return sqrt( a*a + b*b );
}*/

double getDist(Coordonnees c, int l1, int l2)
{
    int a = c->clist[l2]->coordx - c->clist[l1]->coordx;
    int b = c->clist[l2]->coordy - c->clist[l1]->coordy;;
    return sqrt( (double) (a*a + b*b) );
}


void updatePrio(Coordonnees c, Tas* t)
{
    int i,j;
     
    //on visite tous les sommets du graphe
    for( i=1 ; i<=L ; i++ )
    {
        //on vérifie si le sommet à déjà été visité
        if( t->pos[i] == -1 )
        {
            //Si c'est le cas on parcours tous ses voisins
            for( j=1 ; j<=L ; j++ )
            {
                double d = getDist(c,i,j);
                //on vérifie s'ils ont déjà été visité et si sa priorité initiale est sup à celle avec le sommet i
                printf("on vérifie si sommet déja visité\n");
                if( (t->pos[j] != -1) && (d < t->priorite[t->pos[j]]) )
                {
                    //si c'est la cas
                    //on supprime l'element avec la mauvaise priorite
                    printf("on supprime l'élément avec la mauvaise priorité\n\n");
                    supp2(t,j);

                    //on le réinsère avec la priorite mise à jour et on met à jour le sommet d'origine qui a causé la mise à jour
                    insert2(t,j,d,i);
                    printf("\n-Tas après réinsertion et maj de la la prio        (n=%d)",t->n);
                    affiche_tas(*t);

                }
            }
        }
    }
}

void prim(Coordonnees c, int dep)
{
	Graphe g;//on crée un graphe dans prim seulement
    g = creer_graphe(c->n);

    Tas t = new_tas(L+1, dep);
    
    //on crée une variable somme qui va mesurer le poids de l'arbre couvrant
    double s = 0.0;
    
    int i;
    for ( i=0 ; i<L ; i++ )
    {
        //insertion des sommets de 1 à n dans le tas avec comme priorite leur distance au sommet de départ dep
        insert2(&t, i+1, getDist(c,dep,i),dep);
        affiche_tas( t );//OK
        printf("on a inséré le sommet n° %d:\n\n", i);
    }

    printf("\n--------------\n1er tas obtenu avec Prim:	----------------------\n");
    affiche_tas(t);

	//on recupère le min qui est le sommet de départ
	printf("on récupère le minimum \n\n");
    recup_min(&t);
    
    //on examine les sommet et on update les priorités tant que le tas n'est pas vide
    while( !est_vide(t ))
    {
		printf("\n---------\nOn commence l'update des sommets:");
        updatePrio(c,&t);
        printf("-----------\n");
        
        //On ajoute au poids de l'arbre le poids de la liaison la moins coûteuse
        s += getDist(c,t.origine[0],t.tab[0]);
        //On crée l'arrête entre le sommet prioritaire et son origine
        printf("ON AJOUTE L'ARETE \n\n\n");
		// graphe_ajouter_arete(g,t.origine[1], t.tab[1]);
        recup_min(&t);
    }
	afficher_graphe(c,g);
	detruire_graphe(g);
	//On libère le tas
    free_tas(t);
    
    //On affiche le poids de l'arbre
    printf("\n\n----------------------\n\tPOIDS = %lf\n",s);
    printf("----------------------\n");
}



//--------------------------------------MAIN-----------------------------------------------

int main(int argc, char *argv[])
{
	//double mat[L][K] = {{565.0,575.0},{25.0,185.0},{345.0,750.0},{945.0,685.0},{845.0,655.0},{880.0,660.0},{25.0,230.0},{525.0,1000.0},{580.0,1175.0},{650.0,1130.0}};
	Coordonnees c;
	/* lecture de l'instance */
	c = lecture_instance(argv[1]);
	
	//initialisation d'un tableau
	Tas t = new_tas(C,0);
	/*insert2(&t,1,12,0);
	//affiche_tas(t);
	insert2(&t,2,5,0);
	//affiche_tas(t);
	insert2(&t,3,6,0);
	//affiche_tas(t);
	insert2(&t,4,7,0);
	//affiche_tas(t);
	insert2(&t,5,14,0);
	//affiche_tas(t);
	insert2(&t,6,4,0);*/

	affiche_tas(t);
	/*printf("\ntache prioritaire: %d\tpriorite: %3.2f\n",tache_prioritaire(t),*(t.priorite+1));
	supp(&t,6);*/
	
	/*
	printf("\ninsertion de (7,2)");
	insert2(&t,7,2,0);
	affiche_tas(t);

	recup_min(&t);
	affiche_tas(t);
	printf("\ntache prio: %d\n",tache_prioritaire(t));

	printf("Suppression de l'élément 5");
	supp2(&t,5);
	affiche_tas(t);*/

	free_tas(t);
	
	prim(c,0);
	detruire_coordonnees(c);
	
	return 0;
}
