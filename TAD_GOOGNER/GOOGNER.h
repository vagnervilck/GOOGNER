#ifndef GOOGNER_H
   #define GOOGNER_H

   #define N 1000

   typedef struct ocorrencia{
      char nome_arquivo[71];
      long offset;
      struct ocorrencia* prox;
   }Ocorrencia;

   typedef struct{
      char termo[51];
      Ocorrencia* lista;
   }RegistroIndice;

   typedef struct no{
      RegistroIndice reg;
      struct no* prox;
   }No;

   No* tabela[N];

   void hash_init(No* tabela[N]);
   int hash(char* str);
   void hash_insere(No *tabela[], char *termo, char *nome_arquivo, long offset);
   void processar_arquivo(No* tabela[], char* nome_arquivo);
   void buscar(No* tabela[], char* termo);
   void processar_pasta(No* tabela[], char* caminho_pasta);
   void limpar_palavra(char* str);
   void buscar_termo_curto(char* caminho_pasta, char* termo);
   void liberar_tabela(No* tabela[]);


#endif
