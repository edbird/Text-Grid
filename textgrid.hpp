#ifndef TEXTGRID_HPP
#define TEXTGRID_HPP


#include <SDL2/SDL.h>

#include "sdlfonttexture.hpp"


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

    TextGrid(unsigned int size_x, unsigned int size_y,
        std::shared_ptr<SDLFontTexture> sdlfonttexture)

        // NOTE: The above is a shared pointer but the below does not use a
        // shared pointer, does the code in main() use a shared pointer
        // to reference the font data?
        // No it doesn't: Calls to function write() pass the font
        // as a const SDLFontManger& 
        // the SDLFontTexture is then obtained by calling
        // sdlfontmanager.m_sdlfonttexturemanager
        // this returns a shared pointer.
        // It may be more logical to pass the shared pointer to the
        // write() function directly, which would allow this function
        // to take a shared pointer as an argument. However, this raises
        // the question: What is the purpose of a SDLFontManager?
        // Why has it been separated from the SDLFontTexture
        // when the SDLFontTexture should contain all the data
        // required for drawing the font texture to screen? This includes
        // elements such as the base line skip and font ascent.
        // Perhaps the SDLFontManager should be responsible for loading
        // multiple fonts? (Instances of SDLFontTexture manager) and
        // the SDLFontTexture can then be passed as a shared pointer.
        //
        // NOTE: this has now been implemented:
        // [X] SetFont function will now use shared pointer
        // [X] calls to write() pass font as a shared pointer
        // [X] SDLFontTexture instance no longer requires another
        //     level of indirection to obtain
        // [X] SDLFontManager now separated from SDLFontTexture
        //     because it can load multiple fonts of different
        //     font sizes
        // [X] SDLFontTexture contains all data for rendering
        //
        // IGNORE: A pointer could be made from the SDLFontTexture instance
        // IGNORE: itself, to do this, and to make it a shared pointer, it would
        // IGNORE: have to be constructed as a shared pointer (using new called by
        // IGNORE: shared pointer construction). Otherwise, if the address is taken,
        // IGNORE: then in principle, the shared pointer could be left dangling.
        // IGNORE: (This is not how you use a shared pointer.)
        // IGNORE: However this would require changing everything in main to use
        // IGNORE: pointer syntax and referene the SDLFontTexutreManager instance
        // IGNORE: as a pointer. This might be an acceptable solution.
        // The alternative would be for the FontManger to manage a shared
        // pointer to the FontTextureManager, but this might have its own
        // disadvantages. Check main() to see.
        // NOTE: latter comment accepted as solution
        //
        // This question may be resolved by considering how I want to
        // use the FontTextureManager and FontManager when passing these
        // objects to other methods.
        // DONE

        : m_size_x(size_x)
        , m_size_y(size_y)
        , m_sdlfonttexture(sdlfonttexture)
    {
        fill();

        // create a font to use here
        /*std::cout << "loading font" << std::endl;

        SDLFontManager font_manager_liberation_mono;
        try
        {
            // create font texture
            SDLFontManager font_manager_liberation_mono_local(
                sdl_manager,
                //std::shared_ptr<SDL_Renderer>(renderer),
                renderer,
                font_filename,
                12);

            font_manager_liberation_mono =
                std::move(font_manager_liberation_mono_local);
        }
        catch(const SDLLibException &e)
        {
            std::cout << e.what() << std::endl;
        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
        */
        
    }


    //void Draw(std::shared_ptr<SDL_Window> sdlwindow);
    void Draw(std::shared_ptr<SDL_Renderer> sdlrenderer);
        // probably can't be const

    // Same as above but prints to stdout
    void Print(std::ostream &os);


    void SetFont(std::shared_ptr<SDLFontTexture> sdlfonttexture);


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
            // TODO: do not throw instance of std string
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

    // TODO: should really be called clear_and_fill() or just clear()
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

    void draw_char_anywhere(
        std::shared_ptr<SDL_Window> sdlwindow,
        const char c,
        const int x, const int y);

    void draw_char(
        std::shared_ptr<SDL_Window> sdlwindow,
        const char c,
        const unsigned int pos_x,
        const unsigned int pos_y);


    private:

    unsigned int m_size_x;
    unsigned int m_size_y;

    std::shared_ptr<SDLFontTexture> m_sdlfonttexture;

    std::string m_text;


};











#if 0
void TextGrid::Draw(std::shared_ptr<SDL_Window> sdlwindow)
{

    // TODO: what is the best way to do this with smart pointer?
    SDL_Renderer *renderer = sdlwindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars

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

        // block ends here



        // this block draws some arbitrary strings
        // this is some old code which was inherited from
        // the Text-Graphics-Lib project before I modified 
        // the API for that project.

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
#endif
// TODO: Remove this function? This should be covered by some of my code
// in the textgraphics lib folder?












#endif // TEXTGRID_HPP