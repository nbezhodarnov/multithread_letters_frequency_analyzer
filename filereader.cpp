#include "filereader.h"

FileReader::FileReader(LetterStatistics &stats): statistics(stats), out(stdout) {
}

void FileReader::ReadFiles(const FilesList &list) {
    std::vector<std::thread> threads(list.size());
    for (unsigned int i = 0; i < (unsigned int)threads.size(); i++) {
        File file_name = list[i];
        threads[i] = std::thread([this, file_name] {ReadFile(file_name);});
    }
    for (unsigned int i = 0; i < (unsigned int)threads.size(); i++) {
        threads[i].join();
    }
}

void FileReader::ReadFile(const File &file_name) {
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        OutputStatus("Couldn't read file: " + file_name);
        return;
    }
    QTextStream file_stream(&file);
    while (!file_stream.atEnd()) {
        QString letter = file_stream.read(1);
        statistics.ReadLetter(letter[0]);
    }
    file.close();
    OutputStatus(file_name + " has been read successfully!");
}

void FileReader::OutputStatus(const QString &status) {
    std::lock_guard<std::mutex> lock(mutex);
    out << status << '\n' << Qt::flush;
}
