#include "GOOGNER.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <conio.h>

void hash_init(No *tabela[N]){
   for(int i = 0; i < N; i++){
      tabela[i] = NULL;
   }
}

int hash(char *str){
   int id_hash = 0;
   for(int i = 0; str[i] != '\0'; i++){
      id_hash = (id_hash * 31 + str[i]) % N;
   }
   return id_hash;
}

void hash_insere(No *tabela[], char *termo, char *nome_arquivo, long offset){
   if(strlen(termo) < 5)
      return;

   int pos = hash(termo);
   No *atual = tabela[pos];

   // procura se termo já existe
   while(atual != NULL){
      if(strcmp(atual->reg.termo, termo) == 0){
         Ocorrencia *nova = (Ocorrencia*)malloc(sizeof(Ocorrencia));
         strcpy(nova->nome_arquivo, nome_arquivo);
         nova->offset = offset;
         nova->prox = NULL;

         // lista vazia
         if(atual->reg.lista == NULL){
            atual->reg.lista = nova;
         }
         else{
            Ocorrencia* aux =
               atual->reg.lista;

            while(aux->prox != NULL){
               aux = aux->prox;
            }
            aux->prox = nova;
         }
         return;
      }
      atual = atual->prox;
   }


   // termo não existe → cria novo nó
   No *novo = (No*)malloc(sizeof(No));
   strcpy(novo->reg.termo, termo);
   novo->reg.lista = NULL;

   // primeira ocorrência
   Ocorrencia *oc = (Ocorrencia*)malloc(sizeof(Ocorrencia));
   strcpy(oc->nome_arquivo, nome_arquivo);
   oc->offset = offset;
   oc->prox = NULL;
   novo->reg.lista = oc;

   // inserção na tabela hash
   novo->prox = tabela[pos];
   tabela[pos] = novo;
}

void processar_arquivo(No* tabela[], char* nome_arquivo){
   FILE* fp = fopen(nome_arquivo, "r");
   if(!fp) return;

   char palavra[51];
   while(1){
      long pos = ftell(fp); // posição antes de ler

      if(fscanf(fp, "%50s", palavra) != 1)
         break;

      limpar_palavra(palavra);
      hash_insere(tabela, palavra, nome_arquivo, pos);
   }
   fclose(fp);
}

void buscar(No* tabela[], char* termo){
   int pos = hash(termo);
   No* atual = tabela[pos];
   while(atual != NULL){
      if(strcmp(atual->reg.termo, termo) == 0) {
         Ocorrencia* oc = atual->reg.lista;

         while(oc != NULL){
            FILE* fp = fopen(oc->nome_arquivo, "r");

            if(fp){
               long inicio = oc->offset - 25;
               if(inicio < 0) inicio = 0;

               fseek(fp, inicio, SEEK_SET);
               char trecho[51];
               fread(trecho, sizeof(char), 50, fp);
               trecho[50] = '\0';

               for(int i = 0; trecho[i] != '\0'; i++){
                  if(trecho[i] == '\n' || trecho[i] == '\r'){
                     trecho[i] = ' ';
                  }
               }
               printf("%s - \"...%s...\"\n", oc->nome_arquivo, trecho);
               printf("\nENTER = proxima | ESC = sair\n");
               int tecla = getch();
               if(tecla == 27){
                  fclose(fp);
                  return;
               }
               fclose(fp);
            }
            oc = oc->prox;
         }
         return;
      }
      atual = atual->prox;
   }
   printf("Termo nao encontrado.\n");
}

void processar_pasta(No* tabela[], char* caminho_pasta){
   DIR* pasta = opendir(caminho_pasta);

   if(!pasta){
      printf("Erro ao abrir pasta.\n");
      return;
   }

   struct dirent* arquivo;
   char caminho_completo[200];

   while((arquivo = readdir(pasta)) != NULL) {
      // verifica se é .txt
      if(strstr(arquivo->d_name, ".txt")){
         sprintf(caminho_completo, "%s/%s",
                  caminho_pasta,
                  arquivo->d_name);

         printf("Indexando: %s\n", caminho_completo);
         processar_arquivo(tabela, caminho_completo);
      }
   }
   closedir(pasta);
}

void limpar_palavra(char* str){
   int j = 0;
   char limpa[51];

   for(int i = 0; str[i] != '\0'; i++){
      if(isalpha(str[i]) || isdigit(str[i])) {
         limpa[j++] = tolower(str[i]);
      }
   }
   limpa[j] = '\0';
   strcpy(str, limpa);
}

void buscar_termo_curto(char* caminho_pasta, char* termo){
   DIR* pasta = opendir(caminho_pasta);
   if(!pasta){
      printf("Erro ao abrir pasta.\n");
      return;
   }
   struct dirent* arquivo;
   char caminho_completo[200];
   while((arquivo = readdir(pasta)) != NULL){
      if(strstr(arquivo->d_name, ".txt")){
         sprintf(caminho_completo, "%s/%s", caminho_pasta, arquivo->d_name);

         FILE* fp = fopen(caminho_completo, "r");
         if(!fp)
            continue;

         char palavra[51];
         while(1){
            long pos = ftell(fp);
            if(fscanf(fp, "%50s", palavra) != 1)
               break;

            limpar_palavra(palavra);

            if(strcmp(palavra, termo) == 0){

               long inicio = pos - 25;
               if(inicio < 0)
                  inicio = 0;

               fseek(fp, inicio, SEEK_SET);
               char trecho[51];
               int lidos = fread(trecho, sizeof(char), 50, fp);
               trecho[lidos] = '\0';

               // remove quebra de linha
               for(int i = 0; trecho[i] != '\0'; i++){
                  if(trecho[i] == '\n' || trecho[i] == '\r'){
                     trecho[i] = ' ';
                  }
               }

               printf("%s - \"...%s...\"\n", arquivo->d_name, trecho);
               printf("\nENTER = proxima | ESC = sair\n");

               int tecla = getch();
               if(tecla == 27){
                  fclose(fp);
                  closedir(pasta);
                  return;
               }
            }
         }
         fclose(fp);
      }
   }
   closedir(pasta);
}

void liberar_tabela(No* tabela[]){
   for(int i = 0; i < N; i++){
      No* atual = tabela[i];
      while(atual != NULL){
         Ocorrencia* oc = atual->reg.lista;

         while(oc != NULL){
            Ocorrencia* temp_oc = oc;
            oc = oc->prox;
            free(temp_oc);
         }
         No* temp_no = atual;

         atual = atual->prox;
         free(temp_no);
      }
      tabela[i] = NULL;
   }
}



