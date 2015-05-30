#include <QtCore/qmath.h>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

#include "klogoturtlemobileapp.h"
#include "ui_klogoturtlemobileapp.h"
#include "mesamobile.h"

KlogoTurtleMobileApp::KlogoTurtleMobileApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KlogoTurtleMobileApp)
{

    QRect rect = qApp->primaryScreen()->geometry();
    m_ratio = qMin(qMax(rect.width(), rect.height())/1136. , qMin(rect.width(), rect.height())/640.);
    m_sliderHandleWidth = getSizeWithRatio(70);
    m_sliderHandleHeight = getSizeWithRatio(87);
    m_sliderGapWidth = getSizeWithRatio(100);
    m_isPortraitMode = rect.height() > rect.width();
    m_hMargin =  m_isPortraitMode ? 20 * ratio() : 50 * ratio();
    m_applicationWidth = rect.width();
    m_applicationHeight = rect.height();

    connect(qApp->primaryScreen(), SIGNAL(primaryOrientationChanged(Qt::ScreenOrientation)), this, SLOT(notifyPortraitMode(Qt::ScreenOrientation)));

    ui->setupUi(this);

    ///////////////////////////////////////////////////////////////////
    // call inits to invoke all other construction parts
    initView();
}

KlogoTurtleMobileApp::~KlogoTurtleMobileApp()
{
    delete ui;
}

void KlogoTurtleMobileApp::initView()
{

    label1 = new QLabel(this);
    label1->setText(i18n("Commands:"));
    label1->setGeometry(10 * m_ratio, 20 * m_ratio, 200 * m_ratio, 20 * m_ratio);

    int h = m_applicationHeight - label1->y() - label1->height() - (180 * m_ratio);

    ComandoTextEdit = new QTextEdit(this);
    ComandoTextEdit->setGeometry(10 * m_ratio, label1->y() + label1->height() + (40 * m_ratio) , m_applicationWidth - (20 * m_ratio), h);
    ComandoTextEdit->setWordWrapMode(QTextOption::NoWrap);
    QColor myWhite = QColor(Qt::white);
    QPalette p = ComandoTextEdit->palette();
    p.setColor(QPalette::Base, myWhite);
    p.setColor(QPalette::Background, myWhite);
    ComandoTextEdit->setPalette(p);

    runButton  = new QPushButton(this);
    runButton->setText("&Run");
    runButton->setGeometry(m_applicationWidth - (210 * m_ratio), ComandoTextEdit->height() + ComandoTextEdit->y(), 200 * m_ratio, 80 * m_ratio);
    QObject::connect(runButton, SIGNAL(clicked()), this, SLOT(slotRun()));

    saidaComboBox = new QComboBox(this);
    saidaComboBox->setGeometry(0,ComandoTextEdit->height() + ComandoTextEdit->y(),200 * m_ratio,80* m_ratio);
    saidaComboBox->insertItem(0,"Tela");
    saidaComboBox->insertItem(1,"Bluetooth");
    saidaComboBox->insertItem(2,"Todos");
}

void KlogoTurtleMobileApp::slotRun()
{
    mesa = new MesaMobile();
    mesa->plainTextEditContents = ComandoTextEdit->toPlainText();
    mesa->show();
}

void KlogoTurtleMobileApp::notifyPortraitMode(Qt::ScreenOrientation orientation)
{
    switch (orientation) {
    case Qt::LandscapeOrientation:
    case Qt::InvertedLandscapeOrientation:
        setIsPortraitMode(false);
        break;
    case Qt::PortraitOrientation:
    case Qt::InvertedPortraitOrientation:
        setIsPortraitMode(true);
        break;
    default:
        break;
    }
}

void KlogoTurtleMobileApp::setIsPortraitMode(const bool newMode)
{
    if (m_isPortraitMode != newMode) {
        m_isPortraitMode = newMode;
        m_hMargin = m_isPortraitMode ? 20 * ratio() : 50 * ratio();
        emit portraitModeChanged();
        emit hMarginChanged();
    }
}


void KlogoTurtleMobileApp::setApplicationWidth(const int newWidth)
{
    if (newWidth != m_applicationWidth) {
        m_applicationWidth = newWidth;
        emit applicationWidthChanged();
    }
}

QString KlogoTurtleMobileApp::i18n(QString str){
    return str;
}
