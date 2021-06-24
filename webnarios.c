#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>

#pragma pack(1)
/*******************************************************PROFESSORES*********************************************************************/

                                    ///////STRUCTS PROFESSORES//////////
//Estrutura de informações referentes aos professores
struct professor{
int matricula; //identificador do professor
char nome[40],departamento[20];
};

//Elementos da lista_prof dos professores
struct prof_elemento{
    struct professor prof;//Cada elemento da lista_prof tem um conjunto de dados de um professor,esse conjunto está na struct professor
    struct prof_elemento *prox,*ant;
};

                                    ///////FUNÇÕES PROFESSORES//////////

                    ///////OPERAÇÕES na lista_prof

struct prof_elemento *CriarLista_prof() {
	struct prof_elemento *sentinela= (struct prof_elemento *) malloc(sizeof(struct prof_elemento));
	sentinela->prox= sentinela;
	sentinela->ant= sentinela;
	return sentinela;
}

//Cria elemento e já colocar o struct professor
struct prof_elemento *CriarElemento_prof(struct professor prof) {
	struct prof_elemento *elem= (struct prof_elemento *) malloc(sizeof(struct prof_elemento));
	elem->prof = prof;
	elem->ant= elem;
	elem->prox = elem;
	return elem;
}

//Inserir novo elemento e já colocar o struct professor
void InsereFim_prof(struct prof_elemento *lista_prof, struct professor prof){
    struct prof_elemento *novo = CriarElemento_prof(prof);
	novo->prox = lista_prof;
	novo->ant = lista_prof->ant;
	lista_prof->ant->prox = novo;
	lista_prof->ant = novo;
}

//Verifica se a matricula de professor já existe
//se já existe retorna 0, se não retorna 1
int VerificaMatricula(int matricula,struct prof_elemento *gancho){
	struct prof_elemento *aux = gancho;
	do{
		if (matricula == aux->prof.matricula){
			return 0;
		}else{
			aux = aux->prox;
		}
	}while (aux != gancho);
	return 1;
}

//Cadastra um professor
void CADASTRAR(struct prof_elemento *gancho){
	struct professor prof;
	int verif = 0;
	printf("\n\n>Matricula: ");
	scanf("%d",&prof.matricula);
	verif = VerificaMatricula(prof.matricula,gancho);
	if (verif == 1){
		printf("\n>Nome : ");
        fflush(stdin); 
        gets(prof.nome); 
		printf(">Departamento: ");
        fflush(stdin); 
        gets(prof.departamento); 
		InsereFim_prof(gancho, prof);
	}else{
		printf("\n\nMatricula ja' cadastrada.");
	}
}

void DestruirLista_prof(struct prof_elemento *gancho) {
	struct prof_elemento *aux= gancho;

	gancho->ant->prox= NULL;

	while (aux != NULL) {
		gancho= aux->prox;
		free(aux);
		aux= gancho;
	}
}

                    ///////OPERAÇÕES ARQUIVO


struct prof_elemento *LeBin_prof(void){
	struct prof_elemento *gancho = CriarLista_prof();
	struct professor prof;
	FILE *arq;
	arq = fopen ("professores.bin","rb");
	while(fread(&prof, sizeof(struct professor), 1, arq) != 0){
		InsereFim_prof(gancho,prof);
	}
	fclose(arq);
	return gancho;
}

void EscreveArq_prof(struct prof_elemento *gancho){
	FILE *arq;
	struct prof_elemento *aux = gancho->prox;
	arq = fopen ("professores.bin","wb");
	do{
		fwrite(aux, sizeof(struct professor),  1, arq);
		aux = aux->prox;
	}while (aux != gancho);
	fclose(arq);
}

                    ///////OPERAÇÕES 'USUÁRIO'

//Menu para operações referentes a professores professores
void MENU_prof(struct prof_elemento *lista){
    int resp=0;

    printf("\n\nOpcoes\n1 - Cadastrar novos professores\n2 - Sair\n> ");
    scanf("%d",&resp);
    system("cls");

    while(resp!=2){
        switch (resp)
        {
            case 1:
                CADASTRAR(lista);
                break;

            default:
                printf("\n\nOpcao inva'lida!");
                break;
        }
        system("PAUSE");
        system("cls");
        printf("\n\nOpcoes\n1 - Cadastrar novos professores\n2 - Sair\n> ");
        scanf("%d",&resp);
        system("cls");

    }
    printf("\n\nEncerrando cadastro de novos professores...");
    EscreveArq_prof(lista);
}


/************************************************************WEBNÁRIOS********************************************************************/

                                    ///////STRUCTS WEBNÁRIOS//////////

//Estrutura de informações referentes aos webnários
struct webnario{
int id; //identificador do webnário
char titulo[40],url[20];
int dia,mes,ano,hora,minuto;
int qtdProfs; //quantidade de professores que vão participar do webnário
int matriculaProf[3]; //matriculas dos professores que vão participar do webnário
};

//Elementos da lista_web dos webnários
//Cada elemento da lista_web é um conjunto
// de dados de um webnario,esse conjunto está na struct webnario
struct web_elemento{
    struct webnario web;
    struct web_elemento *prox,*ant;
};

                                    ///////FUNÇÕESS WEBNÁRIOS//////////

                     ///////OPERAÇÕES LISTA_web

void DestruirLista_web(struct web_elemento *lista) {
	struct web_elemento *aux= lista;
	lista->ant->prox= NULL;
	while (aux != NULL) {
		lista= aux->prox;
		free(aux);
		aux= lista;
	}
}

struct web_elemento *CriarLista_web() {
	struct web_elemento *sentinela= (struct web_elemento *) malloc(sizeof(struct web_elemento));
	sentinela->prox= sentinela;
	sentinela->ant= sentinela;
	return sentinela;
}

struct web_elemento *CriarElemento_web() {
	struct web_elemento *elem= (struct web_elemento *) malloc(sizeof(struct web_elemento));

	elem->ant= elem;
	elem->prox= elem;
	elem->web.qtdProfs= 0;

	return elem;
}


void InsereInicio_web(struct web_elemento *lista_web){
    struct web_elemento *novo = CriarElemento_web();
          novo->prox = lista_web->prox;
          novo->ant = lista_web;
          lista_web->prox->ant = novo;
          lista_web->prox = novo;

}

// Percorre a lista e retorna o elemento com o id informado
//se estiver na lista, ou NULL, se não estiver
struct web_elemento *Buscar_web(struct web_elemento *lista_web, int id) {
	struct web_elemento *aux= lista_web->prox;

	while (aux != lista_web && aux->web.id != id)
		aux= aux->prox;

	if (aux != lista_web) return aux;
	else return NULL;
}

                ///////OPERAÇÕES ARQUIVO

//Função para ler arquivo webnarios.bin e armazenar os dados em uma lista_web
struct web_elemento* Ler_web(char *nome,struct web_elemento *lista_web){
    FILE *arq=fopen(nome,"rb");
    int quant,verifica;

    fseek(arq,0,SEEK_END);
    quant= ftell(arq)/sizeof(struct webnario);
    fseek(arq,0,SEEK_SET);

    for(int i = 0; i < quant; i++){
        InsereInicio_web(lista_web);
        verifica = fread(&(lista_web->prox->web),sizeof(struct webnario),1,arq);
        if(verifica!=1){
            printf("\n\nErro na busca de webnarios ja' inscritos.");
            return NULL;
        }
    }
    fclose(arq);
    return lista_web;
}

//Função para escrever lista_web webnarios de volta para o arquivo webnarios.bin
void Escrever_web(char *nome, struct web_elemento *lista_web){
    FILE *arq=fopen(nome,"wb");
    struct web_elemento *aux = lista_web->prox;
    do{
		fwrite(aux, sizeof(struct webnario),  1, arq);
		aux = aux->prox;
	}while (aux != lista_web);
	fclose(arq);
}

            ///////OPERAÇÕES 'USUÁRIOS'

// Retorna 0 e exclui o webnário com o id informado, se ele estiver na lista. 
//Ou retorna 1 e não realiza nada, se não estiver.
int ExcluirWebnario(struct web_elemento *lista_web, int id) {
	struct web_elemento *aux= Buscar_web(lista_web, id);

	if (aux) {
		aux->ant->prox= aux->prox;
		aux->prox->ant= aux->ant;
		free(aux);
		return 0;
	}
	return 1;
}

//Função para remover professor de webnário
// Se webnário informado não estiver na lista ou a matrícula não estiver no webnário, não realiza ação nenhuma e retorna 1.
// Caso não ocorra esses problemas, remove o professor e retorna 0.
int RemoverProfessor(struct web_elemento *lista_web, int id, int matricula) {
	int i;
	struct web_elemento *aux= Buscar_web(lista_web, id);
    
	if (aux) {
		for (i= 0; i < aux->web.qtdProfs; i++) {
			if (aux->web.matriculaProf[i] == matricula) {
				aux->web.matriculaProf[i]= aux->web.matriculaProf[aux->web.qtdProfs-1];
				aux->web.qtdProfs--;
				if (aux && !aux->web.qtdProfs){
                    ExcluirWebnario(lista_web, id);
                    printf("\n\nWebnario cancelado por falta de professor.");
                } 
				return 0;
			}
		}
	}
	return 1;
}

//Função para adicionar professor a webnário
// Caso não exista um webnánrio com o id informado na lista ou a quantidade de professores na lista seja a máxima permitida,
// não realiza nenhuma ação e retorna 1. Se não ocorrer nenhum desses problemas, adiciona o professor e retorna 0.
int AdicionarProfessor_web(struct web_elemento *lista_web, int id, int matricula) {
	struct web_elemento *aux= Buscar_web(lista_web, id);

	if(aux && aux->web.qtdProfs < 3) {
		aux->web.matriculaProf[aux->web.qtdProfs]= matricula;
		aux->web.qtdProfs++;
		return 0;
	}
    if(aux->web.qtdProfs==3){
        printf("\n\nEsse webnario ja' atingiu o maximo de incricoes possiveis.");
    }
	return 1;
}


// Preenche id, matrículas e qtd de professores
// Retorna o elemento inserido no caso de sucesso. Se usuário inserir um id que já existe na lista de webnários,
// não executa ação alguma e retorna NULL. Este seria o caso de falha.
struct web_elemento *InserirWebnario(struct web_elemento *lista_web, int id, int matricula) {
	struct web_elemento *elem= CriarElemento_web();

	if (Buscar_web(lista_web, id)){
        printf("\n\nO id informado ja' existe.");
        return NULL;
        }

	elem->web.id= id;

	elem->ant= lista_web->ant;
	elem->prox= lista_web;
	elem->ant->prox= elem;
	lista_web->ant= elem;

	AdicionarProfessor_web(lista_web, id, matricula);

	return elem;
}

//Função para preencher o novo webário já criado
void PreencherNovoWeb(struct web_elemento *lista_web){
    printf("\n> Titulo: ");
    fflush(stdin);
    gets(lista_web->ant->web.titulo);
    printf("\n> URL: ");
    fflush(stdin);
    gets(lista_web->ant->web.url);
    printf("\n> Horario:\n- Dia: ");
    scanf("%d",&lista_web->ant->web.dia);
    printf("\n- Mes: ");
    scanf("%d",&lista_web->ant->web.mes);
    printf("\n- Ano: ");
    scanf("%d",&lista_web->ant->web.ano);
    printf("\n- Horas: ");
    scanf("%d",&lista_web->ant->web.hora);
    printf("\n- Minutos: ");
    scanf("%d",&lista_web->ant->web.minuto);
    printf("\n\nInclusao de webnario concluida.");
}

//Função para verificar se professor já está no webnário para não adicionar novamente
//Se estiver ,retorna 0. Se não estiver retorna 1
int VerificarMatricula_web(int matricula,int id,struct web_elemento *lista_web){
    struct web_elemento *l_web = Buscar_web(lista_web,id);
    if(l_web!=NULL){
        for(int i=0;i<3;i++){
            if(l_web->web.matriculaProf[i] ==matricula)
                return 0;
        }
        return 1;
    }
    return 0;
}

void Listar_web(struct webnario *vet,int i,int j){//02/12
    printf("\n\n\t\t\tWEBNARIOS");
    printf("\n________________________________________________________________\n");
    for (j= 0; j < i; j++) {
        printf("\nId: %d\n",vet[j].id);
        printf("Titulo: %s\nURL: %s\n",vet[j].titulo,vet[j].url);
        printf("Data: %d/%d/%d\tHorario: %d hs %d min\n",vet[j].dia,vet[j].mes,vet[j].ano,vet[j].hora,vet[j].minuto);
        printf("Professores: ");
        for (int k = 0; k < vet[j].qtdProfs; k++){
            if(k+1==vet[j].qtdProfs)
                printf("%d",vet[j].matriculaProf[k]);
            else    
                printf("%d, ",vet[j].matriculaProf[k]);
        }
        printf("\n________________________________________________________________\n");
    }
}

// Função para a ordenação de um vetor de webnarios
void MergeSort(struct webnario vet[],int prim,int ult){
    if (prim==ult) return;

    int meio = (prim+ult+1)/2;
    MergeSort(vet,prim,meio-1);
    MergeSort(vet,meio,ult);

    struct webnario *aux = (struct webnario*) malloc(_msize(vet));
    for (int i=prim;i<=ult;i++)
        aux[i] = vet[i];

    int i=prim;
    int j=meio;
    int k=prim;

    while (i<meio&&j<=ult){
        if (aux[i].ano == aux[j].ano) {
            if (aux[i].mes == aux[j].mes) {
                if (aux[i].dia == aux[j].dia) {
                    if (aux[i].hora == aux[j].hora) {
                        if (aux[i].minuto < aux[j].minuto) {
                            vet[k] = aux[i];
                            i++;
                        } else {
                            vet[k] = aux[j];
                            j++;
                        }

                    } else if (aux[i].hora < aux[j].hora) {
                        vet[k] = aux[i];
                        i++;
                    } else {
                        vet[k] = aux[j];
                        j++;
                    }

                } else if (aux[i].dia < aux[j].dia) {
                    vet[k] = aux[i];
                    i++;
                } else {
                    vet[k] = aux[j];
                    j++;
                }
               
            } else if (aux[i].mes < aux[j].mes) {
                vet[k] = aux[i];
                i++;
            } else {
                vet[k] = aux[j];
                j++;
            }

        } else if (aux[i].ano < aux[j].ano){
            vet[k] = aux[i];
            i++;
        }else{
            vet[k] = aux[j];
            j++;
        }
        k++;
    }

    if (i==meio){//parte da esquerda acabou
        for (;j<=ult;j++){
            vet[k] = aux[j];
            k++;
        }
    }else{//parte da direita acabou
        for (;i<meio;i++){
            vet[k] = aux[i];
            k++;
        }
    }
    free(aux);
}

// Função para mostrar webnários em ordem cronológica //02/12
void MostraWebnarios(struct web_elemento *lista_web) {
            struct web_elemento *aux= NULL;
            aux = lista_web->prox;
            struct webnario *vet= NULL;
            int i= 0, j;

            while (aux != lista_web) {
                vet= (struct webnario *) realloc(vet, (i+1)*sizeof(struct webnario));
                vet[i]= aux->web;
                i++;
                aux= aux->prox;
            }

            MergeSort(vet, 0, i-1);

            Listar_web(vet,i,j);

            free(vet);
}

//Menu para operaçõese com webnarios
void MENU_web(struct web_elemento *lista_web,struct prof_elemento *lista_prof){
    int resp=0,ret,id,matricula;

    printf("\n\n1-Incluir novo webnario\n2-Acrescentar professor\n3-Remover professor de webnario\n4-Excluir webnario\n5-Mostra webnarios\n6-Sair\n> ");
    scanf("%d",&resp);

    while(resp!=6){
        switch (resp)
        {
        case 1:    //Incluir novo webnario
            printf("\n\n> Id: ");
            scanf("%d",&id);
           if(!Buscar_web(lista_web,id)){
               printf("\n> Matricula: ");
               scanf("%d",&matricula);

              if(!VerificaMatricula(matricula,lista_prof->prox)){
                    if(InserirWebnario(lista_web,id,matricula)) PreencherNovoWeb(lista_web);

                    else printf("\n\nNao foi possivel inserir novo webnario.");
                }else 
                    printf("\n\nMatricula de professor e' invalida.");

            }else
                printf("\n\nO Id informado  ja' esta' sendo utilizado.");
            
            break;  
        case 2:    //Acrescentar professor a webnario
            printf("\nId: ");
            scanf("%d",&id);
            if(Buscar_web(lista_web,id)){
                printf("\nMatricula: ");
                scanf("%d",&matricula);
                if(!VerificaMatricula(matricula,lista_prof->prox)){
                     if(VerificarMatricula_web(matricula,id,lista_web)){

                            ret = AdicionarProfessor_web(lista_web,id,matricula);
                            if(!ret) printf("\n\nProfessor adicionado com sucesso.");

                            else printf("\n\nNao foi possivel adicionar esse professor.");

                    }else printf("\n\nEsse professor ja' esta' incluso no webnario.");

                }else  printf("\n\nMatricula de professor e' invalida.");

            }else printf("\n\nIdentificador de webnario invalido.");
            break;

        case 3:    //Retirada de professor de webnario
            printf("\nId: ");
            scanf("%d",&id);
            if(Buscar_web(lista_web,id)){
                printf("\nMatricula: ");
                scanf("%d",&matricula);
                if(!VerificaMatricula(matricula,lista_prof->prox)){
                    if(!VerificarMatricula_web(matricula,id,lista_web)){

                        ret = RemoverProfessor(lista_web,id,matricula);
                        if(!ret)
                            printf("\n\nProfessor removido com sucesso.");
                        else
                            printf("\n\nNao foi possivel remover esse professor.");
                    }else
                        printf("\n\nEsse professor nao esta incluido nesse webnario")   ; 
                }
                else
                    printf("\n\nMatricula de professor e' invalida.");
            }
            else
                printf("\nIdentificador de webnario invalido.\n");
            break;

        case 4:    //Exclusão de webnario

            printf("\n\nId: ");
            scanf("%d",&id);
            if(ExcluirWebnario(lista_web,id))
                printf("\n\nIdentificador de webnario invalido.");
            else
                printf("\n\nWebnaro removido com sucesso.");
            break;
            
        case 5:    //Mostrar webnários em ordem cronológica 
                MostraWebnarios(lista_web);//02/12
            break;

        default:
            printf("\n\nOpcao inva'lida!");
            break;
        }

        system("PAUSE");
        system("cls");
        printf("\n\n1-Incluir novo webnario\n2-Acrescentar professor\n3-Remover professor de webnario\n4-Excluir webnario\n5-Mostra webnarios\n6-Sair\n> ");
        scanf("%d",&resp);
    }
    printf("\n\nEncerrando programa...\n");
    Escrever_web("webnarios.bin",lista_web);
}


/**********************************MAIN()**********************************/
int main(){
    struct prof_elemento *professores = NULL;
    struct web_elemento *webnarios = NULL;


    professores = LeBin_prof();
    MENU_prof(professores);
   
    webnarios = CriarLista_web();
    webnarios = Ler_web("webnarios.bin",webnarios);
    MENU_web(webnarios,professores);

    DestruirLista_web(webnarios);
	DestruirLista_prof(professores);

    return 0;
}
