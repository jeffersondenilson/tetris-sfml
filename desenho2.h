#include "pecas2.h"//este arquivo inclui as peças
using namespace sf;

int cena[24][12];

void desenhacena(){//define a cena inicial
    for(int i=0;i<24;i++){
        cena[i][0]=8;//preenche primeira col
        cena[i][11]=8;//preenche ult col
    }
    for(int j=1;j<11;j++){
        cena[23][j]=8;//preenche ult lin
    }
    for(int i=0;i<23;i++){//o resto é espaço livre
        for(int j=1;j<11;j++){
            cena[i][j]=0;
        }
    }
}
void desenhapeca(int tipo, int rot, int li, int ci){
    //escreve peça na cena em um intervalo baseada na posição das peças de pecas.h
    int contbloco=0;
    for(int i=li,l=0;contbloco!=4;i++,l++){
        for(int j=ci,c=0;j<ci+4 && contbloco!=4;j++,c++){
            if(pecas[tipo][rot][l][c]!=0){
                cena[i][j]=pecas[tipo][rot][l][c];
                contbloco++;//para apos 4 blocos serem desenhados
            }
        }
    }
}
bool simularmov(int tipo, int rot, int li, int ci){
    //retorna verdadeiro se o movimento é possivel
    for(int i=li,l=0;i<li+4;i++,l++){
        for(int j=ci,c=0;j<ci+4;j++,c++){
            if(cena[i][j]!=0 && pecas[tipo][rot][l][c]!=0){
                return false;//se o espaço não estaria livre, não é possível
            }
        }
    }
    return true;
}
void simularot(int tipo, int &rot, int &li, int &ci){
    //verifica se a rotação é possível, se não tenta a rotação com deslocamento
    int proxrot;
    if(rot==3){
        proxrot=0;
    }else{
        proxrot=rot+1;
    }
    if(simularmov(tipo,proxrot,li,ci)==true){
        //rotação possivel
        rot=proxrot;
    }else if(simularmov(tipo,proxrot,li,ci+1)==true){
        //a direita
        rot=proxrot;ci++;
    }else if(simularmov(tipo,proxrot,li,ci-1)==true){
        //posivel a esquerda
        rot=proxrot;ci--;
    }else if(simularmov(tipo,proxrot,li-1,ci)==true){
        //acima
        rot=proxrot;li--;
    }else if(simularmov(tipo,proxrot,li+1,ci)==true){
        //abaixo
        rot=proxrot;li++;
    }else if(tipo==4 && simularmov(tipo,proxrot,li,ci-2)==true){
        //esquerda para peça I
        rot=proxrot;ci=ci-2;
    }else if(tipo==4 && simularmov(tipo,proxrot,li,ci+2)==true){
        //direita para peça I
        rot=proxrot;ci=ci+2;
    }else if(tipo==4 && simularmov(tipo,proxrot,li+2,ci)==true){
        //abaixo para I
        rot=proxrot;li=li+2;
    }
}
int posfantasma(int tipo,int rot,int li,int ci){
    //retorna linha inicial mais baixa que a peça pode existir
    while(simularmov(tipo,rot,li+1,ci)==true){
        li++;
    }
    return li;
}
void baixarlinha(int l){
    //traz todas as linhas para baixo ou até a pŕoxima linha vazia
    for(int i=l-1;i>=0;i--){
        bool lvazia=true;
        for(int j=1;j<11;j++){
            if(cena[i][j]!=0){
                lvazia=false;//se tem pelo menos 1 elemento, não está vazia
                cena[i+1][j]=cena[i][j];
                cena[i][j]=0;
            }
        }
        if(lvazia==true){//para na próxima linha vazia
            break;
        }
    }
}
void apagar(int i){
    for(int j=1;j<11;j++){
        cena[i][j]=0;//apaga todos os elementos na linha
    }
}
int quebralinha(int li){
    //verifica por linhas completas e retorna quantas apagadas
    int lq=0;//linhas quebradas
    for(int i=li;i<li+4 && i<23;i++){
        bool lcompleta=true;
        for(int j=1;j<11;j++){
            if(cena[i][j]==0){//se pelo menos um espaço estiver livre, a linha não está completa
                lcompleta=false;
            }
        }
        if(lcompleta==true){//a linha está completa
            apagar(i);
            baixarlinha(i);
            lq++;
        }
    }
    return lq;
}
void renivelar(int lq,int &objetivo,int &nivel,float &regulador,Time &espera,int &pontos){
    //redefinir objetivo, nivel, tempo e pontos
    objetivo=objetivo-lq;
    if(objetivo<=0){
       nivel++;
       objetivo=(nivel*5)+objetivo;
       if(regulador>40){
            regulador=regulador/1.4;
            espera=milliseconds(regulador);
        }
    }
        pontos=pontos+(lq*lq*100);
}