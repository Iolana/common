#ifndef SPELLHIGHLIGHTER_H
#define SPELLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Hunspell;
class SpellHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SpellHighlighter(QTextDocument *parent = 0, Hunspell* speller = 0);
    void stop(){ stopped = true; }
    void start(){ stopped = false; }
    bool isStopped() { return stopped; }
 protected:
     void highlightBlock(const QString &text);

private:
    Hunspell* mHunspell;
    bool stopped;
    
};

#endif // SPELLHIGHLIGHTER_H
