#include <QCoreApplication>

#include <thread>

#include "lettersfrequencyanalyzer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::thread console_thread([] {
        LettersFrequencyAnalyzer &analyzer = LettersFrequencyAnalyzer::GetInstanceReferrence();
        analyzer.StartConsole();
    });
    console_thread.join();

    a.quit();
    return 0;
}
