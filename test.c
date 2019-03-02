#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define C 100
#define L 10
#define K 2


typedef struct tas
{
	int capacite_max;		//nombre maximum d'éléments dans le tas
	int n; 					//nombre d'élément dans le tas
	int *tab;				//tableau des éléments du tas (de tab[1] à tab[n])
	double *priorite;		//tableau des cles des éléments du tas
	int *pos; 				//tableau des positions des éléments du tas
	int *origine;           //tableau des sommets d'origine qui sont associés à la priorité
}Tas;

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
	

/* retourne l'identifiant de la tache prioritaire */
int tache_prioritaire(Tas t)
{
	return t.tab[1];
}

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

/* supprime du tas l'élément d'identifiant x en supposant qu'il est présent dans le tas */
/*void supp(Tas* T, int x)
{
	int *lg = &(T->tab[0]);
	int ind = T->pos[x];
	printf("\nind: %d",ind);

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

		//printf("\ninversion dans priorite\nprio[ind]: %lf\tprio[n]: %lf\n",T->priorite[ind],T->priorite[*(lg)]);
		double tempd = T->priorite[ind];
		T->priorite[ind] = T->priorite[*(lg)];
		T->priorite[*(lg)] = tempd;

		T->pos[T->tab[ind]] = ind;
		//T->pos[T->tab[ind/2]] = ind/2;
		T->pos[x] = -1;

		*(lg) -= 1;

		printf("\ntableau avant réinsertion sans la tache prio:");
		affiche_tas(*T);

		while( !(estFeuille(*T,ind) ) )
		{
			printf("tab[ind] (%d) n'est pas une feuille\n", T->tab[ind]);
			if( aUnFilsD(*T,ind) && T->priorite[feuilleD(*T,ind)] < T->priorite[ind] && T->priorite[feuilleD(*T,ind)] <= T->priorite[feuilleG(*T,ind)] )
			{
				printf("tab[ind] (%d) a un fils a droite: %d de priorite: %lf\n", T->tab[ind], T->tab[feuilleD(*T,ind)], T->priorite[feuilleD(*T,ind)]);
				int temp = T->tab[ind];
				T->tab[ind] = T->tab[feuilleD(*T,ind)];
				T->tab[feuilleD(*T,ind)] = temp;

				double tempd = T->priorite[ind];
				T->priorite[ind] = T->priorite[feuilleD(*T,ind)];
				T->priorite[feuilleD(*T,ind)] = tempd;

				T->pos[T->tab[ind]] = ind;
				T->pos[T->tab[feuilleD(*T,ind)]] = feuilleD(*T,ind);

				printf("Inversion done!");
				affiche_tas(*T);

				ind = feuilleD(*T,ind);
				printf("nouveau ind: %d\ttache: %d\n", ind, T->tab[ind]);
			}
			else if( T->priorite[feuilleG(*T,ind)] < T->priorite[ind] )
			{
				printf("tab[ind] (%d) a un fils a gauche: %d de priorite: %lf\n", T->tab[ind], T->tab[feuilleG(*T,ind)], T->priorite[feuilleG(*T,ind)]);
				int temp = T->tab[ind];
				T->tab[ind] = T->tab[feuilleG(*T,ind)];
				T->tab[feuilleG(*T,ind)] = temp;

				double tempd = T->priorite[ind];
				T->priorite[ind] = T->priorite[feuilleG(*T,ind)];
				T->priorite[feuilleG(*T,ind)] = tempd;

				T->pos[T->tab[ind]] = ind;
				T->pos[T->tab[feuilleG(*T,ind)]] = feuilleG(*T,ind);

				printf("Inversion done!");
				affiche_tas(*T);

				ind = feuilleG(*T,ind);
				printf("nouveau ind: %d\ttache: %d\n", ind, T->tab[ind]);
			}
			else
			{
			    printf("\npas de fils de priorite inf\n");
                break;
            }

		//affiche_tas(*T);

        }
        
        while( T->priorite[ind] < T->priorite[ind/2] && ind != 1) // *(T->priorite+a) < *(T->priorite+(a/2)) )
		{
			int temp = *(T->tab+(ind/2));
			*(T->tab+(ind/2)) = *(T->tab+ind);
			*(T->tab+ind) = temp;

			double tempd = *(T->priorite+(ind/2));
			*(T->priorite+(ind/2)) = *(T->priorite+ind);
			*(T->priorite+ind) = tempd;

			T->pos[T->tab[ind]] = ind;
			T->pos[T->tab[ind/2]] = ind/2;

			ind = ind/2;
		}
    }
}*/

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

double getDist(double m[L][K], int l1, int l2)
{
    int a = m[l2][0] - m[l1][0];
    int b = m[l2][1] - m[l1][1];
    return sqrt( (double) (a*a + b*b) );
    //return sqrt( a*a + b*b );
}

void updatePrio(double m[L][K], Tas* t)
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
                double d = getDist(m,i,j);
                //on vérifie s'ils ont déjà été visité et si sa priorité initiale est sup à celle avec le sommet i
                if( (t->pos[j] != -1) && (d < t->priorite[t->pos[j]]) )
                {
                    //si c'est la cas
                    //on supprime l'element avec la mauvaise priorite
                    supp2(t,j);

                    //on le reinsert avec la priorite mise à jour et on met à jour le sommet d'origine qui a causé la mise à jour
                    insert2(t,j,d,i);
                    printf("\n-Tas après réinsertion et maj de la la prio        (n=%d)",t->n);
                    affiche_tas(*t);

                }
            }
        }
    }
}

void prim(double m[L][K], int dep)
{
    //creer_graphe(c->n);

    Tas t = new_tas(L+1, dep);
    
    //on crée une variable somme qui va mesurer le poids de l'arbre couvrant
    double s = 0.0;
    
    int i;
    for ( i=0 ; i<L ; i++ )
    {
        //insertion des sommets de 1 à n dans le tas avec comme priorite leur distance au sommet de départ dep
        insert2(&t, i+1, getDist(m,dep,i),dep);
        affiche_tas( t );
    }

    printf("\n--------\n1er tas obtenue avec Prim:");
    affiche_tas(t);

	//on recupère le min qui est le sommet de départ
    recup_min(&t);
    
    //on examine les sommet et on update les priorités tant que le tas n'est pas vide
    while( !est_vide(t ))
    {
		printf("\n---------\nOn commence l'update des sommets:");
        updatePrio(m,&t);
        printf("-----------\n");
        
        //On ajoute au poids de l'arbre le poids de la laison la moins couteuse
        s += getDist(m,t.origine[0],t.tab[0]);
        //On crée l'arrête entre le sommet prioritaire et son origine
        //graphe_ajouter_arete(g,t.origine[1], t.tab[1]);
        recup_min(&t);
    }

	//On libère le tas
    free_tas(t);
    
    //On affiche le poids de l'arbre
    printf("\n\n----------------------\n\tPOIDS = %lf\n",s);
    printf("----------------------\n");
}



//--------------------------------------MAIN-----------------------------------------------

int main()
{
	double mat[L][K] = {{565.0,575.0},{25.0,185.0},{345.0,750.0},{945.0,685.0},{845.0,655.0},{880.0,660.0},{25.0,230.0},{525.0,1000.0},{580.0,1175.0},{650.0,1130.0}};
	
	//initialisation d'un tableau
	Tas t = new_tas(C,0);
	insert2(&t,1,12,0);
	//affiche_tas(t);
	insert2(&t,2,5,0);
	//affiche_tas(t);
	insert2(&t,3,6,0);
	//affiche_tas(t);
	insert2(&t,4,7,0);
	//affiche_tas(t);
	insert2(&t,5,14,0);
	//affiche_tas(t);
	insert2(&t,6,4,0);

	affiche_tas(t);
	/*printf("\ntache prioritaire: %d\tpriorite: %3.2f\n",tache_prioritaire(t),*(t.priorite+1));

	supp(&t,6);*/

	printf("\ninsertion de (7,2)");
	insert2(&t,7,2,0);
	affiche_tas(t);

	recup_min(&t);
	affiche_tas(t);
	printf("\ntache prio: %d\n",tache_prioritaire(t));

	printf("Suppression de l'élément 5");
	supp2(&t,5);
	affiche_tas(t);

	free_tas(t);
	
	prim(mat,0);
	
	return 0;
}
