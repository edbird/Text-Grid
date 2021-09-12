


#include <SDL2/SDL.h>


#include <iostream>


// text grid manages drawing of text to screen as fixed size grid
// text buffer manages file io, buffer, cursor(s), window scrolling,
// text wrapping
// the data inside text grid is filled using data from text buffer
// when it is time to draw


#include "version.hpp"


#include "textgrid.hpp"
#include "textbuffer.hpp"


#include "sdlhelper.hpp"



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

            for(unsigned int i{0}; i < line_length; i += size_x)
            {
                if((textbuffer_wrap_line_count <= scroll_pos) &&
                    (textbuffer_wrap_line_count < scroll_pos + size_y))
                {
                    unsigned int delta = textbuffer_wrap_line_count - scroll_pos;

                    std::string substring = textbuffer.m_lines.at(ix).substr(i, size_x);

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


int main(int argc, char *argv[])
{

    std::cout << "Version: " << textgrid_VERSION_MAJOR
              << " " << textgrid_VERSION_MINOR << std::endl;

    TextBuffer textbuffer();
    textbuffer.ReadFile("README.md");

    TextGrid textgrid(10, 10);

    fillTextGridFromTextBuffer(textgrid, textbuffer);


    // SDL stuff copied from TextGraphicsLib folder
    
    SDLManager &sdl_manager(SDLManager::getInstance());


    // find a font file
    const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string font_filename(font_filename_buffer); 
    std::cout << "Matched font filename: " << font_filename << std::endl;


    // open font with font texture manager
    SDLFontTextureManager font_manager_liberation_mono(font_filename);

    // TODO:
    //
    // font_manager_liberation_mono should either be allocated
    // using a smart pointer (it should be a smart pointer)
    // or the fonttexturemanager class should be a singleton
    // which manages a set of shared pointers, one for each
    // loaded font
    //
    // prefer to avoid singletons -
    // therefore make the font_manager_liberatrion_mono a
    // smart pointer or alternatively wrap the SDL_Texture
    // for the font in its own class which manages the
    // copying of the texture from the SDL_Texture to
    // the SDL_Window SDL_Surface
    //


    // create SDL window
    //std::shared_ptr<SDL_Window> window = sdl_manager.CreateWindow();

    //std::shared_ptr<SDL_Renderer> renderer = window->GetRenderer(); // TODO: ??

    std::shared_ptr<SDL_Window> window(
        sdl_resource_manager.CreateWindow(sdl_manager));

    std::shared_ptr<SDL_Renderer> renderer(
        sdl_resource_manager.GetWindowRenderer());

    
    // rendering code block
    {


        // continue to render stuff
        SDL_Color COLOR_WHITE = SDL_Color(255, 255, 255);
        SDL_Color COLOR_BACKGROUND = COLOR_WHITE;


        // copied from main.cpp in Text-Graphics-Lib
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



        //TextGrid textgrid(font_liberation_mono);
        //textgrid.SetFont(font_liberation_mono);
        textgrid.SetFont(font_manager_liberation_mono);
        //textgrid.Draw(window);

        
        // main infinite loop
        for(bool quit = false; quit == false; )
        {
            


            SDL_Event event;
            while(SDL_PollEvent(&event) != 0)
            {
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }
                else
                {
                    // nothing
                }
            }



            SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND);
            SDL_RenderClear(renderer);

            textgrid.Draw(window);

            SDL_RenderPresent(renderer);
        }

    }




    return 0;
}
