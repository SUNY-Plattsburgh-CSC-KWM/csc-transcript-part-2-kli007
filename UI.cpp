#include "UI.hpp"


Button::Button(const sf::Vector2f& size, const sf::Font& font, const std::string& label, unsigned int charSize){
    rect.setSize(size);
    rect.setFillColor(normalColor);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);

    // center text
    sf::FloatRect tb = text.getLocalBounds();
    text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    buttonCount = 0;
}

void Button::setPosition(const sf::Vector2f& pos) {
    rect.setPosition(pos);
    sf::Vector2f center = pos + rect.getSize() / 2.f;
    text.setPosition(center);
}

void Button::setCallback(std::function<void()> cb) {
    callback = std::move(cb);
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool contains = rect.getGlobalBounds().contains(mousePos);

    if (event.type == sf::Event::MouseMoved) {
        rect.setFillColor(contains ? hoverColor : normalColor);
    }

    if (event.type == sf::Event::MouseButtonPressed ) {
        if (event.mouseButton.button == sf::Mouse::Left && contains) {
            rect.setFillColor(pressedColor);
            buttonCount++;
            if (callback) callback();
        }
    }

    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {

        rect.setFillColor(contains ? hoverColor : normalColor);
    }
}

void Button::draw(sf::RenderTarget& rt) const {
    rt.draw(rect);
    rt.draw(text);
}

int Button::getCount() const{
    return buttonCount;
}


// Textbox class implementation

Textbox::Textbox(const sf::Vector2f& size,
                 const sf::Font& font,
                 unsigned int charSize){
    rect.setSize(size);
    rect.setFillColor(inactiveColor);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.f);

    text.setFont(font);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);

    promptText.setFont(font);
    promptText.setCharacterSize(charSize);
    promptText.setFillColor(sf::Color::Yellow);

    mode = "start";
}


void Textbox::setPosition(const sf::Vector2f& pos){
    rect.setPosition(pos);
    text.setPosition(pos.x + 5, pos.y + 5);
}


void Textbox::handleEvent(const sf::Event& event,
                          const sf::RenderWindow& window){
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    bool hovered = rect.getGlobalBounds().contains(mousePos);

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left){
        active = hovered;
        rect.setFillColor(active ? activeColor : inactiveColor);
    }

    if (!active) return;

    // typing
    if (event.type == sf::Event::TextEntered){
        char c = static_cast<char>(event.text.unicode);

        // handle backspace
        if (event.text.unicode == 8){
            if (!textString.empty())
                textString.pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode <= 126){
            textString += c;
        }

        text.setString(textString);
    }

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter){
        if (!textString.empty()) {
            if (mode == "start") {
                start();
            }
            if (mode == "createSemester") {
                cSemester();
            }
            if (mode == "deleteSemester") {
                dSemester();
            }
            if (mode == "createCourse") {
                cCourse();
            }
            if (mode == "deleteCourse") {
                dCourse();
            }
            if (mode.empty()) {
                active = false;
                rect.setFillColor(inactiveColor);
            }
            textString.clear();
            text.setString("");
            //use mode to case the different methods
        }
    }
}


void Textbox::draw(sf::RenderTarget& rt) const{
    rt.draw(rect);
    rt.draw(text);

    float offsetY = rect.getPosition().y + rect.getSize().y + 10.f;

    
    sf::Text temp;
    temp.setFont(*text.getFont());
    temp.setCharacterSize(text.getCharacterSize());
    temp.setFillColor(sf::Color::White);

    sf::Text p = promptText;
    p.setPosition(rect.getPosition().x,
                  rect.getPosition().y - p.getCharacterSize() - 5.f);
    rt.draw(p);
}


void Textbox::setMode(string newMode, Transcript* newTranscript){
    if (newMode == "start"){
        prompt = "Enter Your Name:";
    }
    if (newMode == "createSemester"){
        SI.identifier = "";
        SI.year = "";
        SI.extension = "";

        prompt = "Enter Semester Year (e.g., 2023):";
    }
    if (newMode == "deleteSemester"){
        SI.identifier = "";

        prompt = "Enter Semester ID to Delete:";
    }
    if (newMode == "createCourse"){
        SI.identifier = "";
        CI.ID = "";
        CI.design = "";
        CI.name = "";
        CI.credits = 0;
        CI.grade = "";

        prompt = "Enter the Semester ID:";
    }
    if (newMode == "deleteCourse"){
        SI.identifier = "";
        CI.ID = "";

        prompt = "Enter the Semester ID:";
    }
        
    step = 0;
    mode = newMode;
    active = true;
    promptText.setString(prompt);
    tempTrans = newTranscript;
    rect.setFillColor(activeColor);
}

void Textbox::start(){
    tempTrans -> setStudentName(textString);
    mode.clear();
    prompt = "Welcome, " + textString + "!";
    promptText.setString(prompt);
}

void Textbox::cSemester(){
    if (step == 0){
        SI.year = textString;
        step = 1;
        prompt = "Enter Semester Extension (e.g., Fall):";
    }
    else if (step == 1){
        SI.extension = textString;
        SI.identifier = SI.year + SI.extension;
        mode.clear();
        prompt = "Semester Created!";
        step = 0;
        tempTrans -> addToSemesterList(SI.identifier, SI.year, SI.extension);
        tempTrans = nullptr;
    }
    promptText.setString(prompt);
}

void Textbox::dSemester(){
    SI.identifier = textString;
    mode.clear();
    prompt = "Semester Deleted!";
    step = 0;
    tempTrans -> deleteSemester(SI.identifier);
    tempTrans = nullptr;
    promptText.setString(prompt);
}

void Textbox::cCourse(){
    switch (step){
        case 0:
            SI.identifier = textString;
            step = 1;
            prompt = "Enter Course ID:";
            break;
        case 1:
            CI.ID = textString;
            step = 2;
            prompt = "Enter Course Designation:";
            break;
        case 2:
            CI.design = textString; 
            step = 3;
            prompt = "Enter Course Name:";
            break;
        case 3:
            CI.name = textString;
            step = 4;
            prompt = "Enter Course Credits:";
            break;
        case 4:
            CI.credits = stoi(textString);
            step = 5;
            prompt = "Enter Course Grade:";
            break;
        case 5:
            CI.grade = textString;
            mode.clear();
            prompt = "Course Created!";
            step = 0;
            Semester* tempSem = tempTrans -> returnSemester(SI.identifier);
            if (tempSem != nullptr) {
                tempSem -> addToCourseList(CI.ID, CI.design, CI.name, CI.credits, CI.grade);
            }
            tempTrans = nullptr;
    }
    promptText.setString(prompt);
}

void Textbox::dCourse(){
    switch (step){
        case 0:
            SI.identifier = textString;
            step = 1;
            prompt = "Enter Course ID to Delete:";
            break;
        case 1:
            CI.ID = textString;
            mode.clear();
            prompt = "Course Deleted!";
            step = 0;
            Semester* tempSem = tempTrans -> returnSemester(SI.identifier);
            if (tempSem != nullptr){
                tempSem -> deleteCourse(CI.ID);
            }
            tempSem = nullptr;
    }
    promptText.setString(prompt);
}