#include "SpellHighlighter.h"
#include "hunspell/hunspell.hxx"
#include <QMessageBox>
#include <QDebug>


SpellHighlighter::SpellHighlighter(QTextDocument *parent, Hunspell* speller) :
    QSyntaxHighlighter(parent)
{
    stopped = true;
    if(speller == 0)
    {
        QMessageBox::critical(0, tr("Error"), tr("Given speller instance is null."));
        return;
    }
    mHunspell = speller;
}
void SpellHighlighter::highlightBlock(const QString &text)
{
    if(stopped)
        return;
    QTextCharFormat format;
    format.setUnderlineColor(QColor(Qt::red));
    format.setUnderlineStyle(QTextCharFormat::UnderlineStyle::SpellCheckUnderline);
    QRegExp expression("\\b\\w+\\b");
    int index = expression.indexIn(text);
    while (index >= 0)
    {
        int length = expression.matchedLength();
        if(mHunspell->spell(expression.cap().toUtf8().constData()) == 0)
            setFormat(index, length, format);
        index = expression.indexIn(text, index + length);
    }
}
