/*  
 *  COPYRIGHT (c) 2015 Leben Asa. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <future>
#include <QtDebug>
#include <QtEvents>
#include <QCoreApplication>
#include "../media_service.h"

class App : public QObject
{
    Q_OBJECT
    MediaService *m_service;
    std::future<void> m_fut;
public:
    App(QObject *parent = nullptr)
        : QObject{ parent }, m_service{ new MediaService(this) }
    {
        if (!m_service->isValid())
        {
            qFatal("Failed to open connection to media player.");
            qApp->quit();
        }
    }

    void printVariant(const QVariant &v)
    {
        qInfo() << v;
    }

    void printMap(const QVariantMap &v)
    {
        for (auto i = v.begin(); i != v.end(); ++i)
            qDebug() << i.key() << ": " << i.value();
    }

    void start()
    {
        while (true)
            QCoreApplication::instance()->sendEvent(this, new QKeyEvent(QKeyEvent::KeyPress, std::getchar(), Qt::NoModifier));
    }

    void startAsync()
    {
        m_fut = std::async(std::launch::async, [this](){ start(); });
    }

    bool event(QEvent *ev)
    {
        if (ev->type() == QEvent::KeyPress)
        {
            auto key = static_cast<QKeyEvent*>(ev);
            switch (key->key())
            {
            case Qt::Key_Space:
                m_service->pause();
                return true;
            case Qt::Key_Enter:
                m_service->play();
                return true;
            case Qt::Key_Escape:
                m_service->stop();
                return true;
            case Qt::Key_Minus:
                m_service->volumeDown(1);
                return true;
            case Qt::Key_Plus:
                m_service->volumeUp(1);
                return true;
            case Qt::Key_0:
                m_service->mute();
                return true;
            case Qt::Key_Comma:
                m_service->prev();
                return true;
            case Qt::Key_Period:
                m_service->next();
                return true;
            case Qt::Key_O:
                m_service->showOSD();
                return true;
            case Qt::Key_Question:
                printMap(m_service->metadata());
                return true;
            case Qt::Key_P:
                qDebug() << m_service->position();
                return true;
            case Qt::Key_Q:
                QCoreApplication::instance()->quit();
                return true;
            }
            return false;
        }
        return QObject::event(ev);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app{ argc, argv };

    App myapp;
    myapp.startAsync();

    return app.exec();
}

#include "metadata_reader_test.moc"
