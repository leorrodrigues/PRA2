#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************/
/* Equipe: Bruna de Oliveira    */
/*         Leonardo Rosa        */
/*         Rafael Rizzatti      */
/***************************************************************************************/

typedef struct{
    char nome_entidade[100];
    int qtd_campos;
    char **campos;
    int *tamanhos;
    char **tipos;
    int tamanho_header;
} tabela;

typedef struct{
    int id;
    int pos;
} heap;

FILE *arqGeral;
FILE *arqIndice;

tabela entidadeGeral;

void createFiles();//FUNCAO QUE CRIA OS ARQUIVOS
void writeEntity(char *nome);//FUNCAO QUE GRAVA ENTIDADES NOS ARQUIVOS
void readFiles();//FUNCAO QUE LE TODO O ARQUIVO E PRINTA NA TELA
void menuCRUD(char *nome); //SUBMENU
void carrega_entidadeGeral(char *nome); //CARREGA A ENTIDADE GERAL DE ACORDO COM O HEADER DO ARQUIVO
tabela findOne(char *nome, int ID);//FUNCAO QUE RETORNA A ENTIDADE CASO ACHE A CHAVE PRIMARIA
tabela *chargeEntity(char *nome);//FUNCAO QUE CARREGA O VETOR DE ENTIDADES
void changeEntity(char *nome); //FUNCAO QUE ALTERA O VALOR DO ID DE UM ELEMENTO
void removeEntity(char *nome);//FUNCAO QUE DELETA UMA ENTIDADE ATRAVES DO ID
void heapsort(heap a[], int n);//ORDENA OS ELEMENTOS ATRAVES DO INDICE
void ordena(char *nome);
void mostraIndex(char *nome);

int main(){
    int opc;
    createFiles();
    while(1){
        puts("\t\tMENU DE ENTIDADES\t\t\n");
        puts("1- Autor");
        puts("2- Leitor");
        puts("3- Livro");
        puts("4- Autor Do Livro");
        puts("0- Sair");
        printf(":>> ");
        scanf("%i",&opc);
        if(opc==1){
            menuCRUD("Autor");
        }
        else if(opc==2){
            menuCRUD("Leitor");
        }
        else if(opc==3){
            menuCRUD("Livro");
        }
        else if(opc==4){
            menuCRUD("AutorDoLivro");
        }
        else if(opc==0)
            break;
        else{
            puts("Opcao Invalida!\n");
        }
    }
    return 0;
}

void menuCRUD(char *nome){
    int opc,i;
    carrega_entidadeGeral(nome);
    puts("\t\tMENU CRUD\t\t\n");
    puts("1- Inserir um novo elemento\n");
    puts("2- Mostrar todos os elementos da entidade\n");
    puts("3- Alterar um elemento\n");
    puts("4- Remover um elemento\n");
    puts("5- Mostrar indices ordenados\n");
    puts("0- Sair");
    printf(":>> ");
    scanf("%i",&opc);
    if(opc==1){
        writeEntity(nome);
    }
    else if(opc==2){
        readFiles(nome);
    }
    else if(opc==3){
        changeEntity(nome);
    }
    else if(opc==4){
        removeEntity(nome);
    }
    else if(opc==5){
        mostraIndex(nome);
    }
    else if(opc==0)
        return;
    else{
        puts("Opcao Invalida!\n");
    }
}

void writeEntity(char *nome){
    char *string,aux[100];
    int numero,i;
    tabela auxTabela;
    string=&aux;
    arqGeral=fopen(nome,"ab");
    for(i=0;i<entidadeGeral.qtd_campos;i++){
        printf("Digite o %s:\n",entidadeGeral.campos[i]);
        if(strcmp(entidadeGeral.tipos[i],"int")==0){
             __fpurge(stdin);
            //fflush(stdin);
            gets(string);
            //fwrite(entidadeGeral.campos[i],sizeof(entidadeGeral.campos[i]),1,arqGeral);
            if(strcmp(entidadeGeral.campos[i],"id")==0){
                fclose(arqGeral);
                auxTabela=findOne(nome,atoi(string));
                arqGeral=fopen(nome,"ab");
                if(auxTabela.qtd_campos!=-1){
                    puts("Entidade com este ID ja existe!\nProcure digitar outro ID\n");
                    return;
                }
            }
            fwrite(string,entidadeGeral.tamanhos[i],1,arqGeral);
            string[0]='\0';
        }
        else if(strcmp(entidadeGeral.tipos[i],"char")==0){
           __fpurge(stdin);
            //fflush(stdin);
            gets(string);
            if(strcmp(entidadeGeral.campos[i],"id")==0){
                auxTabela=findOne(nome,atoi(string));
                if(auxTabela.qtd_campos!=-1){
                    puts("Entidade com este ID ja existe!\nProcure digitar outro ID\n");
                }
                return;
            }
            //fwrite(entidadeGeral.campos[i],sizeof(entidadeGeral.campos[i]),1,arqGeral);
            fwrite(string,entidadeGeral.tamanhos[i],1,arqGeral);
            string[0]='\0';
        }
    }
    fclose(arqGeral);
    ordena(nome);
}

void createFiles(){//Funcao para criar os arquivos casa nao exista, caso exista abre para leitura
    if(!(arqGeral=fopen("Autor","rb"))){
        arqGeral=fopen("Autor","wb");
        fwrite("qnt=107,entidade=[Autor],qnt_campos=[3],campos=[id,nome,sobrenome],tamanho=[6,20,20],tipo=[int,char,char]",sizeof(char),107,arqGeral);
        fclose(arqGeral);
    }
    else
        fclose(arqGeral);
    if(!(arqGeral=fopen("Livro","rb"))){
        arqGeral=fopen("Livro","wb");
        fwrite("qnt=140,entidade=[Livro],qnt_campos=[5],campos=[id,titulo,editora,anoPublicacao,isbn],tamanho=[6,30,30,11,20],tipo=[int,char,char,int,char]",sizeof(char),140,arqGeral);
        fclose(arqGeral);
    }
    else
        fclose(arqGeral);
    if(!(arqGeral=fopen("Leitor","rb"))){
        arqGeral=fopen("Leitor","wb");
        fwrite("qnt=145,entidade=[Leitor],qnt_campos=[6],campos=[id,nome,fone,endereco,cidade,estado],tamanho=[6,30,20,40,40,2],tipo=[int,char,char,int,int,]",sizeof(char),145,arqGeral);
        fclose(arqGeral);
    }
    else
        fclose(arqGeral);
    if(!(arqGeral=fopen("AutorDoLivro","rb"))){
        arqGeral=fopen("AutorDoLivro","wb");
        fwrite("qnt=116,entidade=[AutorDoLivro],qnt_campos=[3],campos=[autorId,livroId,sequence],tamanho=[4,6,2],tipo=[int,int,int]",sizeof(char),116,arqGeral);
        fclose(arqGeral);
    }
    else
        fclose(arqGeral);
}

void readFiles(char *nome){
    char string[1000]="\0";
    int tam_arq=0,i,j,numero;
    arqGeral=fopen(nome,"rb");
    fseek(arqGeral,0,SEEK_END);
    tam_arq=ftell(arqGeral);
    rewind(arqGeral);
    tam_arq-=entidadeGeral.tamanho_header;
    fseek(arqGeral,entidadeGeral.tamanho_header,SEEK_CUR);
    if(tam_arq==0){
        puts("Arquivo Nao contem Entidades!");
    }
    else{
        while(tam_arq!=0){
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                if(strcmp(entidadeGeral.tipos[i],"int")==0){
                    fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                    printf("\t%s:\t%i",entidadeGeral.campos[i],atoi(string));
                }
                else if(strcmp(entidadeGeral.tipos[i],"char")==0){
                    fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                    printf("\t%s:\t%s",entidadeGeral.campos[i],string);
                }
                tam_arq-=entidadeGeral.tamanhos[i];
            }
            puts("");
        }
    }
    fclose(arqGeral);
}

tabela findOne(char *nome,int ID){
    tabela aux;
    char string[1000]="\0";
    int tam_arq=0,i,j,numero;
    aux.campos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
    aux.tipos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
    aux.tamanhos=(int *)malloc(sizeof(int)*entidadeGeral.qtd_campos);
    for(i=0;i<entidadeGeral.qtd_campos;i++){
        aux.campos[i]=(char *)malloc(sizeof(char)*100);
        aux.tipos[i]=(char *)malloc(sizeof(char)*100);
    }
    arqGeral=fopen(nome,"rb");
    fseek(arqGeral,0,SEEK_END);
    tam_arq=ftell(arqGeral);
    rewind(arqGeral);
    tam_arq-=entidadeGeral.tamanho_header;
    fseek(arqGeral,entidadeGeral.tamanho_header,SEEK_CUR);
    if(tam_arq!=0){
        while(tam_arq!=0){
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                if(strcmp(entidadeGeral.tipos[i],"int")==0){
                    fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                    strcpy(aux.campos[i],string);
                    strcpy(aux.tipos[i],entidadeGeral.tipos[i]);
                    aux.tamanhos[i]=entidadeGeral.tamanhos[i];
                    //printf("\t%s:\t%i",entidadeGeral.campos[i],atoi(string));
                }
                else if(strcmp(entidadeGeral.tipos[i],"char")==0){
                    fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                    strcpy(aux.tipos[i],entidadeGeral.tipos[i]);
                    strcpy(aux.campos[i],string);
                    aux.tamanhos[i]=entidadeGeral.tamanhos[i];
                    //printf("\t%s:\t%s",entidadeGeral.campos[i],string);
                }
                tam_arq-=entidadeGeral.tamanhos[i];
            }
            //puts("");
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                //printf("AAA: %i %s %i\n",atoi(aux.campos[i]),entidadeGeral.campos[i],ID);
                if(atoi(aux.campos[i])==ID && strcmp("id",entidadeGeral.campos[i])==0){
                    aux.qtd_campos=entidadeGeral.qtd_campos;
                    strcpy(aux.nome_entidade,entidadeGeral.nome_entidade);
                    aux.tamanho_header=entidadeGeral.tamanho_header;
                    fclose(arqGeral);
                    return aux;
                }
            }
        }
    }
    //printf("Entidade Nao Encontrada!\n");
    fclose(arqGeral);
    aux.qtd_campos=-1;
    return aux;
}

tabela *chargeEntity(char *nome){
    tabela *vet;
    char string[1000]="\0";
    int tam_arq=0,i,j,numero,cont=0;
    vet=(tabela *)malloc(sizeof(tabela));
    arqGeral=fopen(nome,"rb");
    fseek(arqGeral,0,SEEK_END);
    tam_arq=ftell(arqGeral);
    rewind(arqGeral);
    tam_arq-=entidadeGeral.tamanho_header;
    fseek(arqGeral,entidadeGeral.tamanho_header,SEEK_CUR);
    if(tam_arq==0){
        puts("Arquivo Nao contem Entidades!");
    }
    else{
        while(tam_arq!=0){
            cont++;
            vet=(tabela *)realloc(vet,sizeof(tabela)*cont);
            vet[cont-1].campos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
            vet[cont-1].tipos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
            vet[cont-1].tamanhos=(int *)malloc(sizeof(int)*entidadeGeral.qtd_campos);
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                vet[cont-1].campos[i]=(char *)malloc(sizeof(char)*100);
                vet[cont-1].tipos[i]=(char *)malloc(sizeof(char)*100);
            }
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                strcpy(vet[cont-1].tipos[i],entidadeGeral.tipos[i]);
                strcpy(vet[cont-1].campos[i],string);
                vet[cont-1].tamanhos[i]=entidadeGeral.tamanhos[i];
                tam_arq-=entidadeGeral.tamanhos[i];
            }
            vet[cont-1].qtd_campos=entidadeGeral.qtd_campos;
        }
    }
    fclose(arqGeral);
    cont++;
    vet=(tabela *)realloc(vet,sizeof(tabela)*cont);
    vet[cont-1].qtd_campos=-1;
    return vet;
}

void changeEntity(char *nome){
    int ID,qnt_elementos=0,i,j,flag=0;
    tabela *vet;
    char novo[100];
    puts("Digite o ID do elemento que deseja alterar");
    scanf("%i",&ID);
    vet=chargeEntity(nome);
    i=0;
    while(vet[i].qtd_campos!=-1){
        qnt_elementos++;
        i++;
    }
    for(i=0;i<qnt_elementos;i++){
        if(ID==atoi(vet[i].campos[0])){
            for(j=1;j<entidadeGeral.qtd_campos;j++){
                 __fpurge(stdin);
            	//fflush(stdin);
                printf("Digite o novo %s\n",entidadeGeral.campos[j]);
                scanf("%s",novo);
                strcpy(vet[i].campos[j],novo);
                novo[0]='\0';
            }
            flag=1;
            break;
        }
    }
    if(flag==0){
        puts("ID da entidade nao encontrado!");
        return;
    }
    else{
        arqGeral=fopen(nome,"wb");
        if(strcmp(nome,"Autor")==0){
            fwrite("qnt=107,entidade=[Autor],qnt_campos=[3],campos=[id,nome,sobrenome],tamanho=[6,20,20],tipo=[int,char,char]",sizeof(char),107,arqGeral);
        }
        else if(strcmp(nome,"Leitor")==0){
            fwrite("qnt=145,entidade=[Leitor],qnt_campos=[6],campos=[id,nome,fone,endereco,cidade,estado],tamanho=[6,30,20,40,40,2],tipo=[int,char,char,int,int,int]",sizeof(char),145,arqGeral);
        }
        else if(strcmp(nome,"Livro")==0){
            fwrite("qnt=140,entidade=[Livro],qnt_campos=[5],campos=[id,titulo,editora,anoPublicacao,isbn],tamanho=[6,30,30,11,20],tipo=[int,char,char,int,char]",sizeof(char),140,arqGeral);
        }
        else if(strcmp(nome,"AutorDoLivro")==0){
            fwrite("qnt=116,entidade=[AutorDoLivro],qnt_campos=[3],campos=[autorId,livroId,sequence],tamanho=[4,6,2],tipo=[int,int,int]",sizeof(char),116,arqGeral);
        }
        for(i=0;i<qnt_elementos;i++){
            for(j=0;j<entidadeGeral.qtd_campos;j++){
                fwrite(vet[i].campos[j],entidadeGeral.tamanhos[j],1,arqGeral);
            }
        }
    }
    fclose(arqGeral);
}

void removeEntity(char *nome){
    int ID,qnt_elementos=0,i,j,flag=0;
    tabela *vet;
    char novo[100];
    puts("Digite o ID do elemento que deseja remover");
    scanf("%i",&ID);
    vet=chargeEntity(nome);
    i=0;
    while(vet[i].qtd_campos!=-1){
        qnt_elementos++;
        i++;
    }
    for(i=0;i<qnt_elementos;i++){
        if(ID==atoi(vet[i].campos[0])){
            flag=1;
            break;
        }
    }
    if(flag==0){
        puts("ID da entidade nao encontrado!");
        return;
    }
    else{
        arqGeral=fopen(nome,"wb");
        if(strcmp(nome,"Autor")==0){
            fwrite("qnt=107,entidade=[Autor],qnt_campos=[3],campos=[id,nome,sobrenome],tamanho=[6,20,20],tipo=[int,char,char]",sizeof(char),107,arqGeral);
        }
        else if(strcmp(nome,"Leitor")==0){
            fwrite("qnt=145,entidade=[Leitor],qnt_campos=[6],campos=[id,nome,fone,endereco,cidade,estado],tamanho=[6,30,20,40,40,2],tipo=[int,char,char,int,int,int]",sizeof(char),145,arqGeral);
        }
        else if(strcmp(nome,"Livro")==0){
            fwrite("qnt=140,entidade=[Livro],qnt_campos=[5],campos=[id,titulo,editora,anoPublicacao,isbn],tamanho=[6,30,30,11,20],tipo=[int,char,char,int,char]",sizeof(char),140,arqGeral);
        }
        else if(strcmp(nome,"AutorDoLivro")==0){
            fwrite("qnt=116,entidade=[AutorDoLivro],qnt_campos=[3],campos=[autorId,livroId,sequence],tamanho=[4,6,2],tipo=[int,int,int]",sizeof(char),116,arqGeral);
        }
        for(i=0;i<qnt_elementos;i++){
            for(j=0;j<entidadeGeral.qtd_campos;j++){
                if(ID!=atoi(vet[i].campos[0]))
                    fwrite(vet[i].campos[j],entidadeGeral.tamanhos[j],1,arqGeral);
            }
        }
    }
    puts("Entidade Removida com Sucesso!");
    fclose(arqGeral);
}


void carrega_entidadeGeral(char *nome){
    char buffer[200],auxiliar[50];
    int tamanho,i,j,k,cont=0,cont_campos,cont_tamanhos,cont_tipos;
    if(arqGeral=fopen(nome,"rb")){
        fseek(arqGeral,4,SEEK_SET);//andamos para pegar os valores do total do header
        fread(&buffer,sizeof(char),3,arqGeral);//lemos o valor
        buffer[3]='\0';//fechamos a string
        tamanho=atoi(buffer);
        entidadeGeral.tamanho_header=tamanho;
        tamanho-=8;
        fseek(arqGeral,8,SEEK_SET);
        fread(&buffer,sizeof(char),tamanho,arqGeral);
        fclose(arqGeral);//como ja temos o header salvo no buffer, fechamos o arquivo
        for(i=0;i<tamanho;i++){
            j=0;
            cont_campos=0;
            cont_tipos=0;
            cont_tamanhos=0;
            if(buffer[i]=='['){
                auxiliar[0]='\0';
                i++;
                cont++;
                while(buffer[i]!=']'){
                    if(buffer[i]==','){
                        if(cont==3){
                            entidadeGeral.campos[cont_campos]=(char *)malloc(sizeof(char)*100);
                            for(k=0;auxiliar[k]!='\0';k++){
                                entidadeGeral.campos[cont_campos][k]=auxiliar[k];
                                entidadeGeral.campos[cont_campos][k+1]='\0';
                            }
                            cont_campos++;
                            j=0;
                            auxiliar[0]='\0';
                        }
                        else if(cont==4){
                            entidadeGeral.tamanhos[cont_tamanhos]=atoi(auxiliar);
                            cont_tamanhos++;
                            j=0;
                            auxiliar[0]='\0';
                        }
                        else if(cont==5){
                            entidadeGeral.tipos[cont_tipos]=(char *)malloc(sizeof(char)*100);
                            for(k=0;auxiliar[k]!='\0';k++){
                                entidadeGeral.tipos[cont_tipos][k]=auxiliar[k];
                                entidadeGeral.tipos[cont_tipos][k+1]='\0';
                            }
                            cont_tipos++;
                            j=0;
                            auxiliar[0]='\0';
                        }
                    }
                    else{
                        auxiliar[j]=buffer[i];
                        j++;
                        auxiliar[j]='\0';
                    }
                    i++;
                }
                if(cont==1){
                    strcpy(entidadeGeral.nome_entidade,auxiliar);
                    auxiliar[0]='\0';
                }
                else if(cont==2){
                    entidadeGeral.qtd_campos=atoi(auxiliar);
                    entidadeGeral.campos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
                    entidadeGeral.tipos=(char **)malloc(sizeof(char *)*entidadeGeral.qtd_campos);
                    entidadeGeral.tamanhos=(int *)malloc(sizeof(int)*entidadeGeral.qtd_campos);
                    auxiliar[0]='\0';
                }
                else if(cont==3){
                    entidadeGeral.campos[cont_campos]=(char *)malloc(sizeof(char)*100);
                    for(k=0;auxiliar[k]!='\0';k++){
                        entidadeGeral.campos[cont_campos][k]=auxiliar[k];
                        entidadeGeral.campos[cont_campos][k+1]='\0';
                    }
                    auxiliar[0]='\0';
                }
                else if(cont==4){
                    entidadeGeral.tamanhos[cont_tamanhos]=atoi(auxiliar);
                    auxiliar[0]='\0';
                }
                else if(cont==5){
                    entidadeGeral.tipos[cont_tipos]=(char *)malloc(sizeof(char)*100);
                    for(k=0;auxiliar[k]!='\0';k++){
                        entidadeGeral.tipos[cont_tipos][k]=auxiliar[k];
                        entidadeGeral.tipos[cont_tipos][k+1]='\0';
                    }
                    auxiliar[0]='\0';
                }
            }
        }
    }
}

void ordena(char *nome){
    char string[1000]="\0", index[100]="\0";
    int tam_arq=0, tam_arq_aux=0,i,j,numero, count=0, ii=0, pos, v;
    arqGeral=fopen(nome,"rb");
    fseek(arqGeral,0,SEEK_END);
    tam_arq=ftell(arqGeral);
    rewind(arqGeral);
    tam_arq-=entidadeGeral.tamanho_header;
    tam_arq_aux = tam_arq;
    fseek(arqGeral,entidadeGeral.tamanho_header,SEEK_CUR);
    if(tam_arq==0){
        puts("Arquivo Nao contem Entidades!");
    }
    else{
        while(tam_arq!=0){
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                tam_arq-=entidadeGeral.tamanhos[i];
            }
	        count++;
        }
        heap vet[count];

        fseek(arqGeral,entidadeGeral.tamanho_header,SEEK_SET);

        pos = entidadeGeral.tamanho_header;

        while(tam_arq_aux!=0){
            for(i=0;i<entidadeGeral.qtd_campos;i++){
                fread(string,sizeof(char),entidadeGeral.tamanhos[i],arqGeral);
                if(strcmp(entidadeGeral.tipos[i],"int")==0){
                        if((strcmp(entidadeGeral.campos[i],"id")==0)){
                         vet[ii].id = atoi(string);
                    }
                }
                tam_arq_aux-=entidadeGeral.tamanhos[i];
            }
            vet[ii].pos = pos;
            pos = ftell(arqGeral);
            ii++;
        }
        fclose(arqGeral);

        for(i=0;i<count;i++){
            printf("\nid - %d, pos - %d", vet[i].id, vet[i].pos);
        }

        heapsort(vet, count);


        strcpy(index, nome);
        strcat(index, "Index");
        arqIndice = fopen(index,"wb");

        for(i=0;i<count;i++){
            fwrite(&vet[i],sizeof(heap),1,arqIndice);
        }
        fclose(arqIndice);
    }
}

void heapsort(heap a[], int n){

   int i = n / 2, pai, filho, t, k;


   for (;;) {
      if (i > 0) {
          i--;
          t = a[i].id;
          k = a[i].pos;
      } else {
          n--;
          if (n == 0)
            return;
          t = a[n].id;
          k = a[n].pos;
          a[n].id = a[0].id;
          a[n].pos = a[0].pos;
      }

      pai = i;

      //Primeiro será feita a comparação com o filho da esquerda.
      filho = i * 2 + 1;

      while (filho < n) {

         //Se o filho da esquerda for menor do que o filho da direita,então será feita a troca do filho que será comparado.
          if ((filho + 1 < n)  &&  (a[filho + 1].id > a[filho].id))
              filho++;
          if (a[filho].id > t){
             a[pai].id = a[filho].id;
             a[pai].pos = a[filho].pos;
             pai = filho;
             filho = pai * 2 + 1;
          }else
             break;
          }
            a[pai].id = t;
            a[pai].pos = k;
        }
   }

void mostraIndex(char *nome){
    char nomea[100];
    heap aux;
    int tam = 0;
    strcpy(nomea,nome);
    strcat(nomea,"Index");
    arqIndice = fopen(nomea, "rb");
    while(!feof(arqIndice)){
        fread(&aux,sizeof(heap),1,arqIndice);
        printf("\nid: %d, pos: %d", aux.id, aux.pos);
    }
    __fpurge(stdout);
    //fflush(stdout);  //pra windows

    fclose(arqIndice);
}
