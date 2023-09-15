# Arcade - Documentation

Arcade is a gaming platform that allows users to choose a game to play and a graphic library to work on. This documentation is intended to guide developers on how to implement new graphics libraries or game libraries that are compatible with our Arcade system.

## Getting started

Before you begin implementing new graphics libraries or game libraries, you must correctly setup the repository.

We recommend to setup your repository as follows :
```
.
├── include
│   └── YourLib.hpp
├── src
│   └── YourLib.cpp
├── interface
│   ├── IGame.hpp
│   ├── IRenderer.hpp
│   └── types
│       ├── Events.hpp
│       ├── Global.hpp
│       └── Render.hpp
└── Makefile
```
Interface directory are provided interfaces to help you implementing your librararies.

- IGame helps you implementing a game library.
- IRenderer helps you implementing a graphic library.

Your library class, defined in your hpp, must implements one of these interfaces.

```cpp
class MyGame : public IGame
{
    public:
        MyGame();
        ~MyGame();
}
```

```cpp
class MyRenderer : public IRenderer
{
    public:
        MyRenderer();
        ~MyRenderer();
}
```

Your Makefile must compile a shared library (*.so) according to these g++ compilation flags :
```
-std=c++20 -fPIC -shared
```

## Implementation

Your library must override all methods of it's interface as it follows.

You library should provide **extern "C"** methods as "passport" to make the core able to recognize your library.

**Extern "C" must contain:**
- `render::IRenderer *entryPoint()` that return the constructor of the library
- `int getType()` return libtype from given enum **arcade::LibType**.
- `std::string getName()` return the name that must be displayed in the menu to identify your library.


### Game

- **getSpritesheets** must return a vector that contains all spritesheets needed to make game working. Spritesheet path must be relative to arcade core binary.
    > You must provide a path with a textual spritesheet (`<fg><bg><c1><c2>`) and a graphical spritsheet with unit dimentions of a sprite on the spritesheet. We use two characters to obtain a square with monospaced fonts.

    _Example of textual spritesheet :_

    ```
    20//20==20\\20..20 :
    20\\00  20||20: 20..
    00  00  20//20\/20 <
    31##00  00  20> 20/\
    ```

- **compute** takes a delta time and a context as parameters. Delta time is the elapsed time since the last compute call (it must be used to maintain a constant speed of objects and interactions). Context is an object that allows you to interact with any graphic library without caring about the implementations. **Don't render anything on screen with this method.**
    > Context has **saveScore** method that helps you to save players score on game end.

- **handleInput** method will be called on each user input event, providing you the pressed key (or mouse) with the associated event if key is mapped in settings as well ass coordinates on screen for mouse click events.

- **render** will be called according to the core framerate with the context methods. You must use given context to render all elements that must be displayed on screen. **Don't compute any interaction in this method.**
    > Context has **drawSprite** and **drawText** methods that helps you to render elements on screen.

### Graphical

Graphical coordinates will always be passed as unit coordinates. You must use a grid system to render sprite at right position. (eg : one unit can be 32px in the render library).

- **loadSpritesheets** will recieve the output of game's **getSpritesheets** method and must load spritesheet as a sprite matrix based on **unitDims** field. After loading, sprite must be accessed with coordinates on the matrix such as (1, 0) for the second sprite of the first line.
    > For textual libraries you'll have to use given textual spritesheets.

    > Spritesheet are identified by unique id field and will be requested with the same id to make reference to it.

- **pollEvents** must return a vector of all event that happend since the last call. Event must be key inputs _(in that case, set coordinates to 0, 0)_ and also mouse inputs that will have relative coordinates associated.

- **clearWin** must clear the window content. That means removing all displayed and prepared to display sprites.

- **refreshWin** must update window visible content according to the previously drawn sprites and texts.

- **drawSprite** must place sprite extracted from the spritesheet based on provided data in **SpriteCostume** structure at given coordinates. The render of the sprite must be effective at the next **refreshWin** and be removed by calling **clearWin**.
    > **SpriteCostume** will contain spritesheet id and the coordinates of the sprite on it's grid.

- **drawText** works the same as **drawSprite** but use data about text display to show text. Font must be managed standalone in your library but you must be able to handle font color and background color.

## Examples

Here are some implementations example to understand the different structures.

### Game

Simple implementation of an Hello World game.

**Class declaration**

Class must be declared in a **game** namespace.

```cpp
class HelloWorld : public IGame
{
public:
    HelloWorld();
    ~HelloWorld();
    // override all methods here

private:
    // hello or goodbye ?
    _hello = true;
};
```

**Render**

One of the most usefull method will be the render.

```cpp
void game::HelloWorld::render(arcade::IContext &context)
{
    std::string text = _hello ? "Hello world" : "Goodbye world";
    context.drawText({text, 7, 0}, 5, 10);
}
```

**Handle event**

Change text on keyboard input.

```cpp
void game::HelloWorld::handleInput(const arcade::KeyInput &key)
{
    if (key.mapping == arcade::ACTIONA)
        _hello = !_hello;
}
```

**extern "C"**

Provide all needed data to identify your game

```cpp
extern "C" {
    game::IGame *entryPoint()
    {
        return new game::HelloWorld;
    }
    int getType()
    {
        return arcade::GAME;
    }
    std::string getName()
    {
        return "Hello World";
    }
}

```

### Graphical

Example of SFML implementation.

**Class declaration**

Class must be declared in a **render** namespace.

```cpp
// Struct to keep spritesheet loaded texture
struct LoadedSpritesheet {
    Position dims;
    sf::Texture texture;
};

class SFMLDisplay : public IRenderer
{
public:
    SFMLDisplay();
    ~SFMLDisplay();
    // override all methods here

private:
    // method to load a single spritesheet
    void _loadSpritesheet(const Spritesheet &spritesheet);
    // keep track of render window
    sf::RenderWindow _window;
    // loaded spritesheets
    std::map<std::size_t, LoadedSpritesheet> _spritesheets;
    // used font
    sf::Font _font;
};
```

**Constructor**

Constructor must initialize used font and all needed dependencies to open the window.

```cpp
render::SFMLDisplay::SFMLDisplay()
{
    // try to load font
    if (!_font.loadFromFile("./path/to/font.ttf"))
        throw AssetException("...");
    // create SFML window
    _window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
}
```

**Deconstructor**

Deconstructor must free all used assets and close the window.

```cpp
render::SFMLDisplay::~SFMLDisplay()
{
    _window.close();
}
```

**Load spritesheets**

Here we load given spritesheets and store it to our **LoadedSpritesheet** map.

We decided to use a sub **_loadSpritesheet** method to keep Single Responsibility and make code easier to understand.

```cpp
void render::SFMLDisplay::_loadSpritesheet(
const arcade::Spritesheet &spritesheet)
{
    // use graphic path because we create a non-textual library
    std::string path = spritesheet.graphicPath;
    sf::Texture texture;

    // try to load texture from file
    if (!texture.loadFromFile(path))
        throw AssetException("...");
    // store additional data and texture in the map
    _spritesheets[spritesheet.id] = {spritesheet.unitDims, texture};
}

void render::SFMLDisplay::loadSpritesheets(
const std::vector<arcade::Spritesheet> &spritesheets)
{
    // clear previously loaded spritesheets
    _spritesheets.clear();
    // iterate over given spritesheets to load them
    for (const auto &spritesheet : spritesheets)
        _loadSpritesheet(spritesheet);
}
```

**Clear window**

In SFML we can simply call the **clear** method of the render window. The implementation is pretty similar for any library.

```cpp
void render::SFMLDisplay::clearWin()
{
    _window.clear();
}
```

**Refresh window**

The same as clear window, we can simply call **display** method here.

```cpp
void render::SFMLDisplay::refreshWin()
{
    _window.display();
}
```

**Draw sprite**

Draw sprite method must retrieve sprite on spritesheet and place it at the right position in the grid. Position could be a floating number. We finally call **draw** method of the renderWindow to place sprite.

```cpp
void render::SFMLDisplay::drawSprite(
const arcade::SpriteCostume &sprite, double x, double y)
{
    // Get loaded spritesheet
    LoadedSpritesheet &spritesheet = _spritesheets[sprite.spritesheet];
    // Prepare sprite
    sf::Sprite spriteToRender;
    const Position &idx = sprite.pos;
    const Position &dims = spritesheet.dims;

    // Apply whole spritesheet texture
    spriteToRender.setTexture(spritesheet.texture);
    // Select rectangle to get single sprite
    spriteToRender.setTextureRect(
    sf::IntRect(idx.x * dims.x, idx.y * dims.y, dims.x, dims.y));
    // Place and scale sprite according to defined unit (32px for example)
    spriteToRender.setOrigin(0, 0);
    spriteToRender.setPosition(x * SPRITE_SIZE, y * SPRITE_SIZE);
    spriteToRender.setScale(
    SPRITE_SIZE / (float)dims.x, SPRITE_SIZE / (float)dims.y);
    // Call draw method to prepare sprite for next render
    _window.draw(spriteToRender);
}
```

**Draw sprite**

We proceed pretty the same as for **drawSprite** but we now work with text.

```cpp
void render::SFMLDisplay::drawText(
const arcade::TextDisplay &text, double x, double y)
{
    // Prepare the text
    sf::Text textToRender;
    textToRender.setFont(_font);
    textToRender.setString(text.content);
    textToRender.setCharacterSize(23);
    textToRender.setFillColor(text.fg);
    textToRender.setPosition(x * SPRITE_SIZE, y * SPRITE_SIZE);
    // Use draw method
    _window.draw(textToRender);
}
```

**Event polling**

We'll use **pollEvent** method here but we'll need to translate **sf::Event** into our own **arcade::Event**.

```cpp
std::vector<arcade::Event> render::SFMLDisplay::pollEvents()
{
    // Prepare the return value
    std::vector<arcade::Event> events;
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            arcade::RawKey keyId; // implements a way to get a rawKey
            events.push_back({keyId, {0, 0}});
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            // get mouse position and click type to push the same way as abode
        }
    }
    return events;
}
```

**extern "C"**

The implementation of the needed extern "C" to make the core able to recognize your library.

```cpp
extern "C" {
    render::IRenderer *entryPoint()
    {
        return new render::SFMLDisplay;
    }
    int getType()
    {
        return arcade::GRAPHICAL;
    }
    std::string getName()
    {
        return "SFML";
    }
}