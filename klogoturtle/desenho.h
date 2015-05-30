/***************************************************************************
                          desenho.h  -  description
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
 *                                                                         *
 ***************************************************************************/

#ifndef DESENHO_H
#define DESENHO_H

#define PI 3.14159265358979323846

#include <qpainter.h>
#include <qpen.h>
#include <qcolor.h>
#include <qbrush.h>
#include <qframe.h>
#include <qpixmap.h>
#include <stdlib.h>

class Desenho : public QFrame
{
    Q_OBJECT
public:
    explicit Desenho(QWidget *parent = 0,int width = 590, int height = 400);
    ~Desenho();
    void fd(int dist);
    void bk(int dist);
    void lt(int ang);
    void rt(int ang);
    void sx(int pos_x);
    void sy(int pos_yp);
    void pc(QString p_cor);
    void hm();
    void cs();
    void cnew();
    void salva_imagem(QString filename, int qualidade);
    void pu();
    void pd();
    void ht();
    void st();
    void make(QString var_nome, QString var_int);
    void to();
    void tartaruga(int tem_erro);
    int find_var(QString var_name);
    QString show_var(QString var_nam);
    void setBackgroundColor(Qt::GlobalColor color);

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *);
    QPixmap buffer;
    QPixmap buffer_st;
    void bitBlt(int x, int y, const QPixmap& src, int w, int h);

private:
    int x_atual;
    int y_atual;
    int x_novo;
    int y_novo;
    int x_centro;
    int y_centro;
    int pincel;
    int show_tart;
    int tamanho_v;
    int tamanho_h;
    int pen_espessura;
    int n_var;
    double angulo;
    void apaga_variaveis();
    QString pen_cor;
    QString variaveis [100] [2];
};

#endif // DESENHO_H
