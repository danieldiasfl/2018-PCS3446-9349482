#ifndef MONTADOR_H
#define MONTADOR_H

#include <direct.h>
#include <windows.h>
#include <string>
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

class montador
{
    public:
        montador();
        virtual ~montador();
        vector<string> leCodigo(string texto);
        void geraAbsoluto(string instrucao);
        void analisaLinha(string operador, string operando);
        string analisaOperando(string operando);
        string analisaOperacao(char operac, string operando);
        int analisaSimbolo(string operando);

        bool numero(string operando);
        string converteString(int num);


        vector<string> codigoAbsoluto;
    protected:
    private:
        vector<string> codigo;
        vector<string> simboloNome;
        vector<string> simboloValor;
        string inicio;
        int passo;
        int ci;
        int instru;
        int inicioI;
        bool erro;
};

#endif // MONTADOR_H
