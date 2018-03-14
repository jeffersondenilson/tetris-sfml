#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "desenho2.h"//este arquivo inclui as funções de desenho na matriz
#include "window.h"//este arquivo inclui as funções de desenho na janela e movimentação
using namespace sf;//para funções do sfml

int main(){
    srand(time(NULL));
    int tipo,rot;
    bool continuar=true,desce,pausa=false;
    char niv[2],obj[2],pts[6];
	int proximo=rand()%7,manter=-1;//"preview" e troca de peças
    int nivel=1,objetivo=nivel*5,pontos=0;//nivel,objetivo e pontuação
    desenhacena();//estabelece a cena inicial, sem peças
    //sprites
	Texture t,go,m,p,pa;
	t.loadFromFile("tiles20.png");
	go.loadFromFile("gameover.png");
	m.loadFromFile("moldura.png");
	p.loadFromFile("preview.png");
	pa.loadFromFile("pausa.png");
	Sprite bloco,gameover,moldura,preview,pause;
	bloco.setTexture(t);//imagem com blocos
	gameover.setTexture(go);gameover.setPosition(2,150);//imagem de game over
	moldura.setTexture(m);//moldura
	preview.setTexture(p);//imagem do proximo e manter
	pause.setTexture(pa);pause.setPosition(30,120);//imagem de pausa
	//fonte e texto
	Font fonte;fonte.loadFromFile("Cantarell-Bold.otf");
	//define propriedades do texto
	Text texto;texto.setFont(fonte);texto.setCharacterSize(13);texto.setColor(Color::Black);
	RectangleShape fantasma(Vector2f(20,20));//peça fantasma
	fantasma.setFillColor(Color(0,0,0,100));
	RenderWindow window(VideoMode(300,400),"TETRIS");//janela
	window.setPosition(Vector2i(500,40));
    Clock cronometro;//inicia tempo
	float regulador=1000;//define tempo de espera
	Time passado,espera=milliseconds(regulador);//pegar tempo passado,espera
	Event event;

    while(window.isOpen()){//main loop, seleciona peças, checa game over e quebra de linha
        cronometro.restart();//para que a pausa no final não afete o tempo passado
        tipo=proximo;
        proximo=rand()%7;
        rot=0;
        //tipo=4;rot=0;//teste peças especificas aqui
        //posição inicial
        int li=1,ci=4,contmanter=0;
        sprintf(niv,"%d",nivel);
        sprintf(obj,"%d",objetivo);
        sprintf(pts,"%d",pontos);
        desce=true;
        while(desce==true){//game loop, processo de movimento
            while(window.pollEvent(event)){//verifica por eventos
                //fecha a janela no X ou no ESC e encerra o programa
                if(event.type==Event::Closed || event.type==Event::KeyPressed && event.key.code==Keyboard::Escape){
                    window.close();return 0;
                }
                if(event.type==Event::KeyPressed){//eventos de teclado
                	if(pausa==false && continuar==true){//nada sera alterado se estiver em pausa ou game over
	                    movimento(event,tipo,rot,li,ci,proximo,manter,contmanter);
	                }
	                if(event.key.code==Keyboard::Return && continuar==true){//pausa
	                	pausa=!pausa;
	                	if(pausa==true){//desenha tela de pausa
	                		window.draw(pause);
	                		window.display();
	                	}	
	                	cronometro.restart();
	                }else if(event.key.code==Keyboard::Space){//reinicia
	                	continuar=true;
		        		tipo=rand()%7,rot=0,li=1,ci=4;//peça
		        		proximo=rand()%7,manter=-1,contmanter=0;//proximo e manter
    					nivel=1,objetivo=nivel*5,pontos=0;//nivel,objetivo e pontuação
    					sprintf(niv,"%d",nivel);sprintf(obj,"%d",objetivo);sprintf(pts,"%d",pontos);
						regulador=1000;espera=milliseconds(regulador);//tempo de espera
						desenhacena();//estabelece a cena inicial, sem peças
	                }
                }// fim de eventos de teclado
            }//fim de eventos
            //move para baixo
            if(pausa==false && continuar==true){//não desenha nem cai se estiver em pausa ou game over
	            passado=cronometro.getElapsedTime();
	            if(passado>espera || Keyboard::isKeyPressed(Keyboard::Down)){
	                //passado o tempo de espera ou pressionando seta para baixo a peça cai
	                if(Keyboard::isKeyPressed(Keyboard::Down)){
	                    pontos=pontos+5;//pontos por derrubar mais cedo, varia com a altura
	                }
	                cronometro.restart();//reinicia o tempo
	                desce=simularmov(tipo,rot,li+1,ci);
	                if(desce==true){
	                    li++;
	                }
	            }//fim de move para baixo
	            //desenho
		        window.clear(Color(38,141,205));
		        window.draw(moldura);
		        //desenha variaveis de texto
				texto.setString(niv);texto.setPosition(260,227);window.draw(texto);
			    texto.setString(obj);texto.setPosition(240,265);window.draw(texto);
			    texto.setString(pts);texto.setPosition(220,300);window.draw(texto);
		        desenharpreview(manter,proximo,window,preview);
		        pecaativa(tipo,rot,li,ci,window,fantasma,bloco);
		        cenario(window,bloco);
		        window.display();//fim de desenho
	        }
        }//fim do game loop
        desenhapeca(tipo,rot,li,ci);
        int lq=quebralinha(li);
        //pontuação e nivelamento
        renivelar(lq,objetivo,nivel,regulador,espera,pontos);
        //se a proxima peça puder existir na posição inicial,retorna verdadeiro
        continuar=simularmov(proximo,0,1,4);
        if(continuar==false){//tela de game over
            window.draw(gameover);
        	window.display();
    	}
        sleep(milliseconds(300));//pausa para ver melhor a diferença entre antes e depois da quebra
    }//fim main loop
}