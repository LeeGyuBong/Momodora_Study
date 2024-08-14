
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// STL 헤더 파일
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

// DirectX 9
// d3d9: 장치(GPU)를 제어하기 위한 자료형이나 함수를 제공한다.
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
// d3dx9: 다이렉트에서 사용할 벡터, 행렬 자료형이나 연산함수들을 제공한다.
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// 콘솔창 띄우기
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

// VLD 메모리 누수 체크
#ifdef _DEBUG
#include "vld.h"
#endif

// FMOD
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

// Header 파일
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