#include "textgrid.hpp"


// TODO: this function should exactly match the behaviour
// of that in TextGrid::Draw(), however at the moment it
// may not do due to the auto-advance and position variables
void TextGrid::Print(std::ostream &os)
{
    int count = 0;

    for(char c: m_text)
    {
        os << c;

        ++ count;
        if(count > 50) break;
    }
    os << std::endl;
}