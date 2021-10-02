#include "textgrid.hpp"
#include "textbuffer.hpp"


#include <string>
#include <iostream>



void fillTextGridFromTextBuffer(TextGrid& textgrid, TextBuffer &textbuffer)
{

    // TODO: need to clear the line by writing " " characters
    // if the substring is shorter than size_x

    unsigned int size_x{textgrid.m_size_x};
    unsigned int size_y{textgrid.m_size_y};

    std::size_t scroll_pos = textbuffer.m_scroll;
    unsigned int u_scroll_pos = (unsigned int)scroll_pos;
    bool wrap = textbuffer.m_wrap;

    if(wrap)
    {
        std::size_t textbuffer_wrap_line_count(0);
        //std::size_t textbuffer_line_index(0);

        // for each line in the textbuffer object
        // lines of text are stored as: std::vector<std::string>
        for(std::size_t ix{0}; ix < textbuffer.m_lines.size(); ++ ix)
        {
            // line index is ix
            std::size_t line_length{textbuffer.m_lines.at(ix).size()};

            /*
            // number of lines after wrapping
            unsigned int number_of_lines =
                ((unsigned int)line_length + size_x - 1) / size_x;
            */


            // new approach, iterate through line with increment of size_x

            // when the line_length is zero, no blank line is printed
            // should the check be i <= line_length?
            // what happens when i + size_x == line_length ?
            // answer: another blank line is printed where there shouldn't be
            // one
            for(unsigned int i{0};
                (i < line_length) || (i == 0);
                i += size_x)
            {
                // this logic statement draws the correct output depending
                // on the scroll position
                //if((textbuffer_wrap_line_count <= scroll_pos) &&
                if((textbuffer_wrap_line_count >= scroll_pos) &&
                    (textbuffer_wrap_line_count < scroll_pos + size_y))
                {
                    // delta is the y coordinate (index) of the current line
                    // to be drawn to the screen
                    // TODO: I think this logic is wrong
                    unsigned int delta = textbuffer_wrap_line_count - scroll_pos;
                    std::cout << "delta=" << delta << std::endl;

                    // substring to print (current graphical line, a sliced
                    // part of the current text line)
                    std::string substring =
                        textbuffer.m_lines.at(ix).substr(i, size_x);

                    std::cout << "substring=" << substring << std::endl;

                    textgrid.PutString(0, delta, substring); // out of bounds ok

                    ++ textbuffer_wrap_line_count;
                }
            }

            // efficient: quit early once textgrid filled
            if(textbuffer_wrap_line_count >= size_y) break;
        } // for


    }
    else
    {
        // TODO: substr() will throw error if pos > size !

        // no wrap version
        std::size_t textbuffer_line_count(0);

        std::size_t scroll_pos_h = textbuffer.m_scroll_h;

        for(std::size_t ix{0}; ix < textbuffer.m_lines.size(); ++ ix)
        {
            // line index is ix
            std::size_t line_length{textbuffer.m_lines.at(ix).size()};

            // no comment here

            if((textbuffer_line_count <= scroll_pos) &&
                (textbuffer_line_count < scroll_pos + size_y))
            {
                unsigned int delta = textbuffer_line_count - scroll_pos;
        
                std::string substring = textbuffer.m_lines.at(ix).substr(scroll_pos_h, size_x);

                textgrid.PutString(0, delta, substring); // out of bounds ok

                ++ textbuffer_line_count;
            }

            // efficient: quit early once textgrid filled
            if(textbuffer_line_count >= size_y) break;
        } // for

    } // else

}