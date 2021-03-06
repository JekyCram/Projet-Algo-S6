// 1) Faux. Contre exemple parmis l'exemple d'un tas.
// 2) à la racine
// 3) l'entier supérieur de log base 2 de n.

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define C 100

typedef struct tas
{
	int capacite_max;		//nombre maximum d'éléments dans le tas
	int n; 					//nombre d'élément dans le tas
	int *tab;				//tableau des éléments du tas (de tab[1] à tab[n])
	double *priorite;			//tableau des cles des éléments du tas
	int *pos; 				//tableau des positions des éléments du tas
}Tas;

Tas new_tas(int capa_max)
{
	Tas t;
	t.capacite_max = capa_max;
	t.n=0;
	t.tab = (int*) malloc ( t.capacite_max * sizeof(int) );
	t.priorite = (double*) malloc ( t.capacite_max * sizeof(double) );
	t.pos = (int*) malloc ( t.capacite_max * sizeof(int) );

	int i;
	t.tab[0] = 0;
	t.pos[0] = -1;
	for( i=1 ; i<t.capacite_max ; i++ )
	{
		*(t.tab+i) = -1;
		*(t.pos+i) = -1;
	}

	return t;
}

void free_tas(Tas t)
{
	free(t.tab);
	free(t.priorite);
	free(t.pos);
}

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
	printf("\n");
}

// EXO 1.4
void insert(Tas *T, int x, double l)
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

			//printf("\nn=%d\ta=%d\ta/2=%d\n%d\t%d\t%d\n",T->n,a,a/2,*(T->pos+(a/2)),*(T->pos+*(T->pos+(a/2))),*(T->pos+T->n));
			/*temp = *(T->pos+*(T->pos+(a/2))); T->pos[pos[a/2]]
			*(T->pos+*(T->pos+(a/2))) = *(T->pos+T->n);
			*(T->pos+T->n) = temp;*/

			T->pos[T->tab[a]] = a;
			T->pos[T->tab[a/2]] = a/2;

			a = a/2;
		}
	}
}

//EXO 1.5
int tache_prioritaire(Tas t)
{
	return *(t.tab+1);
}

//EXO 2.2
int est_vide(Tas t)
{
	return t.tab[0] == 0 ;
}

int estFeuille(Tas t,int i)
{
    //printf("\n---\nfonction estFeuille\ntache fils gauche: %d\tposition: %d\n---\n",t.tab[2*i],t.pos[t.tab[2*i]]);
	return t.pos[t.tab[2*i]] == -1 || t.tab[2*i] == -1;
}

int aUnFilsD(Tas t, int i)
{
	return !(estFeuille(t,i)) && ((t.pos[t.tab[2*i+1]] != -1) || t.tab[2*i]== -1);
}

int feuilleG(Tas t, int i)
{
	if( !(estFeuille(t,i)) )
	{
		return 2*i;
	}
	else return 0;
}

int feuilleD(Tas t, int i)
{
	if( aUnFilsD(t,i) )
	{
		return 2*i+1;
	}
	else return 0;
}

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
			if( aUnFilsD(*T,ind) && T->priorite[feuilleD(*T,ind)] < T->priorite[ind] )
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

int recup_min(Tas *t)
{
	printf("\ntache prioritaire: %d\tpriorite: %3.2f\n",tache_prioritaire(*t),t->priorite[1]);
	supp(t,t->tab[1]);
	return tache_prioritaire(*t);
}






//--------------------------------------MAIN-----------------------------------------------

int main()
{
	//initialisation d'un tableau
	Tas t = new_tas(C);
	insert(&t,1,12);
	//affiche_tas(t);
	insert(&t,2,5);
	//affiche_tas(t);
	insert(&t,3,6);
	//affiche_tas(t);
	insert(&t,4,7);
	//affiche_tas(t);
	insert(&t,5,14);
	//affiche_tas(t);
	insert(&t,6,4);

	affiche_tas(t);
	/*printf("\ntache prioritaire: %d\tpriorite: %3.2f\n",tache_prioritaire(t),*(t.priorite+1));

	supp(&t,6);*/

	printf("\ninsertion de (7,2)");
	insert(&t,7,2);
	affiche_tas(t);

	recup_min(&t);
	affiche_tas(t);
	printf("\ntache prio: %d\n",tache_prioritaire(t));

	printf("Suppression de l'élément 5");
	supp(&t,5);
	affiche_tas(t);


	free_tas(t);
	return 0;
}
