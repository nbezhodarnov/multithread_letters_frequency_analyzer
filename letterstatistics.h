#ifndef LETTERSTATISTICS_H
#define LETTERSTATISTICS_H

#include <QVector>
#include <QMap>

#include <mutex>

#define Letter QChar
#define LettersList QVector<Letter>

#define OFTEN_LETTERS_MAX_COUNT 5
#define RARE_LETTERS_MAX_COUNT 3

bool le(const unsigned int&, const unsigned int&);
bool lt(const unsigned int&, const unsigned int&);
bool ge(const unsigned int&, const unsigned int&);
bool gr(const unsigned int&, const unsigned int&);

class LetterStatistics
{
public:
    LetterStatistics();
    bool ReadLetter(const Letter&);
    LettersList GetOftenLettersList() const;
    LettersList GetRareLettersList() const;
    unsigned int GetLetterFrequency(const Letter&) const;
    double GetLetterProbability(const Letter&) const;

private:
    QMap<Letter, unsigned int> letters_appearence;
    LettersList often_letters_list;
    LettersList rare_letters_list;
    unsigned int overall_letters;
    std::mutex mutex;
    void (LetterStatistics::*function_pointer)(const Letter&);

    void InsertLetterIntoLettersList(LettersList&, const Letter&, const unsigned int&);
    void FillingRecalculateLettersList(LettersList&, const Letter&, const unsigned int&, bool (&)(const unsigned int&, const unsigned int&));
    void RecalculateLettersList(LettersList&, const Letter&, bool (&compare)(const unsigned int&, const unsigned int&));
    void AddLetter(const Letter&);
    void CheckLetter(const Letter&);
    int CheckLetterExistance(const LettersList&, const Letter&);
};

#endif // LETTERSTATISTICS_H
