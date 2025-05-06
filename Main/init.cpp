    #include "common.hpp"
    using namespace std;

    void InitSDL()
    {   // Declare variables
        int renderflag, windowflag;
        renderflag = SDL_RENDERER_ACCELERATED;
        windowflag = 0;
        // Initialize SDL Image
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

        // Initialize SDL
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            cout << "Couldn 't initialize the SDL " << SDL_GetError();
            exit(1);
        }

        app.window = SDL_CreateWindow("Shooting Star", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowflag);
        
        // Check whelther the window open or not
        if(!app.window)
        {
            cout << "Failed to open " << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << " window: " << SDL_GetError();
            exit(1);  
        }
        
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        app.renderer = SDL_CreateRenderer(app.window, -1, renderflag);

        if(!app.renderer)
        {
            cout << "Failed to create renderer: " << SDL_GetError();
            exit(1);
        }
        SDL_ShowCursor(0);

        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        {
            cout << "Couldn't initialize SDL_MIXER" << endl;
            exit(1);
        }

        Mix_AllocateChannels(MAX_SND_CHANNELS);
    }


    void initGame()
    {

        initStarField();

        initSounds();

        initFont();

        initTablescore();

    }


    void cleanup()
    {
        if(app.renderer)
        {
            SDL_DestroyRenderer(app.renderer);
            app.renderer = nullptr;
        }

        if(app.window)
        {
            SDL_DestroyWindow(app.window);
            app.window = nullptr;
        }
    }