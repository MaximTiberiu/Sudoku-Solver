#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// dimenisunea tablei de joc este: 9x9
#define size 9

/// readSudoku citeste sub forma unei matrice tabla de joc
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

/// printSudoku afiseaza solutia jocului de Sudoku
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

/// destroySudoku elimina din memorie matricea folosita pentru tabla de joc
void destroySudoku(unsigned char** sudo)
{
    for (unsigned int i = 0; i < size; i++)
        free(sudo[i]);

    free(sudo);
}

/*  findEmptyCell cauta o pozitie pe tabla de joc, care este 0;
    Returneaza true, daca exista o pozitie necompletata (=0) returneaza false, in cazul in care toata tabla de joc este completata;
    Prin paramterii *row, *col vom afla prima pozitie a unei celule necompletate, in caz ca se returneaza true.*/
bool findEmptyCell(unsigned char** sudo, unsigned int* row, unsigned int* col)
{
    for (*row = 0; (*row) < size; (*row)++)
        for (*col = 0; (*col) < size; (*col)++)
            if (sudo[*row][*col] == 0) return true; // tabla de joc mai are celule necompletate
    return false; /// tabla de joc este completata in totalitate
}

/*  Testam cele trei conditii pentru a completa celula cu una din cele 9 cifre. Cifra num trebuie sa fie distincta pe rand, pe coloana,
    dar si in zona 3x3 in care se afla.*/

/// verifyRow verifica daca cifra num este distincta pe rand
bool verifyRow(unsigned char** sudo, unsigned int row, unsigned char num)
{
    /// parcurgem cele 9 coloane si testam

    for (unsigned int i = 0; i < 9; i++)
        if (num == sudo[row][i]) return false; // cifra se mai gaseste deja pe acest rand
    return true; /// cifra este valida pe acest rand
}

/// verifyCol verifica daca cifra num este distincta pe coloana
bool verifyCol(unsigned char** sudo, unsigned int col, unsigned char num)
{
    /// parcurgem cele 9 randuri si testam
    for (unsigned int i = 0; i < 9; i++)
        if (num == sudo[i][col]) return false; // cifra se mai gaseste deja pe aceasta coloana
    return true; /// cifra este valida pe aceasta coloana
}

