#ifndef TEXTGRID_HPP
#define TEXTGRID_HPP


#include <string>


class TextBuffer;


// class to hold data for a fixed size text grid
// essentially an N * M array of text characters
// Put methods ignore out of bounds
// Get methods throw error on out of bounds
//
// contains draw method to draw to SDL_Surface
// (not currently implemented)
// needs some kind of reference to a font object
// alternatively implement draw function external
// to this class and pass in instance of this class
// and a font as arguments
//
class TextGrid
{


    friend void fillTextGridFromTextBuffer(TextGrid& textgrid, TextBuffer &textbuffer);


    public:

    TextGrid(unsigned int size_x, unsigned int size_y)
        : m_size_x(size_x)
        , m_size_y(size_y)
    {
        fill();
    }


    void Draw(const SDLWindow &sdlwindow); // probably can't be const

    void SetFont(const SDLFontTextureManager &sdlfonttexturemanager);


    std::string GetBuffer() const
    {
        return m_text;
    }

    // sets size to zero and clears buffer
    void Clear(unsigned int size_x, unsigned int size_y)
    {
        m_size_x = size_x;
        m_size_y = size_y;
        fill();
    }

    // silently ignores out of bounds character put request
    void Put(unsigned int x, unsigned int y, char c)
    {
        if(y < m_size_y)
        {
            if(x < m_size_x)
            {
                m_text[index(x, y)] = c;
            }
        }
    }

    // wrapping/non-wrapping PutString function
    void PutString(unsigned int x, unsigned int y, std::string s, bool wrap = true)
    {
        if(wrap)
        {
            put_string_wrap(x, y, s);
        }
        else
        {
            put_string_no_wrap(x, y, s);
        }
    }

    // will throw an error if out of bounds
    void Get(unsigned int x, unsigned int y, char &c) const
    {
        if((x >= m_size_x) || (y >= m_size_y))
        {
            std::string emsg(std::string("Error: index out of bounds in function ") + std::string(__func__));
            throw emsg;
        }
        c = m_text[index(x, y)];
    }


    private:

    unsigned int index(unsigned int x, unsigned int y) const
    {
        return (y * m_size_x + x);
    }

    // will increment beyond bounds of array
    void increment(unsigned int &x, unsigned int &y) const
    {
        ++ x;
        if(x >= m_size_x)
        {
            x = 0;
            ++ y;
        }
    }

    void fill()
    {
        m_text.clear();

        // fill text grid with spaces
        unsigned int count = m_size_x * m_size_y;
        for(unsigned int i = 0; i < count; ++ i)
        {
            m_text.push_back(' ');
        }
    }

    // non-wrapping PutString function
    void put_string_no_wrap(unsigned int x, unsigned int y, std::string s)
    {
        unsigned int pos_x = x;
        const unsigned int pos_y = y;
        for(auto it{s.cbegin()}; it != s.cend(); ++ it)
        {
            char c = *it;
            Put(pos_x, pos_y, c);
            ++ x;
            if(x >= m_size_x)
            {
                break;
            }
        }
    }

    // wrapping PutString function
    void put_string_wrap(unsigned int x, unsigned int y, std::string s)
    {
        unsigned int pos_x = x;
        unsigned int pos_y = y;
        for(auto it{s.cbegin()}; it != s.cend(); ++ it)
        {
            char c = *it;
            Put(pos_x, pos_y, c);
            increment(x, y);
            if(y >= m_size_y)
            {
                break;
            }
        }
    }

    unsigned int m_size_x;
    unsigned int m_size_y;
    std::string m_text;



};


// draw a single character, literally anywhere
// (position not constrained by grid)
void TextGrid::draw_char_anywhere(
    const SDLWindow &sdlwindow,
    const char c,
    const int x, const int y)
{

}



// draw a single character from the grid
void TextGrid::draw_char(
    const SDLWindow &sdlwindow,
    const char c,
    const unsigned int pos_x,
    const unsigned int pos_y)
{

    // only works for monospace fonts
    int y = pos_y * font_baseline_skip?
    int x = pos_x * char_width?



}




#include <SDL2/SDL.h>


void TextGrid::Draw(const SDLWindow &sdlwindow)
{

    // TODO: what is the best way to do this with smart pointer?
    SDL_Renderer *renderer = sdlwindow.GetRenderer();


    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()

    int text_texture_w = 0;
    int text_texture_h = 0;
    if(SDL_QueryTexture(text_texture, nullptr, nullptr,
        &text_texture_w, &text_texture_h) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
    }
    else
    {
        // query was ok
        // draw whole character texture block
        int rdst_y = 0;

        SDL_Rect rsrc;
        rsrc.x = 0;
        rsrc.y = 0;
        rsrc.w = text_texture_w;
        rsrc.h = text_texture_h;
        SDL_Rect rdst(0, rdst_y, text_texture_w, text_texture_h);
        SDL_Color COLOR_GREEN(0, 255, 0);
        SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
        SDL_RenderFillRect(renderer, &rdst);
        SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);

        // draw arbitary strings
        //rdst_y += text_line_skip;
        //rdst.y = rdst_y;
        rdst.x = 0;
        rdst.y += text_line_skip;

        std::string mytext("hello world 0123456789'''");
        int ticktock = 0;
        for(char c: mytext)
        {
            int offset_x = 0;
            int index = index_of_printable_char(c);
            if(index >= 0)
            {
                for(int count = 0;
                    count < index;
                    ++ count)
                {
                    //rsrc_x += map_text_chars_rect.at(c).w;
                    offset_x += map_text_chars_advance.at(c);
                }
                rsrc.x = map_text_chars_rect.at(c).x + offset_x;
                //rsrc.y = map_text_chars_rect.at(c).y;
            const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y; // TODO: remove?
                rsrc.y = text_ascent - maxy;
                //rsrc.y = text_texture_h - map_text_chars_rect.at(c).y;
                rdst.w = rsrc.w = map_text_chars_rect.at(c).w;
                rdst.h = rsrc.h = map_text_chars_rect.at(c).h;

                //rdst.y += rsrc.y;
                //const int maxy = map_text_chars_rect.at(c).h + map_text_chars_rect.at(c).y;
                rdst.y += text_ascent - maxy;
                rdst.x += map_text_chars_rect.at(c).x;

                // TODO: remove this block
                if(ticktock == 0)
                {
                    //SDL_Color COLOR_GREEN(0, 255, 0);
                    SDL_SetRenderDrawColor(renderer, COLOR_GREEN);
                    SDL_RenderFillRect(renderer, &rdst);
                }
                ticktock += 1;
                ticktock %= 2;

                SDL_RenderCopy(renderer, text_texture, &rsrc, &rdst);
                rdst.x -= map_text_chars_rect.at(c).x;
                rdst.y -= text_ascent - maxy;
                int advance = map_text_chars_advance.at(c);
                
                //rdst.y -= rsrc.y;
                
                rdst.x += advance;
                rdst.y += 0;
            }
            else
            {
                std::cout << "Error in index_of_printable_char" << std::endl;
            }

        }
    }


}




void TextGrid::SetFont(const SDLFontTextureManager &sdlfonttexturemanager)
{

}







#endif // TEXTGRID_HPP