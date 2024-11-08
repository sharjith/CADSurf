// GLView.cpp : implementation file
//

#include "stdafx.h"
#include "CadSurf.h"
#include "CadSurfDoc.h"
#include "GLView.h"
#include "GeomException.h"
#include "ListOfCPoint3D.h"
#include "ListIteratorOfListOfCPoint3D.h"
#include "ListException.h"

#include "ChildFrm.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLView

CGLView::CGLView(CWnd* pWnd, CGLDisplayContext* ctx) : ptrWnd(pWnd), theContext(ctx)
{
	xRot = -45.0f;
	zRot = -45.0f;
	yRot = 0;
	xTrans = yTrans = zTrans = 0.0f;

	myCamera = new CGLCamera();

	myViewType = GLAXONVIEW;
	myProjType = GLORTHOGRAPHIC;

	nRange = 500;

	m_bkRed = 0;
	m_bkGreen = 0.27450980392156862745098039215686;
	m_bkBlue = 0.27450980392156862745098039215686;

	displayMode = GLSHADED;

	m_antialias = false;

	//1
	colorv[0].amb[0] =  0.0F;	colorv[0].amb[1] =  0.0F;	colorv[0].amb[2] =  0.0F;
	colorv[0].diff[0] =  0.1F;	colorv[0].diff[1] =  0.6F;	colorv[0].diff[2] =  0.3F;
	colorv[0].spec[0] =  1.0F;	colorv[0].spec[1] =  1.0F;	colorv[0].spec[2] =  1.0F;
	colorv[0].ratio =  0.75F;
	colorv[0].indexes[0] =  0;	colorv[0].indexes[1] =  0;	colorv[0].indexes[2] =  0;

	//2
	colorv[1].amb[0] =  0.0F;	colorv[1].amb[1] =  0.0F;	colorv[1].amb[2] =  0.0F;
	colorv[1].diff[0] =  0.0F;	colorv[1].diff[1] =  0.2F;	colorv[1].diff[2] =  0.5F;
	colorv[1].spec[0] =  1.0F;	colorv[1].spec[1] =  1.0F;	colorv[1].spec[2] =  1.0F;
	colorv[1].ratio =  0.75F;
	colorv[1].indexes[0] =  0;	colorv[1].indexes[1] =  0;	colorv[1].indexes[2] =  0;

	//3
	colorv[2].amb[0] =  0.0F;	colorv[2].amb[1] =  0.05F;	colorv[2].amb[2] =  0.05F;
	colorv[2].diff[0] =  0.6F;	colorv[2].diff[1] =  0.0F;	colorv[2].diff[2] =  0.8F ;
	colorv[2].spec[0] =  1.0F;	colorv[2].spec[1] =  1.0F;	colorv[2].spec[2] =  1.0F ;
	colorv[2].ratio =  0.75F;
	colorv[2].indexes[0] =  0;	colorv[2].indexes[1] =  0;	colorv[2].indexes[2] =  0 ;

	ZoomON = false;
	PanON = false;
	RotON = false;

	gridOn = false;



	myTrihedron = new CGLTrihedron;
	myGrid = new CGLGrid;

	InitGL();
}

CGLView::~CGLView()
{
	delete myTrihedron;
	delete myGrid;
	delete myCamera;
	::wglDeleteContext(m_hRC);
	ptrWnd->ReleaseDC(ptrWnd->GetDC());
}

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::Draw()
{
	// Make the rendering context current
	::wglMakeCurrent(m_hDC,m_hRC);

	// Call to the rendering function
	RenderScene(GL_RENDER);
	
	// Swap our scene to the front
	::SwapBuffers(m_hDC);

	// Allow other rendering contexts to co-exist
	::wglMakeCurrent(m_hDC,NULL);
}

/////////////////////////////////////////////////////////////////////////////

void CGLView::ReSize(int cx, int cy) 
{
	
	// TODO: Add your message handler code here
	::wglMakeCurrent(m_hDC,m_hRC);
			
	GLsizei w, h;
	w = cx;
	h = cy;

	//prevent divide by 0
	if (h==0)
		h=1;
	if(w==0)
		w=1;
	
	::glViewport(0, 0, w, h);

	// Reset coordinate system
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	/*
	// Ortho Viewing
    if (w <= h) 
		::glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else 
		::glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);
	*/

	if(myProjType == GLORTHOGRAPHIC)//orthographic projection
    {
	if(w <= h)
	    glOrtho(-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		    (nRange*h/w)-yTrans, -(nRange*50000.0f)-zTrans, (nRange*50000.0f)-zTrans);
	else
	    glOrtho(-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		    nRange-yTrans, -nRange*50000.0f, nRange*50000.0f);

    }

    if(myProjType == GLPERSPECTIVE)//perspective projection
    {
	if(w <= h)
	    gluPerspective(30, (GLdouble)h/(GLdouble)w, 1, nRange*50000.0f);
	else
	    gluPerspective(30, (GLdouble)w/(GLdouble)h, 1, nRange*50000.0f);
	glTranslatef(0, 0, -nRange*4);
    }

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

	::wglMakeCurrent(NULL,NULL);
}

void CGLView::Refresh()
{
	ptrWnd->Invalidate(FALSE);
}

void CGLView::InitGL() 
{
	int nPixelFormat;					// Pixel format index
	m_hDC = ptrWnd->GetDC()->m_hDC;			// Get the Device context

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		24,								// Want 24bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	// Set the pixel format for the device context
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));

	// Create the rendering context
	m_hRC = wglCreateContext(m_hDC);

	// Make the rendering context current, perform initialization, then
	// deselect it
	VERIFY(wglMakeCurrent(m_hDC,m_hRC));

	glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0);
	
	wglMakeCurrent(m_hDC, NULL);

	// Create the palette if needed
	InitializePalette();
}


// Initializes the CPalette object
void CGLView::InitializePalette(void)
{

	PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int pixelFormat = GetPixelFormat(m_hDC);
    int paletteSize;

    DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    /*
    ** Determine if a palette is needed and if so what size.
    */
    if (pfd.dwFlags & PFD_NEED_PALETTE) 
	{
		paletteSize = 1 << pfd.cColorBits;
    }else 
	if (pfd.iPixelType == PFD_TYPE_COLORINDEX) 
	{
		paletteSize = 4096;
    }else 
	{
		return;
    }

    pPal = (LOGPALETTE*)
	malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    if (pfd.iPixelType == PFD_TYPE_RGBA) 
	{
		/*
		** Fill the logical paletee with RGB color ramps
		*/
		int redMask = (1 << pfd.cRedBits) - 1;
		int greenMask = (1 << pfd.cGreenBits) - 1;
		int blueMask = (1 << pfd.cBlueBits) - 1;
		int i;

		for (i=0; i<paletteSize; ++i) 
		{
			pPal->palPalEntry[i].peRed =
				(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
			pPal->palPalEntry[i].peGreen =
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			pPal->palPalEntry[i].peBlue =
				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
			pPal->palPalEntry[i].peFlags = 0;
		}
    }else 
	{
		int numRamps = NUM_COLORS;
		int rampSize = (paletteSize - 20) / numRamps;
		int extra = (paletteSize - 20) - (numRamps * rampSize);
		int i, r;

		GetSystemPaletteEntries(m_hDC, 0, paletteSize, &pPal->palPalEntry[0]);

		for (r=0; r<numRamps; ++r) 
		{
			int rampBase = r * rampSize + 10;
			PALETTEENTRY *pe = &pPal->palPalEntry[rampBase];
			int diffSize = (int) (rampSize * colorv[r].ratio);
			int specSize = rampSize - diffSize;

			for (i=0; i<rampSize; ++i) 
			{
				GLfloat *c0, *c1;
				GLint a;
	
				if (i < diffSize) 
				{
					c0 = colorv[r].amb;
					c1 = colorv[r].diff;
					a = (i * 255) / (diffSize - 1);
				}else 
				{
					c0 = colorv[r].diff;
					c1 = colorv[r].spec;
					a = ((i - diffSize) * 255) / (specSize - 1);
				}

				pe[i].peRed = (BYTE) (a * (c1[0] - c0[0]) + 255 * c0[0]);
				pe[i].peGreen = (BYTE) (a * (c1[1] - c0[1]) + 255 * c0[1]);
				pe[i].peBlue = (BYTE) (a * (c1[2] - c0[2]) + 255 * c0[2]);
				pe[i].peFlags = PC_NOCOLLAPSE;
			}

			colorv[r].indexes[0] = rampBase;
			colorv[r].indexes[1] = rampBase + (diffSize-1);
			colorv[r].indexes[2] = rampBase + (rampSize-1);
		}

		for (i=0; i<extra; ++i) 
		{
			int index = numRamps*rampSize+10+i;
			PALETTEENTRY *pe = &pPal->palPalEntry[index];

			pe->peRed = (BYTE) 0;
			pe->peGreen = (BYTE) 0;
			pe->peBlue = (BYTE) 0;
		    pe->peFlags = PC_NOCOLLAPSE;
		}
    }

   m_hPalette2 = CreatePalette(pPal);
   free(pPal);

    if(m_hPalette2) 
	{
		SelectPalette(m_hDC, m_hPalette2, FALSE);
		RealizePalette(m_hDC);
    }		
	
}

BOOL CGLView::EnableRC(HDC m_hDC, HGLRC m_hRC, BOOL bEnable)
{
	if(bEnable)
	{
		if(!wglMakeCurrent(m_hDC, m_hRC))
		{
			CString err("CGLView::EnableRC - wglMakeCurrent failed\n");
			MessageBox(0, err, "OpenGL ERROR", MB_OK);
			return FALSE;
		}
	}
	else
		wglMakeCurrent(m_hDC, NULL);

	return TRUE;
}



BOOL CGLView::QueryNewPalette() 
{
	int nRet;
	// If the palette was created.
	if((HPALETTE)m_GLPalette)
	{
		// Selects the palette into the current device context
		SelectPalette(m_hDC, (HPALETTE)m_GLPalette, FALSE);

		// Map entries from the currently selected palette to
		// the system palette.  The return value is the number 
		// of palette entries modified.
		nRet = RealizePalette(m_hDC);

		// Repaint, forces remap of palette in current window
		ptrWnd->InvalidateRect(NULL,FALSE);

	}
	return nRet;
}

void CGLView::PaletteChanged() 
{
	
		// Select the palette into the device context
		SelectPalette(m_hDC,(HPALETTE)m_GLPalette,FALSE);

		// Map entries to system palette
		RealizePalette(m_hDC);
				
		// Remap the current colors to the newly realized palette
		UpdateColors(m_hDC);
}

void CGLView::RenderScene(GLenum mode)
{
	// Clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(m_antialias)
	{
		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	    glLineWidth (1.125);
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable (GL_LINE_SMOOTH);
		glLineWidth (1);
		glDisable (GL_BLEND);
	}

	glPushMatrix();

	myCamera->Render();

	//Scene and View Rotations
	glRotatef(xRot,1,0,0);		//I am not happy with this b'cos the rotations are always
	glRotatef(yRot,0,1,0);		//about the origin and not about the screen center but
	glRotatef(zRot,0,0,1);		//haven,t yet found out any other method

	myTrihedron->SetSize(nRange/2);
	myTrihedron->Display();

	if(gridOn)
		myGrid->Display();

	//Get the Display Context;
	theContext->Render(displayMode);
	
	glPopMatrix();

	glFlush();
}

void CGLView::ZoomView(CPoint dPoint, CPoint mPoint)
{
	if(!ZoomON)
		DownPoint = dPoint;
	ZoomON = true;

	if(DownPoint.x > mPoint.x || DownPoint.y < mPoint.y)
		nRange *= 1.05;
	else
		nRange /= 1.05;

	DownPoint = mPoint;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

}

void CGLView::PanView(CPoint dPoint, CPoint mPoint)
{
	if(!PanON)
		DownPoint = dPoint;
	PanON = true;
	CSize pan = DownPoint - mPoint;
	DownPoint = mPoint;	

	myCamera->StrafeRight(0.01*nRange/2*pan.cx);
	myCamera->Move(F3dVector(0.0,-0.01*nRange/2*pan.cy,0.0));

	DownPoint = mPoint;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);
}

void CGLView::RotateView(CPoint dPoint, CPoint mPoint)
{
	if(!RotON)
		DownPoint = dPoint;
	RotON = true;
	CSize rotate = DownPoint - mPoint;
	myCamera->RotateX(rotate.cy/2);
	myCamera->RotateY(rotate.cx/2);
	myViewType = GLUNDEFINEDVIEW;
	DownPoint = mPoint;
}


void CGLView::TopView() 
{
	// TODO: Add your command handler code here
	xRot = 0;
	yRot = 0;
	zRot = 0;
	myViewType = GLTOPVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::BottomView() 
{
	// TODO: Add your command handler code here
	xRot = -180;
	yRot = 0;
	zRot = 0;
	myViewType = GLBOTTOMVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::FrontView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 0;
	myViewType = GLFRONTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::BackView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -180;
	myViewType = GLBACKVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::LeftView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = -90;
	myViewType = GLLEFTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::RightView() 
{
	// TODO: Add your command handler code here
	xRot = -90;
	yRot = 0;
	zRot = 90;
	myViewType = GLRIGHTVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::AxonView() 
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	myViewType = GLAXONVIEW;
	myCamera->ResetCamera();
	Refresh();
}

void CGLView::SetProjectionType(const GLProjectionType& aType)
{
	myProjType = aType;
	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);
	Refresh();
}

void CGLView::FitAll() 
{
	// TODO: Add your command handler code here
	if(theContext->IsEmpty())
		return;
	FitView();
	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

	Refresh();
}

void CGLView::ZoomWindow(CRect zcRect)
{		
	CRect rcRect;
	ptrWnd->GetClientRect(&rcRect);

	int rcWidth, rcHeight, zcWidth, zcHeight;
	CPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.Width();
	rcHeight = rcRect.Height();

	zcWidth = zcRect.Width();
	zcHeight = zcRect.Height();

	rcCentre = rcRect.CenterPoint();
	zcCentre = zcRect.CenterPoint();

	wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y-1;
	gluUnProject((GLdouble)zcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y-1;
	gluUnProject((GLdouble)rcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	dx = cx - mx;
	dy = cy - my;

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/0.8;

	//xTrans += dx;
	//yTrans += dy;
	myCamera->StrafeRight(-dx);
	myCamera->MoveForwards(-dy);

	wglMakeCurrent(NULL,NULL);

	ReSize(rcWidth, rcHeight);

	Refresh();
}

void CGLView::ResetView() 
{
	// TODO: Add your command handler code here
	xRot = -45;
	yRot = 0;
	zRot = -45;
	xTrans = 0;
	yTrans = 0;
	zTrans = 0;
	nRange = 500;
	myCamera->ResetCamera();
	myViewType = GLAXONVIEW;

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	ReSize(w, h);

	Refresh();
}

void CGLView::SetBackgroundColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_bkRed = r;
	m_bkGreen = g;
	m_bkBlue = b;
	wglMakeCurrent(m_hDC, m_hRC);
	glClearColor(m_bkRed, m_bkGreen, m_bkBlue, 1.0f);
	::wglMakeCurrent(m_hDC, NULL);
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::GetBackgroundColor(GLfloat& r, GLfloat& g, GLfloat& b)
{
	r = m_bkRed;
	g = m_bkGreen;
	b = m_bkBlue;
}

void CGLView::SetDisplayMode(const GLDisplayMode& aMode)
{
	displayMode = aMode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetAntiAliasing(const bool& mode)
{
	m_antialias = mode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetGridOn(const bool& mode)
{
	gridOn = mode;
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::SetGridPara(const CPlane& plane, const double& size, const double& step)
{
	myGrid->SetPlane(plane);
	myGrid->SetSize(size);
	myGrid->SetStep(step);
	ptrWnd->InvalidateRect(NULL, FALSE);
}

void CGLView::PointToScreen(const CPoint3D& P, int& x, int& y)
{
	CPoint3D p = P;
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLdouble wx, wy, wz;
	::wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	gluProject((GLdouble)p.GetX(), p.GetY(), p.GetZ(), mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	wx = sround(wx);
	wy = sround(wy);
	x = (int)wx; y = (int)wy;
	glPopMatrix();
	::wglMakeCurrent(NULL,NULL);
}

void CGLView::ScreenToPoint(const int& x, const int& y, CPoint3D&  P)
{
	CPoint point(x,y);
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;
	::wglMakeCurrent(m_hDC,m_hRC);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	realy=viewport[3]-(GLint)point.y-1;
	gluUnProject((GLdouble)point.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&wx, &wy, &wz);
	P.SetParam(wx, wy, wz);
	glPopMatrix();
	::wglMakeCurrent(NULL,NULL);
}

void CGLView::FitView()
{
	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	CRect volRect;
	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;
	GLdouble lx, ly, lz, sx, sy, sz;
	CBoundingBox B = theContext->GetBoundingBox();
	lx = B.XMax(); ly = B.YMax(); lz = B.ZMax();
	sx = B.XMin(); sy = B.YMin(); sz = B.ZMin();

	wglMakeCurrent(m_hDC, m_hRC);
	glPushMatrix();

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	gluProject(lx, ly, lz, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	gluProject(sx, sy, sz, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();
	::wglMakeCurrent(m_hDC, NULL);

	volRect.SetRect(cx,cy,mx,my);
	volRect.NormalizeRect();

	Fit3d(volRect);
}

void CGLView::Fit3d(CRect rect)
{	
	CRect zcRect = rect;
	CRect rcRect;
	ptrWnd->GetClientRect(&rcRect);

	int rcWidth, rcHeight, zcWidth, zcHeight;
	CPoint rcCentre, zcCentre;
	int dx, dy, dw, dh;
	GLdouble wRatio, hRatio;
	GLdouble dRange;

	GLint viewport[4];
	GLdouble mvmatrix[16],projmatrix[16];
	GLint realy;

	GLdouble cx, cy, cz;
	GLdouble mx, my, mz;

	rcWidth = rcRect.Width();
	rcHeight = rcRect.Height();

	zcWidth = zcRect.Width();
	zcHeight = zcRect.Height();

	rcCentre = rcRect.CenterPoint();
	zcCentre = zcRect.CenterPoint();

	::wglMakeCurrent(m_hDC, m_hRC);

	glPushMatrix();

	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	realy=viewport[3]-(GLint)zcCentre.y-1;
	gluUnProject((GLdouble)zcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&mx, &my, &mz);

	realy=viewport[3]-(GLint)rcCentre.y-1;
	gluUnProject((GLdouble)rcCentre.x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
				&cx, &cy, &cz);

	glPopMatrix();

	::wglMakeCurrent(m_hDC, NULL);

	dx = cx - mx;
	dy = cy - my;

	dw = rcWidth - zcWidth;
	dh = rcHeight - zcHeight;

	wRatio = (GLdouble) zcWidth/rcWidth;
	hRatio = (GLdouble) zcHeight/rcHeight;

	dRange = (wRatio < hRatio) ? hRatio : wRatio;

	nRange *= dRange/(double)0.8;

	myCamera->SetPosition(F3dVector(0,0,0));
	xTrans += dx;
	yTrans -= dy;
}

void CGLView::Select(const int &x, const int &y)
{
	theContext->Select(this, x, y);
}

void CGLView::MultiSelect(const int &x, const int &y)
{
	theContext->MultiSelect(this, x, y);
}

void CGLView::SweepSelect(const CRect& swRect)
{
	theContext->SweepSelect(this, swRect);
}

int CGLView::ProcessSelection(const int& xPos, const int& yPos , const int& sensitivity)
{
	int id = 0;

	::wglMakeCurrent(m_hDC,m_hRC);

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending n pixels
	// in the vertical and horzontal direction
	int n = sensitivity;
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3] - yPos, n, n, viewport);

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	// Ortho Viewing
    if (w <= h) 
		glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else 
		glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If a single hit occured, display the info.
	if(hits)
	{
		id =  selectBuff[3];		
	}

	::wglMakeCurrent(m_hDC, NULL);

	return id;

}

int CGLView::ProcessSweepSelection(const CRect& swRect, unsigned long int* id)
{
	GLint i;
	GLuint j;
	::wglMakeCurrent(m_hDC,m_hRC);

	// Space for selection buffer
	GLuint selectBuff[512];

	// Hit counter and viewport storeage
	GLint hits = 0, viewport[4];

	// Setup selection buffer
	glSelectBuffer(512, selectBuff);
	
	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be cube around
	// center point of sweeping rectangle and extending pixels
	// falling within the height and width of the sweep rectangle
	// in the vertical and horzontal direction
	glLoadIdentity();
	gluPickMatrix(swRect.CenterPoint().x, viewport[3] - swRect.CenterPoint().y, swRect.Width(), swRect.Height(), viewport);

	CRect rect;
	ptrWnd->GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	// Ortho Viewing
    if (w <= h) 
		glOrtho (-nRange-xTrans, nRange-xTrans, -(nRange*h/w)-yTrans,
		(nRange*h/w)-yTrans, -(nRange*5000.0f)-zTrans, (nRange*5000.0f)-zTrans);
	else 
		glOrtho (-(nRange*w/h)-xTrans, (nRange*w/h)-xTrans, -nRange-yTrans,
		nRange-yTrans, -nRange*5000.0f, nRange*5000.0f);

	// Draw the scene
	RenderScene(GL_SELECT);

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

	// If hits occured, display the info.
	if(hits)
	{
		GLuint *ptr, names;
		ptr = (GLuint*) selectBuff;
		for(i=0; i < hits; i++)
		{
			names = *ptr;
			ptr++; ptr++; ptr++;
			for(j = 0; j < names; j++)
			{
				id[i] = *ptr;
				ptr++;
			}
		}
	}

	::wglMakeCurrent(m_hDC, NULL);

	return hits;
}

