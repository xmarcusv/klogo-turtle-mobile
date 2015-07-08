#ifndef KLOGOTURTLEMOBILEAPP_H
#define KLOGOTURTLEMOBILEAPP_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include "mesamobile.h"
#include <QComboBox>

namespace Ui {
class KlogoTurtleMobileApp;
}

class KlogoTurtleMobileApp : public QMainWindow
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
    explicit KlogoTurtleMobileApp(QWidget *parent = 0);
    ~KlogoTurtleMobileApp();
    qreal hMargin() const { return m_hMargin; }
    qreal ratio() const { return m_ratio; }
    qreal sliderHandleHeight()  { return m_sliderHandleHeight; }
    qreal sliderGapWidth()  { return m_sliderGapWidth; }
    qreal sliderHandleWidth()  { return m_sliderHandleWidth; }

    bool isPortraitMode() const { return m_isPortraitMode; }
    void setIsPortraitMode(const bool newMode);

    int applicationWidth() const { return m_applicationWidth; }
    void setApplicationWidth(const int newWidth);

    QTextEdit *ComandoTextEdit;
    QLabel *label1;
    QPushButton *runButton;
    MesaMobile *mesa;
    QComboBox *saidaComboBox;

protected:
    qreal getSizeWithRatio(const qreal height) { return ratio() * height; }
    void initView();

signals:
    void applicationWidthChanged();
    void portraitModeChanged();
    void hMarginChanged();

protected slots:
    void notifyPortraitMode(Qt::ScreenOrientation);

public slots:
    void slotRun();

private:
    Ui::KlogoTurtleMobileApp *ui;
    int m_applicationWidth;
    int m_applicationHeight;
    qreal m_ratio;
    qreal m_hMargin;
    qreal m_sliderHandleHeight, m_sliderHandleWidth, m_sliderGapWidth;
    bool m_isPortraitMode;

    QString i18n(QString str);
};

#endif // KLOGOTURTLEMOBILEAPP_H
