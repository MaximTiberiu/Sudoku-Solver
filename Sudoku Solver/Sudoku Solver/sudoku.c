#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// dimenisunea tablei de joc este: 9x9
#define size 9

/* readSudoku citeste sub forma unei matrice tabla de joc */
void readSudoku(unsigned char*** sudo, char* inputFile)
{
    FILE* fin = fopen(inputFile, "r");
    assert(fin != NULL);

    // alocare memorie sudo[][]
    *sudo = (unsigned char**)malloc(size * sizeof(unsigned char*));
    assert(*sudo != NULL);

    for (unsigned int i = 0; i < size; i++)
    {
        (*sudo)[i] = (unsigned char*)malloc(size * sizeof(unsigned char));
        assert((*sudo)[i] != NULL);
    }

    // citire sudoku
    for (unsigned int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            fscanf(fin, "%hhu", &(*sudo)[i][j]);

    fclose(fin);
}

/* printSudoku afiseaza solutia jocului de Sudoku */
void printSudoku(unsigned char** sudo, char* outputFile)
{
    FILE* fout = fopen(outputFile, "w");
    assert(fout != NULL);

    for (unsigned int i = 0; i < size; i++)
    {
        if (i % 3 == 0 && i != 0)
            fprintf(fout, "- - - + - - - + - - -  \n");

        for (unsigned int j = 0; j < size; j++)
        {
            if (j % 3 == 0 && j != 0)
                fprintf(fout, "| ");
            
            if (j == 8)
            {
                fprintf(fout, "%hhu", sudo[i][j]);
                if(i != 8)
                    fprintf(fout, "\n");
            }
            else
                fprintf(fout, "%hhu ", sudo[i][j]);
        }
    }
    fclose(fout);
}

/* destroySudoku elimina din memorie matricea folosita pentru tabla de joc */
void destroySudoku(unsigned char** sudo)
{
    for (unsigned int i = 0; i < size; i++)
        free(sudo[i]);

    free(sudo);
}

/*  findEmptyCell cauta o pozitie pe tabla de joc, care este 0;
    Returneaza true, daca exista o pozitie necompletata (=0) returneaza false, in cazul in care toata tabla de joc este completata;
    Prin paramterii *row, *col vom afla prima pozitie a unei celule necompletate, in caz ca se returneaza true.*/
bool findEmptyCell(unsigned char** sudo, unsigned int *row, unsigned int *col)
{
    for (unsigned int i = 0; i < size; i++)
        for (unsigned int j = 0; j < size; j++)
            if (sudo[i][j] == 0)
            {
                *row = i;
                *col = j;
                return true; // tabla de joc mai are celule necompletate
            }
    return false; /// tabla de joc este completata in totalitate
}

/*  Testam cele trei conditii pentru a completa celula cu una din cele 9 cifre. Cifra num trebuie sa fie distincta pe rand, pe coloana,
    dar si in zona 3x3 in care se afla.*/

/* verifyRow verifica daca cifra num este distincta pe rand */
bool verifyRow(unsigned char** sudo, unsigned int row, unsigned char num)
{
    /// parcurgem cele 9 coloane si testam

    for (unsigned int i = 0; i < size; i++)
        if (num == sudo[row][i]) return true; // cifra se mai gaseste deja pe acest rand
    return false; /// cifra este valida pe acest rand
}

/* verifyCol verifica daca cifra num este distincta pe coloana */
bool verifyCol(unsigned char** sudo, unsigned int col, unsigned char num)
{
    /// parcurgem cele 9 randuri si testam
    for (unsigned int i = 0; i < 9; i++)
        if (num == sudo[i][col]) return true; // cifra se mai gaseste deja pe aceasta coloana
    return false; /// cifra este valida pe aceasta coloana
}

/// verifySquare verifica daca cifra num este distincta in submatricea 3x3 in care se afla
bool verifySquare(unsigned char** sudo, unsigned int row, unsigned int col, unsigned char num)
{
    /// !!! Input: row, col sunt coordonatele de start ale submatricei 3x3
    
    /// parcurgem doar 3 randuri, respectiv 3 coloane !!!
    for (unsigned int i = 0; i < 3; i++)
        for (unsigned int j = 0; j < 3; j++)
            if (num == sudo[i + row][j + col]) /// i + row si j + col -> ne trimit in submatricea 3x3 unde se afla celula necompletata
                return true; /// cifra se mai gaseste deja in aceasta submatrice
    return false; /// cifra este valida in aceasta submatrice
}

/// isValid combina cele trei conditii (cele 3 functii de mai sus, iar daca trece de toate conditiile => cifra este valida la acea pozitie)
bool isValid(unsigned char** sudo, unsigned int row, unsigned int col, unsigned char num)
{
    /// conditia necesara - sudo[row][col] == 0, altfel nu are rost sa mai testam mai departe

    if (sudo[row][col]) return false;
    if (verifyRow(sudo, row, num)) return false;
    if (verifyCol(sudo, col, num)) return false;
    row = row - row % 3; /// trebuie sa ajungem la prima pozitie din submatrice
    col = col - col % 3; /// de aceea efectuam scaderea cu row/col % 3 
    if (verifySquare(sudo, row, col, num)) return false;
    return true; /// num indeplineste toate conditiile
}

/// sudokuSolution verifica daca exista o solutie
bool sudokuSolution(unsigned char*** sudo)
{
    int row, col; /// preluam pozitia primei celule necompletate, in caz ca aceasta exista
    
    /// verificam daca exista celule necompletate
    /// in caz ca nu exista celule necompletate => exista o solutie
    if (!findEmptyCell(*sudo, &row, &col)) return true;
    
    /// avem cel putin o celula necompletata => aplicam backtracking pentru a gasi solutia
    
    /// testam toate cifrele, pana gasim una valida
    for(unsigned char i = 1; i <= size ; i++)
    {
        if (isValid(*sudo, row, col, i))
        {
            /// daca este valid, punem cifra num la pozitia (row, col)
            (*sudo)[row][col] = i;
            

            /// apelam functia recursiv si daca in final returneaza true, am gasit solutia

            if (sudokuSolution(sudo)) return true;

            /// in caz contrar, elimnam cifra adaugata anterior, si trecem la urmatoarea cifra
            (*sudo)[row][col] = 0;
        }
    }
    
    return false; // nu exista solutie
}

void solveSudoku(unsigned char** sudo, char* outputFile)
{
    if (sudokuSolution(&sudo))
    {
        printf("Avem o solutie! Vezi fisierul de iesire...\n");
        printSudoku(sudo, outputFile); // avem solutie, afisam
    }
    else
        printf("Nu exista solutie!\n");
}