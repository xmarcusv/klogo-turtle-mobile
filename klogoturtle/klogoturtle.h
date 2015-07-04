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
#include <QTranslator>

#include "desenho.h"

namespace Ui {
class KlogoTurtleApp;
class QAction;
class QMenu;
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
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void changeEvent(QEvent*);

protected slots:
  // this slot is called by the language menu actions
  void slotLanguageChanged(QAction* action);

public slots:
    void slotRun();

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void print();

private:
    Ui::KlogoTurtleApp *ui;
    Desenho *des;

    QString filename;
    QString guarda_to [10] [2];
    QString temp_comando_to;
    QString idioma_escolhido;
    QString m_langPath;

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt

    int id_ingles;
    int id_alemao;
    int id_portugues;
    int id__italiano;
    int id__frances;
    int tem_erro;

    QString i18n(QString str);

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    bool maybeSave();
    void createLanguageMenu(void);
    void loadLanguage(const QString& rLanguage);


    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *languageMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *printAsAct;
};

#endif // KLOGOTURTLE_H
