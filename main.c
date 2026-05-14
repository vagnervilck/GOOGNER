#include "TAD_GOOGNER\GOOGNER.h"
#include <stdio.h>
#include <string.h>

int main(){

   No* tabela[N];
   hash_init(tabela);
   char pasta[200];

   printf("Digite o caminho da pasta:\n");
   scanf("%199s", pasta);
   processar_pasta(tabela, pasta);

   char termo[51];
   while(1){
      printf("\nPesquise o termo: ");
      scanf("%50s", termo);
      limpar_palavra(termo);

      if(strcmp(termo, "sair") == 0)
         break;

      if(strlen(termo) < 5)
         buscar_termo_curto(pasta, termo);
      else
         buscar(tabela, termo);
   }
   liberar_tabela(tabela);
   return 0;
}
