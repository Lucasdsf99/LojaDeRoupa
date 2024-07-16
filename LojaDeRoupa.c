#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <locale.h>

// Constantes para tamanhos máximos de arrays e strings
#define PRODUTOS_SZ 6
#define COMPRADORES_SZ 64
#define STRING_SZ 64
#define STR_SZ 1024

// Estrutura para representar um comprador
typedef struct _Comprador
{
    int id;
    char nome[64];
    int vendas;
    double totalGasto;
} Comprador;

// Estrutura para representar uma peça de roupa
typedef struct _Roupa
{
    unsigned int codigo;
    char nome[64];
    char marca[64];
    double preco;
    int quantidade;
} Roupa;

// Estrutura para representar uma peça de roupa no carrinho do comprador
typedef struct _RoupaNoCarrinho
{
    Roupa* roupa;  // Ponteiro para a roupa correspondente
    int quantidade;
    double preco;
} RoupaNoCarrinho;

// Estrutura para representar os itens vendidos
typedef struct _ItensVendidos
{
    Roupa* roupa;  // Ponteiro para a roupa correspondente
    int quantidade;
    double preco;
} ItensVendidos;

// Declaração de funções
int main();
bool compra();
Roupa criarRoupa(unsigned int codigo, const char* nome, const char* marca, double preco, int quantidade);
int obterId(Roupa* roupas, int roupasSz);
int obterQuantidade(Roupa* roupa);
void printarCarrinho(RoupaNoCarrinho* carrinho, int roupasSz);
time_t gerarIdComprador();
int compararCompradoresDecrescente(const void* v1, const void* v2);
int compararItensVendidos(const void* v1, const void* v2);

// Arrays para armazenar compradores e produtos
Comprador Compradores[COMPRADORES_SZ];
int CompradoresSz = 0;
Roupa Roupas[PRODUTOS_SZ];

// Variáveis globais para desconto e itens vendidos
double DescontoPorcentagem = 0.9;
int DescontoQuantidadeMinima = 3;
ItensVendidos TotalItensVendidos[PRODUTOS_SZ];

int main()
{
    // Configura o locale para imprimir caracteres especiais corretamente
    setlocale(LC_ALL, "Portuguese");

    // Inicialização das roupas disponíveis
    Roupas[0] = criarRoupa(0, "Camisa", "Lacoste", 100, 10);
    Roupas[1] = criarRoupa(1, "Chapeu", "Lacoste", 130, 10);
    Roupas[2] = criarRoupa(2, "Sapato", "Lacoste", 250, 10);
    Roupas[3] = criarRoupa(3, "Short", "Columbia", 150, 10);
    Roupas[4] = criarRoupa(4, "Calça", "Columbia", 130, 10);
    Roupas[5] = criarRoupa(5, "Cinto", "Columbia", 200, 10);

    // Inicialização dos itens vendidos
    for (int i = 0; i < PRODUTOS_SZ; i++)
    {
        TotalItensVendidos[i].roupa = &Roupas[i];
        TotalItensVendidos[i].preco = 0;
        TotalItensVendidos[i].quantidade = 0;
    }

    // Loop principal do programa
    while (true)
    {
        char str[1024];
        snprintf(str, STR_SZ, "");
        char continueKey;
        int option;

        // Menu principal
        system("cls");
        printf("+ ------------------------------------ +\n");
        printf("|         M U L T I M A R C A S        |\n");
        printf("|                                      |\n");
        printf("|   [0] Gerar relatorio e finalizar    |\n");
        printf("|   [1] Entrar na loja                 |\n");
        printf("|                                      |\n");
        printf("+ ------------------------------------ +\n");
        printf("\nSua escolha: ");
        scanf_s("%d", &option);
        system("cls");

        // Opção: Gerar relatório e finalizar
        if (option == 0)
        {
            // Ordena os compradores por total gasto em ordem decrescente
            qsort((void*)Compradores, sizeof(Compradores) / sizeof(Compradores[0]), sizeof(Compradores[0]), compararCompradoresDecrescente);

            // Monta o relatório
            snprintf(str + strlen(str), STR_SZ - strlen(str), "+ ------------------------------------ +\n");
            snprintf(str + strlen(str), STR_SZ - strlen(str), "|         M U L T I M A R C A S        |\n");
            snprintf(str + strlen(str), STR_SZ - strlen(str), "+ ------------------------------------ +\n");
            snprintf(str + strlen(str), STR_SZ - strlen(str), "\nRelátorio: \n\n");
            for (int i = 0; i < CompradoresSz; i++)
            {
                snprintf(str + strlen(str), STR_SZ - strlen(str), "  (%d) %s -> R$%.2f\n", Compradores[i].id, Compradores[i].nome, Compradores[i].totalGasto);
            }
            // Continuação do relatório
            snprintf(str + strlen(str), STR_SZ - strlen(str), "\n");

            // Informações adicionais
            snprintf(str + strlen(str), STR_SZ - strlen(str), "Compradores: %d\n", CompradoresSz);

            // Cálculo do faturamento bruto
            double faturamentoBruto = 0;
            int totalDeItensVendidos = 0;
            for (int i = 0; i < PRODUTOS_SZ; i++)
            {
                faturamentoBruto += TotalItensVendidos[i].preco;
                totalDeItensVendidos += TotalItensVendidos[i].quantidade;
            }
            snprintf(str + strlen(str), STR_SZ - strlen(str), "Itens vendidos: %d\n", totalDeItensVendidos);
            snprintf(str + strlen(str), STR_SZ - strlen(str), "Faturamento Bruto: R$%.2f\n", faturamentoBruto);

            // Encontra o produto mais vendido
            qsort((void*)TotalItensVendidos, sizeof(TotalItensVendidos) / sizeof(TotalItensVendidos[0]), sizeof(TotalItensVendidos[0]), compararItensVendidos);
            int maisVendidoQt = 0;
            Roupa* maisVendido = TotalItensVendidos[0].roupa;
            for (int i = 0; i < PRODUTOS_SZ; i++)
            {
                if (TotalItensVendidos[i].quantidade != 0)
                {
                    maisVendidoQt = TotalItensVendidos[i].quantidade;
                    maisVendido = TotalItensVendidos[i].roupa;
                    break;
                }
            }
            snprintf(str + strlen(str), STR_SZ - strlen(str), "Mais Vendido: %s (%s) - %dx\n", maisVendido->nome, maisVendido->marca, maisVendidoQt);

            // Encontra o produto menos vendido
            int menosVendidoQt = 0;
            Roupa* menosVendido = TotalItensVendidos[PRODUTOS_SZ-1].roupa;
            for (int i = PRODUTOS_SZ-1; i >= 0; i--)
            {
                if (TotalItensVendidos[i].quantidade != 0)
                {
                    menosVendidoQt = TotalItensVendidos[i].quantidade;
                    menosVendido = TotalItensVendidos[i].roupa;
                    break;
                }
            }
            snprintf(str + strlen(str), STR_SZ - strlen(str), "Menos Vendido: %s (%s) - %dx\n", menosVendido->nome, menosVendido->marca, menosVendidoQt);

            // Continuação do relatório
            snprintf(str + strlen(str), STR_SZ - strlen(str), "\n\n");
            printf("%s", str);

            // Grava o relatório em um arquivo
            FILE* file;
            if (fopen_s(&file, "loja_roupa.dat", "a") == 0 && file != NULL) {
                fprintf(file, str);
                fclose(file);
                
                printf("Aperte qualquer tecla para finalizar!\n");
                scanf("%c", &continueKey);
                break;
            }
           
            // Se houver falha ao abrir o arquivo
            if (file != NULL) {
                char line[STR_SZ];
                printf("\nConteudo do arquivo loja_roupa.dat:\n");
                while (fgets(line, sizeof(line), file) != NULL) {
                    printf("%s", line);
                }
                fclose(file);
            } else {
                printf("Nao foi possivel abrir o arquivo loja_roupa.dat\n");
            }

            return 0;
        }
        // Opção: Entrar na loja
        else if (option == 1)
        {
            int resultado = compra();
            if (resultado)
            {
                printf("Obrigado pela preferência! Volte sempre!\n\n");
            }
            else
            {
                printf("Lamentamos por não ter encontrado o que procurava. Volte sempre!\n\n");
            }
        }
        // Opção inválida
        else
        {
            printf("Opção inválida.\n\n");
        }

        system("Pause");
        scanf_s("%c", &continueKey);
    }
}

// Função de comparação para ordenar os itens vendidos por quantidade (decrescente)
int compararItensVendidos(const void* v1, const void* v2)
{
    const ItensVendidos* p1 = (ItensVendidos*)v1;
    const ItensVendidos* p2 = (ItensVendidos*)v2;
    if (p1->quantidade > p2->quantidade)
        return -1;
    else if (p1->quantidade < p2->quantidade)
        return +1;
    else
        return 0;
}

// Função de comparação para ordenar os compradores por total gasto (decrescente)
int compararCompradoresDecrescente(const void* v1, const void* v2)
{
    const Comprador* p1 = (Comprador*)v1;
    const Comprador* p2 = (Comprador*)v2;
    if (p1->totalGasto > p2->totalGasto)
        return -1;
    else if (p1->totalGasto < p2->totalGasto)
        return +1;
    else
        return 0;
}

// Função para gerar um ID de comprador baseado no tempo atual
time_t gerarIdComprador()
{
    return time(NULL);
}

// Função para inicializar o carrinho de compras com quantidades zeradas
void inicializarCarrinho(RoupaNoCarrinho* carrinho, int size)
{   
    for (int i = 0; i < size; i++)
    {
        carrinho[i].quantidade = 0;
    }
}

// Função para simular o processo de compra
bool compra()
{
    // Variáveis locais para representar o comprador, a roupa e o carrinho
    Comprador comprador;
    Roupa* roupa;
    RoupaNoCarrinho carrinho[PRODUTOS_SZ];
    int carrinhoSz = 0;

    // Inicializa o carrinho de compras
    inicializarCarrinho(carrinho, PRODUTOS_SZ);

    // Gera um ID de comprador e solicita o nome do comprador
    comprador.id = gerarIdComprador();
    printf("Faça login com seu nome: ");
    scanf_s("%s", &comprador.nome, STRING_SZ);
    system("cls");

    // Mensagem de boas-vindas ao comprador
    printf("%s, Bem vindo a loja MultiMarcas!\n", comprador.nome);
    printf("\nA partir de 3 itens do mesmo produto no carrinho recebe 10%% de desconto.\n\n");

    // Loop para adicionar produtos ao carrinho
    while (true)
    {
        RoupaNoCarrinho roupa2;

        // Obtém o ID do produto escolhido pelo comprador
        int idRoupa = obterId(Roupas, PRODUTOS_SZ);
        if (idRoupa == 6)
        {
            return false;
        }
        if (idRoupa == 7)
        {
            if (carrinhoSz > 0)
                break;
            system("cls");
            printf("ALERTA: ADICIONE PELO MENOS UMA DAS PEÇAS ABAIXO!\n\n");
            continue;
        }
        else if (idRoupa < 0 || idRoupa >= PRODUTOS_SZ)
        {
            printf("Escolha um dos produtos acima!\n\n");
            system("PAUSE");
            system("cls");
            continue;
        }
        roupa2.roupa = &Roupas[idRoupa];

        // Obtém a quantidade desejada do produto
        int quantidade = 0;
        while (true)
        {
            quantidade = obterQuantidade(roupa2.roupa);
            if (quantidade < 0)
            {
                printf("Quantidade inválida.\n");
                continue;
            }
            else if (quantidade > roupa2.roupa->quantidade)
            {
                printf("Estoque insuficiente.\n\n");
                continue;
            }
            break;
        }

        // Calcula o preço com desconto, se aplicável, e adiciona ao carrinho
        roupa2.quantidade = quantidade;
        if (roupa2.quantidade >= DescontoQuantidadeMinima)
            roupa2.preco = (double)(roupa2.roupa->preco * (double)roupa2.quantidade) * DescontoPorcentagem;
        else
            roupa2.preco = (double)(roupa2.roupa->preco * (double)roupa2.quantidade);
        carrinho[roupa2.roupa->codigo] = roupa2;

        system("cls");
        if (roupa2.quantidade > 0)
        {
            carrinhoSz++;
            printf("%d %s (%s) adicionando(s) ao carrinho por R$%.2f.\n\n", roupa2.quantidade, roupa2.roupa->nome, roupa2.roupa->marca, roupa2.preco);
        }
        else
        {
            carrinhoSz--;
            printf("A peça(s) '%s (%s)' foi removido do carrinho.\n\n", roupa2.roupa->nome, roupa2.roupa->marca);
        }
        printarCarrinho(carrinho, PRODUTOS_SZ);
    }

    // Exibe o carrinho de compras ao finalizar
    system("cls");
    printarCarrinho(carrinho, PRODUTOS_SZ);

    // Calcula o total gasto pelo comprador e atualiza os itens vendidos
    comprador.totalGasto = 0;
    for (int i = 0; i < PRODUTOS_SZ; i++)
    {
        if (carrinho[i].quantidade <= 0) continue;
        comprador.totalGasto += carrinho[i].preco;

        TotalItensVendidos[i].quantidade += carrinho[i].quantidade;
        TotalItensVendidos[i].preco += carrinho[i].preco;
    }

    // Armazena o comprador na lista de compradores
    Compradores[CompradoresSz] = comprador;
    CompradoresSz++;

    return true;
}

// Função para criar uma peça de roupa
Roupa criarRoupa(unsigned int codigo, const char* nome, const char* marca, double preco, int quantidade)
{
    Roupa roupa;
    roupa.quantidade = quantidade;
    roupa.codigo = codigo;
    strcpy_s(roupa.nome, sizeof(roupa.nome), nome);
    strcpy_s(roupa.marca, sizeof(roupa.marca), marca);
    roupa.preco = preco;
    return roupa;
}

// Função para obter o ID do produto escolhido pelo comprador
int obterId(Roupa* roupas, int roupasSz)
{
    printf("+ ------------------------------------ +\n");
    printf("|         M U L T I M A R C A S        |\n");
    printf("|                                      |\n");
    for (int i = 0; i < roupasSz; i++)
    {
        Roupa* roupa = &roupas[i];
        printf("|  [%d] %s (%s) = R$%.2f     |\n", roupa->codigo, roupa->nome, roupa->marca, roupa->preco);
    }
    printf("|                                      |\n");
    printf("|  [6] Cancelar compra                 |\n");
    printf("|  [7] Finalizar compra                |\n");
    printf("+ ------------------------------------ +\n");
    int idRoupa;
    printf("\nSua escolha: ");
    scanf_s("%d", &idRoupa);
    printf("\n");
    return idRoupa;
}

// Função para obter a quantidade desejada do produto
int obterQuantidade(Roupa* roupa)
{
    printf("Você escolheu a peça %s. Somente %d no estoque! \n", roupa->nome, roupa->quantidade);
    printf("Adicionando 3 ou mais itens do mesmo produto você ganha 10%% de desconto!\n");
    printf("Quantidade desejada (0 para remover a peça): ");
    int quantidade;
    scanf_s("%d", &quantidade);
    printf("\n");
    system("Pause");
    return quantidade;
}

// Função para imprimir o carrinho de compras
void printarCarrinho(RoupaNoCarrinho* carrinho, int roupasSz)
{
    printf("Carrinho:\n\n");
    for (int i = 0; i < roupasSz; i++)
    {
        RoupaNoCarrinho* roupa2 = &carrinho[i];
        if (!roupa2 || !roupa2->roupa) { continue; }
        if (roupa2->quantidade <= 0) { continue; }
        printf("  %s (%s) = R$%.2f (%dx %.2f)", roupa2->roupa->nome, roupa2->roupa->marca, roupa2->preco, roupa2->quantidade, roupa2->roupa->preco);
        if (roupa2->quantidade >= 3)
        {
            printf(" [10%% OFF] ");
        }
        printf("\n");
    }
    printf("\n");
}

