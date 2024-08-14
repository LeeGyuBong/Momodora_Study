
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <io.h>
using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// STL ��� ����
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

// DirectX 9
// d3d9: ��ġ(GPU)�� �����ϱ� ���� �ڷ����̳� �Լ��� �����Ѵ�.
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
// d3dx9: ���̷�Ʈ���� ����� ����, ��� �ڷ����̳� �����Լ����� �����Ѵ�.
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// �ܼ�â ����
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

// VLD �޸� ���� üũ
#ifdef _DEBUG
#include "vld.h"
#endif

// FMOD
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

// Header ����
#include "Item.h"
#include "Defined.h"
#include "Enum.h"
#include "Extern.h"
#include "Function.h"
#include "Struct.h"
#include "Typedef.h"
#include "Constant.h"

// -----------------------------

#include "ObjMgr.h"
#include "TerrainMgr.h"
#include "SceneMgr.h"
#include "UiMgr.h"

// -----------------------------

#include "AbstractFactory.h"
#include "Math.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "DataSubject.h"
#include "TextMgr.h"
#include "DataMgr.h"

// -----------------------------

#include "Device.h"