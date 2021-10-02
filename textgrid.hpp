#ifndef TEXTGRID_HPP
#define TEXTGRID_HPP


#include <SDL2/SDL.h>

#include "color.hpp"

#include "sdlfonttexture.hpp"


#include <string>



#include "cursortype.hpp"


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


// have a think about fixed width vs variable width fonts:
//
// this class was designed with the intention of storing the characters
// to be printed as a grid of characters
//
// therefore it was not intended to be used with variable width fonts
//
// the constructor used to take an x * y number of characters to hold in
// the grid
//
// this is a bit strange when considering printing of the characters
// to screen
//
// usually, an x * y pixel resolution would be expected instead
//
// therefore construct class with an x * y pixel resolution and convert
// this into an x * y character grid size. use the pixel resolution to draw
// the background, and use the x * y grid size to draw the characters themselves
//
// if someone uses a non-fixed width font then the grid will not be drawn
// sensibly

class TextGrid
{


    friend void fillTextGridFromTextBuffer(TextGrid& textgrid, TextBuffer &textbuffer);


    public:

    // this constructor uses a character grid size of size_x * size_y
    // to initialize the grid. the size_pixels_x is set manually
    // by the user. this means that a nonsense value of the pixel size
    // can be supplied, the consequence is that the background colored
    // square can be a size which doesn't correspond to the area where
    // characters are printed
    TextGrid(unsigned int size_x, unsigned int size_y,
        const int size_pixels_x, const int size_pixels_y,
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
        , m_size_pixels_x(size_pixels_x)
        , m_size_pixels_y(size_pixels_y)
        , m_character_size_x(0)
        , m_character_size_y(0)
        , m_sdlfonttexture(sdlfonttexture)
        , m_background_color(COLOR_WHITE)
        , m_cursortype{cursortype_t::BLOCK}
    {
        m_character_size_x = sdlfonttexture->GetWidestCharacterAdvance();
        m_character_size_y = sdlfonttexture->GetFontLineSkip();

        // this constructor is weird, and does not auto-calculate
        // the character grid size, size x and y

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


    // this version of the constructor sets the pixel size of the 
    // textgrid object. this is the size of the background square
    // which will be drawn
    // the actual size of the grid in characters is computed from
    // the pixel size
    TextGrid(
        const int size_pixels_x, const int size_pixels_y,
        std::shared_ptr<SDLFontTexture> sdlfonttexture)
        : m_size_x(0)
        , m_size_y(0)
        , m_size_pixels_x(size_pixels_x)
        , m_size_pixels_y(size_pixels_y)
        , m_character_size_x(0)
        , m_character_size_y(0)
        , m_sdlfonttexture(sdlfonttexture)
        , m_background_color(COLOR_WHITE)
    {
        // set the grid size in characters
        const int font_line_skip = sdlfonttexture->GetFontLineSkip();
        const int widest_character_width = sdlfonttexture->GetWidestCharacterWidth();
        const int widest_character_advance = sdlfonttexture->GetWidestCharacterAdvance();

        std::cout << "Widest Character Advance: " << widest_character_advance << std::endl;
        std::cout << "Widest Character Width: " << widest_character_width << std::endl;

        std::cout << "W Character Width " << sdlfonttexture->GetCharacterWidthW() << std::endl;
        std::cout << "W Character Advance " << sdlfonttexture->GetCharacterAdvanceW() << std::endl;

        std::cout << "_ Character Width " << sdlfonttexture->GetCharacterWidthUnderscore() << std::endl;
        std::cout << "_ Character Advance " << sdlfonttexture->GetCharacterAdvanceUnderscore() << std::endl;

        const int character_size_x = widest_character_advance;
        const int character_size_y = font_line_skip;

        // store these for later calculations
        m_character_size_x = character_size_x;
        m_character_size_y = character_size_y;
        
        // truncate rounding (round down)
        m_size_x = size_pixels_x / character_size_x;
        m_size_y = size_pixels_y / character_size_y;

        // fill with blank characters (' ', space)
        fill();
    }

    void SetBackgroundColor(const SDL_Color background_color)
    {
        m_background_color = background_color;
    }


    //void Draw(std::shared_ptr<SDL_Window> sdlwindow);
    void Draw(std::shared_ptr<SDL_Renderer> sdlrenderer);
        // probably can't be const

    void Draw_PixelSize(
        std::shared_ptr<SDL_Renderer> sdlrenderer);
    // maybe can be const since sdlrenderer is the non-const object?

    // Same as above but prints to stdout TODO
    void Print(std::ostream &os);


    void SetFont(std::shared_ptr<SDLFontTexture> sdlfonttexture);


    std::string GetBuffer() const
    {
        return m_text;
    }

    void SetSizePixels(const int size_pixels_x, const int size_pixels_y);

    // sets size to zero and clears buffer
    // note this function is a bit weird because it sets the size
    // of the grid directly, so removed it
    /*
    void Clear(unsigned int size_x, unsigned int size_y)
    {
        m_size_x = size_x;
        m_size_y = size_y;
        fill();
    }
    */

    void Clear();

    // silently ignores out of bounds character put request
    void Put(unsigned int x, unsigned int y, char c)
    {
        if(y < m_size_y)
        {
            if(x < m_size_x)
            {
                //std::cout << "index(" << x << "," << y << ")=" << index(x, y) << std::endl;
                m_text[index(x, y)] = c;

                //std::cout << "m_text[" << x << "," << y << "]=" << c << std::endl;
            }
        }
        //std::cin.get();
        // TODO: remove
    }

    // wrapping/non-wrapping PutString function
    void PutString(unsigned int x, unsigned int y, std::string s, bool wrap = true)
    {
        //std::cout << __func__ << " s=" << s << std::endl;
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
    void fill();

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

    // TODO: I don't think this function works correctly

    // wrapping PutString function
    void put_string_wrap(unsigned int x, unsigned int y, std::string s)
    {
        //std::cout << __func__ << std::endl;

        unsigned int pos_x = x;
        unsigned int pos_y = y;
        for(auto it{s.cbegin()}; it != s.cend(); ++ it)
        {
            char c = *it;

            //std::cout << "Put(" << pos_x << "," << pos_y << "," << c << ")" << std::endl;

            Put(pos_x, pos_y, c);
            increment(pos_x, pos_y);
            if(y >= m_size_y)
            {
                break;
            }
        }
    }

    // TODO: implement these ?
    void draw_char_anywhere(
        std::shared_ptr<SDL_Window> sdlwindow,
        const char c,
        const int x, const int y);

    // this function cannot be easily implemented because it
    // requires knowing the advance of each character which had been
    // printed before it on the same line
    void draw_char(
        std::shared_ptr<SDL_Window> sdlwindow,
        const char c,
        const unsigned int pos_x,
        const unsigned int pos_y);


    private:

    // size in number of characters
    // makes more sense for monospace fonts
    // scales with font size
    // these are being depreciated
    unsigned int m_size_x;
    unsigned int m_size_y;

    // size in pixels
    // makes sense for both monospace and
    // variable width fonts
    // does not scale with font size, independent of font size
    // these are replacing m_size_x and m_size_y
    int m_size_pixels_x;
    int m_size_pixels_y;

    // this class needs to store the font line skip (character size y)
    // and widest character advance (character size x)
    // this is to calculate the size x and y from the pixel size x and y
    int m_character_size_x;
    int m_character_size_y;

    std::shared_ptr<SDLFontTexture> m_sdlfonttexture;

    std::string m_text;

    SDL_Color m_background_color;

    // Note: could inherit and create a new class which has cursors,
    // but for now we simply insert cursor variables and logic into
    // this monolithic class
    // TODO: initialize in class constructor and add setter function for cursortype
    cursortype_t m_cursortype;
    unsigned int m_cursor_pos_x;
    unsigned int m_cursor_pos_y;

    // in order for the cursor logic to work, we need to remember the size
    // of each line in the grid.
    // currently the grid is always populated with characters, some of which
    // are just blank spaces by default
    // should perhaps solve this by making the cursor a member of the text
    // buffer class?
    // or creating a new class which holds the textgrid data, but as individual
    // lines, and this class can then render the textgrid by creating a new
    // object from the class used to hold the textgrid data?

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