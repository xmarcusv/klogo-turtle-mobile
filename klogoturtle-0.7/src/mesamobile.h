#ifndef MESAMOBILE_H
#define MESAMOBILE_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

#include "desenho.h"

namespace Ui {
class MesaMobile;
}

class MesaMobile : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int applicationWidth READ applicationWidth WRITE setApplicationWidth NOTIFY applicationWidthChanged)
    Q_PROPERTY(bool isPortraitMode READ isPortraitMode WRITE setIsPortraitMode NOTIFY portraitModeChanged)
    Q_PROPERTY(qreal ratio READ ratio CONSTANT)
    Q_PROPERTY(qreal hMargin READ hMargin NOTIFY hMarginChanged)
    Q_PROPERTY(qreal sliderHandleWidth READ sliderHandleWidth CONSTANT)
    Q_PROPERTY(qreal sliderHandleHeight READ sliderHandleHeight CONSTANT)
    Q_PROPERTY(qreal sliderGapWidth READ sliderGapWidth CONSTANT)

public:
    explicit MesaMobile(QWidget *parent = 0);
    ~MesaMobile();
    qreal hMargin() const { return m_hMargin; }
    qreal ratio() const { return m_ratio; }
    qreal sliderHandleHeight()  { return m_sliderHandleHeight; }
    qreal sliderGapWidth()  { return m_sliderGapWidth; }
    qreal sliderHandleWidth()  { return m_sliderHandleWidth; }

    bool isPortraitMode() const { return m_isPortraitMode; }
    void setIsPortraitMode(const bool newMode);

    int applicationWidth() const { return m_applicationWidth; }
    void setApplicationWidth(const int newWidth);

    int Repete(int param, int par_ini, QStringList lines);
    int Exe_comando(int pos_comando, QStringList lines);
    int pos_end_repete;
    int find_to(QString to_name);
    int n_to;
    int qual_comando;
    QPushButton *voltarButton;
    QString plainTextEditContents;
    QString idioma_escolhido;
    QString mensagemText;

    void comando_to(QString comando, QString nome, QStringList lines);
    void stop_error();
    void apaga_to();

protected:
    qreal getSizeWithRatio(const qreal height) { return ratio() * height; }
    void initView();
    void run();
    void showEvent(QShowEvent *event);

public slots:
    void slotVoltar();

signals:
    void applicationWidthChanged();
    void portraitModeChanged();
    void hMarginChanged();

private:
    Ui::MesaMobile *ui;
    int m_applicationWidth;
    int m_applicationHeight;
    qreal m_ratio;
    qreal m_hMargin;
    qreal m_sliderHandleHeight, m_sliderHandleWidth, m_sliderGapWidth;
    bool m_isPortraitMode;

    QString temp_comando_to;
    QString guarda_to [10] [2];
    Desenho *des;
    QString i18n(QString str);
    int tem_erro;
};

#endif // MESAMOBILE_H
