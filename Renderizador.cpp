interno void limpar_tela(u32 cor) {
	unsigned int* pixel = (u32*)renderiza��o.memoria;
	for (int y = 0; y < renderiza��o.altura; y++) {
		for (int x = 0; x < renderiza��o.largura; x++) {
			*pixel++ = cor;
		}
	}
}

interno void desenhar_retangulo_em_pixels(int x0, int y0, int x1, int y1, u32 cor) {
	x0 = clamp(0, x0, renderiza��o.largura);
	x1 = clamp(0, x1, renderiza��o.largura);
	y0 = clamp(0, y0, renderiza��o.altura);
	y1 = clamp(0, y1, renderiza��o.altura);


	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)renderiza��o.memoria + x0 + y * renderiza��o.largura; 
		for (int x = x0; x < x1; x++) {
			*pixel++ = cor;
		}
	}
}

variavel_global float renderizacao_escala = 0.01f;

interno void desenhar_bordas(float arena_x, float arena_y, u32 cor) {
	arena_x *= renderiza��o.altura * renderizacao_escala;
	arena_y *= renderiza��o.altura * renderizacao_escala;

	int x0 = (int)((float)renderiza��o.largura * .5f - arena_x);
	int x1 = (int)((float)renderiza��o.largura * .5f + arena_x);
	int y0 = (int)((float)renderiza��o.altura * .5f - arena_y);
	int y1 = (int)((float)renderiza��o.altura * .5f + arena_y);

	desenhar_retangulo_em_pixels(0, 0, renderiza��o.largura, y0, cor);
	desenhar_retangulo_em_pixels(0, y1, x1, renderiza��o.altura, cor);
	desenhar_retangulo_em_pixels(0, y0, x0, y1, cor);
	desenhar_retangulo_em_pixels(x1, y0, renderiza��o.largura, renderiza��o.altura, cor);
}

interno void desenhar_retangulo(float x, float y, float meio_x, float meio_y, u32 cor) {
	x *= renderiza��o.altura * renderizacao_escala;
	y *= renderiza��o.altura * renderizacao_escala;
	meio_x *= renderiza��o.altura * renderizacao_escala;
	meio_y *= renderiza��o.altura * renderizacao_escala;

	x += renderiza��o.largura / 2.f;
	y += renderiza��o.altura / 2.f;

	// Mudar pra pixels.
	int x0 = x - meio_x;// = 204.3
	int x1 = x + meio_x;// = 476.7
	int y0 = y - meio_y;// = 204.3
	int y1 = y + meio_y;// = 476.7

	desenhar_retangulo_em_pixels(x0, y0, x1, y1, cor);
}

const char* letras[][7]{
	" 000 ",
	"0   0",
	"0   0",
	"00000",
	"0   0",
	"0   0",
	"0   0",

	"0000 ",
	"0   0",
	"0   0",
	"0000",
	"0   0",
	"0   0",
	"0000 ",

	" 0000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 0000",

	"0000 ",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0000 ",

	"0000",
	"0",
	"0",
	"0000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"0000",
	"0",
	"0",
	"0",

	" 000",
	"0   ",
	"0   ",
	"0  00",
	"0   0",
	"0   0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0 ",
	" 0 ",
	" 0 ",
	" 0 ",
	" 0 ",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 00 ",

	"0  0",
	"0  0",
	"0 00",
	"000",
	"0 00",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"0     0",
	"00    0",
	"0 0   0",
	"0  0  0",
	"0   0 0",
	"0    00",
	"0     0",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	"00000",
	"0   0",
	"0   0",
	"00000",
	"0 ",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0  00",
	"0   0",
	" 000 0",

	"00000",
	"0   0",
	"0   0",
	"0000",
	"0   0",
	"0   0",
	"0   0",

	" 0000",
	"0",
	"0",
	" 000 ",
	"    0",
	"    0",
	"0000 ",

	"00000",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 000",
};

interno void desenhar_texto(const char *texto, float x, float y, float tamanho, u32 cor){
	float meio = tamanho * .5f;
	float y_original = y;

	while (*texto) {
		const char** letra_a = letras[*texto - 'A'];
		float x_original = x;

		for (int i = 0; i < 7; i++) {
			const char* linha = letra_a[i];
			while (*linha) {
				if (*linha == '0') {
					desenhar_retangulo(x, y, meio, meio, cor);
				}
				x += tamanho;
				linha++;
			}
			y -= tamanho;
			x = x_original;
			}
		texto++;
		x += tamanho * 6.f;
		y = y_original;
	}
}


interno void desenhar_numero(int numero, float x, float y, float tamanho, u32 cor) {
	float meio = tamanho * .5f;
	
	do{

		int digito = numero % 10;
		numero = numero / 10;

		switch (digito) {
		case 0: {
			desenhar_retangulo(x - tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x, y + tamanho * 2, meio, meio, cor);
			desenhar_retangulo(x, y - tamanho * 2, meio, meio, cor);
			x -= tamanho * 4.f;
		} break;

		case 1: {
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			x -= tamanho * 2.f;
		}break;

		case 2: {
			desenhar_retangulo(x, y + tamanho * 2.f, 1.5f * tamanho, meio, cor);
			desenhar_retangulo(x, y, 1.5f * tamanho, meio, cor);
			desenhar_retangulo(x, y - tamanho * 2.f, 1.5 * tamanho, meio, cor);
			desenhar_retangulo(x + tamanho, y + tamanho, meio, meio, cor);
			desenhar_retangulo(x - tamanho, y - tamanho, meio, meio, cor);
			x -= tamanho * 4.f;
		} break;

		case 3: {
			desenhar_retangulo(x - meio, y + tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x - meio, y, tamanho, meio, cor);
			desenhar_retangulo(x - meio, y - tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x + tamanho, y, meio, tamanho * 2.5f, cor);
			x -= tamanho * 4.f;
		}break;

		case 4: {
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x - tamanho, y + tamanho, meio, 1.5f * tamanho, cor);
			desenhar_retangulo(x, y, meio, meio, cor);
			x -= tamanho * 4.f;
		}break;

		case 5: {
			desenhar_retangulo(x, y + tamanho * 2.f, 1.5f * tamanho, meio, cor);
			desenhar_retangulo(x, y, 1.5f * tamanho, meio, cor);
			desenhar_retangulo(x, y - tamanho * 2.f, 1.5 * tamanho, meio, cor);
			desenhar_retangulo(x - tamanho, y + tamanho, meio, meio, cor);
			desenhar_retangulo(x + tamanho, y - tamanho, meio, meio, cor);
			x -= tamanho * 4.f;
		}break;

		case 6: {
			desenhar_retangulo(x + meio, y + tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x + meio, y, tamanho, meio, cor);
			desenhar_retangulo(x + meio, y - tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x - tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x + tamanho, y - tamanho, meio, meio, cor);
			x -= tamanho * 4.f;
		}break;

		case 7: {
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x - meio, y + tamanho * 2.f, tamanho, meio, cor);
			x -= tamanho * 4.f;
		}break;

		case 8: {
			desenhar_retangulo(x - tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x, y + tamanho * 2.f, meio, meio, cor);
			desenhar_retangulo(x, y - tamanho * 2.f, meio, meio, cor);
			desenhar_retangulo(x, y, meio, meio, cor);
			x -= tamanho * 4.f;
		}break;

		case 9: {
			desenhar_retangulo(x - meio, y + tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x - meio, y, tamanho, meio, cor);
			desenhar_retangulo(x - meio, y - tamanho * 2.f, tamanho, meio, cor);
			desenhar_retangulo(x + tamanho, y, meio, 2.5f * tamanho, cor);
			desenhar_retangulo(x - tamanho, y + tamanho, meio, meio, cor);
			x -= tamanho * 4.f;
		}break;
		}
	} while (numero);
}