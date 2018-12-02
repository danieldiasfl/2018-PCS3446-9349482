#include "montador.h"
#include "maquinaVirtual.h"
#include <stdio.h>
#include <list>
#include <vector>
//CONSIDERACOES: WIN32 USA BIBLIO DO WINDOWS
//USA BIBLITECAS COM C++11 (TO_STRING)

using namespace std;

int id = 3;

int main()
{
    string login, pasta, base;
    bool fim = 0;
    bool fim_prog = 0;
    string comando, operacao, operando;
    char delimiter = ' ';
    int numPag = 1;
    int indireto, permissao;

    //DISCO
    vector<vector<string>> prog_abs; // tem o código absoluto de cada programa nos blocos
    string nome_prog[32]; // tem o nome do programa cujo código foi carregado na mesma posicao e a 1a pag

    //PAGINAS
    string pags[32][4]; // programas carregador nas páginas (busy, instrução, no pag, prox pag)

    //BLOCOS
    string bloco[16][4]; // 16 blocos com 16 bytes cada (1 instrucao)
    string bloco_rel[16][4]; // busy, nome prog,proximo bloco (continuação do prog), pagina

    //EXECUÇÃO
    string programas[4][8]; //até 4 instruções; armazena:busy, nome do programa, id programa, status, acumulador, pc, bloco, instrução no bloco.
    list <int> ordem;

    //INTERRUPÇÂO
    list <int> interrupt;

    //"RESETA" MEMORIA
    //for(int i = 0; i < 4096; i++)
    //   memoria[i] = "00";

    //RESETA PROGRAMAS
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 3; j++){
            pags[i][j] = "0";
        }
    }


    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 4; j++){
            bloco[i][j] = "0";
            bloco_rel[i][j] = "0";
        }
        bloco[i][4] = 16*i;
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 8; j++)
            programas[i][j] = "0";
    }

    for(int i = 0; i < 16; i++)
        bloco_rel[i][2] = "-1";

    for(int i = 0; i < 32; i++){
        pags[i][3] = "-1";
    }

    // IDENTIFICACAO DO USUARIO
    while (fim_prog == 0){
        fim = 0;
/*        cout << "Digite seu login" << endl;
        cin >> login;

        pasta = "./" + login;
        // VERIFICA SE USUARIO JA EXISTE (PASTA CRIADA OU NAO)
        DWORD existe = GetFileAttributesA(pasta.c_str());
        if (existe == INVALID_FILE_ATTRIBUTES){ // pasta não existe

            // CRIAR PASTA
            base = "./" + login;
            mkdir(login.c_str());

        }
        else{ */// PASTA JA EXISTE
        string login = "files";
        string pasta = "./files";
        string base = pasta;
            cout << "Digite um comando:" << endl;
            cout << "1. DIR: Informa programas disponiveis para serem carregador na memoria" << endl; //
            cout << "2. LOAD: Carrega programa para memória" << endl; // talvez
            cout << "3. PAG: Mostra páginas de memória" << endl;
            cout << "4. MEM: Mostra blocos de memoria" << endl << endl;

            cout << "5. ADD: Adiciona programa na lista de programas para rodar" << endl; //
            cout << "6. RINFO: Informa quais programas estao na lista para rodar, bem como seus parametros" << endl << endl; //

            cout << "7. DEL: Remove um programa da memoria" << endl;
            cout << "8. REMOVE: Remove um programa da execução" << endl;
            cout << "9. CLOSE: tira programa da execução" << endl << endl;

            cout << "10. RUN: Executa 1 iteração do(s) programa(s):" << endl;
            cout << "11. END: Encerra operacao" << endl;

            while(fim == 0){
                getline(cin, comando);

                //SEPARA EM OPERACAO E OPERANDO
                //VARIAVEIS NECESSARIAS P SEPARAR
                vector <string> tokens;
                stringstream check1(comando);
                string intermediate;
                //SEPARACAO(COLOCA EM TOKENS)
                while(getline(check1, intermediate, ' '))
                {
                    tokens.push_back(intermediate);
                }
                //COLOCA EM OPERACAO E OPERANDO
                for(int i = 0; i < tokens.size(); i++){
                    if (i == 0) operacao = tokens[i];
                    if (i == 1) operando = tokens[i];
                }

                if (operacao == "DIR") {
                    // LISTA TODOS ARQUIVOS NA PASTA
                    string pastatxt = pasta + "\\*txt";

                    WIN32_FIND_DATA arquivos;
                    memset(&arquivos, 0, sizeof(WIN32_FIND_DATA));
                    HANDLE handle = FindFirstFile(pastatxt.c_str(), &arquivos);
                    while(handle != INVALID_HANDLE_VALUE)
                    {
                        cout<<"\n"<<arquivos.cFileName;
                        if(FindNextFile(handle, &arquivos) == FALSE)
                        break;
                    }
                    //Close the handle after use or memory/resource leak
                    FindClose(handle);

                    cout << endl << endl;
                }

                else if(operacao == "LOAD"){
                    //ver tamanho do programa
                    // tentar alocar na ordem (ve se não da conflito)
                    //  aloca em algum lugar e coloca os segmentos na lista de programas alocados (mem_used)
                    //  coloca nome do programa em prog_loaded.

                    string pastatxt = pasta + "\\*txt";

                    WIN32_FIND_DATA arquivos;
                    memset(&arquivos, 0, sizeof(WIN32_FIND_DATA));
                    HANDLE handle = FindFirstFile(pastatxt.c_str(), &arquivos);
                    while(handle != INVALID_HANDLE_VALUE)
                    {
                        cout<<"\n"<<arquivos.cFileName;
                        if(FindNextFile(handle, &arquivos) == FALSE)
                        break;
                    }
                    //Close the handle after use or memory/resource leak
                    FindClose(handle);

                    cout << endl << endl;

                    // Procura no disco
                    string choice;
                    cin >> choice;
                    string programa = pasta + "/" + choice;
                    vector<string> codigoAbs; codigoAbs.clear();
                    //ACIONA MONTADOR
                    montador* monta = new montador();
                    codigoAbs = monta->leCodigo(programa);
                    codigoAbs.erase(codigoAbs.begin(), codigoAbs.begin()+2);

                    prog_abs.push_back(codigoAbs);
                    nome_prog[prog_abs.size() - 1] = choice;

                    // COLOCA PAGINA
                    list<int> free_pag;
                    for(int i = 0; i < 32; i++){
                        if(pags[i][0] == "0"){
                            free_pag.push_back(i);
                        }
                    }

                    int first_p = free_pag.front();
                    int tamanho = codigoAbs.size();
                    int p;
                    int num_pag = 0;
                    while(tamanho > 0){
                        p = free_pag.front();
                        pags[p][0] = "1";
                        pags[p][1] = choice;
                        pags[p][2] = to_string(num_pag);
                        tamanho -= 8;
                        num_pag++;

                        free_pag.pop_front();
                        if(tamanho > 0){
                            pags[p][3] = to_string(free_pag.front());
                        }
                    }
                    free_pag.clear();

                    // BLOCOS
                    list<int> free_block;
                    for(int i = 0; i < 16; i++){
                        if(bloco_rel[i][0] == "0"){
                            free_block.push_back(i);
                        }// bloco livre
                    }
                    if(free_block.size() == 0) free_block.push_back(rand() % 16);

                    int k = free_block.front();
                    tamanho = 8;
                    int alocado = 0;

                    while(tamanho > alocado){
                        bloco_rel[k][3] = to_string(first_p);
                        for(int j = 0; j < 4; j++){
                            if(tamanho>alocado){
                                bloco[k][j] = codigoAbs[alocado];
                            }
                                alocado++;
                        }

                        bloco_rel[k][0] = "1";
                        bloco_rel[k][1] = choice;

                        free_block.pop_front();
                        if(tamanho > alocado){
                            bloco_rel[k][2] = to_string(free_block.front());
                        }
                        k = free_block.front();
                    }

                    free_block.clear();
                    cout << endl;
                }

                else if(operacao == "PAG"){
                    for(int i =0; i <32; i++){
                        if(pags[i][0] == "1"){
                            cout << endl;
                            for(int j = 0; j < 4; j++){
                                cout << pags[i][j] << " ";
                            }
                        }
                    }
                    cout << endl;
                }

                else if(operacao == "MEM"){

                    for(int i = 0; i < 16; i++){
                        //if(bloco_rel[i][0] == "1"){
                            cout << endl;
                            for(int j = 0; j < 4; j++)
                                cout << bloco[i][j] << " ";
                            for(int j = 0; j < 4; j++)
                                cout << bloco_rel[i][j] << " ";
                        //}
                    }
                }

                else if(operacao == "ADD"){

                    int busy[4];
                    int ocup = 0;
                    int next;

                    for(int i = 0; i < 4; i++){
                        if(programas[i][0] == "0"){
                            busy[i] = 0;
                            next = i;
                        }
                        else ocup++;
                    }

                    if(ocup >= 4)
                        cout << "Erro: Maximo numero de programas rodando!" << endl;

                    else{
                        int num_prog;
                        //mostra programas disponíveis
                        cout << "escolha um programa:" << endl;
                        for(int i = 0; i < prog_abs.size(); i++){
                            cout << i << ". " << nome_prog[i] << endl;
                        }
                        // escolhe programa
                        cin >> num_prog;

                        int bl, numbl = 17;
                        for(int i = 0; i < 16; i++){
                              if(bloco_rel[i][1] == nome_prog[num_prog] && strtol(bloco_rel[i][3].c_str(), NULL, 10) < numbl){
                                bl = i;
                                numbl = strtol(bloco_rel[i][3].c_str(), NULL, 10);
                             }
                        }

                        programas[next][0] = "1"; // BUSY
                        programas[next][1] = nome_prog[num_prog]; // NOME
                        programas[next][2] = to_string(id); //id
                        programas[next][3] = "W"; // STATUS
                        programas[next][4] = "0" ;// ACUMULADOR
                        programas[next][5] = "0"; //pc
                        programas[next][6] = to_string(bl); // bloco inicial
                        programas[next][7] = "0"; // offset do bloco
                        id++;
                    }
                    ordem.push_back(next);

                    cout << endl;
                }

                else if(operacao == "RINFO"){
                    cout << endl << "ocupado, programa, status, acumulador, pc, bloco inicial, offset";
                    for(int i =0; i < 4; i++){
                            cout << endl;
                        for(int j = 0; j < 8; j++){
                            cout << programas[i][j] << " ";
                        }
                    }
                    cout << endl << endl;
                }

                else if (operacao == "DEL"){
                    DeleteFile((pasta  + "/" + operando).c_str());
                }

                else if (operacao == "REMOVE"){
                    // remover da memoria
                }

                else if (operacao == "CLOSE"){
                    // remover da memoria
                }

                else if (operacao == "RUN"){
                    bool inter;
                    if(interrupt.size() != 0 ) inter = 1;
                    else                       inter = 0;
                    if(inter == 0){
                        int aux = ordem.front();

                        maquinaVirtual * mv = new maquinaVirtual();

                        string acumuladorr = programas[ordem.front()][4];

                        string pc_s = programas[aux][5];
                        int pc = strtol(pc_s.c_str(), NULL, 10);

                        int bloco_1 = strtol(programas[aux][6].c_str(), NULL, 10);
                        int bloco_2 = strtol(programas[aux][7].c_str(), NULL, 10);
                        string instru = bloco[bloco_1][bloco_2];

                        string valores = mv->interpreta(instru, acumuladorr, pc, indireto, permissao);
                        delete mv;
                        vector <string> tokens;
                        stringstream check1(valores);
                        string intermediate;
                        while(getline(check1, intermediate, ' '))
                        {
                            tokens.push_back(intermediate);
                        }

                        string resp[3];
                        for(int i = 0; i < 3; i++){
                            resp[i] = tokens[i];
                        }

                        if(resp[2] == "1"){
                            programas[aux][0] = "0"; // finaliza execucao (busy)
                            programas[aux][3] = "D";
                            ordem.pop_front();
                        }
                        else{
                            // se pc tiver passado a página(32 bytes), deve-se procura nova página:
                            if(strtol(resp[0].c_str(), NULL, 10) > 32){ //acabou a pagina!
                                int block = strtol(programas[aux][6].c_str(), NULL, 10);
                                string prog_name = bloco_rel[block][1];
                                int adress = strtol(resp[0].c_str(), NULL, 10);
                                int page_no = int(adress/8);
                                // agora, deve-se carregar instruções a partir de 8 * page_no na memoria
                                // BLOCOS
                                list<int> free_block;
                                for(int i = 0; i < 16; i++){
                                    if(bloco_rel[i][0] == "0"){
                                        free_block.push_back(i);
                                    }// bloco livre
                                }
                                if(free_block.size() == 0) free_block.push_back(rand() % 16);

                                vector<string> codigoAbs;
                                for(int i =0; i < 32; i++){
                                    if(prog_name == nome_prog[i])
                                        codigoAbs = prog_abs.at(i);
                                }

                                int k = free_block.front();
                                int tamanho = 8;
                                int alocado = 0;

                                while(tamanho > alocado){
                                    bloco_rel[k][3] = bloco_rel[block][3];
                                    for(int j = 0; j < 4; j++){
                                        if(tamanho>alocado){
                                            bloco[k][j] = codigoAbs[alocado + (8 * page_no)];
                                        }
                                            alocado++;
                                    }

                                    bloco_rel[k][0] = "1";
                                    bloco_rel[k][1] = bloco_rel[block][1];

                                    free_block.pop_front();
                                    if(tamanho > alocado){
                                        bloco_rel[k][2] = to_string(free_block.front());
                                    }
                                    k = free_block.front();
                                }

                                 free_block.clear();
                                 programas[aux][7] = "-1";
                            }


                            //nome nao muda programas[ordem.front()][1] =
                            //id nao muda programas[ordem.front()][2] =
                            programas[aux][3] = "R"; // status RUNNING
                            programas[aux][4] = resp[1]; // acumulador
                            programas[aux][5] = resp[0]; // pc

                            if(programas[aux][7] == "3"){ //se acabou o bloco
                                int aux2 = strtol((programas[aux][6]).c_str(),NULL, 10);
                                programas[aux][6] = bloco_rel[aux2][2]; // = ponteiro para prox bloco
                                programas[aux][7] = "0";
                            }
                            else{
                                programas[aux][7] = to_string(strtol(programas[aux][7].c_str(), NULL, 10) + 1);
                            }
                            ordem.pop_front();
                            ordem.push_back(aux); // coloca no fim da fila
                        }

                    }

                    if(inter == 1){
                        cout << "Tratamento de interrupcao!!" << endl;
                        int type = interrupt.front();

                        if(type == 1){ // entradas/saida;
                            cout << "Dispositivo" << endl;
                            interrupt.pop_front();
                        }

                        if(type == 2){
                            cout << "Entrada do usuário" << endl;
                            string in;
                            cin >> in;
                            interrupt.pop_front();
                        }
                    }
                }

                else if (operacao == "x"){
                    interrupt.push_back(rand() % 2 + 1);
                }

                else if (operacao == "END"){
                    // ENCERRA OPERACAO DO USUARIO E VOLTA PRA TELA DE LOGIN
                    fim = 1;
                    cout << endl << "Fim da sessão" << endl << endl;
                }

            operacao = "z";
        }
    }
return 0;
}
