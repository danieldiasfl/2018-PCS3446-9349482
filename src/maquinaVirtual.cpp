#include "maquinaVirtual.h"

maquinaVirtual::maquinaVirtual()
{

}

maquinaVirtual::~maquinaVirtual()
{
    //dtor
}

//AO INICIAR, CARREGA LOADER "NA MAO"
void maquinaVirtual::carregaLoader()
{cout << "inicio" << endl;
    int p = 0;
    string line;
    vector <string> codigo;
    ifstream file ("loaderAbs.txt");

    if(file.is_open()){
        getline(file,line);
        getline(file,line);
        for(int i = 0; !file.eof(); i++){
            getline(file,line);
            codigo.push_back(line);
        }
    file.close();
    }

    // DIVIDE A PALAVRA EM BYTES E COLOCA OS BYTES NA MEMORIA
    p = 0;
    for(int i = 0; i < codigo.size(); i++){
        if(codigo.at(i).length() == 2){
            string maisSig = codigo.at(i).substr(0,codigo.at(i).length()); // SO TEM 1 BYTE
            memoria[p] = maisSig;
            p++;

        }
        else{
            string maisSig = codigo.at(i).substr(0,codigo.at(i).length()/2); // 2 BYTES
            string menosSig = codigo.at(i).substr(codigo.at(i).length()/2);
            memoria[p] = maisSig;
            p++;
            memoria[p] = menosSig;
            p++;
        }
    }
}

//CARREGA PROGRAMA NA MEMORIA. ESSA FUNCAO PEGA AS ENTRADAS E LE BYTE A BYTE. ESTA FUNCAO COORDENA OS DADOS(ENTRADAS) A SEREM LIDOS. PARA ISSO, SÃO USADOS ALGUNS SINAIS ADICIONAIS
string maquinaVirtual::carrega(vector<string> codigoAbs)
{

    this->codigoAbs = codigoAbs;

 //for(int i = 0; i < 28; i++)
 //       cout << memoria[i] << " ";

    pc = 8; //PROGRAMA LOADER COMECA NO 7o BYTE.
    iCount = 0;
    carregando = 1;

    int p = strtol(codigoAbs.at(0).c_str(), NULL, 16);
    for(int i = 2; i < codigoAbs.size(); i++){
        if(codigoAbs.at(i).length() == 2){
            string maisSig = codigoAbs.at(i).substr(0,codigoAbs.at(i).length()); // SO TEM 1 BYTE
            memoria[p] = maisSig;

            p++;

        }
        else{
            string maisSig = codigoAbs.at(i).substr(0,codigoAbs.at(i).length()/2); // 2 BYTES
            string menosSig = codigoAbs.at(i).substr(codigoAbs.at(i).length()/2);
            memoria[p] = maisSig;

            p++;
            memoria[p] = menosSig;

            p++;
        }
    }
    pc = strtol(codigoAbs.at(0).c_str(), NULL, 16);
    iCount = strtol(codigoAbs.at(1).c_str(), NULL, 16);
    /*if(iCount == 0){
        string maisSig = codigoAbs.at(iCount).substr(0,codigoAbs.at(iCount).length()/2); // 2 BYTES
        string menosSig = codigoAbs.at(iCount).substr(codigoAbs.at(iCount).length()/2);

        memoria[2] = maisSig;
        memoria[3] = menosSig;
    }

    else if (iCount == 1){
        string maisSig = codigoAbs.at(iCount).substr(0,codigoAbs.at(iCount).length()/2); // 2 BYTES
        string menosSig = codigoAbs.at(iCount).substr(codigoAbs.at(iCount).length()/2);
        memoria[4] = maisSig;
        memoria[5] = menosSig;
    }*/
}

void maquinaVirtual::run(string endereco)
{
     while (1){
        string maisSig =  memoria[pc];
        string menosSig = memoria[pc+1];

//            interpreta(maisSig + menosSig);
cout << pc << " " << memoria[300] << " " << memoria[298] << endl;
        if (stop == 1) break;
    }
        return;
}

string maquinaVirtual::interpreta(string instruct, string acumuladorb, int pc, int indireto, int permissao)
{
    stop = 0;
    int a = strtol(acumuladorb.c_str(), NULL, 10);
    int b = pc;
    int e;
    string cc;
    string dd;
    for(int i = 0; i < 3; i++)
        cc[i] = instruct[i+1];

    int c = strtol(cc.c_str(), NULL, 10);
    int d;
    dd[0] = instruct[0];
    d = strtol(dd.c_str(), NULL, 10);

    string operador;

    int operando;
    int operando2;
    string operandoS;
    string operandoS1B;
    string sinalOp;

    string acumuladorS;
    int acumulador2;
    string sinalAc;

    string str;
    operador = instruct[0];
    // PEGA 1 BYTE, CORRESPONDENTE A OPERANDOS DE 1 BYTE
    operandoS1B = instruct.at(1);
    //VAI SEMPRE PEGAR 2 BYTES, MESMO QUE SEJA USADO APENAS 1 DELES (ISSO EH TRATADO EM CADA EVENTO)
    for(int i = 0; i < 3; i++){
        operandoS[i] = instruct[i+1];
        if(i == 0){
            sinalOp = instruct[i+1];
            sinalAc = acumulador[i];
        }
    }

    operando = strtol(operandoS.c_str(), NULL, 16);
    operando2 = complemento2INT(operandoS, sinalOp);

    acumuladorS = acumulador;

    memoria[3] = "02";
    //////////////////////////////////////////
    indireto = 0; pc = 14;memoria[15] = "03"; acumulador = "1100";
    if(operador == "0"){ //JUMP incondicional
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }

        pc = operando;
       // cout << operador << " " << operando << " " << pc <<" " <<acumulador << endl;
    }

    else if(operador == "1"){ //JUMP if zero
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }
            if(strtol(acumulador.c_str(), NULL, 16) == 0)
                pc = operando;
            else
                pc += 2;

        //cout << operador << " " << operando << " " << pc <<" " <<acumulador << endl;
    }
//////////////
    else if(operador == "2"){ //JUMP if negativo
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }

        if( acumulador2 < 0)
            pc = operando;
        else
           pc += 2;
       // cout << operador << " " << operando << " " << pc <<" " <<acumulador << " " << acumulador2 << endl;
    }

    else if(operador == "3"){ //controle
        if(indireto == 1){
            indireto = 0;
        }

        if(operandoS1B == "0") //HM
            stop = 1;
        else if (operandoS1B == "1") //RI
            stop = 0;
        else if(operandoS1B == "2") //IN
            indireto = 1;
        pc += 1;
        memoria[3] = "01";

      //  cout << operador << " " << operandoS1B << " " << indireto << " " << stop << " " << pc << endl;
    }

    else if(operador == "4" || d == 4){ //soma no acumulador
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;

                int m = 0;
        string valorMem = memoria[operando];
        string sinal;
        for(int i = 0; i < valorMem.size(); i++)
            if(i == 0) sinal = valorMem[i];

        int valor1, valor2;
        valor1 = operando;
        valor2 = acumulador2;
        int valorAC = valor2 + valor1;
        acumulador = complemento2STRING(valorAC);
        }

        int valor1, valor2;
        valor1 = operando2;
        valor2 = strtol(acumulador.c_str(),NULL,10);
        int valorAC = valor2 + valor1;

        acumulador = complemento2STRING(valorAC);
        e = a+c;
b+=2;
        pc += 2;

       // cout << "+ " << valorAC <<" " << acumulador << " " << operador << " " << operando << " " << pc <<" " << endl;
    }

    else if(operador == "5"){ //subtrai do acumulador
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }

        int m = 0;
        string valorMem = memoria[operando];
        string sinal;
        for(int i = 0; i < valorMem.size(); i++)
            if(i == 0) sinal = valorMem[i];

        int valor1, valor2;
        valor1 = complemento2INT(valorMem, sinal);
        valor2 = acumulador2;
        int valorAC = valor2 - valor1;
        acumulador = complemento2STRING(valorAC);
e = a - c; b+=2;
        pc += 2;
       //   cout << "- " <<acumulador <<" " << operando  << " " << pc << endl;

}

    else if(operador == "6"){ //multiplica acumulador
        int m = 0;
        string valorMem = memoria[operando];
        string sinal;
        for(int i = 0; i < valorMem.size(); i++)
            if(i == 0) sinal = valorMem[i];

        int valor1, valor2;
        valor1 = operando2;
        valor2 = strtol(acumulador.c_str(), NULL, 10);
        int valorAC = valor2 * valor1;
        acumulador = complemento2STRING(valorAC);
e = a * c; b+=2;
       pc += 2;
      //    cout << valor1 << " " << valor2 << " " << valorAC <<" " << operador << " " << operando << " " << pc <<" " <<acumulador << " " << endl;
   }

    else if(operador == "7"){ //divide acumulador
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }

        int m = 0;
        string valorMem = memoria[operando];
        string sinal;
        for(int i = 0; i < valorMem.size(); i++)
            if(i == 0) sinal = valorMem[i];

        int valor1, valor2;
        valor1 = complemento2INT(valorMem, sinal);
        valor2 = acumulador2;
        int valorAC = valor2 / valor1;
        acumulador = complemento2STRING(valorAC);
e = a / c; b+=2;
        pc += 2;
       //   cout << valor1 << " " << valor2 << " " << valorAC <<" " << operador << " " << operando << " " << pc <<" " <<acumulador << " " << endl;
     }

    else if(operador == "8"){ //carrega da memoria endereço xxx
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
        }

        string maisSig = memoria[operando];      // 1 BYTE CADA
        string menosSig = memoria[operando + 1];

        acumulador = maisSig + menosSig;

        pc += 2;
      //    cout << "MD" <<  " " << acumulador <<" " << operador << " " << operando << " " << pc << endl;

    }

    else if(operador == "9"){ //escreve no endereço xxx
        if(indireto == 1){
            string opIndireto;
            opIndireto = memoria[operando] + memoria[operando + 1];
            operando = strtol(opIndireto.c_str(), NULL, 16);
            indireto = 0;
      //      cout << endl << "INDIRETO " << bloco << "x" << operando << "  "  << memoria[bloco][operando] << " " << memoria[bloco][operando + 1] << endl;
        }

        string menosSig;
        string maisSig;

        if(acumulador.size() == 2){
            memoria[operando +1] = acumulador;
            memoria[operando] = "00";

        }

        else {
        maisSig = acumulador.substr(0,acumulador.length()/2); // 2 BYTES
        menosSig = acumulador.substr(acumulador.length()/2);

        memoria[operando] = maisSig;
        memoria[operando + 1] = menosSig;
        }
   // cout << "MM " <<acumulador <<" " << memoria[operando] << " " << memoria[operando + 1] << " " << pc << endl;
      //  pc += 2;

      //    cout << maisSig << " " << menosSig << " " << acumulador <<" " << operador << " " << operando << " " << pc <<" " << endl;
     //   cout << "MEMWRITE "<< memoria[bloco][operando] << memoria[bloco][operando + 1]<< endl;
    }

    else if(operador == "A"){ //subroutine call - guarda endereço e desvia para xxx
        enderecoSR = pc;
        acumuladorSR = acumulador;

        pc = operando;
    }

    else if(operador == "B"){ //operating system call

    }

    else if(operador == "C"){ //input/output
        if(indireto == 1){
            indireto = 0;
        }

        if (operandoS1B == "0"){ //GD
           string maisSig, menosSig;
                if(codigoAbs.at(iCount).length() == 2)
                    maisSig = codigoAbs.at(iCount).substr(0,codigoAbs.at(iCount).length()/2); // 2 BYTES
                else
                    maisSig = "00";
                menosSig = codigoAbs.at(iCount).substr(codigoAbs.at(iCount).length()/2);

                acumulador = maisSig + menosSig;
            iCount ++;

        }
        else if (operandoS1B == "1"){
                string maisSig, menosSig;
                if(codigoAbs.at(iCount).length() == 2)
                    maisSig = codigoAbs.at(iCount).substr(0,codigoAbs.at(iCount).length()/2); // 2 BYTES
                else
                    maisSig = "00";
                menosSig = codigoAbs.at(iCount).substr(codigoAbs.at(iCount).length()/2);

                saidaDumper.push_back(maisSig + menosSig);
            iCount ++;
        }

        else if (operandoS1B == "2"){
            interrupt = 1;
        }

        else if (operandoS1B == "3")
            interrupt = 0;
        pc += 1;
        memoria[3] = "01";
    }

    else if(operador == "D"){;
    }

    else if(operador == "E"){;
    }

    else if(operador == "F"){;
    }

    if(acumulador.length() == 2) acumulador = "00" + acumulador;
   // cout << "tam " << memoria[0][2] + memoria[0][3] << " pos " <<  memoria[0][4] + memoria[0][5] << " icount " << memoria[0][6] + memoria[0][7] <<
    //" instru " << operador  << operando << " acc " << acumulador << endl;
    string resposta;
    resposta = to_string(b) + " " + to_string(e) + " " + to_string(stop);
     return resposta;
}

int maquinaVirtual::complemento2INT(string num, string signal)
{
    string conv;
    string convF;
    int numero;

    if(num.size() > 2 && (signal == "8" || signal == "9" || signal == "A" || signal == "B" || signal == "C" || signal == "D" || signal == "E" || signal == "F")){
        for(int i = 1; i < num.size(); i++)
            conv[i-1] = num[i];
        if(num.size() == 4){
             convF = "FFFF";
        }

        else if(num.size() == 3){
             convF = "FFF";
        }
        numero = strtol(num.c_str(), NULL, 16) - strtol(convF.c_str(), NULL, 16) - 1;
    }

    else
        numero = strtol(num.c_str(), NULL, 16);

    return numero;
}

string maquinaVirtual::complemento2STRING(int num)
{
    string numS;
    int numero;

    if(num < 0){
        numero = num + 65535 + 1;
        numS = converteString(numero);  // = + FFFF + 1
    }

    else
        numS = converteString(num);

    if(numS.size() == 3 || numS.size() == 1)
        numS = "0" + numS;

    string numS2B;
    for(int i = 0; i < numS.size(); i++)
        numS2B[i] = numS[i];
    return numS;
}

string maquinaVirtual::converteString(int num)
{
    string result;
    std::stringstream ss;
    ss << std::hex << num;
    ss >> result;
    return result;
}
