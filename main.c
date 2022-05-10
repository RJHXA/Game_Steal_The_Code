#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define altura 720
#define largura 1280

int main(void) {    
    InitWindow(largura, altura, "Steal the Code");

    SetTargetFPS(60);

    // Loading Textures
    Texture2D logo_inicial = LoadTexture("textures/logoInicial.png");
    Texture2D logo_jogo = LoadTexture("textures/logoJogo.png");
    Texture2D logo_menu = LoadTexture("textures/logoMenu.png");
    Texture2D botao_start = LoadTexture("textures/Play.png");
    Texture2D botao_instru = LoadTexture("textures/Instructions.png");

    bool inicia_imagem = true;
    bool inicia_texto = false;
	bool inicia_jogo = false;
	int count_imagem = 0;
    int count = 0;
	
    while(!WindowShouldClose())
    {
        ClearBackground(BLACK);
        BeginDrawing();
        if(inicia_imagem == true)
        {
            if(count_imagem < 50)
            {
                DrawTexture(logo_inicial, 0, 0, Fade(BLACK, 1));
            }
            else if(count_imagem > 50 && count_imagem < 150)
            {
                DrawTexture(logo_inicial, 360, 90, WHITE);
            }
            else
            {
                DrawTexture(logo_inicial, 0, 0, Fade(BLACK, 1));
                if(count_imagem > 180)
                {
                    inicia_imagem = false;
                    inicia_texto = true;
                }
            }
            count_imagem++;
        }
        else if(inicia_texto == true)
        {
            if(count < 720)
            {
                DrawText("Todos Desesperados para a Prova de Pif...", 100, 0+count, 50, RED);
                DrawText("...e só uma pessoa pode salvar sua Turma...", 100, 60+count, 50, RED);
                DrawText("...o nome dele é...", 420, 120+count, 50, RED);
                DrawText("* PATRICK *", 350, 200+count, 80, WHITE);
            }
            else
            {
                inicia_texto = false;
            }
            count++;
        }
        else if(inicia_texto == false && inicia_imagem == false)
        {
            if(inicia_jogo == false)
            {
                DrawTexture(logo_menu, 0, 50, WHITE);
                //DrawText("AJUDE PATRICK A ROUBAR O GABARITO!",20,20,30, WHITE);
                DrawTexture(botao_start, 0, 60, WHITE);
                DrawRectangle(largura/2-300, altura-300, largura/2-20, 40, (Color){0,0,0,150});
                DrawTexture(botao_instru, 0, 30, WHITE);
                DrawRectangle(largura/2-300, altura-200, largura/2-20, 40, (Color){0,0,0,150});
                if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    inicia_jogo=true;
                }
            }
        }

        //DrawText("Titi brabo!", 12/2, 12/2, 16, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();        
    return 0;
}