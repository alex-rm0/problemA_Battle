#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int dx[4] = {-1, 1, 0, 0}; // Cima, Baixo, Esquerda, Direita
int dy[4] = {0, 0, -1, 1};

// Função para verificar se está dentro dos limites do grid
int check_limits(int x, int y, int nl, int nc) {
    return (x >= 0 && x < nl && y >= 0 && y < nc);
}

// Função para verificar a existência de outras turrets no grid
int check_others(int nl, int nc, char grid[nl][nc], int actual_line, int actual_col) {
    int found_turret = 0;

    // Verifica a linha
    for (int i = 0; i < nc; i++) {
        if (grid[actual_line][i] == 'T') return 0;
        if (isdigit(grid[actual_line][i])) break;
    }

    // Verifica a coluna
    for (int i = 0; i < nl; i++) {
        if (grid[i][actual_col] == 'T') return 0;
        if (isdigit(grid[i][actual_col])) break;
    }

    return 1;
}

// Função para tentar colocar turrets variando a ordem das direções
int place_turrets(int nl, int nc, char grid[nl][nc]) {
    int number_turrets = 0;
    int turrets_left = 0;

    // Testamos 4 variações de ordem das direções
    for (int attempt = 0; attempt < 4; attempt++) {
        char temp_grid[nl][nc];
        memcpy(temp_grid, grid, sizeof(char) * nl * nc); // Fazemos uma cópia da grid para testar

        number_turrets = 0;
        turrets_left = 0;

        for (int i = 0; i < nl; i++) {
            for (int j = 0; j < nc; j++) {
                if (isdigit(temp_grid[i][j]) && temp_grid[i][j] != '0') {
                    int turrets = temp_grid[i][j] - '0';

                    // Variamos a ordem de tentativa das direções
                    for (int d = 0; d < 4 && turrets > 0; d++) {
                        int index = (d + attempt) % 4; // Rotação das direções
                        int ni = i + dx[index];
                        int nj = j + dy[index];

                        if (check_limits(ni, nj, nl, nc) && check_others(nl, nc, temp_grid, ni, nj) && temp_grid[ni][nj] == '.') {
                            temp_grid[ni][nj] = 'T';
                            turrets--;
                            number_turrets++;
                        }
                    }

                    turrets_left += turrets;
                }
            }
        }

        // Se conseguimos posicionar todas as turrets, usamos essa configuração
        if (turrets_left == 0) {
            memcpy(grid, temp_grid, sizeof(char) * nl * nc);
            return number_turrets;
        }
    }

    // Se nenhuma variação conseguiu cobrir todas as posições corretamente
    return 0;
}

int main() {
    int n_test_cases;
    scanf("%d", &n_test_cases);
    getchar();

    int nl, nc;

    for (int i = 0; i < n_test_cases; i++) {
        printf("\ntest case %d\n", i + 1);
        scanf("%d %d", &nl, &nc);
        getchar();

        char grid[nl][nc];

        // Ler a grid
        for (int j = 0; j < nl; j++) {
            for (int k = 0; k < nc; k++) {
                char ch;
                scanf(" %c", &ch);
                grid[j][k] = ch;
            }
        }

        int result = place_turrets(nl, nc, grid);

        for (int i = 0; i < nl; i++) {
            for (int j = 0; j < nc; j++) {
                printf("%c", grid[i][j]);
            }
            printf("\n");
        }

        if (result != 0) {
            printf("%d\n", result);
        } else {
            printf("noxus will rise!\n");
        }
    }

    return 0;
}
