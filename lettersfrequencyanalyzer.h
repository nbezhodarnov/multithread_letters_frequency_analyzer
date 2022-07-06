#ifndef LETTERSFREQUENCYANALYZER_H
#define LETTERSFREQUENCYANALYZER_H

#include <QTextStream>
#include <QString>
#include <QVector>
#include <memory>
#include <mutex>

#include "letterstatistics.h"
#include "filereader.h"

namespace Commands {
    static const QString OFTEN_LETTERS_LIST = "often";
    static const QString RARE_LETTERS_LIST = "rare";
    static const QString LETTER_FREQUENCY = "frequency";
    static const QString LETTER_PROBABILITY = "probability";
    static const QString ADD_FILE = "add";
    static const QString EXIT = "exit";
    static const QString HELP = "help";
}

class LettersFrequencyAnalyzer
{
public:
    LettersFrequencyAnalyzer(const LettersFrequencyAnalyzer&) = delete;
    void operator=(const LettersFrequencyAnalyzer&) = delete;
    int StartConsole();

    static std::shared_ptr<LettersFrequencyAnalyzer> GetInstancePtr();
    static LettersFrequencyAnalyzer& GetInstanceReferrence();

protected:
    LettersFrequencyAnalyzer();

    static std::shared_ptr<LettersFrequencyAnalyzer> instance;
    static std::mutex instance_mutex;

private:
    QTextStream in, out;
    FileReader filereader;
    LetterStatistics statistics;

    QString ReadCommand();
    void OutputLettersList(const QVector<QChar>&, const QString&);
    FilesList SplitFileNames(const QString&);
};

#endif // LETTERSFREQUENCYANALYZER_H
