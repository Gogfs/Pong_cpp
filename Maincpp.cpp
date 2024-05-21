#include "Uteis.cpp"
#include <windows.h>

variavel_global boolean runtime = true;

struct Renderização {
	int altura, largura;
	void* memoria;

	BITMAPINFO bitmap_info;
};
variavel_global Renderização renderização;


#include "Mecanicas.cpp"
#include "Renderizador.cpp"
#include "Jogo.cpp"

LRESULT CALLBACK retornar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT resultado = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		runtime = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		renderização.largura = rect.right - rect.left;
		renderização.altura = rect.bottom - rect.top;

		int tamanho = renderização.largura * renderização.altura * sizeof(unsigned int);

		if (renderização.memoria) VirtualFree(renderização.memoria, 0, MEM_RELEASE);
		renderização.memoria = VirtualAlloc(0, tamanho, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderização.bitmap_info.bmiHeader.biSize = sizeof(renderização.bitmap_info.bmiHeader);
		renderização.bitmap_info.bmiHeader.biWidth = renderização.largura;
		renderização.bitmap_info.bmiHeader.biHeight = renderização.altura;
		renderização.bitmap_info.bmiHeader.biPlanes = 1;
		renderização.bitmap_info.bmiHeader.biBitCount = 32;
		renderização.bitmap_info.bmiHeader.biCompression = BI_RGB;
		
	}

	default:{
			resultado = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return resultado;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Criando a classe Window.
	WNDCLASS janela_class = {};
	janela_class.style = CS_HREDRAW | CS_VREDRAW;
	janela_class.lpszClassName = L"Janela do jogo";
	janela_class.lpfnWndProc = retornar;

	// Registrando a classe.
	RegisterClass(&janela_class);

	// Criando a janela em si.
	HWND janela = CreateWindow(janela_class.lpszClassName, L"PONG em C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(janela);

	Input input = {};

	float delta_tempo = 0.016666f;
	LARGE_INTEGER frame_inicio_tempo;
	QueryPerformanceCounter(&frame_inicio_tempo);

	float frequencia_performace;

	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		frequencia_performace = (float)perf.QuadPart;
	}

	while (runtime) {
		// Input
		MSG mensagem;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.botoes[i].alterado = false;
		}

		while (PeekMessage(&mensagem, janela, 0, 0, PM_REMOVE)) {
			switch (mensagem.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)mensagem.wParam;
					bool pra_baixo = ((mensagem.lParam & (1 << 31)) == 0);

				#define processar_botao(b, vk)\
				case vk:{\
					input.botoes[b].alterado = pra_baixo != input.botoes[b].pra_baixo;\
					input.botoes[b].pra_baixo = pra_baixo;\
					} break;

					switch (vk_code) {
						processar_botao(BUTTON_UP, VK_UP);
						processar_botao(BUTTON_DOWN, VK_DOWN);
						processar_botao(BUTTON_W, 'W');
						processar_botao(BUTTON_S, 'S');
						processar_botao(BUTTON_M, 'M');
						processar_botao(BUTTON_LEFT, VK_LEFT);
						processar_botao(BUTTON_RIGHT, VK_RIGHT);
						processar_botao(BUTTON_ENTER, VK_RETURN);
					}

				} break;

				default: {
					TranslateMessage(&mensagem);
					DispatchMessage(&mensagem);
				}
			}
			
		}

		// Simulação
		simular_jogo(&input, delta_tempo);


		// Renderização
		StretchDIBits(hdc, 0, 0, renderização.largura, renderização.altura, 0, 0, renderização.largura, renderização.altura, renderização.memoria, &renderização.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_fim_tempo;
		QueryPerformanceCounter(&frame_fim_tempo);
		delta_tempo = (float)(frame_fim_tempo.QuadPart - frame_inicio_tempo.QuadPart) / frequencia_performace;
		frame_inicio_tempo = frame_fim_tempo;
	}

}