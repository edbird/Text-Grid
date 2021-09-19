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
    int t_pos_y = pos_y;

    // TODO: don't expect this to render as expected
    for(char c: m_text)
    {
        write(
            sdlrenderer,
            m_sdlfonttexture,
            c, t_pos_x, t_pos_y, true);

        // TODO: implement the DrawCharacterAtPosition type of functions
    }



}



void TextGrid::SetFont(std::shared_ptr<SDLFontTexture> sdlfonttexture)
{
    // TODO: Can't fill this in until the design decisions on how t
    // Draw the object have been made
    m_sdlfonttexture = sdlfonttexture;
}