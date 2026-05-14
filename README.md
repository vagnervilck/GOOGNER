# 🔍 GOOGNER — Mecanismo de Busca em Arquivos de Texto

GOOGNER é um mecanismo de busca local escrito em C que indexa arquivos `.txt` de uma pasta e permite pesquisar termos com exibição de contexto (trecho ao redor da palavra encontrada), similar a um buscador simplificado.

---

## 📁 Estrutura do Projeto

```
.
├── main.c
├── README.md
├── PASTA_TESTE/
│   ├── arquivo01.txt
│   ├── arquivo02.txt
│   └── arquivo03.txt
└── TAD_GOOGNER/
    ├── GOOGNER.h
    └── GOOGNER.c
```

---

## ⚙️ Como Funciona

### 1. Indexação

Ao iniciar, o programa solicita o caminho de uma pasta. Ele varre todos os arquivos `.txt` dessa pasta e indexa cada palavra encontrada em uma **tabela hash** por encadeamento externo.

Cada entrada na tabela armazena:
- O **termo** (palavra limpa e em minúsculas)
- Uma **lista encadeada de ocorrências**, onde cada ocorrência guarda o nome do arquivo e o *offset* (posição no arquivo) onde a palavra foi encontrada

### 2. Limpeza das Palavras

Antes de indexar ou comparar, toda palavra passa pela função `limpar_palavra`, que:
- Remove pontuação e caracteres especiais
- Converte tudo para **letras minúsculas**

Isso garante que buscas como `"Casa"`, `"casa,"` e `"CASA"` encontrem o mesmo resultado.

### 3. Busca

O comportamento da busca varia conforme o tamanho do termo digitado:

| Tamanho do termo | Método usado |
|---|---|
| 5 ou mais caracteres | Busca na **tabela hash** (rápida) |
| Menos de 5 caracteres | Busca **linear** diretamente nos arquivos |

Em ambos os casos, é exibido um **trecho de contexto** (~50 caracteres) ao redor de onde a palavra foi encontrada, no formato:

```
nome_do_arquivo.txt - "...trecho com a palavra..."
```

Para termos curtos, o usuário pode navegar entre as ocorrências pressionando **ENTER** ou encerrar a busca com **ESC**.

### 4. Encerramento

Digite `sair` para encerrar o programa. A memória alocada dinamicamente é liberada pela função `liberar_tabela`.

---

## 🧩 Estruturas de Dados

### `Ocorrencia`
Representa uma ocorrência de um termo em um arquivo:
```c
typedef struct Ocorrencia {
    char nome_arquivo[200];
    long offset;
    struct Ocorrencia* prox;
} Ocorrencia;
```

### `Registro`
Agrupa o termo e sua lista de ocorrências:
```c
typedef struct {
    char termo[51];
    Ocorrencia* lista;
} Registro;
```

### `No`
Nó da tabela hash com encadeamento externo:
```c
typedef struct No {
    Registro reg;
    struct No* prox;
} No;
```

---

## 🛠️ Funções Principais

| Função | Descrição |
|---|---|
| `hash_init` | Inicializa a tabela hash com `NULL` em todas as posições |
| `hash` | Função de dispersão baseada nos caracteres do termo (multiplicação por 31) |
| `hash_insere` | Insere um termo e sua ocorrência na tabela; ignora termos com menos de 5 caracteres |
| `processar_arquivo` | Lê um arquivo `.txt` e indexa todas as palavras |
| `processar_pasta` | Varre uma pasta e chama `processar_arquivo` para cada `.txt` encontrado |
| `buscar` | Busca um termo na tabela hash e exibe os trechos de contexto |
| `buscar_termo_curto` | Busca linear para termos com menos de 5 caracteres, com navegação interativa |
| `limpar_palavra` | Remove pontuação e normaliza a palavra para letras minúsculas |
| `liberar_tabela` | Libera toda a memória alocada dinamicamente |

---

## ▶️ Como Compilar e Executar

### Compilação (GCC)

```bash
gcc main.c TAD_GOOGNER/GOOGNER.c -o googner
```

> **Observação:** o programa usa `<conio.h>` (função `getch`), que é específico para **Windows**. Em Linux/Mac, pode ser necessário substituir por uma alternativa como `ncurses`.

### Execução

```bash
./googner
```

Ao executar, o programa pedirá o caminho da pasta a ser indexada:

```
Digite o caminho da pasta:
> C:/meus_textos
```

Em seguida, entrará no modo de busca:

```
Pesquise o termo: estrutura
arquivo1.txt - "...uso de estrutura de dados em..."
arquivo2.txt - "...a estrutura principal do sist..."
```

---

## 📌 Observações e Limitações

- Termos com **menos de 5 caracteres** não são indexados na hash; são buscados linearmente em tempo real.
- O programa considera apenas arquivos com extensão **`.txt`**.
- O tamanho máximo de uma palavra lida é de **50 caracteres**.
- O trecho de contexto exibido pode conter caracteres de início/fim de outros termos, pois é baseado em posição de bytes.
- O uso de `while(!feof(fp))` pode causar uma leitura extra no fim do arquivo — considere usar o retorno do `fscanf` como condição de parada para maior robustez.

---

## 👨‍💻 Autores

Trabalho acadêmico desenvolvido como exercício de estruturas de dados em C com auxílio de IA's.
