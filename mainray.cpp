#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "handler.hpp"

std::map<char, Texture2D> pieceTextures;

void LoadPieceTextures() {
    const std::string basePath = "C:/Users/930 RWB/Desktop/Programacion/Chess_Classy/assets/";
    const std::vector<std::pair<char, std::string>> files = {
        {'K', "wK.png"}, {'Q', "wQ.png"}, {'R', "wR.png"},
        {'B', "wB.png"}, {'N', "wN.png"}, {'P', "wP.png"},
        {'k', "bK.png"}, {'q', "bQ.png"}, {'r', "bR.png"},
        {'b', "bB.png"}, {'n', "bN.png"}, {'p', "bP.png"}
    };

    for (auto &[key, filename] : files)
        pieceTextures[key] = LoadTexture((basePath + filename).c_str());
}

void UnloadPieceTextures() {
    for (auto &entry : pieceTextures)
        UnloadTexture(entry.second);
}

int main() {
    bool win = false;
    bool tie = false;
    bool check = false;
    Colors winner = N;
    Handler H;

    const int screenWidth = 480;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "Chess Classy");

    LoadPieceTextures();

    const int cellSize = 60; // 8x8 board
    SetTargetFPS(60);

    // --- Selection state ---
    int selectedX = -1, selectedY = -1;
    int prevMovX = -1, prevMovY = -1, prevMovX2 = -1, prevMovY2 = -1;
    int incheckXY = -1, incheckX = -1, incheckY = -1;
    Piece* selectedPiece = nullptr;
    
    int moveCount = H.getMoveCounter();
    char text[8];
    snprintf(text, sizeof(text), "%d", moveCount);

    while (!WindowShouldClose()) {
        if (!win && !tie){
        // --- INPUT HANDLING ---
        Vector2 mousePos = GetMousePosition();
        int gridX = (int)(mousePos.x / cellSize);
        int gridY = (int)(mousePos.y / cellSize);

        Piece* (&board)[8][8] = H.getBoard();

        // Handle left click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8) {
                Piece* clickedPiece = board[gridX][7-gridY];

                if (clickedPiece != nullptr) {
                    char pieceColor = clickedPiece->getcolor();
                    // Clicked on your own piece
                    if (pieceColor == H.getcurrentcolor()) {
                        // Clicking the same piece unselects it
                        if (selectedX == gridX && selectedY == gridY) {
                            selectedX = selectedY = -1;
                            selectedPiece = nullptr;
                        } else {
                            selectedX = gridX;
                            selectedY = gridY;
                            selectedPiece = clickedPiece;
                        }
                    }else{
                        if (selectedX != -1 && selectedY!=-1){
                            // Debug Options
                                int fromX = selectedX;
                                int fromY = 7 - selectedY; // flip if you stored screen Y
                                int toX   = gridX;
                                int toY   = 7 - gridY;     // flip screen Y to board Y

                            if (H.makemove(fromX, fromY, toX, toY,false)){
                                prevMovX = selectedX;
                                prevMovY = selectedY;
                                prevMovX2 = gridX;
                                prevMovY2 = gridY;
                                moveCount = H.getMoveCounter();
                                snprintf(text, sizeof(text), "%d", moveCount);
                            }
                            // H.printboard(); Debug option
                            check = H.incheck(H.getcurrentcolor(),true);
                            if (check){
                                win = H.checkMate();
                                char offset=(H.getcurrentcolor() == W) ? 0 : 32;
                                incheckXY=H.findPiece('K' + offset);
                                incheckX=incheckXY&0xff;
                                incheckY=7 - (incheckXY >> 8) & 0xFF;
                            }else{
                                incheckX = incheckY = -1;
                            }
                            tie = H.tie();
                        }
                    }
                } 
            }
        }

        // Handle right click: always cancel selection
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            selectedX = selectedY = -1;
            selectedPiece = nullptr;
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw chessboard
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                bool dark = (x + y) % 2 == 1;
                Color color = dark ? (Color){118, 150, 86, 255} : (Color){238, 238, 210, 255};
                DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
            }
        }

        // Highlight hovered square
        if (gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8)
            DrawRectangleLines(gridX * cellSize, gridY * cellSize, cellSize, cellSize, RED);

        // Highlight selected square (persistent)
        if (selectedX != -1 && selectedY != -1)
            DrawRectangle(selectedX * cellSize, selectedY * cellSize, cellSize, cellSize, {255, 165, 0, 100});

        if (prevMovX != -1 && prevMovY != -1 && prevMovX2 != -1 && prevMovY2 != -1){
            DrawRectangle(prevMovX * cellSize, prevMovY * cellSize, cellSize, cellSize, {0, 200, 255, 100});
            DrawRectangle(prevMovX2 * cellSize, prevMovY2 * cellSize, cellSize, cellSize, {0, 200, 255, 100});
        }

        if (incheckX != -1 && incheckY != -1)
            DrawRectangle(incheckX * cellSize, incheckY * cellSize, cellSize, cellSize, {255, 0, 0, 100});

        // Draw pieces
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < 8; ++j) {
                Piece* piecePtr = board[j][i];
                if (piecePtr != nullptr && (piecePtr->getcolor()==B || piecePtr->getcolor()==W)) {
                    char piece = piecePtr->gettype();
                    auto it = pieceTextures.find(piece);
                    if (it != pieceTextures.end()) {
                        Texture2D &tex = it->second;

                        float scale = (float)cellSize / tex.height;

                        // Coordinates: j → x, i flipped → y
                        float drawX = j * cellSize + (cellSize - tex.width * scale) / 2;
                        float drawY = (7 - i) * cellSize + (cellSize - tex.height * scale) / 2;

                        DrawTextureEx(tex, (Vector2){drawX, drawY}, 0.0f, scale, WHITE);
                    }
                }
            }
        }
        DrawText(text, GetScreenWidth() - 30, 10, 44, YELLOW);
    }else{
        if(win) {
            char winner = H.getcurrentcolor(); // 'W' or 'B'

            // Choose colors depending on winner
            Color bgColor  = (winner == B) ? Fade(WHITE, 0.85f) : Fade(BLACK, 0.85f);
            Color textColor = (winner == B) ? BLACK : WHITE;

            // Draw semi-transparent background overlay
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), bgColor);

            std::string message = (winner == B) ? "White Wins!" : "Black Wins!";

            int fontSize = 50;
            int textWidth = MeasureText(message.c_str(), fontSize);

            // Draw main message centered
            DrawText(
                message.c_str(),
                GetScreenWidth()/2 - textWidth/2,
                GetScreenHeight()/2 - fontSize/2,
                fontSize,
                textColor
            );

            // Hint text (restart)
            DrawText(
                "Press R to restart",
                GetScreenWidth()/2 - 100,
                GetScreenHeight()/2 + 60,
                20,
                (winner == B) ? DARKGRAY : LIGHTGRAY
            );

            if (IsKeyPressed(KEY_R)) {
                //todo restart
                win = false;
            }
            }    
        }
        if(tie) {
            int screenW = GetScreenWidth();
            int screenH = GetScreenHeight();

            // --- Draw background halves ---
            DrawRectangle(0, 0, screenW/2, screenH, BLACK);
            DrawRectangle(screenW/2, 0, screenW/2, screenH, WHITE);

            // --- Text setup ---
            const char* message1 = " It's";
            const char* message2 = "       a Tie!";
            const char* restartMsg = "Press R to restart";
            int fontSize = 50;
            int smallFont = 20;

            int textWidth = MeasureText(message2, fontSize);
            int restartWidth = MeasureText(restartMsg, smallFont);

            // --- Draw message centered (split color text) ---
            DrawText(
                message1,
                screenW/2 - textWidth/2,
                screenH/2 - fontSize/2,
                fontSize,
                WHITE
            );

            DrawText(
                message2,
                screenW/2 - textWidth/2,
                screenH/2 - fontSize/2,
                fontSize,
                BLACK
            );

            DrawText(
                restartMsg,
                screenW/2 - restartWidth/2,
                screenH/2 + 60,
                smallFont,
                GRAY
            );

            if (IsKeyPressed(KEY_R)) {
                // Reset tie state
                tie = false;
            }
        }                 
        EndDrawing();
    }

    UnloadPieceTextures();
    CloseWindow();
    return 0;
}