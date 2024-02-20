#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *lett;
int contenitore,counter;

/* Start of tree structure declarations */
//Knot
typedef struct node{
    enum {red , black} colore;
    struct node *p,*left, *right;
    char parola[];
}RBTree;
//Tree
typedef struct{
    struct node *nil;
    struct node *root;
}Albero;

/* Start of list structure of the filtered words declaration */
typedef struct nodo{
    struct nodo *next;
    struct node *x;
}Filtrate;


/* Start of list structure of the constraints declaration */
//List of letters present in the word
typedef struct p{
    struct p *next;
    char Simbolo;
    int min;
    int esatto;
    char alterato;
    char posS[];
}Inside;
//List of letters not present in the word
typedef struct n{
    struct n *next;
    char Simbolo;
}NotIn;

//SORTING ALGORITHMS

//QUICKSORT
//Sort the word (array, start, end)
int Partition(char *A, int p, int r){
    char swap;
    int j;
    int x=A[r];
    int i=p-1;
    for(j=p;j<r;j++){
        if(A[j]<=x){
            i++;
            swap=A[j];
            A[j]=A[i];
            A[i]=swap;
        }
    }
    swap=A[i+1];
    A[i+1]=A[r];
    A[r]=swap;
    return i+1;
}
void Quicksort(char *A, int p, int r){
    int q;
    if(p<r){
        q = Partition(A, p, r);
        Quicksort(A, p, q-1);
        Quicksort(A, q+1, r);
    }
}

//MERGE SORT for lists
Filtrate *OrdinaLista(Filtrate *primo,Filtrate *secondo){
    Filtrate *risult=NULL;
    if(primo==NULL)
        return secondo;
    else if(secondo==NULL)
        return primo;
    if(strcmp(primo->x->parola,secondo->x->parola)<0){
        risult=primo;
        risult->next= OrdinaLista(primo->next,secondo);
    }else{
        risult=secondo;
        risult->next= OrdinaLista(primo,secondo->next);
    }
    return risult;
}

void DividiLista(Filtrate *ori, Filtrate **prima, Filtrate **seconda){
    Filtrate *p1,*p2;
    p2=ori;
    p1=ori->next;
    while(p1!=NULL){
        p1=p1->next;
        if(p1!=NULL){
            p1=p1->next;
            p2=p2->next;
        }
    }
    *prima = ori;
    *seconda=p2->next;
    p2->next=NULL;
}

void MergeSort(Filtrate **f){
    Filtrate *testa = *f,*p1,*p2;
    if((testa==NULL)||(testa->next==NULL)){
        return;
    }
    DividiLista(testa,&p1,&p2);
    MergeSort(&p1);
    MergeSort(&p2);
    *f = OrdinaLista(p1,p2);
}


//FUNCTIONS RED BLACK TREES

//Print tree
void StampaAlbero(Albero *T,RBTree *x){
    if(x != T->nil){
        StampaAlbero(T,x->left);
        printf("%s\n",x->parola);
        StampaAlbero(T, x->right);
    }
}

void RightRotate(Albero *T, RBTree *x){
    RBTree *y;
    y = x->left;
    x->left = y->right;
    if (y->right != T->nil)
        y->right->p = x;
    y->p = x->p;
    if (x->p == T->nil)
        T->root = y;
    else if(x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}

void LeftRotate(Albero *T, RBTree *x){
    RBTree *y;
    y = x->right;
    x->right = y->left;
    if(y->left != T->nil)
        y->left->p=x;
    y->p = x->p;
    if(x->p == T->nil)
        T->root = y;
    else if( x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

//Fixup insert red black tree-> teta(log(n))
void RBInsertFixup(Albero *T, RBTree *z){
    RBTree *x,*y;
    if(z==T->root)
        T->root->colore=black;
    else{
        x = z->p;
        if(x->colore==red){
            if(x == x->p->left) {
                y = x->p->right;
                //case 1
                if (y->colore == red) {
                    x->colore = black;
                    y->colore = black;
                    x->p->colore = red;
                    RBInsertFixup(T, x->p);
                    return;
                }
                    //case 2
                else if (z == x->right) {
                    z = x;
                    LeftRotate(T, z);
                    x = z->p;
                }
                //case 3
                x->colore = black;
                x->p->colore = red;
                RightRotate(T, x->p);
            }
            else{
                y = x->p->left;
                //case 1
                if (y->colore == red) {
                    x->colore = black;
                    y->colore = black;
                    x->p->colore = red;
                    RBInsertFixup(T, x->p);
                    return;
                }
                    //case 2
                else if (z == x->left) {
                    z = x;
                    RightRotate(T, z);
                    x = z->p;
                }
                //case 3
                x->colore = black;
                x->p->colore = red;
                LeftRotate(T, x->p);
            }
        }
    }
}
//T -> tree's root, z -> knot to insert
void RBInsert(Albero *T, RBTree *z){
    RBTree  *y,*x;
    y = T->nil;
    x = T->root;
    while(x!=T->nil){
        y = x;
        if(strcmp(z->parola, x->parola)<0)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if(y == T->nil)
        T->root = z;
    else if(strcmp(z->parola, y->parola)<0)
        y->left = z;
    else
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->colore = red;
    RBInsertFixup(T,z);
}

//Frees the memory allocated for the tree -> teta(n) whit n leght of the tree
void DeallocaAlbero(Albero *T, RBTree *x){
    if(x == T->nil)
        return;
    DeallocaAlbero(T, x->left);
    DeallocaAlbero(T, x->right);
    free(x);
}

//Searches the tree for the word, returning the node that contains it -> teta(log(n))
RBTree *RicercaInAlbero(Albero *T, RBTree *x, char *key){
    if(x == T->nil || strcmp(key, x->parola)==0)
        return x;
    if (strcmp(key,x->parola)<0)
        return RicercaInAlbero(T, x->left,key);
    else
        return RicercaInAlbero(T, x->right,key);
}


//WORDCHECKER FUNCTIONS

//Inserts a symbol into the list of symbols not present or returns its head if already present
NotIn *InserisciNotIn(char s, NotIn **No, int *mod){
    NotIn *corr=*No;
    while(corr!=NULL){
        if(corr->Simbolo==s)
            return *No;
        corr=corr->next;
    }
    
    NotIn *new = (NotIn*)malloc(sizeof(NotIn));
    new->Simbolo=s;
    new->next=*No;
    mod[1]++;
    return new;
}

/* Inserts the new letter into the list of existing letters, if it is not already there. 
Returns the target letter node. */
Inside *InserisciPresenti(char s, Inside **presenti, int l, int *mod){
    Inside *curr=*presenti;
    int i;
    while(curr!=NULL){
        if(curr->Simbolo==s)
            return curr;
        curr=curr->next;
    }
    
    mod[2]=1;
    Inside *new = (Inside*)malloc(sizeof(Inside)+l+1);
    new->Simbolo=s;
    new->min=0;
    new->esatto=0;
    new->alterato='1';
    for(i=0;i<l;i++)
        new->posS[i]='0';
    new->posS[l]='\0';
    new->next=*presenti;
    *presenti=new;
    return new;
}

//Update the constraints after calculating the "+ / |" results
void AggiornaVincoli(char *ris, char *sim, int *rip, int l,char *giusta,Inside **presenti,NotIn **No, int *mod){
    Inside *sup;
    int n,flag;
    int controllata[l],i,k,j;
    for(i=0;i<l;i++)
        controllata[i]=0;

    for(i=0;i<l;i++){
        k=0;
        while(lett[i]!=sim[k] && sim[k]!='\0' && sim[k]!='*')
            k++;
        if(sim[k]=='\0' || sim[k]=='*'){
            *No = InserisciNotIn(lett[i], No, mod);
            controllata[i]=1;
        }else{
            flag=0;
            sup = InserisciPresenti(lett[i], presenti, l, mod);
            n = rip[k];
            for(j=i;j<l;j++) {
                if (ris[j] == '+' && lett[j]==sup->Simbolo && controllata[j]==0){
                    mod[0]=1;
                    giusta[j] = sup->Simbolo;
                    controllata[j]=1;
                    n--;
                } else if (ris[j]=='|' && lett[j]== sup->Simbolo && controllata[j]==0){
                    sup->posS[j]='1';
                    sup->alterato='1';
                    controllata[j]=1;
                    n--;
                }else if(ris[j]=='/' && lett[j]== sup->Simbolo && controllata[j]==0){
                    controllata[j]=1;
                    sup->posS[j]='1';
                    flag=1;
                }
            }
            if(flag==0){
                if(sup->min<rip[k]-n){
                    sup->min=rip[k]-n;
                    sup->alterato='1';
                }
            }
            else{
                if(sup->esatto==0){
                    sup->esatto=rip[k]-n;
                    sup->alterato='1';
                }
            }
            if(sup->alterato=='1')
                mod[2]=1;
        }
    }
}

//Function that updates the vectors: of the repetitions of a letter and of the alphanumeric value of the letter present in the inserted word comparing it with the target word -> teta(n) with n lenght of the word
void contaPres(char *ord,char *sim, int *rip, int n){
    int i,j=0;

    for(i=0;i<n;i++) {
        rip[i] = 0;
        sim[i] = '*';
    }
    sim[0]=ord[0];
    for(i=0;i<n;i++){
        if(sim[j]==ord[i]){
            rip[j]++;
        }else{
            j++;
            sim[j]=ord[i];
            rip[j]=1;
        }
    }
}

//Insert all the input words -> teta(n)
void InserisciInizio(Albero *T, int l){
    contenitore = scanf("%s",lett);
    while(strcmp(lett,"+inserisci_fine")!=0){
        RBTree *new = (RBTree*)malloc(sizeof(RBTree)+l+1);
        strcpy(new->parola,lett);
        RBInsert(T,new);
        contenitore = scanf("%s",lett);
    }
}

//confronta le parole e restituisce un risultato +/|, inoltre riempe il vettore dei simboli presenti nella parola letta e il vettore rip che segna quante ripetizioni ci sono per quella lettera
//caso pessimo teta(4l)
char *Confronto(int l, char *rif, int *rip, char *sim, char *ris){
    int i,j,riplocale[l];
    memcpy(riplocale, rip, sizeof(int)*l);

    for(i=0;i<l;i++) {
        j = 0;
        if (lett[i] == rif[i]) {
            ris[i] = '+';
            while(sim[j] != lett[i] && sim[j] != '*')
                j++;
            riplocale[j]--;
        }else{
            ris[i]='*';
        }
    }
   
    for (i = 0; i < l; i++){
        j=0;
        while(sim[j]!=lett[i] && sim[j]!='\0')
            j++;
        if(ris[i]!='+') {
            if (rif[i] != lett[i] && sim[j] != '\0' && riplocale[j] != 0) {
                ris[i] = '|';
                riplocale[j]--;
            } else
                ris[i] = '/';
        }
    }
    return ris;
}

//controlla che non ci siano lettere non ammesse in RIF
int ControllaNotIn(NotIn *Not, int *mod, char *par,int l){
    int a=mod[1],i;
    while(a>0){
        for(i=0;i<l;i++)
            if(par[i]==Not->Simbolo)
                return 1;
        Not=Not->next;
        a--;
    }
    return 0;
}

//scorre tutte le parole presenti che sono state modificate e ne controlla i vincoli
int ControllaPresenti(Inside *In,int l,char *x){
    int i,n;
    while(In!=NULL){
        n=0;
        if(In->alterato=='1'){
            for (i = 0; i < l; i++) {
                if(In->posS[i]=='1' && x[i]==In->Simbolo)
                    return 1;
                if (x[i] == In->Simbolo)
                    n++;
            }
            if (In->esatto != 0 && In->esatto != n)
                return 1;
            else if (In->min > n)
                return 1;
        }
        In=In->next;
    }
    return 0;
}

//Controlla se la parola in ingresso rispetta i vincoli
int ControllaVincoli(int l,char *x,char *giusta,Inside *In, NotIn *Not, int *mod){
    int i;
    if(mod[1]>0){
        if (ControllaNotIn(Not, mod, x, l) == 1)
            return 1;
    }
    
    if(mod[0]>0){
        for (i = 0; i < l; i++)
            if (x[i] != giusta[i] && giusta[i] != '*')
                return 1;
    }

    if(mod[2]>0){
        if(ControllaPresenti(In, l, x)==1)
            return 1;
    }
    return 0;
}

//Riempe la lista delle filtrate con le parole nell'albero delle possibili
void Riempimento(Albero *T,RBTree *x, Filtrate **f, char *giusta, Inside *In, NotIn *Not, int l, int *mod){
    if(x != T->nil) {
        Riempimento(T, x->left, f, giusta, In, Not, l,mod);
        if (ControllaVincoli(l, x->parola, giusta, In, Not, mod) == 0) {
            counter++;
            Filtrate *new = (Filtrate *) malloc(sizeof(Filtrate));
            new->x = x;
            new->next = *f;
            *f = new;
        }
        Riempimento(T, x->right, f, giusta, In, Not, l, mod);
    }
}

//toglie le parole che non rispettano più i vincoli
void Scremazione(Filtrate **f, char *giusta, Inside *In, NotIn *Not, int l, int *mod){
    Filtrate *curr=*f,*prec=NULL,*elim;
    while(curr!=NULL) {
        if (ControllaVincoli(l, curr->x->parola, giusta, In, Not, mod) == 1) {
            if(prec==NULL){
                counter--;
                elim=curr;
                curr=curr->next;//vado avanti
                free(elim);
                *f=curr;
            }else{
                counter--;
                prec->next=curr->next;
                elim=curr;
                curr=curr->next;
                free(elim);
            }
        }else{
            prec=curr;
            curr=curr->next;
        }
    }
}

//aggiorna la lista delle parole filtrate, se è vuota confronta l'albero delle parole possibili e le inserisce nella lista, se la lista ha già delle parole invece la screma dalle parole che non rispettano più i vincoli
void AggiornaFiltrate(Filtrate **filtr, char *giusta, Inside *In, NotIn *Not, Albero *T,int l, int *mod){
    if(counter==0)
        Riempimento(T,T->root, filtr, giusta, In ,Not,l,mod);
    else
        Scremazione(filtr, giusta, In, Not, l, mod);
    mod[0]=0;
    mod[1]=0;
    mod[2]=0;
    while(In!=NULL){
        if(In->alterato=='1')
            In->alterato='0';
        In=In->next;
    }
}

//stampa la lista delle parole filtrate
void StampaFiltrate(Filtrate *f){
    while(f!=NULL){
        printf("%s\n",f->x->parola);
        f=f->next;
    }
}

//controlla che non ci siano lettere non ammesse
int ControllaNotInIntegrale(NotIn *Not,char *par, int l){
    int i;
    while(Not!=NULL){
        for(i=0;i<l;i++)
            if(par[i]==Not->Simbolo)
                return 1;
        Not=Not->next;
    }
    return 0;
}

//controlla la parola con tutte le lettere in presenti
int ControllaPresentiIntegrale(Inside *In, int l, char *par){
    int i,n;
    while(In!=NULL) {
        n = 0;
        for (i = 0; i < l; i++) {
            if (In->posS[i] == '1' && par[i] == In->Simbolo)
                return 1;
            if (par[i] == In->Simbolo)
                n++;
        }
        if (In->esatto != 0 && In->esatto != n)
            return 1;
        else if (In->min > n)
            return 1;
        In = In->next;
    }
    return 0;
}

//controlla tutti i vincoli senza tener conto di quali sono stati modificati per ultimi
int ControllaVincoliIntegrale(int l, char *par, char *giusta, Inside *In, NotIn *Not){
    int i;
    if (ControllaNotInIntegrale(Not, par, l) == 1)
        return 1;
    for (i = 0; i < l; i++)
        if (par[i] != giusta[i] && giusta[i] != '*')
            return 1;
    if(ControllaPresentiIntegrale(In, l, par)==1)
        return 1;
    return 0;
}

//Inserisce le parole nell'albero delle parole possibili, e nel filtrato se rispettano i vincoli
void InserisciInizioGame(Albero *T,Filtrate **filtr,char *giusta,NotIn *Not,Inside *In, int l){
    contenitore = scanf("%s",lett);
    while(strcmp(lett,"+inserisci_fine")!=0){
        RBTree *new = (RBTree *) malloc(sizeof(RBTree) + l + 1);
        strcpy(new->parola,lett);
        RBInsert(T, new);
        if(ControllaVincoliIntegrale(l, new->parola, giusta, In,  Not)==0){
            Filtrate *nuovo = (Filtrate*)malloc(sizeof(Filtrate));
            nuovo->x=new;
            nuovo->next=*filtr;
            *filtr=nuovo;
            counter++;
        }
        contenitore = scanf("%s",lett);
    }
}

//Dealloca i nodi rimanenti nell'albero filtrato
void DeallocaFiltrate(Filtrate *f){
    Filtrate *elim;
    while(f!=NULL){
        elim=f;
        f=f->next;
        free(elim);
    }
}

//Dealloca i nodi degli elementi di NotIN
void DeallocaNotIn(NotIn *Not){
    NotIn *Elim;
    while(Not!=NULL){
        Elim=Not;
        Not=Not->next;
        free(Elim);
    }
}

//Dealloca i nodi degli elementi in Presenti
void DeallocaPresenti(Inside *Pres){
    Inside *Elim;
    while(Pres!=NULL){
        Elim=Pres;
        Pres=Pres->next;
        free(Elim);
    }
}

//funzione della paertita
void NuovaPartita(Albero *T,int l){
    int i,mod[3]={0,0,0};//mod[0] serve a dire se giuste è stato modificato, mod[1]=indica quanti caratteri nuovi sono stati aggiunti a notIN, mod[2] indica che alcuni caratteri in presenti sono stati modificati
    char rif[l+1],sim[l+1],app[l+1];//sim tiene conto dei simboli di rif, app stringa dì appoggio della lunghezza della parola per tutte le operazioni
    char ris[l+1];
    ris[0]='*';
    ris[l]='\0';

    //dichiarazioni per i vincoli
    char giusta[l+1];//vettore in cui metto le posizioni giuste
    for(i=0;i<l;i++)
        giusta[i]='*';
    giusta[l]='\0';

    Inside *presenti=NULL;//alloco la lista per le lettere presenti nel riferimento e i relativi vincolo
    NotIn *nInside=NULL;//alloco la lista delle lettere che non sono contenute nel riferimento

    int tentativi, rip[l];//rip contiene le ripetizioni nel riferimento ordinate in modo da non ricalcolarle ogni volta in confronta
    //acquisisco il riferimento della partita
    contenitore = scanf("%s",rif);
    strcpy(app,rif);
    Quicksort(app,0,l-1);
    contaPres(app, sim, rip, l);//ora sim contiene i simboli del riferimento e rip la relativa molteplicità
    sim[l]='\0';

    //LISTA DELLE PAROLE FILTRATE
    Filtrate *filtr=NULL;

    //acquisisco il numero di tentativi della partita
    contenitore = scanf("%d",&tentativi);

    //inizializzo il counter dell'albero filtrato a 0
    counter =0;
    //inizio della partita
    while(tentativi>0) {
        //acquisisco parola da confrontare
        contenitore = scanf("%s", lett);

        //stampo la struttura delle filtrate
        if (strcmp(lett, "+stampa_filtrate") == 0){
            if(ris[0]!='*'){//se ho inserito almeno un risultato significa che l'albero delle filtrate è stato modificato, se non lo ho fatto tutte le parole possono essere la soluzione
                MergeSort(&filtr);
                StampaFiltrate(filtr);
            }else{
                StampaAlbero(T,T->root);
            }
        }
            //inserisco le parole nell'albero e se rispettano i vincoli anche nella struttura delle parole filtrate
        else if (strcmp(lett, "+inserisci_inizio") == 0) {
            if(counter==0)
                InserisciInizio(T,l);
            else
                InserisciInizioGame(T,&filtr,giusta,nInside,presenti,l);
        }
            //tentativo
        else {
            //se non ho indovinato controllo
            if(strcmp(lett,rif)!=0) {
                //se trovo la parola nelle ammissibili allora la controllo
                if (strcmp(RicercaInAlbero(T, T->root, lett)->parola, lett) == 0) {

                    //confronto la parola con il riferimento e tiro fuori un risultato -> obiettivo teta(1) ottenuto teta(4l)
                    printf("%s\n", Confronto(l, rif, rip, sim, ris));

                    //aggiorno i vincoli --> min e esatto più facili da calcolare perchè avro la parolina ordinata
                    AggiornaVincoli(ris, sim, rip, l, giusta, &presenti, &nInside, mod);

                    //filtro la struttura delle filtrate --> controllavincoli da ottimizzare al massimo (parte più importante)
                    AggiornaFiltrate(&filtr, giusta, presenti, nInside, T, l, mod);

                    //stampo la sua lunghezza (tranquillamente con un contatore)
                    printf("%d\n",counter);

                    //decremento i tentativi
                    tentativi--;
                    //se dopo aver controllato tutto non ci sono più tentativi stampo ko
                    if (tentativi == 0)
                        printf("ko\n");
                } else {
                    //se non trovo la parola
                    printf("not_exists\n");
                }
            }else{
                //se ho indovinato
                printf("ok\n");
                tentativi=0;
            }
        }
    }
    //libera le filtrate
    DeallocaFiltrate(filtr);
    //libera notin
    DeallocaNotIn(nInside);
    //libera presenti
    DeallocaPresenti(presenti);
}

//MAIN FUNCTION
int main(){
    int l;
    //allocation memory for Tree of potential words and init of the nil knot
    Albero *a=(Albero*)malloc(sizeof(Albero));
    a->nil = (RBTree *) malloc(sizeof(RBTree)+2);
    a->nil->colore=black;
    a->nil->parola[0]=' ';
    a->nil->parola[1]='\0';
    a->root=a->nil;

    contenitore=scanf("%d",&l);

    //init variables
    if(l<17){
        lett = (char*)malloc(sizeof(char)*18);
    }else{
        lett = (char*)malloc(sizeof(char)*l+1);
    }
    contenitore=scanf("%s",lett);

    //init Tree of potential words
    while(strcmp(lett,"+nuova_partita")!=0 && strcmp(lett,"+inserisci_inizio")!=0){
        RBTree *new = (RBTree*)malloc(sizeof(RBTree)+l+1);//inizializzo un nuovo nodo dell'albero
        strcpy(new->parola,lett);
        RBInsert(a,new);//lo inserisco nell'albero
        contenitore=scanf("%s",lett);
    }

    //start new game or insert new possible words
    do{
        if(strcmp(lett,"+nuova_partita")==0)
            NuovaPartita(a,l);
        else if(strcmp(lett,"+inserisci_inizio")==0){
            InserisciInizio(a,l);
        }
    }while(scanf("%s", lett) != EOF);

    //free all the memory
    DeallocaAlbero(a,a->root);
    free(a->nil);
    free(a);
    free(lett);
    return 0;
}
