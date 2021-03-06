/***************************************************************************
                          desenho.cpp  -  description
                             -------------------
    begin                : Sáb Nov 1 2003
    copyright            : (C) 2003 by Euclides Lourenco Chuma
    email                : euclideschuma@yahoo.com.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                           *
 ***************************************************************************/


#include "desenho.h"
#include <qpainter.h>
#include <qwidget.h>
#include <qframe.h>
#include <cmath>
#include <cstdlib>
#include <qcolor.h>

Desenho::Desenho(QWidget *parent, int width, int height) : QFrame(parent), buffer(height,width), buffer_st(height,width)
{                                                      
    QColor myWhite = QColor(Qt::white);
    setBackgroundColor(Qt::white);
    buffer.fill(myWhite);
    buffer_st.fill(myWhite);
    repaint();
    tamanho_h = height;
    tamanho_v = width;
    x_centro = tamanho_v/2;
    y_centro = tamanho_h/2;
    show_tart = 1;
    x_atual = x_centro;
    y_atual = y_centro;
    angulo = PI/2;
    pincel = 1;
    pen_cor = "blue";
    pen_espessura = 1;
    n_var = 0;
    apaga_variaveis();
    tartaruga(0);
}

Desenho::~Desenho()
{

}

void Desenho::fd(int dist)
{
    buffer = buffer_st;

    QPainter paint;
    paint.begin (&buffer);
    QPen pen;
    pen.setColor(pen_cor);
    pen.setWidth(pen_espessura);

    //inicializa pen
    if(pincel == 1)
    {
        pen.setStyle(Qt::SolidLine);
    }
    else
    {
        pen.setStyle(Qt::NoPen);
    }

    paint.setPen(pen);

    //calcula posicao de destino
    x_novo = static_cast <int> (x_atual + static_cast <int> (dist * cos(angulo)));
    y_novo = static_cast <int> (y_atual - static_cast <int> (dist * sin(angulo)));

    paint.drawLine(x_atual, y_atual, x_novo, y_novo);

    //muda posicao de origem
    x_atual = x_novo;
    y_atual = y_novo;

    paint.end();

    repaint();

    buffer_st = buffer;

}

void Desenho::bk(int dist)
{
    buffer = buffer_st;

    QPainter paint;
    paint.begin (&buffer);
    QPen pen;
    pen.setColor(pen_cor);
    pen.setWidth(pen_espessura);

    //inicializa pen
    if(pincel == 1)
    {
        pen.setStyle(Qt::SolidLine);
    }
    else
    {
        pen.setStyle(Qt::NoPen);
    }

    paint.setPen(pen);

    //calcula posicao de destino
    x_novo = static_cast <int> (x_atual - static_cast <int> (dist * cos(angulo)));
    y_novo = static_cast <int> (y_atual + static_cast <int> (dist * sin(angulo)));

    // desenha linha
    paint.drawLine(x_atual, y_atual, x_novo, y_novo);

    //muda posicao de origem
    x_atual = x_novo;
    y_atual = y_novo;

    paint.end();

    repaint();

    buffer_st = buffer;

}

void Desenho::tartaruga(int tem_erro)
{
    if(tem_erro == 0)
    {
       buffer_st = buffer;
    }

    if (show_tart == 1)
    {
        int x_ponta_novo;
        int y_ponta_novo;
        int x_corpo_novo1;
        int y_corpo_novo1;
        int x_corpo_novo2;
        int y_corpo_novo2;

        QPainter paint;
        paint.begin (&buffer);
        QPen pen;
        pen.setColor(Qt::lightGray);
        pen.setWidth(pen_espessura);
        pen.setStyle(Qt::SolidLine);
        paint.setPen(pen);

        //desenha ponta
        x_ponta_novo = static_cast <int> (x_atual + static_cast <int> (10 * cos(angulo)));
        y_ponta_novo = static_cast <int> (y_atual - static_cast <int> (10 * sin(angulo)));
        paint.drawLine(x_atual, y_atual, x_ponta_novo, y_ponta_novo);

        //desenha perpendicular
        x_corpo_novo1 = static_cast <int> (x_atual + static_cast <int> (10 * cos(angulo + (PI/2))));
        y_corpo_novo1 = static_cast <int> (y_atual - static_cast <int> (10 * sin(angulo + (PI/2))));
        paint.drawLine(x_atual, y_atual, x_corpo_novo1, y_corpo_novo1);

        x_corpo_novo2 = static_cast <int> (x_atual + static_cast <int> (10 * cos(angulo - (PI/2))));
        y_corpo_novo2 = static_cast <int> (y_atual - static_cast <int> (10 * sin(angulo - (PI/2))));
        paint.drawLine(x_atual, y_atual, x_corpo_novo2, y_corpo_novo2);

        paint.end();
    }
    repaint();
}

void Desenho::lt(int ang)
{
    buffer = buffer_st;
    angulo = angulo + (PI*(static_cast <double> (ang)/180));
}


void Desenho::rt(int ang)
{
    buffer = buffer_st;
    angulo = angulo - (PI*(static_cast <double> (ang)/180));
}

void Desenho::sx(int pos_x)
{
    buffer = buffer_st;
    x_atual = pos_x + x_centro;
}


void Desenho::sy(int pos_y)
{
    buffer = buffer_st;
    y_atual = abs(pos_y - y_centro);
}


void Desenho::hm()
{
    buffer = buffer_st;
    angulo = PI/2;
    x_atual = x_centro;
    y_atual = y_centro;
}


void Desenho::ht()
{
    bitBlt(0, 0, buffer_st, tamanho_h, tamanho_v);
    show_tart = 0;
}

void Desenho::st()
{
    show_tart = 1;
    tartaruga(0);
}


void Desenho::cs()
{
    setBackgroundColor(Qt::white);
    buffer.fill(Qt::white);
    buffer_st.fill(Qt::white);
    repaint();
    tartaruga(0);
}

void Desenho::cnew()
{
    setBackgroundColor(Qt::white);
    buffer.fill(Qt::white);
    buffer_st.fill(Qt::white);
    repaint();
    x_atual = x_centro;
    y_atual = y_centro;
    angulo = PI/2;
    pincel = 1;
    pen_cor = "blue";
    pen_espessura = 1;
    show_tart = 1;
    n_var = 0;
    apaga_variaveis();
    tartaruga(0);
}

/**
 * Função que salva o desenho em imagem, adaptação para QT5
 * @brief Desenho::salva_imagem
 * @param filename
 * @param qualidade
 */
void Desenho::salva_imagem(QString filename,int qualidade)
{
    buffer.save(filename.append(".png"),0,qualidade);
}

void Desenho::make(QString var_nome, QString var_int)
{
    int pos_var_temp;

    pos_var_temp = find_var(var_nome);

    if(pos_var_temp == -1)
    {
        variaveis [n_var] [0] = var_nome;
        variaveis [n_var] [1] = var_int;
        n_var = n_var + 1;
    }
    else
    {
        variaveis [pos_var_temp] [0] = var_nome;
        variaveis [pos_var_temp] [1] = var_int;
    }
}

void Desenho::pu()
{
    pincel = 0;
}


void Desenho::pd()
{
    pincel = 1;
}


void Desenho::pc(QString p_cor)
{
    buffer = buffer_st;
    pen_cor = p_cor;
}

int Desenho::find_var(QString var_name)
{
    int volta = -1;

    for(int l = 0; l <= n_var; l++)
    {
        if(variaveis [l] [0] == var_name)
        {
            volta = l;
            break;
        }

    }
    return volta;
}


QString Desenho::show_var(QString var_nam)
{
    int temp_pos_var;
    QString retorno;

    retorno = "Variable not find";
    temp_pos_var = find_var(var_nam);

    if(temp_pos_var != -1)
    {
        retorno = variaveis [temp_pos_var] [1];
    }

    return retorno;
}

void Desenho::apaga_variaveis()
{
    for(int h = 0; h < 100; h++)
    {
        variaveis[h][0] = "0";
        variaveis[h][1] = "0";
    }
}

void Desenho::paintEvent(QPaintEvent *)
{
    bitBlt(0,0,buffer,tamanho_h,tamanho_v);
}

/**
 * Realiza o desenho no frame - adaptação para QT5
 * @brief Desenho::bitBlt
 * @param x
 * @param y
 * @param src
 * @param w
 * @param h
 */
void Desenho::bitBlt(int x, int y, const QPixmap& src, int w, int h){
    QPainter p(this);
    p.drawPixmap(x,y,w, h,src);
}

/**
 * Seta o background do frame - adaptação para QT5
 * @brief Desenho::setBackgroundColor
 * @param color
 */
void Desenho::setBackgroundColor(Qt::GlobalColor color){
    QPalette palette;
    palette.setColor(QPalette::Background,color);
    setAutoFillBackground(true);
    setPalette(palette);
}
