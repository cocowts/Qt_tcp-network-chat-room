#include "mainwindow.h"

#include <QIcon>
#include <QGroupBox>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), m_pLoginDialog(new LoginDalog(this))
{
    setStyleSheet("background-color:rgb(200,200,200)");

    initUi();
    initControl();
    initSlot();
}

void MainWindow::initUi()
{
    QVBoxLayout *pLayout = new QVBoxLayout(this);

    pLayout->addWidget(initMsgBox());

    initInputBox();

    pLayout->addWidget(m_pInputBox);

    setLayout(pLayout);
    setWindowTitle(tr("    TIM"));
    setMinimumSize(600, 450);
    setWindowIcon(QIcon(":/image/image/logo.png"));
}

void MainWindow::initControl()
{
    m_client = new TcpClient(this);
    m_client->setHandler(this);
}

void MainWindow::initSlot()
{
    connect(m_pLoginOutBtn, &QPushButton::clicked, this, &MainWindow::slotLogInOutBtnClicked);
    connect(m_pSendMsgBtn, &QPushButton::clicked, this, &MainWindow::slotSendBtnClicked);
    connect(m_pListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::slotListWidgetMenuRequested);
}

QWidget *MainWindow::initMsgBox()
{
    QGroupBox *pGroupBox = new QGroupBox(this);
    QHBoxLayout *pLayout = new QHBoxLayout;

    m_pMessageEdit = new QPlainTextEdit(pGroupBox);
    m_pListWidget = new QListWidget(pGroupBox);
    m_pListWidgetMenu = new QMenu(m_pListWidget);

    m_pListWidgetMenu->addAction(tr("silent"), this, &MainWindow::slotListWidgetActionClicked)->setObjectName("silent");
    m_pListWidgetMenu->addAction(tr("recover"), this, &MainWindow::slotListWidgetActionClicked)->setObjectName("recover");
    m_pListWidgetMenu->addSeparator();
    m_pListWidgetMenu->addAction(tr("kick"), this, &MainWindow::slotListWidgetActionClicked)->setObjectName("kick");

    m_pMessageEdit->setReadOnly(true);
    m_pMessageEdit->setFocusPolicy(Qt::NoFocus);
    m_pListWidget->setFocusPolicy(Qt::NoFocus);
    m_pListWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    pLayout->addWidget(m_pMessageEdit, 7);
    pLayout->addWidget(m_pListWidget, 3);

    pGroupBox->setLayout(pLayout);
    pGroupBox->setTitle(tr("Message"));

    return pGroupBox;
}

void MainWindow::initInputBox()
{
    QGridLayout *pLayout = new QGridLayout;

    m_pInputBox = new QGroupBox(this);
    m_pInputEdit = new QPlainTextEdit(m_pInputBox);
    m_pStatusLbl = new QLabel(m_pInputBox);
    m_pLoginOutBtn = new QPushButton(m_pInputBox);
    m_pSendMsgBtn = new QPushButton(m_pInputBox);

    m_pStatusLbl->setText(tr("Status : not connected"));
    m_pLoginOutBtn->setText(tr("Login"));
    m_pSendMsgBtn->setText(tr("Send"));
    m_pSendMsgBtn->setEnabled(false);
    m_pInputEdit->installEventFilter(this);

    pLayout->addWidget(m_pInputEdit,   0, 0, 1, 5);
    pLayout->addWidget(m_pStatusLbl,   1, 0, 1, 2);
    pLayout->addWidget(m_pLoginOutBtn, 1, 3, 1, 1);
    pLayout->addWidget(m_pSendMsgBtn,  1, 4, 1, 1);

    m_pInputBox->setLayout(pLayout);
    m_pInputBox->setTitle(tr("User"));
    m_pInputBox->setFixedHeight(200);
}

void MainWindow::setControlEnabled(bool enabled)
{
    m_pStatusLbl->setText(enabled ? tr("Status : logged") : tr("Status : not connected"));
    m_pLoginOutBtn->setText(enabled ? tr("Logout") : tr("Login"));
    m_pSendMsgBtn->setEnabled(enabled);

    if (!enabled)
        m_pListWidget->clear();
}
