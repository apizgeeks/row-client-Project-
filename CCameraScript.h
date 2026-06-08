#ifndef __CCAMERASCRIPT_H__
#define __CCAMERASCRIPT_H__

#include <d3d8.h>
#include <d3dx8.h>

#include <vector>
#include <windows.h>
#include <string.h>
#include "matrix.h"


using namespace std;

enum C_INTERPOLATION {
	C_LINE,						// Linear 
	C_BEZIER1,					// Bezier (제어점 3)
	C_BEZIER2,					// Bezier (제어점 4)
	C_NOTINTERPOLATION,			// 보간 없음
};

#define CAMERAEVENTEMPTY -1
#define CAMERAZEROFRAME 0.0f
#define CAMERASCRIPTVERSION 1.0f
#define CAMERAFRAMESTEP	30.0f
#define CAMERASPLINEUNIT	100
class CCameraScript {
private:
	// 시퀀스 unit
	class CCameraEvent {
	public:
			int				m_iIndex;				// 시퀀스 인덱스
			int				m_iInterpolation;		// 전 Event와의 보간 방법 선택
			float			m_fFrame;				// 시작으로 부터의 프레임
			D3DXVECTOR3		m_vecControlPoint[2];	// 베지어 곡선 Control Point
			D3DXVECTOR3		m_vecCameraPos;
			D3DXVECTOR3		m_vecCameraLook;
			D3DXVECTOR3		m_vecCameraUp;
			
			int				m_iPosStart;			// Pos Spline 에서의 시작 index
			int				m_iPosEnd;				// Pos Spline 에서의 종료 index
			
			int				m_iLookStart;			// Look Spline 에서의 시작 index
			int				m_iLookEnd;				// Look Spline 에서의 종료 index

		CCameraEvent() {
			m_iIndex = CAMERAEVENTEMPTY;
			m_iInterpolation = C_LINE;	
			m_fFrame = CAMERAZEROFRAME;
			m_vecCameraPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			m_vecCameraUp = D3DXVECTOR3(0.0f,0.0f,0.0f);
			m_vecCameraLook = D3DXVECTOR3(0.0f,0.0f,0.0f);
			m_iPosStart = CAMERAEVENTEMPTY;
			m_iPosEnd = CAMERAEVENTEMPTY;
			m_iLookStart = CAMERAEVENTEMPTY;
			m_iLookEnd = CAMERAEVENTEMPTY;

		}
		~CCameraEvent() {}
	};
	vector<CCameraEvent>	m_lstEvent;


	vector<D3DXVECTOR3>		m_lstPosSpline;
	bool					m_bCreateSpline;

	vector<D3DXVECTOR3>		m_lstLookSpline;
	bool					m_bCreateLookSpline;
	int						m_iInsertLook;

	int						m_iEventNum;
	int						m_iBeforeEvent;
	float					m_fRunFrame;		// 시작한 다음의 지나간 시간 frame			
	D3DXMATRIX				m_matBeforeView;
	

	CCameraEvent			m_CurrentEvent;		// 현재 프레임에 맞는, Event Unit
	D3DXMATRIX				m_matCurrentFrame;	// 현재 Frame View Matrix
	LPDIRECT3DDEVICE8			m_pDevice;			// Direct3D Device

	DWORD					m_dwBeforeTick;
	DWORD					m_dwCurrentTick;
	bool					m_bStart;

	
	matrix m_matBeforePos;
	matrix m_matBeforeView2;
	
	int						m_iFixCount;

	bool					m_bBackWord;

	D3DXVECTOR3				m_vecCameraInitPos;
	D3DXVECTOR3				m_vecCameraInitLook;
	bool					m_bInitSet;

public:
	CCameraScript();
	~CCameraScript();

	void			InitCameraScript(bool bInitPos = false,D3DXVECTOR3 vecInitPos = D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3 vecInitLook = D3DXVECTOR3(0.0f,0.0f,0.0f));
	void			InitFrame();

	int				InsertEvent(D3DXVECTOR3 vecPos,D3DXVECTOR3 vecLook,D3DXVECTOR3 vecUp,float fFrame,int iInterpol);
	void			InsertLookPos(D3DXVECTOR3 vecLook);
	
	void			FixEvent(int iEvent,D3DXVECTOR3 vecPos,D3DXVECTOR3 vecLook,D3DXVECTOR3 vecUp);
	void			FixEvent(int iEvent,D3DXVECTOR3 vecPos,D3DXVECTOR3 vecLook,D3DXVECTOR3 vecUp,float fFrame,int iInterpol);
	void			FixLook(int iEvent,D3DXVECTOR3 vecLook);

	void			DeleteEvent(float fFrame,int iInterpol);
	void			DeleteEvent(int iIndex);
	
	bool			SaveScript(char *strFileName);
	bool			LoadScript(char *strFileName);

	void			CreateSpline();

	void			PlayScript(int iRoll,int iLoop,bool bFix);
	void			Rewind();
	void			Fowind();
	void			Jump(int iEvent);	//iEvent로 jump 
//	void			SetPlay(bool bFlag);
	void			SetPlay(bool bFlag,bool bBack = false,bool bStartPos = false);

	void            SetVecTakePos();   //좌표 리셋 함수

	D3DXVECTOR3		GetVecPos() { return m_CurrentEvent.m_vecCameraPos; }
	D3DXVECTOR3		GetVecLook() { return m_CurrentEvent.m_vecCameraLook; }
	D3DXVECTOR3		GetVecUp() { return m_CurrentEvent.m_vecCameraUp; }
	D3DXMATRIX		GetCurrentViewMat() { return m_matCurrentFrame; }
	
	void			RenderPos_Up();
	void			RenderLook();
	void			RenderLine();
	void			RenderPosSpline();
	void			RenderLookSpline();
	bool			ISPlay() { return m_bStart;}
	int				GetExistEventNum() { return m_iEventNum;}
	int				GetFixCount(int &iIndex,float &fFrame);
	void			ReWindFix();
	void			FoWindFix();

	void			BackWordEvent();

	//
	void HALT()
	{
		m_bStart = false;
	}

};

#endif
