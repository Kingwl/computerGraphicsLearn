#include "d3dUtility.h"
#include "Vertex.h"
#include "vector"
const int Height = 600;
const int Width = 800;
int num = 10;
IDirect3DDevice9 *D3DDevice = nullptr;
bool Setup()
{
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	D3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}
void Cleanup()
{

}
void draw(std::vector<std::pair<int, int>> &pos)
{
	IDirect3DVertexBuffer9 *VertexBuffer;
	D3DDevice->CreateVertexBuffer(pos.size() * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &VertexBuffer, 0);
	Vertex *v = nullptr;
	VertexBuffer->Lock(0, 0, (void**)&v, 0);
	for (int i = 0; i < pos.size(); ++i)
	{
		v[i] = Vertex(pos[i].first, pos[i].second);
	}
	VertexBuffer->Unlock();
	D3DDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
	D3DDevice->SetFVF(Vertex::FVF);
	D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, pos.size());
	VertexBuffer->Release();
	VertexBuffer = nullptr;
}

void drawLineSimple(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float k = dy / dx;
	std::vector<std::pair<int, int>>pos;
	for (int x = x1; x <= x2; ++x)
	{
		float y = k * x;
		int py = (y + 0.5f);
		pos.push_back(std::make_pair(x, py));
	}
	draw(pos);
}
void drawLineDDA(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float k = dy / dx;
	std::vector<std::pair<int, int>>pos;
	for (int x = x1; x <= x2; ++x)
	{
		int py = (y1 + 0.5f);
		pos.push_back(std::make_pair(x, py));
		y1 += k;
	}
	draw(pos);
}
void drawLineMid(float x1, float y1, float x2, float y2)
{
	int a = y1 - y2;
	int b = x2 - x1;
	int x = x1;
	int y = y1;
	int d = a * 2 + b;
	int d1 = a * 2;
	int d2 = (a + b) * 2;
	std::vector<std::pair<int, int>>pos;
	while (x <= x2)
	{
		if (d < 0)
		{
			++x;
			++y;
			d += d2;
		}
		else
		{
			++x;
			d += d1;
		}
		pos.push_back(std::make_pair(x, y));
	}
	draw(pos);
}
void drawLineBrese(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float k = dy / dx;
	float e = -0.5f;
	int y = y1;
	int x = x1;
	std::vector<std::pair<int, int>>pos;
	for (int i = 0; i <= dx; ++i)
	{
		pos.push_back(std::make_pair(x, y));
		++x;
		e += k;
		if (e >= 0)
		{
			++y;
			--e;
		}
	}
	draw(pos);
}

void addCPoint(int x, int y, std::vector<std::pair<int, int>> &pos)
{
	pos.push_back(std::make_pair(x, y));
	pos.push_back(std::make_pair(y, x));

	pos.push_back(std::make_pair(-x, y));
	pos.push_back(std::make_pair(y, -x));

	pos.push_back(std::make_pair(x, -y));
	pos.push_back(std::make_pair(-y, x));

	pos.push_back(std::make_pair(-x, -y));
	pos.push_back(std::make_pair(-y, -x));
}
void PointCriclrMid(int r)
{
	int x, y;
	float d;
	x = 0;
	y = r;
	d = 1.25f - r;
	std::vector<std::pair<int, int>> pos;
	addCPoint(x, y, pos);
	while (x <= y)
	{
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			d += 2 * (x - y) + 5;
			--y;
		}
		++x;
		addCPoint(x, y, pos);
	}
	draw(pos);
}
bool Display(float timeDelta)
{
	if (D3DDevice)
	{

		D3DXVECTOR3 pos(0.0f, 0.0f, -num);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXVECTOR3 look(0.0f, 0.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &pos, &look, &up);
		D3DDevice->SetTransform(D3DTS_VIEW, &V);
		D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		D3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , 0xff000000, 1.0f, 0);
		D3DDevice->BeginScene();

		//drawLineBrese(1.0f, 1.0f, 5.0f, 5.0f);
		PointCriclrMid(num);
		D3DDevice->EndScene();
		D3DDevice->Present(0, 0, 0, 0);
	}
	return true;
}
LRESULT CALLBACK d3d::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			::DestroyWindow(hWnd);
		}
		if (wParam == 'W')
		{
			if (num <= 500)
			num *= 2;
		}
		if (wParam == 'S')
		{
			if (num >= 10)
			num /= 2;
		}
		if (wParam == VK_SPACE)
		{
			char c[100];
			sprintf_s(c, "%d", num);
			::MessageBox(0, c, 0, 0);
		}
		break;
	case WM_MOUSEMOVE:
		
		break;
	default:
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!d3d::initD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &D3DDevice))
	{
		::MessageBox(nullptr, "initD3d failed", "error", 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(nullptr, "Setup failed", "error", 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();
	D3DDevice->Release();
	return 0;
}