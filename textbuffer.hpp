#ifndef TEXTBUFFER_HPP
#define TEXTBUFFER_HPP



#include "textgrid.hpp"


#include <string>
#include <vector>
#include <fstream>


class TextBuffer
{


    friend void fillTextGridFromTextBuffer(TextGrid& textgrid, TextBuffer &textbuffer);


    public:

    TextBuffer()
    {

    }


    void ReadFile(std::string filename)
    {
        std::ifstream ifs(filename);
        std::string line;
        while(std::getline(ifs, line))
        {
            m_lines.emplace_back(std::move(line)); // TODO: check
        }
    }


    void WriteFile(std::string filename)
    {
        std::ofstream ofs(filename);
        for(auto it{m_lines.cbegin()}; it != m_lines.cend(); )
        {
            ofs << *it;
            ++ it;
            if(it != m_lines.cend())
            {
                ofs << "\n";
            }
        }
        ofs.close();
    }


    // should this be external or use inheritance?
    void ProcessEvent()
    {

    }

    // TODO: need some kind of function which can fill
    // a textgrid object from a textbuffer object


    private:


    std::vector<std::string> m_lines;
    std::vector<std::size_t> m_cursor_pos;
    bool m_wrap;
    std::size_t m_scroll;
    std::size_t m_scroll_h; // horrisontal scroll

};






#endif // TEXTBUFFER_HPP