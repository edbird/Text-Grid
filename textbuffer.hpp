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
        : m_wrap(true) // TODO: just put these values in for testing
        , m_scroll(0)
        , m_scroll_h(0)
    {

    }


    void ReadFile(std::string filename)
    {
        std::cout << __func__ << std::endl;

        std::ifstream ifs(filename);

        if(!ifs.is_open())
        {
            std::cout << "Failed to open file " << filename << std::endl;
        }
        else
        {
            std::string line;
            while(std::getline(ifs, line))
            {
                //std::cout << line << std::endl;
                m_lines.emplace_back(std::move(line)); // TODO: check
            }
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
    std::size_t m_scroll_h; // horizontal scroll

};






#endif // TEXTBUFFER_HPP