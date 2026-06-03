#include "render.h"
#include <algorithm>
#include <iostream>
#include <sstream>

static const char* patternNames[] = {
    "Kolovrat",
    "Face",
    "Akademgorodok",
    "NSU (NGU)",
    "Predator / Photo"
};
static constexpr int NUM_PATTERNS = 5;

Render::Render(const Field& field)
    : field(field)
{
    int fieldPixelW = field.get_size_W() * CELL_SIZE;
    int fieldPixelH = field.get_size_H() * CELL_SIZE;
    int windowW = fieldPixelW + UI_PANEL_WIDTH;
    int windowH = std::max(fieldPixelH, 400);

    window.create(sf::VideoMode(windowW, windowH),
                  "Bacteria Simulation", sf::Style::Close);
    window.setFramerateLimit(60);

    fieldRT.create(fieldPixelW, fieldPixelH);

    ImGui::SFML::Init(window);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding  = 4.0f;
    style.GrabRounding   = 4.0f;
    style.Colors[ImGuiCol_WindowBg]   = ImVec4(0.12f, 0.12f, 0.15f, 0.97f);
    style.Colors[ImGuiCol_Button]     = ImVec4(0.22f, 0.22f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.35f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.45f, 0.45f, 0.55f, 1.0f);

    updateFieldTexture();
}

Render::~Render() {
    ImGui::SFML::Shutdown();
}

void Render::updateFieldTexture() {
    fieldRT.clear(sf::Color(15, 15, 25)); 

    const int W = field.get_size_W();
    const int H = field.get_size_H();

    for (int x = 0; x < W; ++x) {
        for (int y = 0; y < H; ++y) {
            const auto& cell = field.get_cell(x, y);
            if (cell.GetCode() != 1) continue;

            const Bacterium* bac = cell.as<Bacterium>();
            if (!bac) continue;

            int bSize = bac->GetState().bacteriumSize;
            float radius = static_cast<float>(bSize) * (CELL_SIZE * 0.5f);

            sf::CircleShape circle(radius);
            float cx = x * CELL_SIZE + CELL_SIZE * 0.5f - radius;
            float cy = y * CELL_SIZE + CELL_SIZE * 0.5f - radius;
            circle.setPosition(cx, cy);

            int en = bac->GetState().energy;
            int brightness = std::clamp(80 + en * 4, 80, 255);

            if (bac->GetType() == BacteriumType::PREDATOR) {
                circle.setFillColor(sf::Color(brightness, 25, 25));
            } else {
                circle.setFillColor(sf::Color(25, brightness, 25));
            }

            fieldRT.draw(circle);
        }
    }

    fieldRT.display();
}

void Render::update(unsigned int step, int totalCount,
                    int greenCount, int redCount)
{
    currentStep    = step;
    totalBacteria  = totalCount;
    greenBacteria  = greenCount;
    redBacteria    = redCount;
    updateFieldTexture();
}

bool Render::is_open() const {
    return window.isOpen();
}

void Render::drawUI() {
    ImGui::SFML::Update(window, deltaClock.restart());

    float panelX = static_cast<float>(field.get_size_W() * CELL_SIZE) + 8.0f;
    ImGui::SetNextWindowPos(ImVec2(panelX, 8.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(
        ImVec2(static_cast<float>(UI_PANEL_WIDTH) - 16.0f, 0.0f),
        ImGuiCond_Always);

    ImGui::Begin("##Controls", nullptr,
        ImGuiWindowFlags_NoResize  |
        ImGuiWindowFlags_NoMove    |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(nullptr);
    ImGui::TextColored(ImVec4(0.7f, 0.85f, 1.0f, 1.0f),
                       "BACTERIA SIMULATION");
    ImGui::PopFont();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Step:     %u", currentStep);
    ImGui::Text("Total:    %d", totalBacteria);
    ImGui::TextColored(ImVec4(0.3f, 0.95f, 0.3f, 1.0f),
                       "Green:    %d", greenBacteria);
    ImGui::TextColored(ImVec4(0.95f, 0.3f, 0.3f, 1.0f),
                       "Red:      %d", redBacteria);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (paused) {
        if (ImGui::Button("  Play  ", ImVec2(-1, 32))) {
            paused = false;
        }
    } else {
        if (ImGui::Button("  Pause  ", ImVec2(-1, 32))) {
            paused = true;
        }
    }

    if (ImGui::Button("  Single Step  ", ImVec2(-1, 32))) {
        singleStep = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.85f, 0.75f, 0.4f, 1.0f),
                       "Initial Pattern:");
    ImGui::Combo("##pattern", &selectedPattern, patternNames, NUM_PATTERNS);

    if (ImGui::Button("  Reset  ", ImVec2(-1, 32))) {
        resetRequested = true;
        paused = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("  Save PNG  ", ImVec2(-1, 32))) {
        std::string filename = "simulation_step_"
                             + std::to_string(currentStep) + ".png";
        savePNG(filename);
        std::cout << "Saved: " << filename << std::endl;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Legend:");
    ImGui::TextColored(ImVec4(0.3f, 0.95f, 0.3f, 1.0f),
                       "  Green = Photosynthesizer");
    ImGui::TextColored(ImVec4(0.95f, 0.3f, 0.3f, 1.0f),
                       "  Red   = Predator");
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f),
                       "  Circle size = bacteria size");

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f),
                       "Space = Play / Pause");

    ImGui::End();
}

void Render::draw() {
    window.clear(sf::Color(30, 30, 40));

    sf::Sprite fieldSprite(fieldRT.getTexture());
    window.draw(fieldSprite);

    float panelX = static_cast<float>(field.get_size_W() * CELL_SIZE);
    float panelH = static_cast<float>(window.getSize().y);
    sf::RectangleShape panelBg(sf::Vector2f(
        static_cast<float>(UI_PANEL_WIDTH), panelH));
    panelBg.setPosition(panelX, 0.0f);
    panelBg.setFillColor(sf::Color(25, 25, 35));
    window.draw(panelBg);

    drawUI();
    ImGui::SFML::Render(window);

    window.display();
}

void Render::process_events() {
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Space)
        {
            paused = !paused;
        }
    }
}

void Render::savePNG(const std::string& filename) {
    sf::Vector2u sz = window.getSize();
    sf::Texture tex;
    tex.create(sz.x, sz.y);
    tex.update(window);
    sf::Image img = tex.copyToImage();
    img.saveToFile(filename);
}
