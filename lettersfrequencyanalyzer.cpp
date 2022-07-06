#include "lettersfrequencyanalyzer.h"

std::shared_ptr<LettersFrequencyAnalyzer> LettersFrequencyAnalyzer::instance = nullptr;
std::mutex LettersFrequencyAnalyzer::instance_mutex;

LettersFrequencyAnalyzer::LettersFrequencyAnalyzer(): in(stdin), out(stdout), filereader(statistics) {
}

std::shared_ptr<LettersFrequencyAnalyzer> LettersFrequencyAnalyzer::GetInstancePtr() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    if (instance == nullptr) {
        instance = std::shared_ptr<LettersFrequencyAnalyzer>(new LettersFrequencyAnalyzer());
    }
    return instance;
}

LettersFrequencyAnalyzer& LettersFrequencyAnalyzer::GetInstanceReferrence() {
    return *GetInstancePtr();
}

int LettersFrequencyAnalyzer::StartConsole() {
    bool continue_work = true;
    while (continue_work) {
        QString input_line = ReadCommand(), command = input_line.section(' ', 0, 0);
        if (command == Commands::OFTEN_LETTERS_LIST) {
            OutputLettersList(statistics.GetOftenLettersList(), "The most often letters");
        } else if (command == Commands::RARE_LETTERS_LIST) {
            OutputLettersList(statistics.GetRareLettersList(), "The most rare letters");
        } else if (command == Commands::LETTER_FREQUENCY || command == Commands::LETTER_PROBABILITY) {
            QString letter = input_line.section(' ', 1, 1);
            if (letter.size() != 1 || !letter[0].isLetter()) {
                out << "You must specify a letter!\n";
                continue;
            }
            if (command == Commands::LETTER_FREQUENCY) {
                out << "Letter " << letter[0] << " frequency: " << statistics.GetLetterFrequency(letter[0]) << '\n';
            } else {
                out << "Letter " << letter[0] << " probability: " << statistics.GetLetterProbability(letter[0]) << '\n';
            }
        } else if (command == Commands::ADD_FILE) {
            filereader.ReadFiles(SplitFileNames(input_line.section(' ', 1).trimmed()));
        } else if (command == Commands::HELP) {
            out << Commands::OFTEN_LETTERS_LIST << " - see (up to) " << OFTEN_LETTERS_MAX_COUNT << " most often letters\n";
            out << Commands::RARE_LETTERS_LIST << " - see (up to) " << RARE_LETTERS_MAX_COUNT << " most rare letters\n";
            out << Commands::LETTER_FREQUENCY << " <letter> - see specified letter frequency\n";
            out << Commands::LETTER_PROBABILITY << " <letter> - see specified letter probability\n";
            out << Commands::ADD_FILE << " <file(s)> - add file(s) to recalculate letters frequency\n";
            out << Commands::EXIT << " - exit the program\n";
        } else if (command == Commands::EXIT) {
            out << "Exiting the program!\n";
            continue_work = false;
        } else {
            out << "Unrecognized command!\n";
        }
    }
    return 0;
}

QString LettersFrequencyAnalyzer::ReadCommand() {
    QString result;
    while (result == "") {
        out << "Input command: " << Qt::flush;
        result = in.readLine();
    }
    return result.trimmed();
}

void LettersFrequencyAnalyzer::OutputLettersList(const QVector<QChar> &list, const QString &message) {
    if (list.empty()) {
        out << "The list is empty now!\n";
        return;
    }
    out << message << ": ";
    for(int i = 0; i < list.size(); i++) {
        out << list[i] << ' ';
    }
    out << '\n';
}


FilesList LettersFrequencyAnalyzer::SplitFileNames(const QString &line) {
    if (line.isEmpty()) {
        return FilesList();
    }
    FilesList result;
    result.append("");
    unsigned int i = 0, j = 0;
    bool not_in_brackets = true;
    while (i < (unsigned int)line.size()) {
        if (line[i] == "\"" || line[i] == "\'") {
            not_in_brackets = !not_in_brackets;
        } else if (line[i] == ' ' && (line[i - 1] != '\\' && not_in_brackets)) {
            if (result[j] != "") {
                result.append("");
                j++;
            }
        } else if (line[i] != '\\' || (i < (unsigned int)line.size() - 1 && line[i + 1] != ' ')) {
            result[j] += line[i];
        }
        i++;
    }
    return result;
}
