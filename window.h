using namespace sf;

void movimento(Event event,int &tipo,int &rot,int &li,int &ci,int &proximo,int &manter,int &contmanter){
	//recebe e executa o movimento
	if(event.key.code==Keyboard::Left && simularmov(tipo,rot,li,ci-1)==true){
        ci--;//move para esq
    }
    else if(event.key.code==Keyboard::Right && simularmov(tipo,rot,li,ci+1)==true){
        ci++;//move para dir
    }
    else if(event.key.code==Keyboard::Up && tipo!=3){
        //rotaciona
        simularot(tipo,rot,li,ci);
    }else if(event.key.code==Keyboard::RShift && contmanter==0){//troca peças
	    contmanter++;//troca so uma vez durante o movimento
	    if(manter==-1){//se não tiver peça em manter, é igual ao próximo
	    	manter=proximo;
	        proximo=rand()%7;
	    }//trocar o tipo atual com manter
	    int aux=tipo;
	    tipo=manter;
	    manter=aux;
	    li=1;ci=4;rot=0;
    }
}

void desenharpreview(int manter,int proximo,RenderWindow &window,Sprite preview){
	//desenha preview e manter
    preview.setTextureRect(IntRect(proximo*80,0,80,80));
    preview.setPosition(210,30);
    window.draw(preview);//desenhar proximo
    if(manter!=-1){
        preview.setTextureRect(IntRect(manter*80,0,80,80));
        preview.setPosition(210,140);
        window.draw(preview);//desenhar manter
    }
}

void pecaativa(int tipo,int rot,int li,int ci,RenderWindow &window,RectangleShape fantasma,Sprite bloco){
	//desenha peça ativa e fantasma
	int contbloco=0,lif=posfantasma(tipo,rot,li,ci);
    for(int i=li,l=0;contbloco!=4;i++,lif++,l++){
        for(int j=ci,c=0;j<ci+4 && contbloco!=4;j++,c++){
            if(pecas[tipo][rot][l][c]!=0){
            	contbloco++;
            	fantasma.setPosition((j-1)*20,(lif-3)*20);
            	window.draw(fantasma);//desenha fantasma
            	bloco.setTextureRect(IntRect((tipo+1)*20,0,20,20));
            	bloco.setPosition((j-1)*20,(i-3)*20);
            	window.draw(bloco);//desenha peça
            }
        }
    }
}

void cenario(RenderWindow &window,Sprite bloco){//desenha peças paradas
	for(int i=22;i>=3;i--){
	    bool lvazia=true;
	    for(int j=10;j>=1;j--){
	        if(cena[i][j]!=0){
	            lvazia=false;
	            bloco.setTextureRect(IntRect(cena[i][j]*20,0,20,20));
	            bloco.setPosition((j-1)*20,(i-3)*20);
	            window.draw(bloco);
	        }
	    }
	    if(lvazia==true){
	        break;//procura só até onde tem peças
		}
	}
}