#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include "banner.hpp"
#include <functional>
#include <vector>
#include <string>

using namespace std;

struct CourseInput {
    string ID;
    string design;
    string name;
    int credits;
    string grade;
};

struct SemesterInput {
    string identifier;
    string year;
    string extension;
};

class Button {
    private:
        sf::RectangleShape rect;
        sf::Text text;
        function<void()> callback;

        int buttonCount;

        const sf::Color normalColor  = sf::Color(70, 130, 180);
        const sf::Color hoverColor   = sf::Color(100, 160, 210);
        const sf::Color pressedColor = sf::Color(40, 100, 150);

    public:
        
        Button(const sf::Vector2f& size, const sf::Font& font, const std::string& label, unsigned int charSize);

        void setPosition(const sf::Vector2f& pos);

        void setCallback(std::function<void()> cb);

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

        void draw(sf::RenderTarget& rt) const ;

        int getCount() const;
};

class Textbox {
    private:
        sf::RectangleShape rect;
        sf::Text text;
        sf::Text promptText;

        bool active = false;
        string textString;
        string displayText = "";

        string mode;
        int step;

        CourseInput CI;
        SemesterInput SI;

        string prompt;
        Transcript* tempTrans = nullptr;

        const sf::Color inactiveColor = sf::Color(80, 80, 80);
        const sf::Color activeColor   = sf::Color(120, 120, 120);

    public:
        Textbox(const sf::Vector2f& size,
                const sf::Font& font,
                unsigned int charSize);

        void setPosition(const sf::Vector2f& pos);

        void setDisplayText(const string& text) {displayText = text;}

        void handleEvent(const sf::Event& event,
                        const sf::RenderWindow& window);

        void draw(sf::RenderTarget& rt) const;

        const string& getText() const { return textString; }

        void clear() { textString.clear(); text.setString(""); }

        void setMode(string newMode, Transcript* newTranscript);

        void start();

        void cSemester();

        void dSemester();

        void cCourse();

        void dCourse();


};



#endif
