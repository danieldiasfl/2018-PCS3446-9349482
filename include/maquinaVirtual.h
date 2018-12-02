#ifndef MAQUINAVIRTUAL_H
#define MAQUINAVIRTUAL_H

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

using namespace std;

class maquinaVirtual
{
    public:
        maquinaVirtual();
        virtual ~maquinaVirtual();
        string interpreta(string instruct, string acumuladorb, int pc, int indireto, int permissao);
        void carregaLoader();
        string carrega(vector<string> codigoAbs);
        string converteString(int num);
        string memoria[4096];
        int complemento2INT(string num, string signal);
        string complemento2STRING(int num);
        void run(string endereco);

    protected:

    private:
        int pc;
        int enderecoSR;
        int iCount;
        string acumulador;
        string acumuladorSR;

        bool indireto;
        bool stop;
        bool carregando;
        bool interrupt;

        vector<string> saidaDumper;
        vector<string> codigoAbs;
};

#endif // MAQUINAVIRTUAL_H
