#ifndef KLOGOTURTLE_H
#define KLOGOTURTLE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTextDocument>
#include <QPushButton>
#include <QAction>
#include <QLabel>
#include <QStringList>
#include <QComboBox>

#include "desenho.h"

namespace Ui {
class KlogoTurtleApp;
}

class KlogoTurtleApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit KlogoTurtleApp(QWidget *parent = 0);
    ~KlogoTurtleApp();

    /** returns a pointer to the current document connected to the KTMainWindow instance and is used by
     * the View class to access the document object's methods
     */
    QAction *saveImage;
    QPushButton *runButton;
    QTextEdit *ComandoTextEdit;
    QTextEdit *MensagemTextEdit;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QComboBox *saidaComboBox;

    int Repete(int param, int par_ini, QStringList lines);
    int Exe_comando(int pos_comando, QStringList lines);
    int pos_end_repete;
    int find_to(QString to_name);
    int n_to;
    int qual_comando;

    void comando_to(QString comando, QString nome, QStringList lines);
    void stop_error();
    void apaga_to();


protected:
    /** creates the centerwidget of the KTMainWindow instance and sets it as the view
     */
    void initView();

public slots:
    void slotRun();

private:
    Ui::KlogoTurtleApp *ui;
    Desenho *des;

    QString filename;
    QString guarda_to [10] [2];
    QString temp_comando_to;
    QString idioma_escolhido;

    int id_ingles;
    int id_alemao;
    int id_portugues;
    int id__italiano;
    int id__frances;
    int tem_erro;

    QString i18n(QString str);
};

#endif // KLOGOTURTLE_H
