struct Botao_estado {
	bool pra_baixo;
	bool alterado;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_M,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_COUNT, // Deve ser o último item.
};

struct Input {
	Botao_estado botoes[BUTTON_COUNT];
};