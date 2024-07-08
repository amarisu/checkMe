#ifndef MARK_H
#define MARK_H


namespace CheckMe{
struct Mark;
}


struct Mark
{
    Mark(unsigned char itemPos_);

    unsigned char itemPos;
    bool isEnabled;
};

#endif // MARK_H
