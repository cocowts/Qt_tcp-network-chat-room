#include "mainwindow.h"

#include <QMessageBox>
#include <QCursor>
#include <QKeyEvent>

static bool validUserIdFunc(const QString &id)
{
    bool ret = true;
    QString invalid = "~`!@#$%^&*()_-+={}[]|\\:;\'\",.?/<>";

    for (int i=0; i<invalid.length(); ++i)
    {
        if (id.contains(invalid[i]))
        {
            ret = false;
            break;
        }
    }

    return ret;
}

MainWindow::~MainWindow()
{
    m_client->close();
}

QString MainWindow::getCheckedUserId()
{
    QString ret;

    for (int i=0; i<m_pListWidget->count(); ++i)
    {
        QListWidgetItem *item = m_pListWidget->item(i);

        if (item->checkState() == Qt::Checked)
        {
            ret += item->text() + '\r';
        }
    }

    return ret;
}

void MainWindow::CONN_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)
    Q_UNUSED(message)
}

void MainWindow::DSCN_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)
    Q_UNUSED(message)

    setControlEnabled(false);
    m_pInputBox->setTitle(tr("user"));
    m_client->close();
}

void MainWindow::LIOK_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    QStringList rl = message.data().split("\r");
    QString id = rl[0];
    QString status;

    if (rl.count() == 3)
    {
        status = rl[1];
        m_level = rl[2];
    }

    if (status == "silent")
    {
        setControlEnabled(true);
        m_pInputBox->setTitle(id);
        m_pSendMsgBtn->setEnabled(false);
    }
    else if (status == "kick")
    {
        m_client->close();

        QMessageBox::information(this, tr("Info"), tr("The current account is not allowed to enter the chat room"));
    }
    else
    {
        setControlEnabled(true);
        m_pInputBox->setTitle(id);
    }
}

void MainWindow::LIER_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)
    Q_UNUSED(message)

    QMessageBox::critical(this, tr("Error"), tr("Authentication failed"));
    m_client->close();
}

void MainWindow::MSGA_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)
    Q_UNUSED(message)

    m_pMessageEdit->appendHtml(message.data());

    activateWindow();
}

void MainWindow::USER_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    QStringList users = message.data().split("\r", QString::SkipEmptyParts);
    QStringList checked = getCheckedUserId().split("\r", QString::SkipEmptyParts);

    m_pListWidget->clear();

    for (const auto &user : users)
    {
        QListWidgetItem *item = new QListWidgetItem();

        if (item != nullptr)
        {
            item->setText(user);
            item->setCheckState(Qt::Unchecked);
            m_pListWidget->addItem(item);
        }
    }

    for (int i=0; i<m_pListWidget->count(); ++i)
    {
        QListWidgetItem *item = m_pListWidget->item(i);

        for (int j=0; j<checked.length(); ++j)
        {
            if (checked.at(j) == item->text())
            {
                item->setCheckState(Qt::Checked);
                break;
            }
        }
    }
}

void MainWindow::CTRL_handler(QTcpSocket &socket, TextMessgae &message)
{
    Q_UNUSED(socket)

    if (message.data() == "silent")
    {
        QMessageBox::information(this, tr("Info"), tr("You have been banned by the administrator!"));

        m_pSendMsgBtn->setEnabled(false);
    }
    else if (message.data() == "recover")
    {
        QMessageBox::information(this, tr("Info"), tr("You have been banned by the administrator!"));

        m_pSendMsgBtn->setEnabled(true);
    }
    else if (message.data() == "kick")
    {
        QMessageBox::information(this, tr("Info"), tr("You have been removed from the chat room!"));

        m_client->close();
    }
}

void MainWindow::hanle(QTcpSocket &socket, TextMessgae &message)
{
    if (message.type() == "CONN")
        CONN_handler(socket, message);
    else if (message.type() == "DSCN")
        DSCN_handler(socket, message);
    else if (message.type() == "LIOK")
        LIOK_handler(socket, message);
    else if (message.type() == "LIER")
        LIER_handler(socket, message);
    else if (message.type() == "MSGA")
        MSGA_handler(socket, message);
    else if (message.type() == "USER")
        USER_handler(socket, message);
    else if (message.type() == "CTRL")
        CTRL_handler(socket, message);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if ((watched == m_pInputEdit) && (event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ((keyEvent->key() == Qt::Key_Return) && (keyEvent->modifiers() & Qt::ControlModifier))
        {
            slotSendBtnClicked();

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainWindow::slotLogInOutBtnClicked()
{
    if (!m_client->isValid())
    {
        m_pLoginOutBtn->setEnabled(false);

        m_pLoginDialog->setValidUserId(validUserIdFunc);

        if (m_pLoginDialog->exec() == QDialog::Accepted)
        {
            QString user = m_pLoginDialog->user();
            QString password = m_pLoginDialog->password();

            if (m_client->connectToHost("127.0.0.1", 8890))
            {
                m_client->send(TextMessgae("LIGN", user + '\r' + password));
            }
            else
            {
                QMessageBox::critical(this, tr("Error"), tr("Can't connect to server..."));
            }
        }

        m_pLoginOutBtn->setEnabled(true);
    }
    else
    {
       m_client->close();
    }
}

void MainWindow::slotSendBtnClicked()
{
    QString input = m_pInputEdit->toPlainText();

    if (!input.isEmpty())
    {
        QString self = m_pInputBox->title();
        QString uid = getCheckedUserId();
        bool ok = false;

        QString text = QString("<html> \
               <head>   \
               <style>  \
               font{color:gray;} #f{font-size:25px; color: green;} \
               </style> \
               </head>  \
               <body>   \
               <font>%1<br></font><font id=\"f\">%2</font><br> \
               </html>").arg(self + ":\n").arg(input);

        if (uid.isEmpty())
        {
            ok = m_client->send(TextMessgae("MSGA", text));
        }
        else
        {
            QString sid = (uid.indexOf(self) >=0) ? uid : (uid + self + '\r');

            ok = m_client->send(TextMessgae("MSGP", sid + text));
        }

        if (ok)
        {
            m_pInputEdit->clear();
        }
    }
}

void MainWindow::slotListWidgetMenuRequested(const QPoint &pos)
{
    if (m_level == "admin")
    {
        QListWidgetItem *item = m_pListWidget->itemAt(pos);

        if ((item != nullptr) && (item->text() != "admin"))
        {
            m_pListWidgetMenu->exec(QCursor::pos());
        }
    }

    return QWidget::customContextMenuRequested(pos);
}

void MainWindow::slotListWidgetActionClicked()
{
    QAction *act = qobject_cast<QAction*>(sender());

    if (act != nullptr)
    {
        const QList<QListWidgetItem*> &sl = m_pListWidget->selectedItems();

        if (sl.length() > 0)
        {
            QString user = sl.at(0)->text();
            QString tip = tr("Are you sure to ") + act->text() + " [ " + user  + " ] ";

            if (QMessageBox::question(this, tr("Question"), tip, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                QString data = act->objectName() + '\r' + user;

                m_client->send(TextMessgae("ADMN", data));
            }
        }
        else
        {
            QMessageBox::information(this, tr("Info"), tr("Please select a chat member"));
        }
    }
}
