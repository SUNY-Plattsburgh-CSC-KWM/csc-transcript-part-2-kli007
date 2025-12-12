#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "UI.hpp"
#include "banner.hpp"

using namespace std;

//Kevin Li SFML 2.6.1

int main() {
    Transcript* transcript = new Transcript();
    bool NumberOn = true;

    sf::RenderWindow window(sf::VideoMode(800, 600), "UI Test");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Failed to load font.\n";
        return 1;
    }

    // ======================
    // BUTTONS (STACKED LEFT)
    // ======================
    Button btnCSemester({200, 50}, font, "Create Semester", 22);
    Button btnDSemester({200, 50}, font, "Delete Semester", 22);
    Button btnCCourse  ({200, 50}, font, "Create Course",   22);
    Button btnDCourse  ({200, 50}, font, "Delete Course",   22);
    Button btnExport   ({200, 50}, font, "Export Transcript", 22);
    Button btnImport   ({200, 50}, font, "Import Transcript", 22);

    btnCCourse.setPosition   ({10,  40});
    btnDCourse.setPosition   ({10, 110});
    btnCSemester.setPosition ({230, 40});
    btnDSemester.setPosition ({230, 110});
    btnExport.setPosition    ({10, 180});
    btnImport.setPosition    ({230, 180});

    // ======================
    // TEXTBOX (RIGHT SIDE)
    // ======================
    Textbox textbox({260, 40}, font, 20);
    textbox.setPosition({480, 60});

    // starting mode from your UI.hpp
    textbox.setMode("start", transcript);

    btnCCourse.setCallback([&]() {
        textbox.setMode("createCourse", transcript);
    });

    btnDCourse.setCallback([&]() {
        textbox.setMode("deleteCourse", transcript);
    });

    btnCSemester.setCallback([&]() {
        textbox.setMode("createSemester", transcript);
    });

    btnDSemester.setCallback([&]() {
        textbox.setMode("deleteSemester", transcript);
    });

    btnExport.setCallback([&]() {
        transcript->storeData();
    });

    btnImport.setCallback([&]() {
        transcript->readData();
    });

    // ======================
    // INFO TEXT (BOTTOM PANEL)
    // ======================
    sf::Text info;
    info.setFont(font);
    info.setCharacterSize(18);
    info.setFillColor(sf::Color::White);

    // -------- scrolling + view --------
    const float panelTop    = 320.f;              // where info panel starts on the window
    const float panelHeight = 260.f;              // how tall the panel is
    float scroll = 0.f;                           // how far down we scrolled (0 = top)

    // View in "world" coords: shows [0..panelHeight] of the info area
    sf::View infoView(sf::FloatRect(0.f, 0.f, 800.f, panelHeight));

    // Viewport in window coords: maps that view to the bottom strip of the window
    infoView.setViewport(sf::FloatRect(
        0.f,                 // left edge of window (0–1)
        panelTop / 600.f,    // top as percentage of window height
        1.f,                 // full width
        panelHeight / 600.f  // height as percentage
    ));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            btnCSemester.handleEvent(event, window);
            btnDSemester.handleEvent(event, window);
            btnCCourse.handleEvent(event, window);
            btnDCourse.handleEvent(event, window);
            btnExport.handleEvent(event, window);
            btnImport.handleEvent(event, window);
            textbox.handleEvent(event, window);

            // Mouse wheel → update scroll
            if (event.type == sf::Event::MouseWheelScrolled) {
                sf::Vector2i mpos = sf::Mouse::getPosition(window);

                // Only scroll if mouse is over the info panel area
                if (mpos.y >= panelTop) {
                    // In SFML: delta > 0 when wheel up, < 0 when down.
                    // We want wheel down to move content UP (see later lines),
                    // so increase scroll when delta < 0:
                    scroll -= event.mouseWheelScroll.delta * 20.f;
                }
            }
        }

        // ---------- BUILD INFO STRING ----------
        string out;
        int lineCount = 0;

        if (transcript != nullptr) {
            transcript->calculateGPA();
            out += "Student Name: " + transcript->getStudentName();
            out += " | GPA: " + to_string(transcript->getGPA()) + "\n\n";
            lineCount += 2;

            auto& semMap = transcript->getSemList();  // map<string, Semester>
            for (auto& s : semMap) {
                Semester& sem = s.second;
                sem.calcSemGPA();

                out += "Semester ID: " + s.first;
                out += " | Year: "      + sem.getYear();
                out += " | Extension: " + sem.getExtension();
                out += " | Semester GPA: " + to_string(sem.getSemGPA()) + "\n";
                out += "Courses:\n";
                lineCount += 2;

                auto& courseMap = sem.getCourseList();
                for (auto& c : courseMap) {
                    out += "ID: " + c.second.getIdentification();
                    out += " | Designation: " + c.second.getDesignation();
                    out += " | Name: " + c.second.getName();
                    out += " | Credits: " + to_string(c.second.getCredit());
                    out += " | Grade: " + c.second.getGrade() + "\n";
                    lineCount++;
                }

                out += "\n";
                lineCount++;
            }
        }

        // ---------- CLAMP SCROLL RANGE ----------
        float lineHeight    = 22.f;                     // pixels per line
        float contentHeight = lineCount * lineHeight;   // total text height

        if (contentHeight <= panelHeight) {
            // Everything fits, show top only.
            scroll = 0.f;
        } else {
            // View shows [scroll .. scroll + panelHeight] in world coords.
            // scroll ∈ [0, contentHeight - panelHeight]
            float maxScroll = contentHeight - panelHeight;
            if (scroll < 0.f)        scroll = 0.f;
            if (scroll > maxScroll)  scroll = maxScroll;
        }

        // ---------- UPDATE TEXT POSITION ----------
        info.setString(out);
        // text lives in its own "info world", starting at y = 0
        info.setPosition(20.f, 0.f);

        // Center the view over the correct slice of the info text
        float centerY = scroll + panelHeight / 2.f;
        infoView.setCenter(400.f, centerY);  // x = half window width, y = scroll + half panel

        // ---------- DRAW ----------
        window.clear(sf::Color(50, 50, 50));

        // Draw UI (default full-window view)
        window.setView(window.getDefaultView());
        btnCSemester.draw(window);
        btnDSemester.draw(window);
        btnCCourse.draw(window);
        btnDCourse.draw(window);
        btnExport.draw(window);
        btnImport.draw(window);
        textbox.draw(window);

        // Draw info text using the clipped view in the bottom panel
        window.setView(infoView);
        window.draw(info);

        // Restore default view for anything else later
        window.setView(window.getDefaultView());

        window.display();
    }

    return 0;
}
