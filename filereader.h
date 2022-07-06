#ifndef FILEREADER_H
#define FILEREADER_H

#include <QStringList>
#include <QTextStream>
#include <QFile>

#include <thread>
#include <vector>
#include <mutex>

#include "letterstatistics.h"

#define FilesList QStringList
#define File QString

class FileReader
{
public:
    FileReader(LetterStatistics&);
    void ReadFiles(const FilesList&);

private:
    LetterStatistics &statistics;
    std::mutex mutex;
    QTextStream out;

    void ReadFile(const File&);
    void OutputStatus(const QString&);
};

#endif // FILEREADER_H
