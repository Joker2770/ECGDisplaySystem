#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <QTimer>

using namespace std;

namespace Ui
{
    class HomeWindow;
}

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

public:
    quint64 m_out_text_size;

private slots:
    void OnTimer1TimeOut();
    void OnTimer2TimeOut();
    void OnSave();
    void OnAutoSave();
    void OnClear();

private:
    Ui::HomeWindow *ui;

    QTimer *m_timer_1;

    QTimer *m_timer_2;

    QString m_qsAutoSavePath;
};

#endif // HOMEWINDOW_H
