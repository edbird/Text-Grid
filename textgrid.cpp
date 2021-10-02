#include "textgrid.hpp"


#include "sdlhelper.hpp"


// draw a single character, literally anywhere
// (position not constrained by grid)
void TextGrid::draw_char_anywhere(
    std::shared_ptr<SDL_Window> sdlwindow,
    const char c,
    const int x, const int y)
{

}



// draw a single character from the grid
void TextGrid::draw_char(
    std::shared_ptr<SDL_Window> sdlwindow,
    const char c,
    const unsigned int pos_x,
    const unsigned int pos_y)
{

    // only works for monospace fonts
    //int y = pos_y * font_baseline_skip?
    //int x = pos_x * char_width?



}




void TextGrid::Draw(std::shared_ptr<SDL_Renderer> sdlrenderer)
//std::shared_ptr<SDL_Window> sdlwindow)
{

    // TODO: what is the best way to do this with smart pointer?
    ////SDL_Renderer *renderer = sdlwindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars


    int font_line_skip =
        m_sdlfonttexture->GetFontLineSkip();
        
    int font_ascent =
        m_sdlfonttexture->GetFontAscent();

    int pos_x = 0;
    int pos_y = 0;
    int t_pos_x = pos_x;
    //int t_pos_y = pos_y; // (A)

    // TODO: don't expect this to render as expected

    std::size_t index(0);
    for(auto c: m_text)
    {
        //std::cout << index << " > " << c << std::endl;
        // TODO: remove

        // TODO: the data is stored in m_text as a flat
        // array, and was converted to a flat array from
        // a std::vector<std::string> (not a flat array)
        // now it has been converted back to something
        // 2 dimensional. This might not be the most
        // elegant way to write this.

        // convert the character index to x, y index
        const int t_index_x = (index % m_size_x);
        const int t_index_y = (index / m_size_x);

        // TODO: remove above? (A)
        //t_pos_x = 6 * t_index_x;
        const int t_pos_y = pos_y + font_line_skip * t_index_y;

        // required to reset the x position when a new line starts
        if(t_index_x == 0)
        {
            t_pos_x = pos_x;
        }

        // exit when the number of lines in y exceeds the size
        // of the textgrid
        if(t_index_y >= m_size_y) break;

        // this function takes a position as pixel coordinates
        // final argument true is the autoadvance=true argument
        write(
            sdlrenderer,
            m_sdlfonttexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
        
        // increment the index of the current character in m_text
        // to print
        ++ index;
    }

}



void TextGrid::Draw_PixelSize(std::shared_ptr<SDL_Renderer> sdlrenderer)
//std::shared_ptr<SDL_Window> sdlwindow)
{

    /*
    std::cout << "Filling rect: "
              << 0 << " "
              << 0 << " "
              << m_size_pixels_x << " "
              << m_size_pixels_y << " " << std::endl;*/

    SDL_Rect rdst{0, 0, m_size_pixels_x, m_size_pixels_y};
    SDL_SetRenderDrawColor(sdlrenderer.get(), m_background_color);
    SDL_RenderFillRect(sdlrenderer.get(), &rdst);
    SDL_SetRenderDrawColor(sdlrenderer.get(), COLOR_WHITE);


    // TODO: what is the best way to do this with smart pointer?
    ////SDL_Renderer *renderer = sdlwindow->GetRenderer();
    // TODO: is this different to my code in main() of Text-Graphics-Lib

    // rendering code copied from other project
    //SDL_Rect rsrc = map_text_chars_rect.at()


    // the following section draws a complete block of text containing
    // all rendered chars


    int font_line_skip =
        m_sdlfonttexture->GetFontLineSkip();
        
    int font_ascent =
        m_sdlfonttexture->GetFontAscent();

    int pos_x = 0;
    int pos_y = 0;
    int t_pos_x = pos_x;
    //int t_pos_y = pos_y; // (A)

    // TODO: don't expect this to render as expected

    std::size_t index(0);
    for(auto c: m_text)
    {
        //std::cout << index << " > " << c << std::endl;
        // TODO: remove

        // TODO: the data is stored in m_text as a flat
        // array, and was converted to a flat array from
        // a std::vector<std::string> (not a flat array)
        // now it has been converted back to something
        // 2 dimensional. This might not be the most
        // elegant way to write this.

        // convert the character index to x, y index
        const int t_index_x = (index % m_size_x);
        const int t_index_y = (index / m_size_x);

        // TODO: remove above? (A)
        //t_pos_x = 6 * t_index_x;
        const int t_pos_y = pos_y + font_line_skip * t_index_y;

        // required to reset the x position when a new line starts
        if(t_index_x == 0)
        {
            t_pos_x = pos_x;
        }

        // exit when the number of lines in y exceeds the size
        // of the textgrid
        if(t_index_y >= m_size_y) break;

        // this function takes a position as pixel coordinates
        // final argument true is the autoadvance=true argument
        write(
            sdlrenderer,
            m_sdlfonttexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
        
        // increment the index of the current character in m_text
        // to print
        ++ index;
    }

}




void TextGrid::SetFont(std::shared_ptr<SDLFontTexture> sdlfonttexture)
{
    // TODO: Can't fill this in until the design decisions on how t
    // Draw the object have been made
    m_sdlfonttexture = sdlfonttexture;

    // need to set the grid size after changing font

    const int widest_character_advance = sdlfonttexture->GetWidestCharacterAdvance();
    const int font_line_skip = sdlfonttexture->GetFontLineSkip();

    const int character_size_x = widest_character_advance;
    const int character_size_y = font_line_skip;

    // store these for later calculations
    m_character_size_x = character_size_x;
    m_character_size_y = character_size_y;

    // truncate rounding (round down)
    m_size_x = m_size_pixels_x / character_size_x;
    m_size_y = m_size_pixels_y / character_size_y;

    // fill with blank characters (' ', space)
    // no logical way to truncate / expand the grid if
    // the character dimensions (and thus grid size)
    // changes
    fill();

}



void TextGrid::SetSizePixels(const int size_pixels_x, const int size_pixels_y)
{
    m_size_pixels_x = size_pixels_x;
    m_size_pixels_y = size_pixels_y;

#if 0
    // set the grid size in characters
    const int font_line_skip = sdlfonttexture->GetFontLineSkip();
    //const int widest_character_width = sdlfonttexture->GetWidestCharacterWidth();
    const int widest_character_advance = sdlfonttexture->GetWidestCharacterAdvance();

    const int character_size_x = widest_character_advance;
    const int character_size_y = font_line_skip;

    // store these for later calculations
    m_character_size_x = character_size_x;
    m_character_size_y = character_size_y;

    // truncate rounding (round down)
    m_size_x = size_pixels_x / character_size_x;
    m_size_y = size_pixels_y / character_size_y;
#endif

    // truncate rounding (round down)
    m_size_x = size_pixels_x / m_character_size_x;
    m_size_y = size_pixels_y / m_character_size_y;

    // there is no sensible way to crop or expand the textgrid
    // because some lines may have been printed using a "wrap"
    // mode
    // it is expected that the user will re-populate the grid
    // with characters after calling this function
    fill();
}


void TextGrid::Clear()
{
    fill();
}


void TextGrid::fill()
{
    m_text.clear();

    // fill text grid with spaces
    unsigned int count = m_size_x * m_size_y;

    //std::cout << "fill() called, count=" << count << std::endl;
    //std::cin.get(); // TODO remove

    for(unsigned int i = 0; i < count; ++ i)
    {
        m_text.push_back(' ');
    }
}