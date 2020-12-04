#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QListWidget>
#include <QMenu>

#include "logindalog.h"
#include "tcpclient.h"
#include "textmessagehandler.h"
#include "textmessgae.h"

class MainWindow : public QWidget, public TextMessageHandler
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void hanle(QTcpSocket &socket, TextMessgae &message) override;

protected slots:
    void slotLogInOutBtnClicked();
    void slotSendBtnClicked();
    void slotListWidgetMenuRequested(const QPoint &pos);
    void slotListWidgetActionClicked();

protected:
    void initUi();
    void initControl();
    void initSlot();

    QWidget *initMsgBox();
    void initInputBox();

    void setControlEnabled(bool enabled);
    QString getCheckedUserId();

    void CONN_handler(QTcpSocket &socket, TextMessgae &message);
    void DSCN_handler(QTcpSocket &socket, TextMessgae &message);
    void LIOK_handler(QTcpSocket &socket, TextMessgae &message);
    void LIER_handler(QTcpSocket &socket, TextMessgae &message);
    void MSGA_handler(QTcpSocket &socket, TextMessgae &message);
    void USER_handler(QTcpSocket &socket, TextMessgae &message);
    void CTRL_handler(QTcpSocket &socket, TextMessgae &message);

    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QGroupBox *m_pInputBox = nullptr;
    QPlainTextEdit *m_pMessageEdit = nullptr;
    QPlainTextEdit *m_pInputEdit = nullptr;
    QListWidget *m_pListWidget = nullptr;
    QMenu *m_pListWidgetMenu = nullptr;

    QLabel *m_pStatusLbl = nullptr;
    QPushButton *m_pLoginOutBtn = nullptr;
    QPushButton *m_pSendMsgBtn = nullptr;
    LoginDalog *m_pLoginDialog = nullptr;

    TcpClient *m_client = nullptr;

    QString m_level;
};
#endif // MAINWINDOW_H
