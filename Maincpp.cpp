#include "Uteis.cpp"
#include <windows.h>

variavel_global boolean runtime = true;

struct Renderiza��o {
	int altura, largura;
	void* memoria;

	BITMAPINFO bitmap_info;
};
variavel_global Renderiza��o renderiza��o;


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
		renderiza��o.largura = rect.right - rect.left;
		renderiza��o.altura = rect.bottom - rect.top;

		int tamanho = renderiza��o.largura * renderiza��o.altura * sizeof(unsigned int);

		if (renderiza��o.memoria) VirtualFree(renderiza��o.memoria, 0, MEM_RELEASE);
		renderiza��o.memoria = VirtualAlloc(0, tamanho, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderiza��o.bitmap_info.bmiHeader.biSize = sizeof(renderiza��o.bitmap_info.bmiHeader);
		renderiza��o.bitmap_info.bmiHeader.biWidth = renderiza��o.largura;
		renderiza��o.bitmap_info.bmiHeader.biHeight = renderiza��o.altura;
		renderiza��o.bitmap_info.bmiHeader.biPlanes = 1;
		renderiza��o.bitmap_info.bmiHeader.biBitCount = 32;
		renderiza��o.bitmap_info.bmiHeader.biCompression = BI_RGB;
		
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

		// Simula��o
		simular_jogo(&input, delta_tempo);


		// Renderiza��o
		StretchDIBits(hdc, 0, 0, renderiza��o.largura, renderiza��o.altura, 0, 0, renderiza��o.largura, renderiza��o.altura, renderiza��o.memoria, &renderiza��o.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_fim_tempo;
		QueryPerformanceCounter(&frame_fim_tempo);
		delta_tempo = (float)(frame_fim_tempo.QuadPart - frame_inicio_tempo.QuadPart) / frequencia_performace;
		frame_inicio_tempo = frame_fim_tempo;
	}

}