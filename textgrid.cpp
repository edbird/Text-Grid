#include "textgrid.hpp"




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

        const int t_index_x = (index % m_size_x);
        const int t_index_y = (index / m_size_x);

        // TODO: remove above? (A)
        //t_pos_x = 6 * t_index_x;
        const int t_pos_y = pos_y + font_line_skip * t_index_y;

        if(t_index_x == 0)
        {
            t_pos_x = pos_x;
        }

        if(t_index_y >= m_size_y) break;

        // TODO: what units does write take? pixels? I think
        // so above calculations are wrong
        // but rendered output is still unexpected
        write(
            sdlrenderer,
            m_sdlfonttexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
        
        ++ index;
    }



}



void TextGrid::SetFont(std::shared_ptr<SDLFontTexture> sdlfonttexture)
{
    // TODO: Can't fill this in until the design decisions on how t
    // Draw the object have been made
    m_sdlfonttexture = sdlfonttexture;
}