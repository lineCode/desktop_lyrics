#include <algorithm>

#include "file_search.h"
#include "desktop_lyrics.h"
using namespace std;

AppEngine::AppEngine(QObject *parent)
    : QObject{ parent }, m_media{ nullptr }, m_lastlrcln{ begin(m_lrcmap) }
{
    m_engine.rootContext()->setContextProperty("app", this);
}

MediaService* AppEngine::media() const
{
    return m_media;
}

void AppEngine::setMedia(MediaService *val)
{
    if (val != m_media && val)
    {
        m_media = val;
        connect(val, &MediaService::positionChanged, this, &AppEngine::setCurrentLine);
        m_engine.rootContext()->setContextProperty("media", val);
        emit mediaChanged(val);
    }
}

QUrl AppEngine::lyricsFile() const
{
    return m_lyricsFile;
}

void AppEngine::setLyricsFile(const QUrl& val)
{
    if (val != m_lyricsFile)
    {
        m_lyricsFile = val;
        emit lyricsFileChanged(val);
        update_lyrics();
        if (m_media)
            store_chosen(m_media->location(), val);
    }
}

void AppEngine::update_lyrics()
{
    if (QFile::exists(lyricsFile().toLocalFile()))
    {
        QFile lf{ lyricsFile().toLocalFile() };
        if (lf.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream lt{ &lf };
            auto ls = lt.readAll().split("\n");
            m_lrcmap = lyrics::parse_all(ls);
        }
        auto ioffset = m_lrcmap.find(-1);
        if (ioffset != end(m_lrcmap))
        {
            setOffset(ioffset->second.toInt());
            m_lrcmap.erase(ioffset);
        }
        m_lastlrcln = begin(m_lrcmap);
    }
    else
    {
        m_lrcmap.clear();
        m_lastlrcln = begin(m_lrcmap);
    }
}

QString AppEngine::currentLine() const
{
    return m_currentLine;
}

void AppEngine::setCurrentLine(int mpos)
{
    if (m_lrcmap.empty())
    {
        m_currentLine = "";
        emit currentLineChanged("");
        return;
    }
    mpos += offset();
    auto pred = [mpos](pair<int, QString> const &v) { return v.first > mpos; };
    auto it = find_if(m_lastlrcln, end(m_lrcmap), pred);
    if (it != end (m_lrcmap) && it->first - mpos > 1000)
        it = end(m_lrcmap);
    if (it == end(m_lrcmap))
        it = find_if(begin(m_lrcmap), end(m_lrcmap), pred);
    if (it != begin(m_lrcmap))
        --it;
    if (it->second != m_currentLine)
    {
        m_currentLine = it->second;
        emit currentLineChanged(m_currentLine);
        m_lastlrcln = it;
    }
}

QUrl AppEngine::lyricsDir() const
{
    return m_lyricsDir;
}

void AppEngine::setLyricsDir(const QUrl& val)
{
    if (val != m_lyricsDir)
    {
        m_lyricsDir = val;
        emit lyricsDirChanged(val);
        refresh_db();
    }
}

void AppEngine::refresh_db()
{
    QDir ld{ m_lyricsDir.toLocalFile() };
    QStringList nfs;
    nfs << "*.lrc" << "*.txt";
    m_lyricsDirFiles = ld.entryInfoList(nfs, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::CaseSensitive,
                                        QDir::Name);
}

QList<QObject *> AppEngine::search_lyrics(const QString &artist, const QString &title)
{
    QList<QObject*> res;
    if (m_media)
    {
        auto last_chosen = load_chosen(m_media->location());
        if (!last_chosen.isEmpty())
        {
            QFileInfo fi{ last_chosen.toLocalFile() };
            auto li = new LyricsMatch(fi.fileName(), last_chosen, 2.0, this);
            res.push_back(li);
        }
    }
    for (const auto &fi : m_lyricsDirFiles)
    {
        auto score = lyrics::match_score(fi.fileName(), artist, title);
        if (score >= 0.5)
        {
            auto li = new LyricsMatch(fi.fileName(), QUrl::fromLocalFile(fi.filePath()), score, this);
            res.push_back(qobject_cast<QObject*>(li));
        }
    }
    sort(res.begin(), res.end(),
         [](QObject *v1, QObject *v2) {
        return v1->property("score").toDouble() > v2->property("score").toDouble(); });
    return res;
}

void AppEngine::loadMainUI()
{
    if (!m_mainUILoaded)
    {
        m_engine.load(QUrl{"qrc:///ui/main.qml"});
        m_mainUILoaded = true;
    }
    else
        emit reloadMainUI();
}

void AppEngine::loadCompactUI()
{
    if (!m_compactUILoaded)
    {
        m_engine.load(QUrl{"qrc:///ui/CompactViewer.qml"});
        m_compactUILoaded = true;
    }
    else
        emit reloadCompactUI();
}

int AppEngine::offset() const
{
    return m_offset;
}

void AppEngine::setOffset(const int& val)
{
    if (val != m_offset)
    {
        m_offset = val;
        emit offsetChanged(val);
    }
}

void AppEngine::store_chosen(const QUrl &music, const QUrl &lyrics)
{
    QSettings s("leben", "desktop_lyrics", this);
    s.beginGroup(music.toLocalFile());
    s.setValue("lyrics", lyrics.toLocalFile());
    s.endGroup();
}

QUrl AppEngine::load_chosen(const QUrl &music)
{
    QSettings s("leben", "desktop_lyrics", this);
    s.beginGroup(music.toLocalFile());
    auto lrc = s.value("lyrics").toString();
    s.endGroup();
    if (lrc.isEmpty())
        return QUrl{ };
    return QUrl::fromLocalFile(lrc);
}

LyricsMatch::LyricsMatch(QObject *parent)
    : QObject{ parent }
{
}

LyricsMatch::LyricsMatch(const QString &name, const QUrl &path,
                         double score, QObject *parent)
    : QObject{ parent }, m_name{ name }, m_path{ path }, m_score{ score }
{
}

QString LyricsMatch::name() const
{
    return m_name;
}

void LyricsMatch::setName(const QString& val)
{
    if (val != m_name)
    {
        m_name = val;
        emit nameChanged(val);
    }
}

QUrl LyricsMatch::path() const
{
    return m_path;
}

void LyricsMatch::setPath(const QUrl& val)
{
    if (val != m_path)
    {
        m_path = val;
        emit pathChanged(val);
    }
}

double LyricsMatch::score() const
{
    return m_score;
}

void LyricsMatch::setScore(double val)
{
    if (val != m_score)
    {
        m_score = val;
        emit scoreChanged(val);
    }
}
