#include <iostream>
#include <time.h> 
#include <SFML/Graphics.hpp>

using namespace sf;

float offsetX=0, offsetY=0;

const int H = 12;
const int W = 40;


String TileMap[H] =
    {

    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                B     B",
    "B                                B     B",
    "B                                B     B",
    "B                                B     B",
    "B         0000                BBBB     B",
    "B                                B     B",
    "BBB                              B     B",
    "B              BB                BB    B",
    "B              BB                      B",
    "B    B         BB         BB           B",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

    }; 

class PLAYER {

public:

    float dx,dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;
    int weaponState;    //Положение ножа (0 - не бьет, 1 - бьет)
    int health;         //Здоровье (0 - плохо, больше 0 - жив)

    PLAYER(Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(7*32,9*32,40,50);

        dx = dy = 0.1;
        currentFrame = 0;
        health = 10;
        weaponState = 0;
    }


    void update(float time)
    {    
        rect.left += dx * time;    
        Collision(0);

        if (!onGround) dy=dy+0.0005*time;
        rect.top += dy*time;
        onGround=false;
        Collision(1);
  
     
        currentFrame += 0.005*time;
        if (currentFrame > 6) currentFrame -=6 ;

        if (dx>0 && weaponState == 0) sprite.setTextureRect(IntRect(40*int(currentFrame)   ,244, 40,50));
        if (dx<0 && weaponState == 0) sprite.setTextureRect(IntRect(40*int(currentFrame)+40,244,-40,50));
        if (weaponState > 0)  sprite.setTextureRect(IntRect(90   ,570, 40,50));
        if (weaponState < 0)  sprite.setTextureRect(IntRect(90+40,570,-40,50));


        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx=0;
    }



    void Collision(int dir)
    {
        for (int i = rect.top/32 ; i<(rect.top+rect.height)/32; i++)
            for (int j = rect.left/32; j<(rect.left+rect.width)/32; j++)
            { 
                if (TileMap[i][j]=='B') 
                { 
                    if ((dx>0) && (dir==0)) rect.left =  j*32 -  rect.width; 
                    if ((dx<0) && (dir==0)) rect.left =  j*32 + 32;
                    if ((dy>0) && (dir==1))  { rect.top =   i*32 -  rect.height;  dy=0;   onGround=true; }
                    if ((dy<0) && (dir==1))  { rect.top = i*32 + 32;   dy=0;}
                }

                if (TileMap[i][j]=='0') 
                { 
                    TileMap[i][j]=' ';
                }

            }

    }
};

int main()
{
    RenderWindow window( VideoMode(600, 400), "Test!");
    srand (time(NULL));

    Texture t;
    t.loadFromFile("fang.png");

    float currentFrame=0;

    PLAYER p(t);
    PLAYER vrag(t);

    Clock clock;

    RectangleShape rectangle( Vector2f(32,32));

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time/700;
        
        if (time>20) time = 20;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


        //Сделать движения функциями
        if (p.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::Left)) 
        {
            p.dx = -0.1;
        }

        if (p.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::Right)) 
        {
            p.dx = 0.1;
        }

        if (p.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::Up)) 
        {
            if (p.onGround) { p.dy=-0.35; p.onGround=false;}
        }

        if (Keyboard::isKeyPressed(Keyboard::Space)) 
        {
            if      (p.weaponState == 0 && p.dx > 0) { p.weaponState =  1; p.dx = 0;    p.dy = 0;}
            else if (p.weaponState == 0 && p.dx < 0) { p.weaponState = -1; p.dx = 0;    p.dy = 0;}
            else if (p.weaponState == 1)             { p.weaponState =  0; p.dx =  0.1; p.dy = 0.1;}
            else if (p.weaponState == -1)            { p.weaponState =  0; p.dx = -0.1; p.dy = 0.1;}
        }


        if (vrag.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::A)) 
        {
            vrag.dx = -0.1;
        }

        if (vrag.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::D)) 
        {
            vrag.dx = 0.1;
        }

        if (vrag.weaponState == 0 && Keyboard::isKeyPressed(Keyboard::W)) 
        {
            if (vrag.onGround) { vrag.dy=-0.35; vrag.onGround=false;}
        }

        if (Keyboard::isKeyPressed(Keyboard::X)) 
        {
            if      (vrag.weaponState == 0 && vrag.dx > 0) { vrag.weaponState =  1; vrag.dx = 0;    vrag.dy = 0;}
            else if (vrag.weaponState == 0 && vrag.dx < 0) { vrag.weaponState = -1; vrag.dx = 0;    vrag.dy = 0;}
            else if (vrag.weaponState == 1)                { vrag.weaponState =  0; vrag.dx =  0.1; vrag.dy = 0.1;}
            else if (vrag.weaponState == -1)               { vrag.weaponState =  0; vrag.dx = -0.1; vrag.dy = 0.1;}
        }

        //Столкнулись
        if (abs(p.rect.left - vrag.rect.left) < 40 && abs(p.rect.top - vrag.rect.top) < 40)
        {
            if (p.weaponState == 1 && vrag.weaponState == 0) 
            {
                vrag.health = vrag.health - 1;
            }
            if (p.weaponState == 0 && vrag.weaponState == 1) 
            {
                p.health = p.health - 1;
            }
        }


        p.update(time);
        vrag.update(time);

        if (p.rect.left>300) offsetX = p.rect.left - 300;
        offsetY = p.rect.top - 200;

        window.clear(Color::White);


        for (int i=0; i<H; i++)
            for (int j=0; j<W ; j++)
            { 
                if (TileMap[i][j]=='B') rectangle.setFillColor(Color::Black);

                if (TileMap[i][j]=='0') rectangle.setFillColor(Color::Green);

                if (TileMap[i][j]==' ') continue;

                rectangle.setPosition(j*32-offsetX,i*32 - offsetY) ; 
                window.draw(rectangle);
            }
    
        if (p.health <= 0) {
            sf::Font font;
            sf::Text text;
            sf::Time t2 = sf::milliseconds(3000);
            if (!font.loadFromFile("arial.ttf"))
            {
             // ошибка...
            }
            // select the font
            text.setFont(font); // font is a sf::Font

            // set the string to display
            text.setString("You are looser");

            // set the character size
            text.setCharacterSize(24); // in pixels, not points!

            // set the color
            text.setColor(sf::Color::Red);

            // set the text style
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);

            

            // inside the main loop, between window.clear() and window.display()
            window.draw(text);
            window.display();
            sleep(t2);

            exit(0);
        }

        if (vrag.health <= 0) {
            sf::Font font;
            sf::Text text;
            sf::Time t2 = sf::milliseconds(3000);
            if (!font.loadFromFile("arial.ttf"))
            {
             // ошибка...
            }
            // select the font
            text.setFont(font); // font is a sf::Font

            // set the string to display
            text.setString("You are winner");

            // set the character size
            text.setCharacterSize(24); // in pixels, not points!

            // set the color
            text.setColor(sf::Color::Red);

            // set the text style
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);

            

            // inside the main loop, between window.clear() and window.display()
            window.draw(text);
            window.display();
            sleep(t2);

            exit(0);
        }

        window.draw(p.sprite);
        window.draw(vrag.sprite);
        window.display();
    }

    return 0;
}