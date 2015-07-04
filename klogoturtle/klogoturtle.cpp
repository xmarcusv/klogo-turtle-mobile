#include "klogoturtle.h"
#include "ui_klogoturtle.h"
#include "stdio.h"
#include "sys/ioctl.h"
#include "unistd.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QCloseEvent>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QToolBar>
#include <QLocale>
#include <QStatusBar>
#include <QMenuBar>

//That's for the StatusBar
//If you define more, it will allow you to have several items in the StatusBar
const int ID_STATUS_MSG     = 1;

KlogoTurtleApp::KlogoTurtleApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KlogoTurtleApp)
{

    qApp->installTranslator(&m_translator);
    qApp->installTranslator(&m_translatorQt);
    ui->setupUi(this);

    idioma_escolhido = "pt_BR";

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initView();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    apaga_to();
    n_to = 0;
    qual_comando = 1;

    //setCentralWidget(des);
}

void KlogoTurtleApp::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        //writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void KlogoTurtleApp::changeEvent(QEvent* event)
{
    if(0 != event) {
        switch(event->type()) {
        // this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;

            // this event is send, if the system, language changes
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            loadLanguage(locale);
        }
            break;
        }
    }
    QMainWindow::changeEvent(event);
}

KlogoTurtleApp::~KlogoTurtleApp()
{
    delete ui;
}

void KlogoTurtleApp::initView()
{
    ////////////////////////////////////////////////////////////////////
    // create the main widget here that is managed by KTMainWindow's view-region and
    // connect the widget to your document to display document contents.
    des = new Desenho(this,590,400);
    des->move(5,60);
    des->setFixedSize(590,400);
    des->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    des->show();

    label1 = new QLabel(this);
    label1->setText(tr("Commands:"));
    label1->setGeometry(610,60,120,20);

    label2 = new QLabel(this);
    label2->setText(tr("Messages:"));
    label2->setGeometry(610,460,120,20);

    ComandoTextEdit = new QTextEdit(this);
    ComandoTextEdit->setGeometry(610,80,180,315);
    ComandoTextEdit->setWordWrapMode(QTextOption::NoWrap);

    MensagemTextEdit = new QTextEdit(this);
    MensagemTextEdit->setGeometry(610,480,180,75);
    MensagemTextEdit->setReadOnly(true);

    runButton  = new QPushButton(this);
    runButton->setText("&Run");
    runButton->setGeometry(710,410,80,30);

    saidaComboBox = new QComboBox(this);
    saidaComboBox->setGeometry(610,410,100,30);
    saidaComboBox->insertItem(0,"Tela");
    saidaComboBox->insertItem(1,"Script");
    saidaComboBox->insertItem(2,"Todos");

    QObject::connect(runButton, SIGNAL(clicked()), this, SLOT(slotRun()));
}

void KlogoTurtleApp::newFile()
{
    if (maybeSave()) {
        statusBar()->showMessage(tr("Creating new document..."), ID_STATUS_MSG);
        ComandoTextEdit->setText("");
        filename = "";
        des->cnew();
        statusBar()->showMessage(tr("Ready."), ID_STATUS_MSG);
    }
}

void KlogoTurtleApp::open()
{
    if (maybeSave()) {
         statusBar()->showMessage(tr("Opening file..."), ID_STATUS_MSG);

        QString fn = QFileDialog::getOpenFileName(this);
        if (!fn.isEmpty())
        {
            QFile f(fn);
            if (!f.open(QFile::ReadOnly))
                return;
            QTextStream ts(&f);
            ComandoTextEdit->setText(ts.readAll());
             statusBar()->showMessage(tr("Loaded document."), ID_STATUS_MSG);
            filename = fn;
        }
        else
             statusBar()->showMessage(tr("Loading aborted."), ID_STATUS_MSG);


         statusBar()->showMessage(tr("Ready."),ID_STATUS_MSG);
    }
}

bool KlogoTurtleApp::save()
{
    statusBar()->showMessage(tr("Saving file..."), ID_STATUS_MSG);
    if (filename.isEmpty())
    {
        return saveAs();
    }

    QString text = ComandoTextEdit->toPlainText();
    QFile f(filename);
    if (!f.open(QFile::WriteOnly))
    {
        statusBar()->showMessage(tr("Could not write to file."), ID_STATUS_MSG);
        return true;
    }

    QTextStream t(&f);
    t << text;
    f.close();

    statusBar()->showMessage(tr("File saved."), ID_STATUS_MSG);
    statusBar()->showMessage(tr("Ready."),ID_STATUS_MSG );
    return true;
}

bool KlogoTurtleApp::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    filename = files.at(0);
    return save();
}

void KlogoTurtleApp::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

bool KlogoTurtleApp::maybeSave()
{
    if (ComandoTextEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


void KlogoTurtleApp::print()
{
    statusBar()->showMessage(tr("Printing..."),ID_STATUS_MSG);

    const int Margin = 10;
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));

    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter printpainter(&printer);

        printpainter.setFont(ComandoTextEdit->font());
        int yPos = 0;                               // y-position for each line
        QFontMetrics fm = printpainter.fontMetrics();

        QString plainTextEditContents = ComandoTextEdit->toPlainText();
        QStringList lines = plainTextEditContents.split("\n");

        for(int i = 0 ; i < lines.count() ; i++)
        {
            if (Margin + yPos > printer.height() - Margin)
            {
                printer.newPage();                 // no more room on this page
                yPos = 0;                           // back to top of page
            }
            printpainter.drawText(Margin, Margin + yPos, printer.width(), fm.lineSpacing(), Qt::TextExpandTabs | Qt::TextDontClip, lines[i]);
            yPos = yPos + fm.lineSpacing();
        }

        printpainter.end();
        statusBar()->showMessage(tr("Printing completed."), ID_STATUS_MSG);
    }
    else
    {
        statusBar()->showMessage(tr("Printing aborted."), ID_STATUS_MSG);
    }

    statusBar()->showMessage(tr("Ready."), ID_STATUS_MSG);
}

void KlogoTurtleApp::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));


    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    printAsAct = new QAction(QIcon(":/images/print.png"), tr("Print..."), this);
    printAsAct->setShortcuts(QKeySequence::Print);
    printAsAct->setStatusTip(tr("Print..."));
    connect(printAsAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), ComandoTextEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), ComandoTextEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), ComandoTextEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(ComandoTextEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(ComandoTextEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void KlogoTurtleApp::createLanguageMenu(void)
{
    QActionGroup* langGroup = new QActionGroup(this);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    // format systems language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    //defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

    QDir dir(":/languages");
    QStringList fileNames = dir.entryList(QStringList("klogoturtle_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = fileNames[i]; // "klogoturtle_de.qm"
        locale.truncate(locale.lastIndexOf('.')); // "klogoturtle_de"
        locale.remove(0, locale.indexOf('_') + 1); // "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        languageMenu->addAction(action);
        langGroup->addAction(action);

        // set default translators and language checked
        if (defaultLocale == locale)
        {
            action->setChecked(true);
        }
    }
}

void KlogoTurtleApp::slotLanguageChanged(QAction* action)
{
    if(0 != action) {
        // load the language dependant on the action content
        loadLanguage(action->data().toString());
        setWindowIcon(action->icon());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(filename, ":/languages"))
        qApp->installTranslator(&translator);
}

void switchTranslatorQT(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(filename))
        qApp->installTranslator(&translator);
}

void KlogoTurtleApp::loadLanguage(const QString& rLanguage)
{
    if(idioma_escolhido != rLanguage) {
        idioma_escolhido = rLanguage;
        QLocale locale = QLocale(idioma_escolhido);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("klogoturtle_%1.qm").arg(rLanguage));
        switchTranslatorQT(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        statusBar()->showMessage(tr("Current Language changed to %1").arg(languageName));
    }
}

void KlogoTurtleApp::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    languageMenu = menuBar()->addMenu(tr("&Language"));
    createLanguageMenu();

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void KlogoTurtleApp::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAsAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void KlogoTurtleApp::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

int KlogoTurtleApp::Exe_comando(int pos_comando, QStringList lines){
    QString comando;
    QString parametro_s;
    QString var_nome;
    QString var_int;
    int prox_comando;
    int ini_repete;
    int tamanho;
    int parametro;
    int parametro_rep;
    int existe_comando;
    bool ok;
    bool okt;
    bool okt_rep;
    bool okt_des;

    existe_comando = -1;
    prox_comando = pos_comando + 1;


    if(qual_comando == 1)
    {
        comando = ((lines[pos_comando]).toUpper()).simplified();
    }
    else
    {
        comando = (temp_comando_to.section('#', pos_comando, pos_comando).toUpper()).simplified();
    }

    comando.replace(" ", "");

    //Commands in english
    if(idioma_escolhido == "ingles")
    {
        if(comando.startsWith("FORWARD")) comando.replace("FORWARD","FD");
        if(comando.startsWith("BACKWARD")) comando.replace("BACKWARD","BK");
        if(comando.startsWith("RIGHT")) comando.replace("RIGHT","RT");
        if(comando.startsWith("LEFT")) comando.replace("LEFT","LT");
        if(comando.startsWith("SETX")) comando.replace("SETX","SX");
        if(comando.startsWith("SETY")) comando.replace("SETY","SY");
        if(comando.startsWith("PENDOWN")) comando.replace("PENDOWN","PD");
        if(comando.startsWith("PENUP")) comando.replace("PENUP","PU");
        if(comando.startsWith("CLEAR")) comando.replace("CLEAR","CS");
        if(comando.startsWith("HOME")) comando.replace("HOME","HM");
        if(comando.startsWith("HIDETURTLE")) comando.replace("HIDETURTLE","HT");
        if(comando.startsWith("SHOWTURTLE")) comando.replace("SHOWTURTLE","ST");
        if(comando.startsWith("PENCOLOR")) comando.replace("PENCOLOR","PC");
        if(comando.startsWith("SUM")) comando.replace("SUM","ADD");
        if(comando.startsWith("SUBTRACT")) comando.replace("SUBTRACT","SUB");
        if(comando.startsWith("MULTIPLY")) comando.replace("MULTIPLY","MUL");
        if(comando.startsWith("DIVIDE")) comando.replace("DIVIDE","DIV");
        if(comando.startsWith("REMAINDER")) comando.replace("REMAINDER","REM");
    }


    //Commands in portuguese
    if(idioma_escolhido == "portugues")
    {
        if(comando.startsWith("FRENTE")) comando.replace("FRENTE","FD");
        if(comando.startsWith("ATRAS")) comando.replace("ATRAS","BK");
        if(comando.startsWith("DIREITA")) comando.replace("DIREITA","RT");
        if(comando.startsWith("ESQUERDA")) comando.replace("ESQUERDA","LT");
        if(comando.startsWith("MUDAX")) comando.replace("MUDAX","SX");
        if(comando.startsWith("X")) comando.replace("X","SX");
        if(comando.startsWith("MUDAY")) comando.replace("MUDAY","SY");
        if(comando.startsWith("Y")) comando.replace("Y","SY");
        if(comando.startsWith("DESCELAPIS")) comando.replace("DESCELAPIS","PD");
        if(comando.startsWith("DESCE")) comando.replace("DESCE","PD");
        if(comando.startsWith("SOBELAPIS")) comando.replace("SOBELAPIS","PU");
        if(comando.startsWith("SOBE")) comando.replace("SOBE","PU");
        if(comando.startsWith("LIMPA")) comando.replace("LIMPA","CS");
        if(comando.startsWith("CENTRO")) comando.replace("CENTRO","HM");
        if(comando.startsWith("ESCONDE")) comando.replace("ESCONDE","HT");
        if(comando.startsWith("MOSTRA")) comando.replace("MOSTRA","ST");
        if(comando.startsWith("COR")) comando.replace("COR","PC");
        if(comando.startsWith("ADICAO")) comando.replace("ADICAO","ADD");
        if(comando.startsWith("SOMA")) comando.replace("SOMA","ADD");
        if(comando.startsWith("SUBTRACAO")) comando.replace("SUBTRACAO","SUB");
        if(comando.startsWith("SUBTRAI")) comando.replace("SUBTRAI","SUB");
        if(comando.startsWith("MULTIPLICACAO")) comando.replace("MULTIPLICACAO","MUL");
        if(comando.startsWith("MULTIPLICA")) comando.replace("MULTIPLICA","MUL");
        if(comando.startsWith("DIVISAO")) comando.replace("DIVISAO","DIV");
        if(comando.startsWith("DIVIDE")) comando.replace("DIVIDE","DIV");
        if(comando.startsWith("RESTO")) comando.replace("RESTO","REM");
        if(comando.startsWith("SE")) comando.replace("SE","IF");
        if(comando.startsWith("FIMSE")) comando.replace("FIMSE","ENDIF");
        if(comando.startsWith("REPETE")) comando.replace("REPETE","REPEAT");
        if(comando.startsWith("FIMREPETE")) comando.replace("FIMREPETE","ENDREPEAT");
        if(comando.startsWith("DEFINE")) comando.replace("DEFINE","TO");
        if(comando.startsWith("FIMDEFINE")) comando.replace("FIMDEFINE","ENDTO");
        if(comando.startsWith("FACA")) comando.replace("FACA","MAKE");
        if(comando.startsWith("CONTEUDO")) comando.replace("CONTEUDO","SHOW");
        if(comando.startsWith("NOVO")) comando.replace("NOVO","NEW");
    }


    if(idioma_escolhido == "alemao")
    {
        if(comando.startsWith("VORWAERTS")) comando.replace("VORWAERTS","FD");
        if(comando.startsWith("ZURUECK")) comando.replace("ZURUECK","BK");
        if(comando.startsWith("RECHTS")) comando.replace("RECHTS","RT");
        if(comando.startsWith("LINKS")) comando.replace("LINKS","LT");
        if(comando.startsWith("SETZEX")) comando.replace("SETZEX","SX");
        if(comando.startsWith("SETZEY")) comando.replace("SETZEY","SY");
        if(comando.startsWith("ABSETZEN")) comando.replace("ABSETZEN","PD");
        if(comando.startsWith("ANHEBEN")) comando.replace("ANHEBEN","PU");
        if(comando.startsWith("LOESCHEN")) comando.replace("LOESCHEN","CS");
        if(comando.startsWith("MITTE")) comando.replace("MITTE","HM");
        if(comando.startsWith("VERSTECKE")) comando.replace("VERSTECKE","HT");
        if(comando.startsWith("ZEIGE")) comando.replace("ZEIGE","ST");
        if(comando.startsWith("FARBE")) comando.replace("FARBE","PC");
        if(comando.startsWith("ADDIERE")) comando.replace("ADDIERE","ADD");
        if(comando.startsWith("SUBTRAHIERE")) comando.replace("SUBTRAHIERE","SUB");
        if(comando.startsWith("MULTIPLIZIERE")) comando.replace("MULTIPLIZIERE","MUL");
        if(comando.startsWith("DIVIDIERE")) comando.replace("DIVIDIERE","DIV");
        if(comando.startsWith("REST")) comando.replace("REST","REM");
        if(comando.startsWith("WENN")) comando.replace("WENN","IF");
        if(comando.startsWith("ENDEWENN")) comando.replace("ENDEWENN","ENDIF");
        if(comando.startsWith("WIEDERHOLE")) comando.replace("WIEDERHOLE","REPEAT");
        if(comando.startsWith("ENDEWIEDERHOLE")) comando.replace("ENDEWIEDERHOLE","ENDREPEAT");
        if(comando.startsWith("ZU")) comando.replace("ZU","TO");
        if(comando.startsWith("ENDEZU")) comando.replace("ENDEZU","ENDTO");
        if(comando.startsWith("SETZE")) comando.replace("SETZE","MAKE");
        if(comando.startsWith("INHALT")) comando.replace("INHALT","SHOW");
        if(comando.startsWith("NEU")) comando.replace("NEU","NEW");
    }

    if(idioma_escolhido == "italiano")
    {
        if(comando.startsWith("AVANTI")) comando.replace("AVANTI","FD");
        if(comando.startsWith("INDIETRO")) comando.replace("INDIETRO","BK");
        if(comando.startsWith("DESTRA")) comando.replace("DESTRA","RT");
        if(comando.startsWith("SINISTRA")) comando.replace("SINISTRA","LT");
        if(comando.startsWith("ASSEX")) comando.replace("ASSEX","SX");
        if(comando.startsWith("ASSEY")) comando.replace("ASSEY","SY");
        if(comando.startsWith("SCRIVI")) comando.replace("SCRIVI","PD");
        if(comando.startsWith("NONSCRIVERE")) comando.replace("NONSCRIVERE","PU");
        if(comando.startsWith("PULISCI")) comando.replace("PULISCI","CS");
        if(comando.startsWith("CENTRA")) comando.replace("CENTRA","HM");
        if(comando.startsWith("NASCONDI")) comando.replace("NASCONDI","HT");
        if(comando.startsWith("MOSTRA")) comando.replace("MOSTRA","ST");
        if(comando.startsWith("COLORE")) comando.replace("COLORE","PC");
        if(comando.startsWith("SOMMA")) comando.replace("SOMMA","ADD");
        if(comando.startsWith("SOTTRAI")) comando.replace("SOTTRAI","SUB");
        if(comando.startsWith("MOLTIPLICA")) comando.replace("MOLTIPLICA","MUL");
        if(comando.startsWith("DIVIDI")) comando.replace("DIVIDI","DIV");
        if(comando.startsWith("RESTO")) comando.replace("RESTO","REM");
        if(comando.startsWith("NUOVO")) comando.replace("NUOVO","NEW");
        if(comando.startsWith("SE")) comando.replace("SE","IF");
        if(comando.startsWith("FINESE")) comando.replace("FINESE","ENDIF");
        if(comando.startsWith("RIPETI")) comando.replace("RIPETI","REPEAT");
        if(comando.startsWith("FINERIPETI")) comando.replace("FINERIPETI","ENDREPEAT");
        if(comando.startsWith("IN")) comando.replace("IN","TO");
        if(comando.startsWith("FINEIN")) comando.replace("FINEIN","ENDTO");
        if(comando.startsWith("CREA")) comando.replace("CREA","MAKE");
        if(comando.startsWith("CONTENUTO")) comando.replace("CONTENUTO","SHOW");
    }

    if(idioma_escolhido == "frances")
    {
        if(comando.startsWith("AVANCE")) comando.replace("AVANCE","FD");
        if(comando.startsWith("RECULE")) comando.replace("RECULE","BK");
        if(comando.startsWith("DROITE")) comando.replace("DROITE","RT");
        if(comando.startsWith("GAUCHE")) comando.replace("GAUCHE","LT");
        if(comando.startsWith("POSX")) comando.replace("POSX","SX");
        if(comando.startsWith("POSY")) comando.replace("POSY","SY");
        if(comando.startsWith("BAISSECRAYON")) comando.replace("BAISSECRAYON","PD");
        if(comando.startsWith("LEVECRAYON")) comando.replace("LEVECRAYON","PU");
        if(comando.startsWith("VIDECRAN")) comando.replace("VIDECRAN","CS");
        if(comando.startsWith("ORIGINE")) comando.replace("ORIGINE","HM");
        if(comando.startsWith("CACHETORTUE")) comando.replace("CACHETORTUE","HT");
        if(comando.startsWith("MONTRETORTUE")) comando.replace("MONTRETORTUE","ST");
        if(comando.startsWith("CHANGECOULEUR")) comando.replace("CHANGECOULEUR","PC");
        if(comando.startsWith("SOMME")) comando.replace("SOMME","ADD");
        if(comando.startsWith("DIFFERENCE")) comando.replace("DIFFERENCE","SUB");
        if(comando.startsWith("PRODUIT")) comando.replace("PRODUIT","MUL");
        if(comando.startsWith("QUOTIENT")) comando.replace("QUOTIENT","DIV");
        if(comando.startsWith("RESTE")) comando.replace("RESTE","REM");
        if(comando.startsWith("NOUVEAU")) comando.replace("NOUVEAU","NEW");
        if(comando.startsWith("SI")) comando.replace("SI","IF");
        if(comando.startsWith("FINSI")) comando.replace("FINSI","ENDIF");
        if(comando.startsWith("REPETE")) comando.replace("REPETE","REPEAT");
        if(comando.startsWith("FINREPETE")) comando.replace("FINREPETE","ENDREPEAT");
        if(comando.startsWith("POUR")) comando.replace("POUR","TO");
        if(comando.startsWith("FINPOUR")) comando.replace("FINPOUR","ENDTO");
        if(comando.startsWith("CREE")) comando.replace("CREE","MAKE");
        if(comando.startsWith("VALEUR")) comando.replace("VALEUR","SHOW");
    }



    if(comando != "")
    {

        if(comando.startsWith("REPEAT"))
        {
            ini_repete = pos_comando + 1;
            tamanho = comando.length();
            parametro_rep = (comando.mid(6,tamanho)).toInt(&okt,10);

            QString texto_temp = ((ComandoTextEdit->toPlainText()).toUpper()).simplified();
            texto_temp.replace(" ", "");

            if(texto_temp.contains("FIMREPETE",Qt::CaseInsensitive)) comando.replace("FIMREPETE","ENDREPEAT");

            int n_endrepeat = texto_temp.count("ENDREPEAT", Qt::CaseInsensitive);
            int n_repeat = (texto_temp.count("REPEAT", Qt::CaseInsensitive)) - n_endrepeat;

            if (n_repeat == n_endrepeat)
            {
                if(okt)
                {
                    prox_comando =  Repete(parametro_rep, ini_repete, lines);
                    existe_comando = 1;
                }
                else
                {
                    parametro_rep = (des->show_var(comando.mid(6,tamanho))).toInt(&okt_rep,10);
                    if(okt_rep)
                    {
                        prox_comando =  Repete(parametro_rep, ini_repete, lines);
                        existe_comando = 1;
                    }
                    else
                    {
                        MensagemTextEdit->append(tr("REPEAT with invalid argument"));
                        stop_error();
                    }
                }
            }
            else
            {
                MensagemTextEdit->append(tr("REPEAT without END REPEAT"));
                stop_error();
            }

        }
        else
        {
            if(comando.startsWith("ENDREPEAT"))
            {
                prox_comando = -1;
                pos_end_repete = pos_comando + 1;
                existe_comando = 1;

            }
            else
            {
                if(comando.startsWith("MAKE"))
                {
                    bool ok_make_temp;
                    bool ok_make_temp2;

                    prox_comando = pos_comando + 1;
                    tamanho = comando.length();

                    int pos_igual = comando.indexOf('=',0);

                    if(pos_igual != -1)
                    {
                        int temp_make;
                        int temp_make2;
                        var_nome = (comando.mid(4,(pos_igual-4)));
                        var_int = (comando.mid(pos_igual+1,tamanho-(pos_igual+1)));
                        temp_make = var_int.toInt(&ok_make_temp,10);
                        temp_make2 = var_nome.toInt(&ok_make_temp2,10);
                        if(ok_make_temp2)
                        {
                            MensagemTextEdit->append(tr("Variable of MAKE is not correct"));
                            stop_error();
                        }
                        else
                        {
                            if(ok_make_temp)
                            {
                                des->make(var_nome.toUpper(),var_int);
                                existe_comando = 1;
                            }
                            else
                            {
                                MensagemTextEdit->append(tr("Argument of MAKE is not number"));
                                stop_error();
                            }
                        }
                    }
                    else
                    {
                        MensagemTextEdit->append(tr("MAKE without ="));
                        stop_error();
                    }
                }
                else
                {
                    if(comando.startsWith("PC"))
                    {
                        prox_comando = pos_comando + 1;
                        tamanho = comando.length();
                        parametro_s = (comando.mid(2,tamanho));
                        des->pc(parametro_s);
                        existe_comando = 1;

                    }
                    else
                    {
                        if(comando.startsWith("SHOW"))
                        {
                            prox_comando = pos_comando + 1;
                            tamanho = comando.length();
                            parametro_s = (comando.mid(4,tamanho));
                            //MensagemTextEdit->append(tr(des->show_var(parametro_s)));
                            MensagemTextEdit->append(des->show_var(parametro_s));
                            existe_comando = 1;
                        }
                        else
                        {
                            if(comando.startsWith("TO"))
                            {
                                int existe_endto = -1;

                                for(int y = pos_comando; y <= lines.count(); y++)
                                {
                                    QString com_endto = ((lines[y]).toUpper()).simplified();
                                    com_endto.replace(" ", "");

                                    if(com_endto.startsWith("FIMDEFINE")) com_endto.replace("FIMDEFINE","ENDTO");

                                    if(com_endto.startsWith("ENDTO"))
                                    {
                                        existe_endto = y;
                                        break;
                                    }

                                }

                                if(existe_endto != -1)
                                {
                                    QString cop_to_temp;
                                    cop_to_temp = "";

                                    for(int rr = pos_comando; rr <= existe_endto; rr++)
                                    {
                                        cop_to_temp.append(lines[rr]);
                                        cop_to_temp.append("#");
                                    }

                                    int pos_pontos = comando.indexOf(':',0);

                                    int pos_marca_nome;

                                    if(pos_pontos != -1)
                                    {
                                        pos_marca_nome  = pos_pontos;

                                        int qtd_pontos = comando.count(':',Qt::CaseInsensitive);
                                        for(int jj = 1; jj <= qtd_pontos; jj++)
                                        {
                                            if((comando.section(':',jj,jj)).length() < 4)
                                            {
                                                MensagemTextEdit->append(tr("Command variable of TO have length less than 4"));
                                                stop_error();
                                                break;
                                            }
                                        }

                                    }
                                    else
                                    {
                                        pos_marca_nome = comando.length();
                                    }

                                    QString nome_to = comando.mid(2,(pos_marca_nome-2));
                                    int pos_to_temp = find_to(nome_to);

                                    if(pos_to_temp != -1)
                                    {
                                        guarda_to [pos_to_temp] [0] = nome_to.toUpper();
                                        guarda_to [pos_to_temp] [1] = cop_to_temp;
                                    }
                                    else
                                    {
                                        guarda_to [n_to] [0] = nome_to.toUpper();
                                        guarda_to [n_to] [1] = cop_to_temp;
                                        n_to = n_to + 1;
                                    }
                                    prox_comando = existe_endto + 1;
                                    existe_comando = 1;

                                }
                                else
                                {
                                    MensagemTextEdit->append(tr("TO without END TO"));
                                    stop_error();
                                }

                            }
                            else
                            {
                                if (comando.startsWith("ADD") || comando.startsWith("SUB") || comando.startsWith("MUL") || comando.startsWith("DIV") || comando.startsWith("REM"))
                                {
                                    prox_comando = pos_comando + 1;

                                    if (comando.startsWith("ADD"))
                                    {
                                        QString var_recebe;
                                        QString var_param1;
                                        QString var_param2;
                                        int pos_ident = comando.indexOf('=',0);
                                        int pos_sign = comando.indexOf('+',0);
                                        if(pos_ident == -1 || pos_sign == -1)
                                        {
                                            MensagemTextEdit->append(tr("Command ADD without = or +"));
                                            stop_error();
                                        }
                                        else
                                        {
                                            int taman_temp = comando.length();
                                            var_recebe = comando.mid(3,(pos_ident-3));
                                            var_param1 = comando.mid((pos_ident+1),(pos_sign-pos_ident-1));
                                            var_param2 = comando.mid((pos_sign+1),taman_temp-(pos_sign+1));
                                            bool ok_param2;
                                            int i_param2 = var_param2.toInt(&ok_param2, 10);
                                            if(ok_param2 != true)
                                            {
                                                QString param2_temp = des->show_var(var_param2);
                                                bool ok_param2_temp;
                                                i_param2 = param2_temp.toInt(&ok_param2_temp, 10);
                                                if(ok_param2_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of ADD is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_param1;
                                            int i_param1 = var_param1.toInt(&ok_param1, 10);
                                            if(ok_param1 != true)
                                            {
                                                QString param1_temp = des->show_var(var_param1);
                                                bool ok_param1_temp;
                                                i_param1 = param1_temp.toInt(&ok_param1_temp, 10);
                                                if(ok_param1_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of ADD is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_recebe;
                                            int recebe_ok_temp = var_recebe.toInt(&ok_recebe, 10);
                                            recebe_ok_temp = 0;
                                            if(ok_recebe == true)
                                            {
                                                MensagemTextEdit->append(tr("Variable of ADD is number"));
                                                stop_error();
                                            }
                                            else
                                            {
                                                des->make(var_recebe.toUpper(),QString::number((i_param1 + i_param2), 10));
                                            }
                                        }
                                    }

                                    if (comando.startsWith("SUB"))
                                    {
                                        QString var_recebe;
                                        QString var_param1;
                                        QString var_param2;
                                        int pos_ident = comando.indexOf('=',0);
                                        int pos_sign = comando.indexOf('-',0);
                                        if(pos_ident == -1 || pos_sign == -1)
                                        {
                                            MensagemTextEdit->append(tr("Command SUB without = or -"));
                                            stop_error();
                                        }
                                        else
                                        {
                                            int taman_temp = comando.length();
                                            var_recebe = comando.mid(3,(pos_ident-3));
                                            var_param1 = comando.mid((pos_ident+1),(pos_sign-pos_ident-1));
                                            var_param2 = comando.mid((pos_sign+1),taman_temp-(pos_sign+1));
                                            bool ok_param2;
                                            int i_param2 = var_param2.toInt(&ok_param2, 10);
                                            if(ok_param2 != true)
                                            {
                                                QString param2_temp = des->show_var(var_param2);
                                                bool ok_param2_temp;
                                                i_param2 = param2_temp.toInt(&ok_param2_temp, 10);
                                                if(ok_param2_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of SUB is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_param1;
                                            int i_param1 = var_param1.toInt(&ok_param1, 10);
                                            if(ok_param1 != true)
                                            {
                                                QString param1_temp = des->show_var(var_param1);
                                                bool ok_param1_temp;
                                                i_param1 = param1_temp.toInt(&ok_param1_temp, 10);
                                                if(ok_param1_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of SUB is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_recebe;
                                            int recebe_ok_temp = var_recebe.toInt(&ok_recebe, 10);
                                            recebe_ok_temp = 0;
                                            if(ok_recebe == true)
                                            {
                                                MensagemTextEdit->append(tr("Variable of SUB is number"));
                                                stop_error();
                                            }
                                            else
                                            {
                                                des->make(var_recebe.toUpper(),QString::number((i_param1 - i_param2), 10));
                                            }
                                        }
                                    }


                                    if (comando.startsWith("MUL"))
                                    {
                                        QString var_recebe;
                                        QString var_param1;
                                        QString var_param2;
                                        int pos_ident = comando.indexOf('=',0);
                                        int pos_sign = comando.indexOf('*',0);
                                        if(pos_ident == -1 || pos_sign == -1)
                                        {
                                            MensagemTextEdit->append(tr("Command MUL without = or *"));
                                            stop_error();
                                        }
                                        else
                                        {
                                            int taman_temp = comando.length();
                                            var_recebe = comando.mid(3,(pos_ident-3));
                                            var_param1 = comando.mid((pos_ident+1),(pos_sign-pos_ident-1));
                                            var_param2 = comando.mid((pos_sign+1),taman_temp-(pos_sign+1));
                                            bool ok_param2;
                                            int i_param2 = var_param2.toInt(&ok_param2, 10);
                                            if(ok_param2 != true)
                                            {
                                                QString param2_temp = des->show_var(var_param2);
                                                bool ok_param2_temp;
                                                i_param2 = param2_temp.toInt(&ok_param2_temp, 10);
                                                if(ok_param2_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of MUL is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_param1;
                                            int i_param1 = var_param1.toInt(&ok_param1, 10);
                                            if(ok_param1 != true)
                                            {
                                                QString param1_temp = des->show_var(var_param1);
                                                bool ok_param1_temp;
                                                i_param1 = param1_temp.toInt(&ok_param1_temp, 10);
                                                if(ok_param1_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of MUL is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_recebe;
                                            int recebe_ok_temp = var_recebe.toInt(&ok_recebe, 10);
                                            recebe_ok_temp = 0;
                                            if(ok_recebe == true)
                                            {
                                                MensagemTextEdit->append(tr("Variable of MUL is number"));
                                                stop_error();
                                            }
                                            else
                                            {
                                                des->make(var_recebe.toUpper(),QString::number((i_param1 * i_param2), 10));
                                            }
                                        }
                                    }


                                    if (comando.startsWith("DIV"))
                                    {
                                        QString var_recebe;
                                        QString var_param1;
                                        QString var_param2;
                                        int pos_ident = comando.indexOf('=',0);
                                        int pos_sign = comando.indexOf('/',0);
                                        if(pos_ident == -1 || pos_sign == -1)
                                        {
                                            MensagemTextEdit->append(tr("Command DIV without = or /"));
                                            stop_error();
                                        }
                                        else
                                        {
                                            int taman_temp = comando.length();
                                            var_recebe = comando.mid(3,(pos_ident-3));
                                            var_param1 = comando.mid((pos_ident+1),(pos_sign-pos_ident-1));
                                            var_param2 = comando.mid((pos_sign+1),taman_temp-(pos_sign+1));
                                            bool ok_param2;
                                            int i_param2 = var_param2.toInt(&ok_param2, 10);
                                            if(ok_param2 != true)
                                            {
                                                QString param2_temp = des->show_var(var_param2);
                                                bool ok_param2_temp;
                                                i_param2 = param2_temp.toInt(&ok_param2_temp, 10);
                                                if(ok_param2_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of DIV is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            if (i_param2 <= 0)
                                            {
                                                MensagemTextEdit->append(tr("Variable of DIV is zero"));
                                                stop_error();
                                            }
                                            bool ok_param1;
                                            int i_param1 = var_param1.toInt(&ok_param1, 10);
                                            if(ok_param1 != true)
                                            {
                                                QString param1_temp = des->show_var(var_param1);
                                                bool ok_param1_temp;
                                                i_param1 = param1_temp.toInt(&ok_param1_temp, 10);
                                                if(ok_param1_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of DIV is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_recebe;
                                            int recebe_ok_temp = var_recebe.toInt(&ok_recebe, 10);
                                            recebe_ok_temp = 0;
                                            if(ok_recebe == true)
                                            {
                                                MensagemTextEdit->append(tr("Variable of DIV is number"));
                                                stop_error();
                                            }
                                            else
                                            {
                                                des->make(var_recebe.toUpper(),QString::number((i_param1 / i_param2), 10));
                                            }
                                        }
                                    }



                                    if (comando.startsWith("REM"))
                                    {
                                        QString var_recebe;
                                        QString var_param1;
                                        QString var_param2;
                                        int pos_ident = comando.indexOf('=',0);
                                        int pos_sign = comando.indexOf('/',0);
                                        if(pos_ident == -1 || pos_sign == -1)
                                        {
                                            MensagemTextEdit->append(tr("Command REM without = or /"));
                                            stop_error();
                                        }
                                        else
                                        {
                                            int taman_temp = comando.length();
                                            var_recebe = comando.mid(3,(pos_ident-3));
                                            var_param1 = comando.mid((pos_ident+1),(pos_sign-pos_ident-1));
                                            var_param2 = comando.mid((pos_sign+1),taman_temp-(pos_sign+1));
                                            bool ok_param2;
                                            int i_param2 = var_param2.toInt(&ok_param2, 10);
                                            if(ok_param2 != true)
                                            {
                                                QString param2_temp = des->show_var(var_param2);
                                                bool ok_param2_temp;
                                                i_param2 = param2_temp.toInt(&ok_param2_temp, 10);
                                                if(ok_param2_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of REM is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            if (i_param2 <= 0)
                                            {
                                                MensagemTextEdit->append(tr("Variable of REM is zero"));
                                                stop_error();
                                            }
                                            bool ok_param1;
                                            int i_param1 = var_param1.toInt(&ok_param1, 10);
                                            if(ok_param1 != true)
                                            {
                                                QString param1_temp = des->show_var(var_param1);
                                                bool ok_param1_temp;
                                                i_param1 = param1_temp.toInt(&ok_param1_temp, 10);
                                                if(ok_param1_temp != true)
                                                {
                                                    MensagemTextEdit->append(tr("Variable of REM is not number or not defined"));
                                                    stop_error();
                                                }
                                            }

                                            bool ok_recebe;
                                            int recebe_ok_temp = var_recebe.toInt(&ok_recebe, 10);
                                            recebe_ok_temp = 0;
                                            if(ok_recebe == true)
                                            {
                                                MensagemTextEdit->append(tr("Variable of REM is number"));
                                                stop_error();
                                            }
                                            else
                                            {
                                                des->make(var_recebe.toUpper(),QString::number((i_param1 % i_param2), 10));
                                            }
                                        }
                                    }


                                    existe_comando = 1;
                                }
                                else
                                {

                                    //If command
                                    if(comando.startsWith("IF") || comando.startsWith("ENDIF"))
                                    {
                                        int existe_endif = -1;
                                        int taman_if_temp = comando.length();
                                        QString var_param1_if;
                                        QString var_param2_if;

                                        prox_comando = pos_comando + 1;

                                        for(int y = pos_comando; y <= lines.count(); y++)
                                        {
                                            QString com_endif = ((lines[y]).toUpper()).simplified();
                                            com_endif.replace(" ", "");

                                            if(com_endif.startsWith("FIMSE")) com_endif.replace("FIMSE","ENDIF");

                                            if(com_endif.startsWith("ENDIF"))
                                            {
                                                existe_endif = y;
                                                break;
                                            }

                                        }

                                        if(existe_endif != -1)
                                        {
                                            int pos_operando;
                                            pos_operando = comando.indexOf("=", 0);
                                            if(pos_operando != -1)
                                            {
                                                var_param1_if = comando.mid(2,(pos_operando-2));
                                                var_param2_if = comando.mid((pos_operando+1),taman_if_temp-(pos_operando+1));

                                                bool ok_if_param1;
                                                int i_if_param1 = var_param1_if.toInt(&ok_if_param1, 10);
                                                if(ok_if_param1 != true)
                                                {
                                                    QString param1_if_temp = des->show_var(var_param1_if);
                                                    bool ok_param1_if_temp;
                                                    i_if_param1 = param1_if_temp.toInt(&ok_param1_if_temp, 10);
                                                    if(ok_param1_if_temp != true)
                                                    {
                                                        MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                        stop_error();
                                                    }
                                                }

                                                bool ok_if_param2;
                                                int i_if_param2 = var_param2_if.toInt(&ok_if_param2, 10);
                                                if(ok_if_param2 != true)
                                                {
                                                    QString param2_if_temp = des->show_var(var_param2_if);
                                                    bool ok_param2_if_temp;
                                                    i_if_param2 = param2_if_temp.toInt(&ok_param2_if_temp, 10);
                                                    if(ok_param2_if_temp != true)
                                                    {
                                                        MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                        stop_error();
                                                    }
                                                }

                                                if(i_if_param1 == i_if_param2)
                                                {
                                                    prox_comando = pos_comando + 1;
                                                }
                                                else
                                                {
                                                    prox_comando = existe_endif + 1;
                                                }
                                            }
                                            else
                                            {
                                                pos_operando = comando.indexOf(">", 0);
                                                if(pos_operando != -1)
                                                {
                                                    var_param1_if = comando.mid(2,(pos_operando-2));
                                                    var_param2_if = comando.mid((pos_operando+1),taman_if_temp-(pos_operando+1));

                                                    bool ok_if_param1;
                                                    int i_if_param1 = var_param1_if.toInt(&ok_if_param1, 10);
                                                    if(ok_if_param1 != true)
                                                    {
                                                        QString param1_if_temp = des->show_var(var_param1_if);
                                                        bool ok_param1_if_temp;
                                                        i_if_param1 = param1_if_temp.toInt(&ok_param1_if_temp, 10);
                                                        if(ok_param1_if_temp != true)
                                                        {
                                                            MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                            stop_error();
                                                        }
                                                    }

                                                    bool ok_if_param2;
                                                    int i_if_param2 = var_param2_if.toInt(&ok_if_param2, 10);
                                                    if(ok_if_param2 != true)
                                                    {
                                                        QString param2_if_temp = des->show_var(var_param2_if);
                                                        bool ok_param2_if_temp;
                                                        i_if_param2 = param2_if_temp.toInt(&ok_param2_if_temp, 10);
                                                        if(ok_param2_if_temp != true)
                                                        {
                                                            MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                            stop_error();
                                                        }
                                                    }

                                                    if(i_if_param1 > i_if_param2)
                                                    {
                                                        prox_comando = pos_comando + 1;
                                                    }
                                                    else
                                                    {
                                                        prox_comando = existe_endif + 1;
                                                    }

                                                }
                                                else
                                                {
                                                    pos_operando = comando.indexOf("<", 0);
                                                    if(pos_operando != -1)
                                                    {
                                                        var_param1_if = comando.mid(2,(pos_operando-2));
                                                        var_param2_if = comando.mid((pos_operando+1),taman_if_temp-(pos_operando+1));

                                                        bool ok_if_param1;
                                                        int i_if_param1 = var_param1_if.toInt(&ok_if_param1, 10);
                                                        if(ok_if_param1 != true)
                                                        {
                                                            QString param1_if_temp = des->show_var(var_param1_if);
                                                            bool ok_param1_if_temp;
                                                            i_if_param1 = param1_if_temp.toInt(&ok_param1_if_temp, 10);
                                                            if(ok_param1_if_temp != true)
                                                            {
                                                                MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                                stop_error();
                                                            }
                                                        }

                                                        bool ok_if_param2;
                                                        int i_if_param2 = var_param2_if.toInt(&ok_if_param2, 10);
                                                        if(ok_if_param2 != true)
                                                        {
                                                            QString param2_if_temp = des->show_var(var_param2_if);
                                                            bool ok_param2_if_temp;
                                                            i_if_param2 = param2_if_temp.toInt(&ok_param2_if_temp, 10);
                                                            if(ok_param2_if_temp != true)
                                                            {
                                                                MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                                stop_error();
                                                            }
                                                        }

                                                        if(i_if_param1 < i_if_param2)
                                                        {
                                                            prox_comando = pos_comando + 1;
                                                        }
                                                        else
                                                        {
                                                            prox_comando = existe_endif + 1;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        pos_operando = comando.indexOf("!", 0);
                                                        if(pos_operando != -1)
                                                        {
                                                            var_param1_if = comando.mid(2,(pos_operando-2));
                                                            var_param2_if = comando.mid((pos_operando+1),taman_if_temp-(pos_operando+1));

                                                            bool ok_if_param1;
                                                            int i_if_param1 = var_param1_if.toInt(&ok_if_param1, 10);
                                                            if(ok_if_param1 != true)
                                                            {
                                                                QString param1_if_temp = des->show_var(var_param1_if);
                                                                bool ok_param1_if_temp;
                                                                i_if_param1 = param1_if_temp.toInt(&ok_param1_if_temp, 10);
                                                                if(ok_param1_if_temp != true)
                                                                {
                                                                    MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                                    stop_error();
                                                                }
                                                            }

                                                            bool ok_if_param2;
                                                            int i_if_param2 = var_param2_if.toInt(&ok_if_param2, 10);
                                                            if(ok_if_param2 != true)
                                                            {
                                                                QString param2_if_temp = des->show_var(var_param2_if);
                                                                bool ok_param2_if_temp;
                                                                i_if_param2 = param2_if_temp.toInt(&ok_param2_if_temp, 10);
                                                                if(ok_param2_if_temp != true)
                                                                {
                                                                    MensagemTextEdit->append(tr("Variable of IF is not number or not defined"));
                                                                    stop_error();
                                                                }
                                                            }

                                                            if(i_if_param1 != i_if_param2)
                                                            {
                                                                prox_comando = pos_comando + 1;
                                                            }
                                                            else
                                                            {
                                                                prox_comando = existe_endif + 1;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            if(comando.startsWith("ENDIF") != true)
                                                            {
                                                                MensagemTextEdit->append(tr("IF without operand"));
                                                                stop_error();
                                                            }
                                                        }
                                                    }

                                                }
                                            }

                                        }
                                        else
                                        {
                                            MensagemTextEdit->append(tr("IF without END IF"));
                                            stop_error();
                                        }

                                        existe_comando = 1;

                                    }


                                    else
                                    {

                                        //Others commands

                                        prox_comando = pos_comando + 1;
                                        tamanho = comando.length();
                                        parametro = (comando.mid(2,tamanho)).toInt(&ok,10);
                                        if (comando.startsWith("FD"))
                                        {
                                            if (ok)
                                            {
                                                des->fd(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->fd(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("FD with invalid argument"));
                                                    stop_error();
                                                }
                                            }
                                        }

                                        if (comando.startsWith("BK"))
                                        {
                                            if (ok)
                                            {
                                                des->bk(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->bk(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("BK with invalid argument"));
                                                    stop_error();
                                                }
                                            }
                                        }

                                        if (comando.startsWith("LT"))
                                        {
                                            if (ok)
                                            {
                                                des->lt(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->lt(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("LT with invalid argument"));
                                                    stop_error();
                                                }
                                            }
                                        }

                                        if (comando.startsWith("RT"))
                                        {
                                            if (ok)
                                            {
                                                des->rt(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->rt(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("RT with invalid argument"));
                                                    stop_error();
                                                }
                                            }
                                        }

                                        if (comando.startsWith("SX"))
                                        {
                                            if (ok)
                                            {
                                                des->sx(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->sx(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("SX with invalid argument"));
                                                    stop_error();
                                                }
                                            }
                                        }

                                        if (comando.startsWith("SY"))
                                        {
                                            if (ok)
                                            {
                                                des->sy(parametro);
                                                existe_comando = 1;
                                            }
                                            else
                                            {
                                                parametro = (des->show_var(comando.mid(2,tamanho))).toInt(&okt_des,10);
                                                if(okt_des)
                                                {
                                                    des->sy(parametro);
                                                    existe_comando = 1;
                                                }
                                                else
                                                {
                                                    MensagemTextEdit->append(tr("SY with invalid argument"));
                                                    stop_error();
                                                }
                                            }

                                        }



                                        if (comando.startsWith("CS"))
                                        {
                                            des->cs();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("PD"))
                                        {
                                            des->pd();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("PU"))
                                        {
                                            des->pu();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("HM"))
                                        {
                                            des->hm();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("HT"))
                                        {
                                            des->ht();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("ST"))
                                        {
                                            des->st();
                                            existe_comando = 1;
                                        }

                                        if (comando.startsWith("NEW"))
                                        {
                                            des->cnew();
                                            MensagemTextEdit->setText("");
                                            ComandoTextEdit->setText("");
                                            existe_comando = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if(existe_comando != -1)
        {
            return prox_comando;
        }
        else
        {
            int pos_tt = comando.indexOf(':',0);

            if(pos_tt != -1)
            {
                parametro_s = (comando.mid(0,pos_tt));
            }
            else
            {
                parametro_s = (comando.mid(0,comando.length()));
            }

            if(find_to(parametro_s) != -1)
            {
                comando_to(comando, parametro_s, lines);
                return prox_comando;
            }
            else
            {
                MensagemTextEdit->append(tr("Invalid command:"));
                MensagemTextEdit->append(comando);
                tem_erro = 1;
                stop_error();
                return ((lines.count())+1);
            }
        }
    }
    else
    {
        return pos_comando + 1;
        ;
    }
}

void KlogoTurtleApp::slotRun()
{
    int pos_comando;
    int prox_comando;
    int n_comandos;

    tem_erro = 0;

    //saveImage->setEnabled(true);

    QString plainTextEditContents = ComandoTextEdit->toPlainText();
    QStringList lines = plainTextEditContents.split("\n");
    n_comandos = lines.count();
    pos_comando = 0;

    while (pos_comando < n_comandos)
    {
        prox_comando = Exe_comando(pos_comando, lines);
        pos_comando = prox_comando;
        qual_comando = 1;
    }

    if(tem_erro == 0) des->tartaruga(tem_erro);

}

int KlogoTurtleApp::Repete(int param, int par_ini, QStringList lines)
{
    int pos_comando;
    int prox_comando;

    for(int h = 1; h <= param; h++)
    {
        pos_comando = par_ini;

        while (pos_comando != -1)
        {
            prox_comando = Exe_comando(pos_comando,lines);
            pos_comando = prox_comando;
        }
    }

    return pos_end_repete;

}


void KlogoTurtleApp::stop_error()
{
    des->tartaruga(tem_erro);
}

void KlogoTurtleApp::comando_to(QString com_to_name, QString nome, QStringList lines)
{
    int pos_comando;
    int prox_comando;
    int n_comandos;
    int pos = find_to(nome);

    QString comand_to_all = guarda_to [pos] [1];

    int teste_arg = com_to_name.count(':',Qt::CaseInsensitive);

    temp_comando_to = comand_to_all;

    if(teste_arg > 0)
    {
        if(teste_arg == comand_to_all.count(':',Qt::CaseInsensitive))
        {
            for(int w = 1; w <= teste_arg; w++)
            {
                QString valor_para_temp = com_to_name.section(':', w, w);
                QString nome_para_temp = (comand_to_all.section('#', 0, 0)).section(':', w, w);
                temp_comando_to.replace(nome_para_temp,valor_para_temp);
            }

            qual_comando = 0;
            n_comandos = comand_to_all.count('#',Qt::CaseInsensitive) - 2;
            pos_comando = 1;

            while (pos_comando <= n_comandos)
            {
                prox_comando = Exe_comando(pos_comando, lines);
                pos_comando = prox_comando;
            }

        }

        qual_comando = 1;

    }
    else
    {
        qual_comando = 0;
        n_comandos = comand_to_all.count('#',Qt::CaseInsensitive) - 2;
        pos_comando = 1;

        while (pos_comando <= n_comandos)
        {
            prox_comando = Exe_comando(pos_comando, lines);
            pos_comando = prox_comando;
        }

        qual_comando = 1;
    }
}

void KlogoTurtleApp::apaga_to()
{
    for(int h = 0; h < 10; h++)
    {
        for(int g = 0; g < 2; g++)
        {
            guarda_to [h] [g] = "0";
        }
    }
}

int KlogoTurtleApp::find_to(QString to_name)
{
    int volta = -1;

    for(int l = 0; l < 10; l++)
    {
        if(guarda_to [l] [0] == to_name)
        {
            volta = l;
            break;
        }

    }
    return volta;

}
