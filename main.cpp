


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




#include "sdlmanager.hpp"
#include "sdlresourcemanager.hpp"
#include "sdlfontmanager.hpp"


#include "sdlhelper.hpp"


#include "color.hpp"






int main(int argc, char *argv[])
{

    std::cout << "Version: " << VERSION_MAJOR
              << " " << VERSION_MINOR << std::endl;

    TextBuffer textbuffer;
    textbuffer.ReadFile("README.md");

    // TextGrid moved below because it requires a SDLFontTexture
    // for initialization
    // TODO: move TextBuffer as well


    // SDL stuff copied from TextGraphicsLib folder
    
    SDLManager &sdl_manager(SDLManager::getInstance());

    SDLResourceManager sdl_resource_manager(sdl_manager);



    // create window and renderer - required to create font texture
    std::shared_ptr<SDL_Window> window(
        //sdl_resource_manager.CreateWindow(sdl_manager));
        sdl_resource_manager.CreateWindow());

    std::shared_ptr<SDL_Renderer> renderer(
        sdl_resource_manager.GetWindowRenderer());



    std::cout << "init font manager..." << std::endl;
    SDLFontManager font_manager(sdl_manager);
    std::cout << "font manager initialized" << std::endl;

    std::string font_filename_liberation_mono;
    try
    {
        std::string string_liberation_mono("Liberation Mono");

        std::string font_filename =
            fontConfigGetFontFilename(string_liberation_mono);
        std::cout << "font_filename=" << font_filename << std::endl;

        font_filename_liberation_mono = font_filename;

        // this function loads a font using the SDL TTF functions
        // taking a font filename (full path) as an argument.
        // The path to the font to be loaded is obtained from the fc
        // font config functions.
        font_manager.LoadFontTexture(renderer, font_filename_liberation_mono, 12);
    }
    catch(const SDLLibException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "catch ... case" << std::endl;
        throw;
    }

    // the to get the actual FontTexture
    // possibly the LoadFontTexture should return the same value
    std::shared_ptr<SDLFontTexture> font_texture_liberation_mono =
        font_manager.GetFontTexture(
            font_filename_liberation_mono, 12);
        // return a shared pointer; good, can pass this to other
        // functions and classes


    // TODO: this constructor call makes the SetFont function
    // redundant
    TextGrid textgrid(40, 20, font_texture_liberation_mono);
    //textgrid.SetFont(font_texture_liberation_mono);

    // here is a potential problem:
    // the textgrid size is set assuming fixed width / monospace font
    // the arguments are the number of characters in x and y
    // whereas in reality the arguments should be the pixel size of
    // the drawable textgrid area
    // otherwise, when using different font sizes or non-monospace fonts
    // the drawable area of the textgrid scales (changes) rather than
    // remaining static

    // TODO: removed this to test base functions
    fillTextGridFromTextBuffer(textgrid, textbuffer);
    

    // TODO: need to change the X and Y around so the indices
    // are row, column
    /*
    textgrid.Put(0, 0, '0');
    textgrid.Put(1, 0, '1');
    textgrid.Put(2, 0, '2');
    textgrid.Put(3, 0, '3');
    textgrid.Put(4, 0, '4');

    textgrid.PutString(0, 1, "hello wor.");
    textgrid.PutString(0, 2, "hello world");
    textgrid.PutString(0, 3, "hello world");

    textgrid.Put(4, 4, 'X');
    textgrid.PutString(0, 4, "sho");
    */

    /*
    // find a font file
    const int FONT_FILENAME_BUFFER_SIZE = 4096;
    char font_filename_buffer[FONT_FILENAME_BUFFER_SIZE];
    fontConfigGetFontFilename(font_filename_buffer,
        FONT_FILENAME_BUFFER_SIZE, "Liberation Mono");

    std::string font_filename(font_filename_buffer); 
    std::cout << "Matched font filename: " << font_filename << std::endl;


    // open font with font texture manager
    SDLFontTexture font_manager_liberation_mono(font_filename);
    */

    // TODO:
    //
    // font_manager_liberation_mono should either be allocated
    // using a smart pointer (it should be a smart pointer)
    // or the fonttexturemanager class should be a singleton
    // which manages a set of shared pointers, one for each
    // loaded font
    //
    // NOTE: this is now done, but fonttexturemanager is not
    // a singleton (no reason for it to be)
    //
    // prefer to avoid singletons -
    // therefore make the font_manager_liberatrion_mono a
    // smart pointer or alternatively wrap the SDL_Texture
    // for the font in its own class which manages the
    // copying of the texture from the SDL_Texture to
    // the SDL_Window SDL_Surface
    //



    
    // rendering code block
    {


        // continue to render stuff
        //SDL_Color COLOR_WHITE = SDL_Color(255, 255, 255);
        SDL_Color COLOR_BACKGROUND = COLOR_WHITE;


        



        //TextGrid textgrid(font_liberation_mono);
        //textgrid.SetFont(font_liberation_mono);
        //textgrid.SetFont(font_manager_liberation_mono);
        // NOTE: moved
        //textgrid.Draw(window);


        textgrid.Print(std::cout);
        

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




            SDL_SetRenderDrawColor(renderer.get(), COLOR_BACKGROUND);
            SDL_RenderClear(renderer.get());

            textgrid.Draw(renderer);

            SDL_RenderPresent(renderer.get());
        }

    }




    return 0;
}
