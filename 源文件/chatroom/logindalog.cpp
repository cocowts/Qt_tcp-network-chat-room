#include "logindalog.h"

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QTime>

LoginDalog::LoginDalog(QWidget *parent) : QDialog(parent, Qt::Drawer | Qt::Desktop)
{
    initUi();
    initSlot();

    qsrand(static_cast<uint>(QTime::currentTime().second()*1000 + QTime::currentTime().msec()));
    m_captcha = getCaptcha();
    m_color = getColor();

    m_timer.setParent(this);
    m_timer.start(100);
}

void LoginDalog::initUi()
{
    m_cancelBtn.setText(tr("Cancel"));
    m_loginBtn.setText(tr("Login"));

    m_userEdit.setMaxLength(8);
    m_passwordEdit.setMaxLength(8);
    m_passwordEdit.setEchoMode(QLineEdit::Password);
    m_captchaEdit.setMaxLength(4);
    m_captchaEdit.setFixedWidth(110);

    QGridLayout *pLayout = new QGridLayout();
    pLayout->addWidget(new QLabel(tr("User")),     0, 0, 1, 1);
    pLayout->addWidget(&m_userEdit,                0, 1, 1, 2);
    pLayout->addWidget(new QLabel(tr("PassWord")), 1, 0, 1, 1);
    pLayout->addWidget(&m_passwordEdit,            1, 1, 1, 2);
    pLayout->addWidget(new QLabel(tr("Captcha")),  2, 0, 1, 2);
    pLayout->addWidget(&m_captchaEdit,             2, 1, 1, 1);
    pLayout->addWidget(&m_cancelBtn,               3, 1, 1, 1);
    pLayout->addWidget(&m_loginBtn,                3, 2, 1, 1);
    pLayout->setSpacing(10);

    setLayout(pLayout);
    setWindowTitle(tr("Login"));
}

void LoginDalog::initSlot()
{
    connect(&m_cancelBtn, SIGNAL(clicked()), this, SLOT(slotCancelBtnClicked()));
    connect(&m_loginBtn, SIGNAL(clicked()), this, SLOT(slotLoginBtnClicked()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

QString LoginDalog::user()
{
    return m_user;
}

QString LoginDalog::password()
{
    return m_password;
}

void LoginDalog::setValidUserId(validUserIdFunc func)
{
    m_vf = func;
}

QString LoginDalog::getCaptcha()
{
    QString ret;

    for (int i=0; i<4; ++i)
    {
        int c = (qrand() % 2) ? 'a' : 'A';
        ret += static_cast<QChar>(c + (qrand() % 26));
    }

    return ret;
}

Qt::GlobalColor *LoginDalog::getColor()
{
    static Qt::GlobalColor color[4];

    for (int i=0; i<4; ++i)
    {
        color[i] = static_cast<Qt::GlobalColor>(3 + (qrand() % 16));
    }

    return color;
}

void LoginDalog::showEvent(QShowEvent *event)
{
    qsrand(static_cast<uint>(QTime::currentTime().second()*1000 + QTime::currentTime().msec()));
    m_captcha = getCaptcha();
    m_color = getColor();
    m_captchaEdit.clear();

    QDialog::showEvent(event);
}

void LoginDalog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int x = m_loginBtn.x();
    int y = m_captchaEdit.y();
    int w = m_loginBtn.width();
    int h = m_captchaEdit.height();
    int setp = w / 4;

    painter.fillRect(x, y, w, h, Qt::white);

    for (int i=0; i<150; ++i)
    {
        painter.setPen(m_color[i%4]);
        painter.drawPoint(x+qrand()%w, y+qrand()%h);
    }

    for (int i=0; i<4; ++i)
    {
        painter.setPen(m_color[i]);
        painter.drawText(QRect(x+setp*i, y, setp, h), Qt::AlignCenter, m_captcha.at(i));
    }

    QDialog::paintEvent(event);
}

void LoginDalog::slotCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void LoginDalog::slotLoginBtnClicked()
{
    m_user = m_userEdit.text().trimmed();
    m_password = m_passwordEdit.text();

    if (m_captcha.toLower() == m_captchaEdit.text().trimmed().toLower())
    {
        if (m_user.isEmpty() || m_password.isEmpty())
        {
            QMessageBox(QMessageBox::Information, tr("Info"), tr("Password or Username is invalid")).exec();
        }
        else if ((m_vf != nullptr) && !m_vf(m_user))
        {
            QMessageBox(QMessageBox::Information, tr("Info"), tr("Username is invalid, please re-enter")).exec();
        }
        else
        {
            done(QDialog::Accepted);
        }
    }
    else
    {
        QMessageBox(QMessageBox::Information, tr("Info"), tr("Captcha is invalid")).exec();

        m_captcha = getCaptcha();

        qsrand(static_cast<uint>(QTime::currentTime().second()*1000 + QTime::currentTime().msec()));
    }
}

void LoginDalog::slotTimeout()
{
    m_color = getColor();

    update();
}
