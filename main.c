#include "raylib.h"
#include "physac.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define altura 720
#define largura 1280
#define framesSpeed 8

int main(void) {    
    InitWindow(largura, altura, "Steal the Code");

    SetTargetFPS(30);

    // Loading Textures BOTÕES E LOGO
    Texture2D logo_inicial = LoadTexture("textures/logoInicial.png");
    Texture2D logo_jogo = LoadTexture("textures/logoJogo.png");
    Texture2D logo_menu = LoadTexture("textures/logoMenu.png");
    Texture2D botao_start = LoadTexture("textures/Play.png");
    Texture2D botao_instru = LoadTexture("textures/Instructions.png");
		
    // Texture Cenario Stage 2
    Texture2D teste = LoadTexture("textures/test.png");

    // Loading Texture and PATRICK
    Texture2D room13bj = LoadTexture("textures/Cenario1.6.png");
    Vector2 pat_position = { (float)largura/2, (float)altura/2 };
    Texture2D patrickEtapa1 = LoadTexture("textures/PatrickPersonagem.png");
    Texture2D patrickEtapa2 = LoadTexture("textures/Patrick_etapa2.png");
    Rectangle pat1Rec = { 0.0f, 0.0f, (float)patrickEtapa1.width/6, (float)patrickEtapa1.height/2 }; 
    Rectangle frameRec = { 0.0f, 0.0f, (float)patrickEtapa2.width/3, (float)patrickEtapa2.height };
    Vector2 pericles_position = { (float)20, (float)altura };
    Texture2D periclesEtapa1 = LoadTexture("textures/PatrickPersonagem.png");
    Rectangle periclesRec1 = { 0.0f, 0.0f, (float)patrickEtapa1.width/6, (float)patrickEtapa1.height/2 };

    // initializing gabarito
    Texture2D gabarito = LoadTexture("textures/GABARITO.png");
    Vector2 gabarito_position = { (float)largura/4, (float)altura/4 };
    Rectangle gabarito_rec = { 0.0f, 0.0f, (float)gabarito.width, (float)gabarito.height };

    // Variables Booleanas
    bool inicia_imagem = true;
    bool inicia_texto = false;
    bool inicia_jogo = false;
    bool inicia_pericles = false;
    bool pegou_gabarito = false;

    // Counters for the Intro
    int count_imagem = 0;
    int count = 0;

    // counters for stage 1
    int count_l = 0, count_r = 3, count_u = 0, count_d = 3;

    // Counters for Stage 2
	int currentFrame = 0;
	int framesCounter = 0;
	int count_cenario = 0;

    //Flag Last Key
    char last_key;
    
	
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
                    inicia_texto = false;
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
        else if(inicia_texto == false && inicia_imagem == false && inicia_jogo == false)
        {
            if(inicia_jogo == false)
            {
                DrawTexture(logo_menu, 0, 50, WHITE);
                DrawTexture(botao_start, 0, 60, WHITE);
                DrawRectangle(largura/2-300, altura-300, largura/2-20, 40, (Color){0,0,0,150});
                DrawTexture(botao_instru, 0, 30, WHITE);
                DrawRectangle(largura/2-300, altura-200, largura/2-20, 40, (Color){0,0,0,150});
                if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
                {
                    inicia_jogo=true;
                    inicia_pericles = true;
                }
            }
        }
        else if(inicia_jogo == true)
		{
            framesCounter++;
            count_cenario = count_cenario + 2;

            if (IsKeyDown(KEY_D)) {
                pat1Rec.y = (float)patrickEtapa1.height/2;

                if (framesCounter >= (60/framesSpeed)) {
                    framesCounter = 0;
                    currentFrame++;

                    count_r++;
                    if (count_r > 5) count_r = 3;

                    pat1Rec.x = (float)count_r * (float)patrickEtapa1.width/6;
                }
                last_key = 'D';
                pat_position.x += 2.0f;
            }
            if (IsKeyDown(KEY_A)) {
                pat1Rec.y = (float)patrickEtapa1.height/2;

                if (framesCounter >= (60/framesSpeed)) {
                    framesCounter = 0;

                    count_l++;
                    if (count_l > 2) count_l = 0;

                    pat1Rec.x = (float)count_l * (float)patrickEtapa1.width/6;
                }
                last_key = 'A';
                pat_position.x -= 2.0f;
            }
            if (IsKeyDown(KEY_W)) {
                pat1Rec.y = (float)patrickEtapa1.height - (float)patrickEtapa1.height;

                if (framesCounter >= (60/framesSpeed)) {
                    framesCounter = 0;

                    count_u++;
                    if (count_u > 2) count_u = 0;

                    pat1Rec.x = (float)count_u * (float)patrickEtapa1.width/6;
                }

                pat_position.y -= 2.0f;
                last_key = 'W';
            }
            if (IsKeyDown(KEY_S)) {
                pat1Rec.y = (float)patrickEtapa1.height - (float)patrickEtapa1.height;

                if (framesCounter >= (60/framesSpeed)) {
                    framesCounter = 0;

                    count_d++;
                    if (count_d > 5) count_d = 3;

                    pat1Rec.x = (float)count_d * (float)patrickEtapa1.width/6;
                }

                    last_key = 'S';
                    pat_position.y += 2.0f;
            }

            if (((pat_position.x - gabarito_position.x <= 5) && (pat_position.y - gabarito_position.y <= 5) && ((pat_position.x - gabarito_position.x >= 0) && (pat_position.x - gabarito_position.x <= 5)))) {
                pegou_gabarito = true;
            }
            
            DrawTexture(room13bj, 0, 0, WHITE);
            DrawTextureRec(patrickEtapa1, pat1Rec, pat_position, WHITE);
            DrawTextureRec(periclesEtapa1, periclesRec1, pericles_position, WHITE);

            if (!pegou_gabarito) {
                DrawTextureRec(gabarito, gabarito_rec, gabarito_position, WHITE);
                DrawText("Maravilha! Péricles saiu e deixou cair o gabarito!", 100, 100, 25, BLACK);
            }
            else {
                DrawText("Corra antes que Péricles volte!", 100, 100, 25, BLACK);
            }
        }

        EndDrawing();
    }

    CloseWindow();        
    return 0;
}