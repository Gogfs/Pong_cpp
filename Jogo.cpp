#define pra_baixo(b) input->botoes[b].pra_baixo
#define pressionado(b) (input->botoes[b].pra_baixo && input->botoes[b].alterado)
#define solto(b) (!input->botoes[b].pra_baixo && input->botoes[b].alterado)

float player_1_p, player_1_dp; // player_p = posição do jogador, player_dp = velocidade do jogador.
float player_2_p, player_2_dp;
float metade_arena_x = 85; // Metade_arena_x = Bordas da arena na horizontal.
float metade_arena_y = 45; // Metade_arena_y = Bordas da arena na vertical.
float metade_tamanho_jogador_x = 2.5; // Metade_tamanho_jogador = metade do retângulo que o jogador controla.
float metade_tamanho_jogador_y = 12;
float bola_p_x, bola_p_y; // Posição x e y da bola, respectivamente.
float bola_dp_x = 130, bola_dp_y; // Velocidade horizontal e vertical da bola, respectivamente.
float metade_bola_tamanho = 1; // Metade_bola_tamanho = borda da bola. 

int player_1_pontuacao, player_2_pontuacao; // A pontuação dos dois jogadores.

interno void simular_movimentacao(float *p, float *dp, float *ddp, float *dt) {
	*ddp -= *dp * 10.f; // Esse cálculo cria o atrito.

	*p = *p + *dp * *dt + *ddp * *dt * *dt * .5f; // Esse cálculo muda a posição do jogador
	*dp = *dp + *ddp * *dt; // Esse cálculo muda a velocidade do jogador.

	// O if-else faz a mecânica de colisão.
	if (*p + metade_tamanho_jogador_y > metade_arena_y) {
		*p = metade_arena_y - metade_tamanho_jogador_y;
		*dp = 0;
	}
	else if (*p - metade_tamanho_jogador_y < -metade_arena_y) {
		*p = -metade_arena_y + metade_tamanho_jogador_y;
		*dp = 0;
	}
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode atual_gamemode;
int botao;
bool adversario_ia;

interno void simular_jogo(Input* input, float dt) {
	desenhar_retangulo(0, 0, metade_arena_x, metade_arena_y, 0xadd8e6);
	desenhar_bordas(metade_arena_x, metade_arena_y, 0x0d214f);

	if (atual_gamemode == GM_GAMEPLAY) {
		float player_1_ddp = 0.f; // player_ddp = Aceleração do jogador.
		if (!adversario_ia) {
			if (pra_baixo(BUTTON_UP)) player_1_ddp += 2000;

			if (pra_baixo(BUTTON_DOWN)) player_1_ddp -= 2000;

		}
		else {
			if (bola_p_y > player_1_p) player_1_ddp += 1500;
			if (bola_p_y < player_1_p) player_1_ddp -= 1500;

		}

		float player_2_ddp = 0.f;
		if (pra_baixo(BUTTON_W)) player_2_ddp += 2000;

		if (pra_baixo(BUTTON_S)) player_2_ddp -= 2000;

		// Física de movimentação dos jogadores.
		simular_movimentacao(&player_1_p, &player_1_dp, &player_1_ddp, &dt);
		simular_movimentacao(&player_2_p, &player_2_dp, &player_2_ddp, &dt);


		// Criando a física de movimentação da bola.
		{
			bola_p_x += bola_dp_x * dt;
			bola_p_y += bola_dp_y * dt;

			desenhar_retangulo(bola_p_x, bola_p_y, metade_bola_tamanho, metade_bola_tamanho, 0x000000); // Desenhando a bola na tela.

			if (bola_p_x + metade_bola_tamanho > 80 - metade_tamanho_jogador_x &&
				bola_p_x - metade_bola_tamanho < 80 + metade_tamanho_jogador_x &&
				bola_p_y + metade_bola_tamanho > player_1_p - metade_tamanho_jogador_y &&
				bola_p_y + metade_bola_tamanho < player_1_p + metade_tamanho_jogador_y) {

				bola_p_x = 80 - metade_tamanho_jogador_x - metade_bola_tamanho;
				bola_dp_x *= -1;
				bola_dp_y = (bola_p_y - player_1_p) * 2 + player_1_dp * .75f;
			}
			else if (bola_p_x + metade_bola_tamanho > -80 - metade_tamanho_jogador_x &&
				bola_p_x - metade_bola_tamanho < -80 + metade_tamanho_jogador_x &&
				bola_p_y + metade_bola_tamanho > player_2_p - metade_tamanho_jogador_y &&
				bola_p_y + metade_bola_tamanho < player_2_p + metade_tamanho_jogador_y) {

				bola_p_x = -80 + metade_tamanho_jogador_x + metade_bola_tamanho;
				bola_dp_x *= -1;
				bola_dp_y = (bola_p_y - player_2_p) * 2 + player_2_dp * .75f;
			}
			else if (bola_p_y + metade_bola_tamanho > metade_arena_y) {
				bola_p_y = metade_arena_y - metade_bola_tamanho;
				bola_dp_y *= -1;
			}
			else if (bola_p_y - metade_bola_tamanho < -metade_arena_y) {
				bola_p_y = -metade_arena_y + metade_bola_tamanho;
				bola_dp_y *= -1;
			}
			else if (bola_p_x + metade_bola_tamanho > metade_arena_x) {
				bola_dp_x *= -1;
				bola_dp_y = 0;
				bola_p_x = 0;
				bola_p_y = 0;
				player_1_pontuacao++;
			}
			else if (bola_p_x - metade_bola_tamanho < -metade_arena_x) {
				bola_dp_x *= -1;
				bola_dp_y = 0;
				bola_p_x = 0;
				bola_p_y = 0;
				player_2_pontuacao++;
			}
		}

		desenhar_numero(player_1_pontuacao, -10, 40, 1.f, 0xff0000);
		desenhar_numero(player_2_pontuacao, 10, 40, 1.f, 0xff0000);

		desenhar_retangulo(80, player_1_p, metade_tamanho_jogador_x, metade_tamanho_jogador_y, 0x577ae4);
		desenhar_retangulo(-80, player_2_p, metade_tamanho_jogador_x, metade_tamanho_jogador_y, 0x577ae4);

		if (pressionado(BUTTON_M)) {
			atual_gamemode = GM_MENU;
		}
	}
	else {
		if (pressionado(BUTTON_LEFT) || pressionado(BUTTON_RIGHT)) {
			botao = !botao;
		}

		if (pressionado(BUTTON_ENTER)) {
			atual_gamemode = GM_GAMEPLAY;
			adversario_ia = botao ? 0 : 1;
		}

		if (botao == 0) {
			desenhar_texto("JOGADOR", -70, 0, 1, 0x000000);
			desenhar_texto("COMPUTADOR", 20, 0, 1, 0xff0000);;
		}
		else {
			desenhar_texto("JOGADOR", -70, 0, 1, 0xff0000);
			desenhar_texto("COMPUTADOR", 20, 0, 1, 0x000000);
		};
	}
};