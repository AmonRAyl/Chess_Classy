#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "handler.hpp"

std::map<char, Texture2D> pieceTextures;

void LoadPieceTextures() {
    const std::string basePath = "assets/";
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
    Handler H;

    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Chess Classy");

    LoadPieceTextures();

    const int cellSize = 100; // 8x8 board
    SetTargetFPS(60);

    // --- Selection state ---
    int selectedX = -1, selectedY = -1;
    int prevMovX = -1, prevMovY = -1, prevMovX2 = -1, prevMovY2 = -1;
    Piece* selectedPiece = nullptr;

    while (!WindowShouldClose()) {
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

                                // // Debug prints
                                // std::cout << "==== Move Attempt ====" << std::endl;
                                // std::cout << "From (screen): (" << selectedX << ", " << selectedY << ")" << std::endl;
                                // std::cout << "To   (screen): (" << gridX << ", " << gridY << ")" << std::endl;
                                // std::cout << "From (board):  (" << fromX << ", " << fromY << ")" << std::endl;
                                // std::cout << "To   (board):  (" << toX << ", " << toY << ")" << std::endl;

                                // // Print involved pieces
                                // Piece* fromPiece = H.getBoard()[fromX][fromY];
                                // Piece* toPiece   = H.getBoard()[toX][toY];

                                // std::cout << "From piece: " 
                                //         << (fromPiece ? fromPiece->gettype() : ' ') << std::endl;
                                // std::cout << "To piece:   " 
                                //         << (toPiece ? toPiece->gettype() : ' ') << std::endl;
                                // std::cout << "======================" << std::endl;
                            if (H.makemove(fromX, fromY, toX, toY)){
                                prevMovX = selectedX;
                                prevMovY = selectedY;
                                prevMovX2 = gridX;
                                prevMovY2 = gridY;
                            }
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
                bool dark = (x + y) % 2 == 0;
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
        // Draw pieces
        for (int i = 7; i >= 0; --i) {
            for (int j = 0; j < 8; ++j) {
                Piece* piecePtr = board[j][i];
                if (piecePtr != nullptr) {
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

        EndDrawing();
    }

    UnloadPieceTextures();
    CloseWindow();
    return 0;
}