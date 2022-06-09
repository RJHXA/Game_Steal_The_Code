#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define altura 720
#define largura 1280
#define framesSpeed 8
#define MAX_ENEMIES 10

int main(void) {    

    InitWindow(largura, altura, "Steal the Code");

    SetTargetFPS(60);

    //************************************************ STAGE 1 **********************************************/

    //*********************** Loading Textures Stage 1 *********************/ 
    Texture2D logo_inicial = LoadTexture("textures/logoInicial.png");
    Texture2D logo_jogo = LoadTexture("textures/logoJogo.png");
    Texture2D logo_menu = LoadTexture("textures/logoMenu.png");
    Texture2D botao_start = LoadTexture("textures/Play.png");
    Texture2D botao_instru = LoadTexture("textures/Instructions.png");
    Texture2D room13bj = LoadTexture("textures/Cenario1.6.png");

    //*********************** Loading Texture and PATRICK *********************/
    Texture2D patrickEtapa1 = LoadTexture("textures/PatrickPersonagem.png");
    Vector2 pat_position = { (float)largura/2, (float)altura/2 };
    Rectangle pat1Rec = { 0.0f, 0.0f, (float)patrickEtapa1.width/6, (float)patrickEtapa1.height/2 }; 
    
    //*********************** Initializing gabarito *********************/
    Texture2D gabarito = LoadTexture("textures/GABARITO.png");
    Vector2 gabarito_position = { (float)largura/4, (float)altura/4 };
    Rectangle gabarito_rec = { 0.0f, 0.0f, (float)gabarito.width, (float)gabarito.height };

    //*********************** Variables Booleanas *********************/
    bool inicia_imagem = true;
    bool inicia_texto = false;
    bool inicia_jogo = false;
    bool inicia_instrucoes = false;
    bool inicia_pericles = false;
    bool inicia_Stage2 = false;
    bool pegou_gabarito = false;

    //*********************** Counters *********************/
    /* Counters for the Intro */
    int count_imagem = 0;
    int count = 0;

    /* Counters for Stage 1 */
    int count_l = 0, count_r = 3, count_u = 0, count_d = 3;
	int currentFrame_Stage1 = 0;
	int framesCounter_Stage1 = 0;
	int count_cenario = 0;

    //Flag Last Key
    char last_key;

    //************************************************ STAGE 2 **********************************************/

    //*********************** Fonte da letra **************************/
    Font fonte = LoadFont("textures/fonte_letra.png");

    //************************************* Cenario Stage 2 ***********************************/
    Texture2D cenario_predio = LoadTexture("textures/predios_fase2.png");
    int backScrolling = 0;
    Texture2D cenario_rua = LoadTexture("textures/rua_fase2.png");
    int ruaScrolling = 0;

    //******************************* Patrick ***********************************/
    Texture2D pat = LoadTexture("textures/Patrick_etapa2.png");
    Vector2 position_Pat_stage2 = { (largura/2) - 300, (altura/2) + 120 };
    Rectangle frameRec_Pat = { 0.0f, 0.0f, (float)pat.width/2, (float)pat.height };
    bool patrick_alive = true;
    int playerRail = 1;
    int currentFrame = 0;

    //************************************* Pericles *******************************************/
    Texture2D pericles = LoadTexture("textures/Pericles_etapa2.png");
    Vector2 position_Pericles = { (largura/2) - 600, (altura/2) + 120 };
    Rectangle frameRec_Pericles = { 0.0f, 0.0f, (float)pericles.width/2, (float)pericles.height };

    //*************************** Contuers Stage 2 **********************************/
    int framesCounter = 0;
    int framesSpeed_Stage2 = 4;
    
    //****************************** Enemy's ************************************/
    Texture2D cachorro = LoadTexture("textures/cachorro_obstaculo.png");
    Texture2D cone = LoadTexture("textures/cone_obstaculo.png");
    Texture2D policial = LoadTexture("textures/policial_obstaculo.png");

    Rectangle enemyBounds[MAX_ENEMIES];
    int enemyRail[MAX_ENEMIES];
    int enemyType[MAX_ENEMIES];
    bool enemyActive[MAX_ENEMIES];
    double enemySpeed = 10;
    int frames_enemy = 0;

    //****************************** Score and Distance ************************************/
    Texture2D coin = LoadTexture("textures/coin_score.png");
    float distance = 0.0f;
    int score = 0;
    int contador_batida = 0;

    //****************************** Audios ************************************/
    Music audio_jogo;
    Sound coin_audio;
    Sound obstaculo_audio;

    InitAudioDevice();
    audio_jogo = LoadMusicStream("audios/audio_jogo.mp3");
    PlayMusicStream(audio_jogo);

    coin_audio = LoadSound("audios/coin_audio.mp3");
    obstaculo_audio = LoadSound("audios/obstaculo_audio.mp3");

    SetSoundVolume(obstaculo_audio, 1.5);

    //****************************** Init ENEMY's ************************************/
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        // Probability system for enemies type
        int enemyProb = GetRandomValue(0, 100);

        if (enemyProb < 30) enemyType[i] = 0;
        else if (enemyProb < 60) enemyType[i] = 1;
        else if (enemyProb < 90) enemyType[i] = 2;
        else enemyType[i] = 3;

        // define enemy rail
        enemyRail[i] = GetRandomValue(0, 2);

        // Make sure not two consecutive enemies in the same row
        if (i > 0) while (enemyRail[i] == enemyRail[i - 1]) enemyRail[i] = GetRandomValue(0, 2);

        // Centralize Enemy's
        switch (enemyRail[i])
        {
        case 0:
            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i] * 85) + (altura/2) - 40, 70, 70 };
            break;
        case 1:
            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*85) + (altura/2), 70, 70 };
            break;
        case 2:
            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*100) + (altura/2), 70, 70 };
            break;
        }

        enemyActive[i] = false;
    }
	
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
                }
            }
            count_imagem++;
        }
        else if(inicia_texto == true)
        {
            if(count < 720)
            {
                DrawText("Patrick conseguiu roubar a prova", 260, 0+count, 40, RED);
                DrawText("mas não esperava por pericles voltando para a sala", 100, 60+count, 40, RED);
                DrawText("e então correu para a rua tentando escapar", 150, 120+count, 40, RED);
                DrawText("FUJA!", largura/2 - 120, 200+count, 80, WHITE);
                DrawText("Consiga 3000 pontos para salvar a turma!", largura/2 - 450, 350+count, 40, GRAY);
                if(IsKeyPressed(KEY_ENTER))
                {
                    inicia_texto = false;
                    inicia_Stage2 = true;
                }
            }
            else
            {
                inicia_texto = false;
                inicia_Stage2 = true;
            }
            count++;
        }
        else if(inicia_texto == false && inicia_imagem == false && inicia_jogo == false && inicia_Stage2 == false)
        {
            if(inicia_jogo == false)
            {
                DrawTexture(logo_menu, 0, 50, WHITE);
                DrawTexture(botao_start, 0, 60, WHITE);
                DrawRectangle(largura/2-300, altura-300, largura/2-20, 40, (Color){0,0,0,150});
                DrawText("ENTER -", largura/2-350, altura-300, 30, GRAY);
                DrawTexture(botao_instru, 0, 30, WHITE);
                DrawRectangle(largura/2-300, altura-230, largura/2-20, 40, (Color){0,0,0,150});
                DrawText("SPACE -", largura/2-350, altura-230, 30, GRAY);
                if(IsKeyPressed(KEY_ENTER))
                {
                    inicia_jogo=true;
                    inicia_pericles = true;
                }
                else if(IsKeyPressed(KEY_SPACE))
                {
                    inicia_instrucoes = true;
                    inicia_jogo = true;
                }
            }
        }
        else if(inicia_instrucoes == true && inicia_jogo == true)
        {
            DrawText("COMO JOGAR:", 150, 20, 30, MAROON);
            DrawText("FASE 1:", 150, 110, 20, ORANGE);
            DrawText("VOCÊ CONTROLARA PATRICK NA SALA DE AULA E DEVE PROCURAR O GABARITO DA PROVA DE PERICLES.", 150, 150, 20, ORANGE);
            DrawText("UTILIZE 'W', 'A', 'S', 'D' PARA MOVIMENTAR-LO.", 150, 200, 20, ORANGE);

            DrawText("FASE 2:", 150, 280, 20, ORANGE);
            DrawText("VOCÊ CONTROLARA PATRICK NA RUA BOM JESUS, VOCE DEVE FUGIR DE PERICLES \nE DESVIAR DOS OBSTACULOS PARA NÃO SER PEGO.", 150, 350, 20, ORANGE);
            DrawText("UTILIZE UP ou DOWN PARA MOVIMENTAR-LO.", 150, 420, 20, ORANGE);
            DrawText("PRECIONE ENTER PARA VOLTAR AO MENU.", 150, 550, 20, LIGHTGRAY);
            if(IsKeyPressed(KEY_ENTER))
            {
                inicia_jogo = false;
                inicia_instrucoes = false;
            }
        }
        else if(inicia_jogo == true && inicia_instrucoes == false)
		{
            framesCounter_Stage1++;
            count_cenario = count_cenario + 2;

            if (IsKeyDown(KEY_D)) {
                pat1Rec.y = (float)patrickEtapa1.height/2;

                if (framesCounter_Stage1 >= (60/framesSpeed)) {
                    framesCounter_Stage1 = 0;
                    currentFrame_Stage1++;

                    count_r++;
                    if (count_r > 5) count_r = 3;

                    pat1Rec.x = (float)count_r * (float)patrickEtapa1.width/6;
                }
                last_key = 'D';
                pat_position.x += 2.0f;
            }
            if (IsKeyDown(KEY_A)) {
                pat1Rec.y = (float)patrickEtapa1.height/2;

                if (framesCounter_Stage1 >= (60/framesSpeed)) {
                    framesCounter_Stage1 = 0;

                    count_l++;
                    if (count_l > 2) count_l = 0;

                    pat1Rec.x = (float)count_l * (float)patrickEtapa1.width/6;
                }
                last_key = 'A';
                pat_position.x -= 2.0f;
            }
            if (IsKeyDown(KEY_W)) {
                pat1Rec.y = (float)patrickEtapa1.height - (float)patrickEtapa1.height;

                if (framesCounter_Stage1 >= (60/framesSpeed)) {
                    framesCounter_Stage1 = 0;

                    count_u++;
                    if (count_u > 2) count_u = 0;

                    pat1Rec.x = (float)count_u * (float)patrickEtapa1.width/6;
                }

                pat_position.y -= 2.0f;
                last_key = 'W';
            }
            if (IsKeyDown(KEY_S)) {
                pat1Rec.y = (float)patrickEtapa1.height - (float)patrickEtapa1.height;

                if (framesCounter_Stage1 >= (60/framesSpeed)) {
                    framesCounter_Stage1 = 0;

                    count_d++;
                    if (count_d > 5) count_d = 3;

                    pat1Rec.x = (float)count_d * (float)patrickEtapa1.width/6;
                }

                    last_key = 'S';
                    pat_position.y += 2.0f;
            }

            if (((pat_position.x - gabarito_position.x <= 5) && (pat_position.y - gabarito_position.y <= 5) && ((pat_position.x - gabarito_position.x >= 0) && (pat_position.x - gabarito_position.x <= 5)))) {
                pegou_gabarito = true;
                inicia_texto = true;
                inicia_jogo = false;
            }
            
            DrawTexture(room13bj, 0, 0, WHITE);
            DrawTextureRec(patrickEtapa1, pat1Rec, pat_position, WHITE);

            if (!pegou_gabarito) {
                DrawTextureRec(gabarito, gabarito_rec, gabarito_position, WHITE);
                DrawRectangle(largura/2-260, altura/2+300, largura/2-20, 40, BLACK);
                DrawText("Maravilha! Péricles saiu e deixou cair o gabarito!", largura/2-250, altura/2+300, 25, ORANGE);
            }
            else {
                DrawRectangle(largura/2-240, altura/2+300, largura/2-100, 40, BLACK);
                DrawText("Corra antes que Péricles volte!", largura/2-200, altura/2+300, 25, ORANGE);
            }
        }
        else if(inicia_Stage2 == true && inicia_jogo == false)
        {
            if(patrick_alive == true && score < 500)
            {
                SetMusicVolume(audio_jogo, 1.5);
                UpdateMusicStream(audio_jogo);

                // Background and Street Movements
                backScrolling--;
                if (backScrolling <= -largura) backScrolling = 0;

                ruaScrolling -= (enemySpeed - 2);
                if (ruaScrolling <= -largura) ruaScrolling = 0;

                // Sprints Movements
                framesCounter++;
                if (framesCounter >= (60/framesSpeed_Stage2))
                {
                    framesCounter = 0;
                    currentFrame++;

                    if (currentFrame > 1) currentFrame = 0;

                    frameRec_Pat.x = (float)currentFrame*(float)pat.width/2;
                    frameRec_Pericles.x = (float)currentFrame*(float)pericles.width/2;
                }

                // Player movement logic and Check player not out of rails
                if (IsKeyPressed(KEY_DOWN)) playerRail++;
                else if (IsKeyPressed(KEY_UP)) playerRail--;

                if (playerRail > 2) playerRail = 2;
                else if (playerRail < 0) playerRail = 0;

                switch (playerRail)
                {
                case 0:
                    position_Pat_stage2.y = (playerRail*85) + (altura/2) - 40;
                    position_Pericles.y = (playerRail*85) + (altura/2) - 40;
                    break;
                case 1:
                    position_Pat_stage2.y = (playerRail*85) + (altura/2);
                    position_Pericles.y = (playerRail*85) + (altura/2);
                    break;
                case 2:
                    position_Pat_stage2.y = (playerRail*100) + (altura/2);
                    position_Pericles.y = (playerRail*100) + (altura/2);
                    break;
                }

                // Active the enemy's on the map
                frames_enemy++;
                if (frames_enemy > 40)
                {
                    for (int i = 0; i < MAX_ENEMIES; i++)
                    {
                        if (enemyActive[i] == false)
                        {
                            enemyActive[i] = true;
                            i = MAX_ENEMIES;
                        }
                    }

                    frames_enemy = 0;
                }

                // Enemies logic
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemyActive[i])
                    {
                        enemyBounds[i].x -= enemySpeed;
                    }

                    // Check enemies out of screen
                    if (enemyBounds[i].x <= 0 - 128)
                    {
                        enemyActive[i] = false;
                        enemyType[i] = GetRandomValue(0, 3);
                        enemyRail[i] = GetRandomValue(0, 2);

                        // Make sure not two consecutive enemies in the same row
                        if (i > 0) while (enemyRail[i] == enemyRail[i - 1]) enemyRail[i] = GetRandomValue(0, 2);

                        // Centralize Enemy's
                        switch (enemyRail[i])
                        {
                        case 0:
                            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i] * 85) + (altura/2) - 40, 70, 70 };
                            break;
                        case 1:
                            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*85) + (altura/2), 70, 70 };
                            break;
                        case 2:
                            enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*100) + (altura/2), 70, 70 };
                            break;
                        }
                    }
                }

                // Check collision player vs enemies
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemyActive[i])
                    {
                        if (CheckCollisionPointRec(position_Pat_stage2, enemyBounds[i]))
                        {
                            if (enemyType[i] < 3)   // Bad enemies
                            {
                                contador_batida++;
                                position_Pericles.x += 100;
                                printf("Contador: %i\n", contador_batida);

                                enemyActive[i] = false;
                                enemyType[i] = GetRandomValue(0, 3);
                                enemyRail[i] = GetRandomValue(0, 2);

                                // Make sure not two consecutive enemies in the same row
                                if (i > 0) while (enemyRail[i] == enemyRail[i - 1]) enemyRail[i] = GetRandomValue(0, 2);

                                // Centralize Enemy's
                                switch (enemyRail[i])
                                {
                                case 0:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i] * 85) + (altura/2) - 40, 70, 70 };;
                                    break;
                                case 1:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*85) + (altura/2), 70, 70 };
                                    break;
                                case 2:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*100) + (altura/2), 70, 70 };
                                    break;
                                }

                                if(contador_batida > 2)
                                {
                                    patrick_alive = false;
                                    frames_enemy = 0;
                                }

                                PlaySound(obstaculo_audio);
                                //PlaySound(obstaculo_audio);
                            }
                            else      // Coin
                            {
                                enemyActive[i] = false;
                                enemyType[i] = GetRandomValue(0, 3);
                                enemyRail[i] = GetRandomValue(0, 2);

                                // Make sure not two consecutive enemies in the same row
                                if (i > 0) while (enemyRail[i] == enemyRail[i - 1]) enemyRail[i] = GetRandomValue(0, 2);

                                // Centralize Enemy's
                                switch (enemyRail[i])
                                {
                                case 0:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i] * 85) + (altura/2) - 40, 70, 70 };;
                                    break;
                                case 1:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*85) + (altura/2), 70, 70 };
                                    break;
                                case 2:
                                    enemyBounds[i] = (Rectangle){ largura + 14, (enemyRail[i]*100) + (altura/2), 70, 70 };
                                    break;
                                }

                                PlaySound(coin_audio);
                                //PlaySoundMulti(coin_audio);
                                score += 50;
                            }
                        }
                    }
                }

                distance += 0.5;
                if(distance > 1000 && distance < 1001)
                {
                    enemySpeed += 1;  
                    frames_enemy += 5;
                }
                else if(distance > 2000 && distance < 2001)
                {
                    enemySpeed += 2;
                    frames_enemy += 5;
                }
                else if(distance > 3000 && distance < 3001)
                {
                    enemySpeed += 3;
                    frames_enemy += 5;
                }
                else if(distance > 4000 && distance < 4001)
                {
                    enemySpeed += 4;
                    frames_enemy += 5;
                }
                else if(distance > 5000)
                {
                    enemySpeed += 0.05;
                    frames_enemy++;
                }

                // Background and Street
                DrawTexture(cenario_predio, backScrolling, 0, WHITE);
                DrawTexture(cenario_predio, largura + backScrolling, 0, WHITE);
                DrawTexture(cenario_rua, ruaScrolling, 0, WHITE);
                DrawTexture(cenario_rua, largura + ruaScrolling, 0, WHITE);

                // Drawing Patrick, Pericles and Enemy's
                DrawTextureRec(pat, frameRec_Pat, position_Pat_stage2, WHITE);
                DrawTextureRec(pericles, frameRec_Pericles, position_Pericles, WHITE);

                // Draw enemies
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (enemyActive[i])
                    {
                        // Draw enemies
                        switch(enemyType[i])
                        {
                            case 0: DrawTexture(cachorro, enemyBounds[i].x, enemyBounds[i].y+10, WHITE); break;
                            case 1: DrawTexture(cone, enemyBounds[i].x, enemyBounds[i].y+10, WHITE); break;
                            case 2: DrawTexture(policial, enemyBounds[i].x, enemyBounds[i].y+10, WHITE); break;
                            case 3: DrawTexture(coin, enemyBounds[i].x, enemyBounds[i].y+10, WHITE); break;
                        }
                    }
                }        

                // Distance and Score
                DrawTextEx(fonte, TextFormat("DISTANCE: %04i", (int)distance), (Vector2){ 20, 20 }, fonte.baseSize, -2, ORANGE);
                DrawTextEx(fonte, TextFormat("SCORE: %04i", score), (Vector2){ 400, 20 }, fonte.baseSize, -2, ORANGE);
            }
            else if(patrick_alive == true && score >= 500)
            {
                DrawTextEx(fonte, TextFormat("VOCE ESCAPOU DE PERICLES E TODOS PASSARAM EM PIF!"), (Vector2){ 170, 250 }, fonte.baseSize, -2, GREEN);
                DrawTextEx(fonte, TextFormat("SCORE: %04i", score), (Vector2){ 680, 350 }, fonte.baseSize, -2, GOLD);
                DrawTextEx(fonte, TextFormat("DISTANCE: %04i", (int)distance), (Vector2){ 290, 350 }, fonte.baseSize, -2, GOLD);
            }
            else // Patrick morreu
            {
                DrawTextEx(fonte, TextFormat("VOCE FOI PEGO POR PERICLES E TEVE A PROVA ZERADA!"), (Vector2){ 170, 250 }, fonte.baseSize, -2, RED);
                DrawTextEx(fonte, TextFormat("SCORE: %04i", score), (Vector2){ 680, 350 }, fonte.baseSize, -2, GOLD);
                DrawTextEx(fonte, TextFormat("DISTANCE: %04i", (int)distance), (Vector2){ 290, 350 }, fonte.baseSize, -2, GOLD);
            }
        }

        EndDrawing();
    }

    UnloadTexture(logo_inicial);
    UnloadTexture(logo_jogo);
    UnloadTexture(logo_menu);
    UnloadTexture(botao_start);
    UnloadTexture(botao_instru);
    UnloadTexture(room13bj);
    UnloadTexture(patrickEtapa1);
    UnloadTexture(gabarito);
    UnloadTexture(cenario_predio);
    UnloadTexture(cenario_rua);
    UnloadTexture(pat);
    UnloadTexture(pericles);
    UnloadTexture(cachorro);
    UnloadTexture(cone);
    UnloadTexture(policial);
    UnloadTexture(coin);

    UnloadMusicStream(audio_jogo);

    UnloadSound(coin_audio);
    UnloadSound(obstaculo_audio);

    StopSoundMulti();
    CloseWindow();        
    CloseAudioDevice();
    return 0;
}