#include "homewindow.h"
#include "ui_homewindow.h"
#include "global.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <atomic>

// vector<uint16_t> gVecDataQueue;
atomic_char16_t g_data_unit;

HomeWindow::HomeWindow(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::HomeWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::NonModal);

    this->m_timer_1 = new QTimer();
    this->m_timer_2 = new QTimer();

    this->m_timer_1->setTimerType(Qt::PreciseTimer);
    this->m_timer_1->start(3);

    this->m_timer_2->setTimerType(Qt::PreciseTimer);
    this->m_timer_2->start(1000);

    this->m_out_text_size = 0;

    connect(this->m_timer_1, SIGNAL(timeout()), this, SLOT(OnTimer1TimeOut()));
    connect(this->m_timer_2, SIGNAL(timeout()), this, SLOT(OnTimer2TimeOut()));
    connect(this->ui->home_save_btn, SIGNAL(clicked()), this, SLOT(OnSave()));
    connect(this->ui->home_clear_btn, SIGNAL(clicked()), this, SLOT(OnClear()));
    connect(this->ui->home_autosave_cb, SIGNAL(clicked()), this, SLOT(OnAutoSave()));
}

HomeWindow::~HomeWindow()
{
    delete ui;

    if (nullptr != this->m_timer_1)
    {
        delete this->m_timer_1;
        this->m_timer_1 = nullptr;
    }
    if (nullptr != this->m_timer_2)
    {
        delete this->m_timer_2;
        this->m_timer_2 = nullptr;
    }
}

void HomeWindow::OnTimer1TimeOut()
{
    this->m_out_text_size = ui->textBrowser->toPlainText().length();

    // qDebug() << gVecDataSeries;
    if (!gVecDataSeries.empty())
    {
        g_data_unit.store(gVecDataSeries.front(), std::memory_order_relaxed);
        gVecDataSeries.erase(gVecDataSeries.begin());

        if (this->ui->home_data_out_rbtn->isChecked())
        {
            QString sOut;
            if (this->ui->home_datetime_rbtn->isChecked())
            {
                QString qsDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
                sOut = QString::number(g_data_unit.load(std::memory_order_relaxed)) + "," + qsDateTime;
            }
            else
            {
                sOut = QString::number(g_data_unit.load(std::memory_order_relaxed)) + ",";
            }
            ui->textBrowser->append(sOut);
        }

//        if (gVecDataQueue.size() < MAX_QUEUE_SIZE)
//        {
//            gVecDataQueue.push_back(g_data_unit.load(std::memory_order_relaxed));
//        }
//        else if (gVecDataQueue.size() == MAX_QUEUE_SIZE)
//        {
//            gVecDataQueue.erase(gVecDataQueue.begin());
//            gVecDataQueue.push_back(g_data_unit.load(std::memory_order_relaxed));
//        }
//        else
//        {
//            qDebug() << "gVecDataQueue size overflow!";
//            gVecDataQueue.erase(gVecDataQueue.begin());
//        }
    }
}

void HomeWindow::OnTimer2TimeOut()
{

    if (this->m_out_text_size > 1024 * 128 && this->ui->home_autosave_cb->isChecked())
    {
        if (!this->m_qsAutoSavePath.isEmpty())
        {
            QFile file(this->m_qsAutoSavePath);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
            {
                this->ui->home_autosave_cb->setChecked(false);
                return;
            }
            QTextStream out(&file);
            out << ui->textBrowser->toPlainText();
            file.close();

            this->OnClear();
        }
        else
        {
            this->ui->home_autosave_cb->setChecked(false);
        }
    }
}

void HomeWindow::OnSave()
{
    QFileDialog qfd;
    qfd.setWindowModality(Qt::NonModal);
    QString path = qfd.getSaveFileName(this, tr("Save File"), ".", tr("Text Files") + "(*.csv)");
    if (!path.isEmpty())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Write File"), (tr("Cannot open file:") + "\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << ui->textBrowser->toPlainText();
        file.close();
    }
    else
    {
        QMessageBox::warning(this, tr("Path"), tr("You did not input any text."));
    }
}

void HomeWindow::OnAutoSave()
{
    if (this->ui->home_autosave_cb->isChecked())
    {
        QFileDialog qfd;
        qfd.setWindowModality(Qt::NonModal);
        this->m_qsAutoSavePath.clear();
        this->m_qsAutoSavePath = qfd.getSaveFileName(this, tr("Save File"), ".", tr("Text Files") + "(*.csv)");
        if (!this->m_qsAutoSavePath.isEmpty())
        {
            QFile file(this->m_qsAutoSavePath);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
            {
                this->ui->home_autosave_cb->setChecked(false);
                QMessageBox::warning(this, tr("Write File"), (tr("Cannot open file:") + "\n%1").arg(this->m_qsAutoSavePath));
                return;
            }
            QTextStream out(&file);
            out << ui->textBrowser->toPlainText();
            file.close();
            this->OnClear();
        }
        else
        {
            this->ui->home_autosave_cb->setChecked(false);
            QMessageBox::warning(this, tr("Path"), tr("You did not input any text."));
        }
    }
}

void HomeWindow::OnClear()
{
    this->ui->textBrowser->clear();
    this->ui->textBrowser->append("\n");
}
