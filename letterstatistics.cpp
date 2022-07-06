#include "letterstatistics.h"

bool le(const unsigned int &first, const unsigned int &second) {
    return first <= second;
}

bool lt(const unsigned int &first, const unsigned int &second) {
    return first < second;
}

bool ge(const unsigned int &first, const unsigned int &second) {
    return first >= second;
}

bool gr(const unsigned int &first, const unsigned int &second) {
    return first > second;
}

LetterStatistics::LetterStatistics(): overall_letters(0), function_pointer(&LetterStatistics::AddLetter) {
};

bool LetterStatistics::ReadLetter(const Letter &letter) {
    if (letter.isLetter()) {
        Letter letter_lowercase = letter.toLower();
        std::lock_guard<std::mutex> lock(mutex);
        letters_appearence[letter_lowercase]++;
        overall_letters++;
        (this->*function_pointer)(letter_lowercase);
        return true;
    }
    return false;
}

LettersList LetterStatistics::GetOftenLettersList() const {
    return often_letters_list;
}

LettersList LetterStatistics::GetRareLettersList() const {
    return rare_letters_list;
}

unsigned int LetterStatistics::GetLetterFrequency(const Letter &letter) const {
    return letters_appearence[letter];
}

double LetterStatistics::GetLetterProbability(const Letter &letter) const {
    if (overall_letters == 0) {
        return 0;
    }
    return (double)letters_appearence[letter] / (double)overall_letters;
}

void LetterStatistics::InsertLetterIntoLettersList(LettersList &list, const Letter &letter, const unsigned int &position) {
    int letterPosition = CheckLetterExistance(list, letter);
    if (letterPosition == (int)position) {
        return;
    }
    if (letterPosition != -1) {
        if (letterPosition < (int)position) {
            return;
        }
        list.removeAt(letterPosition);
    } else {
        list.removeLast();
    }
    list.insert(position, letter);
}

void LetterStatistics::FillingRecalculateLettersList(LettersList &list, const Letter &letter, const unsigned int &max_count, bool (&compare)(const unsigned int&, const unsigned int&)) {
    unsigned int i = 0;
    while (i < (unsigned int)list.size() && compare(letters_appearence[list[i]], letters_appearence[letter])) {
        i++;
    }
    if ((unsigned int)list.size() < max_count && CheckLetterExistance(list, letter) == -1) {
        list.insert(i, letter);
    } else if (i < (unsigned int)list.size()) {
        InsertLetterIntoLettersList(list, letter, i);
    }
}

void LetterStatistics::RecalculateLettersList(LettersList &list, const Letter &letter, bool (&compare)(const unsigned int&, const unsigned int&)) {
    for (unsigned int i = 0; i < (unsigned int)list.size(); i++) {
        if (compare(letters_appearence[list[i]], letters_appearence[letter])) {
            InsertLetterIntoLettersList(list, letter, i);
            return;
        }
    }
}

void LetterStatistics::AddLetter(const Letter &letter) {
    FillingRecalculateLettersList(often_letters_list, letter, OFTEN_LETTERS_MAX_COUNT, ge);
    FillingRecalculateLettersList(rare_letters_list, letter, RARE_LETTERS_MAX_COUNT, le);
    if (often_letters_list.size() == OFTEN_LETTERS_MAX_COUNT && rare_letters_list.size() == RARE_LETTERS_MAX_COUNT) {
        function_pointer = &LetterStatistics::CheckLetter;
    }
}

void LetterStatistics::CheckLetter(const Letter &letter) {
    RecalculateLettersList(often_letters_list, letter, lt);
    RecalculateLettersList(rare_letters_list, letter, gr);
}

int LetterStatistics::CheckLetterExistance(const LettersList &list, const Letter &letter) {
    for (unsigned int i = 0; i < (unsigned int)list.size(); i++) {
        if (list[i] == letter) {
            return i;
        }
    }
    return -1;
}
