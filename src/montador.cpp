#include "montador.h"

montador::montador()
{
    //ctor
    this->ci = 0;
    erro = 0;
    instru = 0;
};

montador::~montador()
{
    //dtor
};

//LE LINHAS E MANDA PARA INTERPRETADORES. AO FIM, RETORNA CODIGO ABSOLUTO
vector<string> montador::leCodigo(string texto)
{
    //ABRE ARQUIVO
    string line;
    vector<string> absoluto;
    ifstream file (texto);
    if(file.is_open()){
        while(! file.eof()){
            getline(file,line);
            codigo.push_back(line);
        }
        file.close();
    }
    else {
        cout << texto <<" Invalido" << endl;
        return absoluto;
    }

    //PASSO 1
    ci = 0;
    instru = 0;
    passo = 1;
    for(int i = 0; i < codigo.size(); i++){
        this->geraAbsoluto(codigo[i]);
    }

    //PASSO 2
    ci = 0;
    instru = 0;
    passo = 2;
    for(int i = 0; i < codigo.size(); i++){
        this->geraAbsoluto(codigo[i]);
    }

    //VE SE TABELA ESTA COMPLETA
    for(int i = 0; i < simboloValor.size(); i++){
        if(simboloValor[i] == "*" && ( simboloNome[i] != "EQU" ) && simboloNome[i] != "#" && simboloNome[i] != "GD"&& simboloNome[i] != "PD"&& simboloNome[i] != "EI"
           && simboloNome[i] != "DI" && simboloNome[i] != "RI" && simboloNome[i] != "IN" && simboloNome[i] != "HM"){
              cout <<  endl << "erro: ha simbolos nao declarados -> " << simboloNome[i]  << endl;
              erro = 1;
           }
    }


    //CRIA CODIGO NO FORMATO DESEJADO: INICIO, NUMERO INSTRUCOES, CODIGO
    string ciS = converteString(ci);
    while(inicio.size() < 4)
        inicio = "0" + inicio;
    while(ciS.size() < 4)
        ciS = "0" + ciS;

    absoluto.push_back(inicio);
    absoluto.push_back(ciS);

    for(int i = 0; i < codigoAbsoluto.size(); i++){
        absoluto.push_back(codigoAbsoluto[i]);
    }

    string output;
    string c;
    output = texto.at(0);
    for(int i = 1; c != "."; i++){
        c = texto.at(i);
        if(c != ".")
            output = output + texto.at(i);
    }


    //GUARDA EM TXT
    ofstream out (output + "Abs.txt");
    ostream_iterator<string> out_i(out, "\n");
    copy(absoluto.begin(), absoluto.end(), out_i);

    return absoluto;
};

//CHECA SE HA ROTULOS. INSTRUCAO DE 3 PARTES => ROTULO EM 1 DELAS (OU EQU). IDENTIFICA TIPO DE INSTRUCAO E ENVIA PARA PROXIMO PASSO
void montador::geraAbsoluto(string instrucao)
{
    string tok1,tok2,tok3;
    string operador, operando;
    string simb[2];
    bool sfound = 0;
    string sum_operation = "+";
    string sub_operation = "-";
    bool operation;
    bool simbol;

    //SEPARA OPERADOR DE OPERANDO
    vector <string> tokens;
    stringstream check1(instrucao);
    string intermediate;
    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }
    for(int i = 0; i < tokens.size(); i++){
        if (i == 0) tok1 = tokens[i];
        if (i == 1) tok2 = tokens[i];
        if (i == 2) tok3 = tokens[i];
    }

    //VERIRICA SE HA ROTULO
    if (tokens.size() == 2){
        // NAO TEM ROTULO
        operador = tok1;
        operando = tok2;
    }

    else if (tokens.size() == 3){
        // HA 2 POSSIBILIDADES PARA 3 OPERANDO: ROTULO OU EQU
        if(tok2 == "EQU"){
            //TEM 'VARIAVEL'(EX K EQU 3 => K = 3)
            operador = tok2;
            operando = tok3;

            for(int i = 0; i < simboloNome.size(); i++){
                if(simboloNome[i] == tok1){
                    sfound = 1;
                    if(simboloValor[i] == "*"){
                        simboloValor[i] = tok3;
                    }
                }
            }

            if(sfound == 0){
                simboloNome.push_back(tok1);
                simboloValor.push_back(tok3);
            }

        }
        else{
            // TEM ROTULO
            operador = tok2;
            operando = tok3;
            // COLOCA ROTULO NA TABELA DE SIMBOLOS JUNTO COM LINHA DE INSTRUCAO
            //  CONVERTE CI PARA HEX STRING
            int c = instru + inicioI;
            string result = converteString(c);
            simb[0] = tok1;
            simb[1] = result;

            for(int i = 0; i < simboloNome.size(); i++){
                if(simboloNome[i] == simb[0]){
                    sfound = 1;
                    if(simboloValor[i] == "*"){
                        simboloValor[i] = simb[1];
                    }
                }
            }

            if(sfound == 0){
                simboloNome.push_back(simb[0]);
                simboloValor.push_back(simb[1]);
            }
        }
    }

    else if(instrucao == "#") {
        operando = "#";
    }

    else{
            cout << endl << "erro: formato de instrucao invalido -> "  << instrucao << endl;
            erro = 1;
    }


    //PROCURA MNEMONICO
    analisaLinha(operador, operando);
}

//TRADUZ O CODIGO "ASSEMBLY" PARA LINGUAEM DE MAQUINA NO PASSO 2(SEM PENDENCIAS). ENVIA OPERANDOS PARA ANALISAR CONTEUDO
void montador::analisaLinha(string operador, string operando)
{
    string operandoAbs;
    string instrucaoAbs;
     //TABELA DE TRANSFORMACAO

        if(operador == "0" || operador == "JP"){ //JUMP incondicional
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            //NO PASSO 2, MONTA INSTRUCAO
            if(passo == 2){

//                cout << operandoAbs << endl;
                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;

                instrucaoAbs = "0" + operandoAbs; //0xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }

        }

        else if(operador == "1" || operador == "JZ"){ //JUMP if zero
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){
                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "1" + operandoAbs; //1xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "2" || operador == "JN"){ //JUMP if negativo
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){
                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "2" + operandoAbs; //2xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "3" || operador == "CN"){ //controle
            ci++;
            instru += 1;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){
                //while(operandoAbs.length() < 3)
                //    operandoAbs = "0" + operandoAbs;
                if(operando == "0" || operando == "HM")
                    operandoAbs = "0";
                else if (operando == "1" || operando == "RI")
                    operandoAbs = "1";
                else if(operando == "2" || operando == "IN")
                    operandoAbs = "2";
                else //erro
                    ;
                instrucaoAbs = "3" + operandoAbs; //3x
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "4" || operador == "+"){ //soma no acumulador
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;

                instrucaoAbs = "4" + operandoAbs; //4xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "5" || operador == "-"){ //subtrai do acumulador
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "5" + operandoAbs; //5xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "6" || operador == "*"){ //multiplica acumulador
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "6" + operandoAbs; //6xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "7" || operador == "/"){ //divide acumulador
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "7" + operandoAbs; //7xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "8" || operador == "MD"){ //carrega da memoria no endereço xxx
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "8" + operandoAbs; //8xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "9" || operador == "MM"){ //escreve no endereço xxx
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "9" + operandoAbs; //9xxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "A" || operador == "SC"){ //subroutine call - guarda endereço e desvia para xxx
            ci++;
            instru += 2;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

                while(operandoAbs.length() < 3)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "A" + operandoAbs; //Axxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "B" || operador == "OS"){ //operating system call
            ci++;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){
                while(operandoAbs.length() < 1)
                    operandoAbs = "0" + operandoAbs;
                instrucaoAbs = "B" + operandoAbs; //Axxx
                codigoAbsoluto.push_back(instrucaoAbs);
            }

        }

        else if(operador == "C" || operador == "IO"){ //input/output
            ci++;
            instru += 1;
            operandoAbs = analisaOperando(operando);
            if(passo == 2){

            //    while(operandoAbs.length() < 3)
            //        operandoAbs = "0" + operandoAbs;
                if (operando == "0" || operando == "GD")
                    operandoAbs = "0";
                if (operando == "1" || operando == "PD")
                    operandoAbs = "1";
                if (operando == "2" || operando == "EI")
                    operandoAbs = "2";
                if (operando == "3" || operando == "DI")
                    operandoAbs = "3";
                instrucaoAbs = "C" + operandoAbs; //Cx
                codigoAbsoluto.push_back(instrucaoAbs);
            }
        }

        else if(operador == "D"){
        }

        else if(operador == "E" ){
        }

        else if(operador == "F" ){
        }

        // PSEUDOINSTRUCOES
        else if(operador == "@"){ //origin
            operandoAbs = analisaOperando(operando);
            inicio = operandoAbs;
            int k = strtol(operandoAbs.c_str(), NULL , 16);
            inicioI = k;
        }

        else if(operador == "#"){//end
           // operandoAbs = analisaOperando(operando);

        }

        else if(operador == "EQU"){//array - define area de trabalho com tamanho xxx
            operandoAbs = analisaOperando(operando);

        }

        else if(operador == "DW"){//constante 2 bytes
            //ci++;
            instru += 2;
            string limita = "0000"; // LIMITA A 2 BYTES (MENOS SIGNIFICATIVO)
            operandoAbs = analisaOperando(operando);

            if(passo == 2){
                while(operandoAbs.length() < 4)
                        operandoAbs = "0" + operandoAbs;

                limita[0] = operandoAbs[0];
                limita[1] = operandoAbs[1];
                limita[2] = operandoAbs[2];
                limita[3] = operandoAbs[3];

                codigoAbsoluto.push_back(limita);
            }
        }
        else{
            if (operador != "#")
            cout <<  endl << "erro: operacao invalida -> " << operador  << operando << endl;
        }
}

//PROCURA SOMA OU SUBTRACAO NO OPERANDO
string montador::analisaOperando(string operando)
{
    string operandoAbs;
    int simbo;

    //VERIFICA SE HA SOMA OU SUBTRACAO NO OPERANDO
    size_t found1 = operando.find("+");
    size_t found2 = operando.find("-");

    //HA SOMA. VE SE HA SIMBOLOS NA ROTINA analisaOperacao
    if(found1 != std::string::npos)
        operandoAbs = analisaOperacao('+', operando);

    //HA SUBTRACAO. VE SE HA SIMBOLOS NA ROTINA analisaOperacao
    else if(found2 != std::string::npos)
        operandoAbs = analisaOperacao('-', operando);

    //NAO ACHOU SOMA NEM SUBTRACAO. VE SE HA SIMBOLOS
    else{
        simbo = analisaSimbolo(operando);

        if (simbo == -2){
            operandoAbs = operando; // NAO PODE OCORRER NO PASSO 2
        }

        else if (simbo == -1){     //NAO HA SIMBOLOS (SO NUMEROS)
            operandoAbs = operando;
        }

        else
            operandoAbs = simboloValor[simbo];
        }

    return operandoAbs;
}

string montador::analisaOperacao(char operac, string operando)
{
    string op1;
    string op2;
    vector <string> tokensOP;
    int simbo1 = -1;  // 0: nao ha simbolo no operando1
    int simbo2 = -1;
    int num1;
    int num2;

    //SEPARACAO(COLOCA EM TOKENS)
    stringstream check1(operando);
    string intermediate;
    while(getline(check1, intermediate, operac))
        {
            tokensOP.push_back(intermediate);
        }

    //COLOCA EM OP1 e OP2
    for(int i = 0; i < tokensOP.size(); i++){
        if (i == 0) op1 = tokensOP[i];
        if (i == 1) op2 = tokensOP[i];
    }

    //VERIFICA SE HA SIMBOLOS
    simbo1 = analisaSimbolo(op1);
    simbo2 = analisaSimbolo(op2);

    //SE NENHUM DOS DOIS EH SIMBOLO, FAZ A CONTA
    if(simbo1 == -1 && simbo2 == -1){
        num1 = strtol(op1.c_str(), NULL, 16);
        num2 = strtol(op2.c_str(), NULL, 16);
        num1 += num2;

        //string result;
        //std::stringstream ss;
        //ss << std::hex <<num1;
        //ss >> result;
        string result =  converteString(num1);
        return result;
    }

    //UM DOS DOIS EH SIMBOLO, MAS UM DELES NAO ESTA NA TABELA
    else if (simbo1 == -2 || simbo2 == -2)
        return operando;//nao ha nada a fazer

    //HA SIMBOLOS E TODOS ESTAO NA TABELA
    else{
        if (simbo1 != -1)
            op1 = simboloValor[simbo1];
        if (simbo2 != -1)
            op2 = simboloValor[simbo2];

        num1 = strtol(op1.c_str(), NULL, 16);
        num2 = strtol(op2.c_str(), NULL, 16);
        num1 += num2;

        string result =  converteString(num1);
        return result;
    }
}

//PROCURA SIMBOLOS NO OPERANDO
int montador::analisaSimbolo(string operando)
{
    int sfound = -2;
    string simb[2];
    string simbTab[2];

    if(numero(operando)){ //EH NUMERO (SEM SIMBOLO)
        return -1;
    }
    else{ // EH SIMBOLO
        //PROCURA SIMBOLO NA TABELA
        for(int i = 0; i < simboloNome.size(); i++){
            if(simboloNome[i] == operando)
                sfound = i;
        }

        //SE ACHOU, RETORNA NUMERO DA LINHA DA TABELA
        if(sfound != -2){
            return sfound;
            }

        //SENAO, CRIA NOVO SIMBOLO DE VALOR NULO REPRESENTADO POR *
        else{
            simb[0] = operando;
            simb[1] = "*";
            simboloNome.push_back(simb[0]);
            simboloValor.push_back(simb[1]);
            return -2; //ha simboo mas fora da tabela
        }
    }
}

//CHECA SE E NUMERO. SE 1, NUMERO. SENAO, SIMBOLO
bool montador::numero(string operando)
{
    for (char d : operando) {
        if (!isxdigit(d))
            return 0;
    }
    return 1;
}

string montador::converteString(int num)
{
    string result;
    std::stringstream ss;
    ss << std::hex << num;
    ss >> result;
    return result;
}
