#include<stdio.h>
#include<stdlib.h>

typedef struct no{
    int id;
    int cor; // 0 preto, 1 vermelho
    struct no *pai;
    struct no *esq;
    struct no *dir;
}No;

typedef struct raiz{
    No* raiz;
}Raiz;

void inicializar(Raiz *r);
No *novoNo(int id);
void rotacaoEsq(Raiz *r, No *no);
void rotacaoDir(Raiz *r, No *no);
void insertFixup(Raiz *r, No *no);
void insert(Raiz *r, int id);
void deleteNode(Raiz *r, Raiz *auxR, int id);
void deleteFixup(Raiz *r, No *x);
void exibirArvore(No *no);
int alturaTotal(No *no);
int alturaNegra(No *no);
int calcularCustoBusca(No *no, int chave, int custoAtual);
void calcularPorcentagem(No *no, int *numRubros, int *numNegros);
void calcularPorcentagemNos(Raiz *r);

int main(){

    int id, op;
    Raiz r;

    inicializar(&r);

    do{
        printf("\n\t0 - Sair\n\t1 - Inserir\n\t2 - Excluir\n\t3 - Exibir\n\t4 - Exibir altura total\n\t5 - Exibir altura negra");
        printf("\n\t6 - Exibir o custo de uma busca\n\t7 - Calcular porcentagem rubra e porcentagem negra\n\t");
        scanf("%d", &op);

        switch(op){
        case 0:
            printf("Programa finalizado\n");
            break;
        case 1:
            printf("Digite o ID do aluno: ");
            scanf("%d", &id);
            insert(&r, id);
            break;
        case 2:
            printf("Digite o ID a ser excluido: ");
            scanf("%d", &id);
            deleteNode(&r, &r, id);
            break;
        case 3:
            printf("Arvore em Ordem com parenteses para os filhos: \n");
            exibirArvore(r.raiz);
            break;
        case 4:
            printf("Altura total da arvore: %d\n", alturaTotal(r.raiz));
            break;
        case 5:
            printf("Altura negra da arvore: %d\n", alturaNegra(r.raiz));
            break;
        case 6:
            printf("Digite o ID do aluno para calcular o custo de busca: ");
            scanf("%d", &id);
            printf("Custo de busca para o aluno %d: %d nos\n", id, calcularCustoBusca(r.raiz, id, 1));
            break;
        case 7:
            calcularPorcentagemNos(&r);
            break;
        default:
            printf("Opcao invalida\n");
        }
    } while(op != 0);

    return 0;
}

void inicializar(Raiz *r){
    r->raiz = NULL;
}

No *novoNo(int id){
    No *novo = malloc(sizeof(No));

    if(novo){
        novo->id = id;
        novo->cor = 1;
        novo->pai = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        
    } else {
        printf("Erro ao alocar memoria\n");
    }
    return novo;
}

void rotacaoEsq(Raiz *r, No *x){
    No *y;
    y = x->dir; // y = fDir
    x->dir = y->esq;

    if(y->esq != NULL){
        y->esq->pai = x;
    }
    y->pai = x->pai;

    if(x->pai == NULL){
        r->raiz = y;
    }

    else if(x == x->pai->esq){
        x->pai->esq = y;
    }

    else{
        x->pai->dir = y;
    }

    y->esq = x;
    x->pai = y;
}

void rotacaoDir(Raiz *r, No *y){
    No *x;
    x= y->esq; // x = fEsq
    y->esq = x->dir;

    if(x->dir != NULL){
        x->dir->pai = y;
    }
    x->pai = y->pai;

    if(y->pai == NULL){
        r->raiz = x;
    }

    else if(y == y->pai->dir){
        y->pai->dir = x;
    }

    else{
        y->pai->esq = x;
    }

    x->dir = y;
    y->pai = x;
}

void insertFixup(Raiz *r, No *no){
    No *y;
    while(no != r->raiz && no->pai->cor == 1){
        /*Caso: A
        Pai do no é filho esquerdo do avo do no*/
        if(no->pai == no->pai->pai->esq){
            y = no->pai->pai->dir;

            /*Caso: 1
            Tio do nó também é vermelho, apenas recolorir*/
            if(y != NULL && y->cor == 1){
                printf("Caso 1: O pai fica negro, irmao fica negro e avo rubro.\n");
                no->pai->cor = 0;
                y->cor = 0;
                no->pai->pai->cor = 1;
                no = no->pai->pai;
            }

            else{
                /*Caso: 2
                Nó é filho direito, realiza rotação esquerda*/
                if(no == no->pai->dir){
                    printf("Caso 2: O pai sofre rotacao a esquerda.\n");
                    no = no->pai;
                    rotacaoEsq(r, no);
                }

                /*Caso: 3
                Nó é filho esquerdo, realiza rotação direita*/
                printf("Caso 3: O pai fica negro, o avo fica rubro e o avo sofre rotacao a direita.\n");
                no->pai->cor = 0;
                no->pai->pai->cor = 1;
                rotacaoDir(r, no->pai->pai);
            }
        }

        /*Caso: B
        Pai do no é filho direito do avô do nó*/
        else{
            y = no->pai->pai->esq;

            /*Caso: 1
            Tio do nó também é vermelho, apenas recolorir*/
            if(y != NULL && y->cor == 1){
                printf("Caso 1: O pai fica negro, irmao fica negro e avo rubro.\n");
                no->pai->cor = 0;
                y->cor = 0;
                no->pai->pai->cor = 1;
                no = no->pai->pai;
            }

            else{
                /*Caso: 2
                Nó é filho esquerdo, realizar rotação direita*/
                if(no == no->pai->esq){
                    printf("Caso 2: O pai sofre rotacao a direita.\n");
                    no = no->pai;
                    rotacaoDir(r, no);
                }

                /*Caso: 3
                Nó é filho direito, realizar rotação esquerda*/
                printf("Caso 3: O pai fica negro, o avo fica rubro e o avo sofre rotacao a esquerda.\n");
                no->pai->cor = 0;
                no->pai->pai->cor = 1;
                rotacaoEsq(r, no->pai->pai);
            }
        }
    }
    r->raiz->cor = 0; // Raiz sempre preta
}

void insert(Raiz *r, int id){
    No *z = novoNo(id);
    No *y = NULL; // y= pai
    No *aux = r->raiz; // Ponteiro para percorrer a arvore

    while(aux != NULL){
        y = aux;

        if(id < aux->id){
            aux = aux->esq;
        }

        else if(id > aux->id){
            aux = aux->dir;
        }

        else{
            printf("Insercao nao realizada, aluno presente na arvore\n");
            return;
        }
    }

    z->pai = y;

    if(y == NULL){
        r->raiz = z;
    }

    else if(id < y->id){
        y->esq = z;
    }

    else{
        y->dir = z;
    }

    z->esq = NULL;
    z->dir = NULL;
    z->cor = 1;

    printf("Aluno com ID %d inserido na arvore\n", id);
    insertFixup(r, z);
}

void deleteFixup(Raiz *r, No *x){
    No *w;
    while(x != r->raiz && x->cor == 0){
        if(x == x->pai->esq){
            w = x->pai->dir; // w é irmão de x

            //Caso 1: O irmão w de x é vermelho, w precisa ter irmão preto
            if(w->cor == 1){
                printf("Caso 1: O irmao fica negro, pai fica rubro e sofre rotacao a esquerda\n");
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoEsq(r, x->pai);
                w = x->pai->dir;
            }

            //Caso 2: O irmão w de x é preto, e ambos os filhos de w são pretos
            if(w != NULL && (w->esq == NULL || w->esq->cor == 0) && (w->dir == NULL || w->dir->cor == 0)){
                printf("Caso 2: O irmao fica rubro e verifica o balanceamento do pai do no removido\n");
                w->cor = 1;
                x = x->pai;
            }

            //Caso 3: O irmão w de x é preto, o filho esquerdo de w é vermelho e o direito preto
            else if(w != NULL && (w->dir == NULL || w->dir->cor == 0)){
                printf("Caso 3: O irmao fica rubro, sobrinho esquerdo fica negro e o irmao sofre rotacao a direita\n");
                w->esq->cor = 0;
                w->cor = 1;
                rotacaoDir(r, w);
                w = x->pai->dir;
            }

            //Caso 4: O irmao w de x é preto e filho direito é vermelho
            else{
                printf("Caso 4: O pai fica negro, irmao fica rubro, sobrinho direito fica negro e o pai sofre rotacao a esquerda\n");
                w->cor = x->pai->cor;
                x->pai->cor = 0;
                w->dir->cor = 0;
                rotacaoEsq(r, x->pai);
                x = r->raiz;
            }

        // x é filho direito
        } else {
            w = x->pai->esq;
            //Caso 1: O irmão w de x é vermelho, w precisa ter irmão preto
            if(w->cor == 1){
                printf("Caso 1: O irmao fica negro, pai fica rubro e sofre rotacao a direita\n");
                w->cor = 0;
                x->pai->cor = 1;
                rotacaoDir(r, x->pai);
                w = x->pai->esq;
            }

            //Caso 2: O irmão w de x é preto, e ambos os filhos de w são pretos/
            if(w != NULL && (w->dir == NULL || w->dir->cor == 0) && (w->esq == NULL || w->esq->cor == 0)){
                printf("Caso 2: O irmao fica rubro e verifica o balanceamento do pai do no removido\n");
                w->cor = 1;
                x = x->pai;
            }

            //Caso 3: O irmão w de x é preto, o filho esquerdo de w é vermelho e o direito preto/
            else if(w != NULL && (w->esq == NULL || w->esq->cor == 0)){
                printf("Caso 3: O irmao fica rubro, sobrinho direito fica negro e o irmao sofre rotacao a esquerda\n");
                w->dir->cor = 0;
                w->cor = 1;
                rotacaoEsq(r, w);
                w = x->pai->esq;
            }

            //Caso 4: O irmao w de x é preto e filho direito é vermelho/
            else{
                printf("Caso 4: O pai fica negro, irmao fica rubro, sobrinho esquerdo fica negro e o pai sofre rotacao a direita\n");
                w->cor = x->pai->cor;
                x->pai->cor = 0;
                w->esq->cor = 0;
                rotacaoDir(r, x->pai);
                x = r->raiz;
            }
        }
    }
    x->cor = 0; // Raiz sempre negra
}

void deleteNode(Raiz *r, Raiz *auxR, int id){
    No *aux = auxR->raiz;

    if(aux == NULL){
        printf("A arvore esta vazia, nao ha elementos para remover\n");
        return;
    }

    // Busca no 
    while(aux != NULL && aux->id != id){
        if(id < aux->id){   
            aux = aux->esq;
        }
        else{
            aux = aux->dir;
        }
    }

    if(aux == NULL){
        printf("Aluno nao encontrado\n");
        return;
    }

    // Remove raiz
    if(aux == r->raiz && aux->esq == NULL && aux->dir == NULL && aux->pai == NULL){
        free(aux);
        r->raiz = NULL;
        return;
    }
    
    // Remove folha
    if(aux->esq == NULL && aux->dir == NULL){
        if(aux->cor == 0){
            deleteFixup(r, aux);
        }

        if(aux == aux->pai->esq){
            aux->pai->esq = NULL;
        }

        else{
            aux->pai->dir = NULL;
        }
        free(aux);
        printf("O aluno com ID %d foi removido da arvore\n", id);
    } 
    
    // Remove com filhos
    else{
        No *sucessor;
        Raiz rSucessor;

        if(aux->esq != NULL){
            sucessor = aux->esq;

            while (sucessor->dir != NULL){
                sucessor = sucessor->dir;
            }
            aux->id = sucessor->id;
            sucessor->id = id;
            rSucessor.raiz = aux->esq;
        }
        else {
            sucessor = aux->dir;

            while (sucessor->esq != NULL){
                sucessor = sucessor->esq;
            }

            aux->id = sucessor->id;
            sucessor->id = id;
            rSucessor.raiz = aux->dir;
        }
        // Chama recursivamente a função para remover o nó sucessor
        deleteNode(r, &rSucessor, id);
    }
    printf("O aluno com ID %d foi removido da arvore.\n", id);
}  

void exibirArvore(No *no){
    if (no != NULL) {
        printf("id: %d cor: %d", no->id, no->cor);
        if (no->esq != NULL || no->dir != NULL) {
            printf(" (");
            exibirArvore(no->esq);
            printf(", ");
            exibirArvore(no->dir);
            printf(")");
        }
    }
}

int alturaTotal(No *no){
    if (no == NULL){
        return 0;
    } else {
        int alturaEsq = alturaTotal(no->esq);
        int alturaDir = alturaTotal(no->dir);
        return alturaEsq > alturaDir ? alturaEsq + 1 : alturaDir + 1;
    }
}

int alturaNegra(No *no){
    if (no == NULL){
        return 0;
    }

    int esquerda = alturaNegra(no->esq);
    int direita = alturaNegra(no->dir);

    if (no->cor == 1){
        return esquerda > direita ? esquerda : direita;
    }
    else{
        return 1 + (esquerda > direita ? esquerda : direita);
    }
}

int calcularCustoBusca(No *no, int chave, int custoAtual){
    if (no == NULL){
        return 0; 
    }

    if (chave == no->id){
        return custoAtual;
    } else if (chave < no->id){
        return calcularCustoBusca(no->esq, chave, custoAtual + 1);
    } else{
        return calcularCustoBusca(no->dir, chave, custoAtual + 1);
    }
}

void calcularPorcentagem(No *no, int *numRubros, int *numNegros){
    if (no == NULL) {
        return;
    }

    if (no->cor == 0) { 
        (*numNegros)++;
    } 
    
    else {
        (*numRubros)++;
    }

    calcularPorcentagem(no->esq, numRubros, numNegros);
    calcularPorcentagem(no->dir, numRubros, numNegros);
}


void calcularPorcentagemNos(Raiz *r){
    int numRubros = 0;
    int numNegros = 0;

    calcularPorcentagem(r->raiz, &numRubros, &numNegros);

    int totalNos = numRubros + numNegros;
    double porcentagemRubros = ((double)numRubros / totalNos) * 100;
    double porcentagemNegros = ((double)numNegros / totalNos) * 100;

    printf("Porcentagem de nos rubros: %.2lf%%\n", porcentagemRubros);
    printf("Porcentagem de nos negros: %.2lf%%\n", porcentagemNegros);
}