#ifndef LOGINDALOG_H
#define LOGINDALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QTimer>

class LoginDalog : public QDialog
{
    Q_OBJECT
protected:
    using validUserIdFunc = bool(*)(const QString &);

public:
    explicit LoginDalog(QWidget *parent = nullptr);

    QString user();
    QString password();

    void setValidUserId(validUserIdFunc func);

protected slots:
    void slotCancelBtnClicked();
    void slotLoginBtnClicked();
    void slotTimeout();

protected:
    void initUi();
    void initSlot();

    QString getCaptcha();
    Qt::GlobalColor *getColor();

    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLineEdit m_userEdit;
    QLineEdit m_passwordEdit;
    QLineEdit m_captchaEdit;
    QPushButton m_cancelBtn;
    QPushButton m_loginBtn;

    QString m_user;
    QString m_password;
    QString m_captcha;
    Qt::GlobalColor *m_color = nullptr;
    validUserIdFunc m_vf = nullptr;

    QTimer m_timer;
};

#endif // LOGINDALOG_H
