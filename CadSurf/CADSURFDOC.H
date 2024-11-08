//############################################################################

//  Author:	Sharjith Nair
//  Email:	sharjith_ssn@hotmail.com

// Program:	OpenGL, C++ MFC Surface Modeler Demo Application

//							Important Notice:

//	This Idea and the Application is Copyright(c) Sharjith Nair 2002, 2003.
//	You can freely use it as long as you credit me for it.

//	No guarantee/warranty is given on this app and I will not be responsible 
//	for any damage to you, your property or any other person from using it.
//	USE IT ON YOUR OWN RISK.

//	Thankyou
//	Sharjith Nair.

//#############################################################################

// CadSurfDoc.h : interface of the CCadSurfDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADSURFDOC_H__738EE27E_383E_4C83_93DB_0C149EBE624B__INCLUDED_)
#define AFX_CADSURFDOC_H__738EE27E_383E_4C83_93DB_0C149EBE624B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLView.h"
#include "CadSurfView.h"

//Geometry 
#include <Geometry.h>
#include <GeomException.h>
#include <Point3D.h>
#include <Vector3D.h>
#include <OneAxis.h>
#include <AxisSystem.h>
#include <Line3D.h>
#include <Circle3D.h>
#include <Ellipse3D.h>
#include <Hyperbola3D.h>
#include <Parabola3D.h>
#include <BezierCurve.h>
#include <BSplineCurve.h>
#include <OffsetCurve.h>
#include <Plane.h>
#include <Cone.h>
#include <Cylinder.h>
#include <Sphere.h>
#include <Torus.h>
#include <ExtrudedSurface.h>
#include <RevolvedSurface.h>
#include <RuledSurface.h>
#include <PipeSurface.h>
#include <OffsetSurface.h>


//Graphics
#include <GLObject.h>
#include <GLOneAxis.h>
#include <GLAxisSystem.h>
#include <GLPoint.h>
#include <GLPlane.h>
#include <GLCurve.h>
#include <GLSurface.h>
#include <GLTrihedron.h>
#include <GLFont.h>
#include <BoundingBox.h>

#include "Helix.h"

//Display Context
#include "GLDisplayContext.h"

class CCadSurfDoc : public CDocument
{
protected: // create from serialization only
	CCadSurfDoc();
	DECLARE_DYNCREATE(CCadSurfDoc)

// Attributes
public:
	CGLDisplayContext* dContext;

// Operations
public:
	void UpdateViews();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCadSurfDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCadSurfDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCadSurfDoc)
	afx_msg void OnPoints();
	afx_msg void OnLines();
	afx_msg void OnCircles();
	afx_msg void OnEllipse();
	afx_msg void OnParabola();
	afx_msg void OnHyperbola();
	afx_msg void OnBezier();
	afx_msg void OnBspline();
	afx_msg void OnPlane();
	afx_msg void OnCylinder();
	afx_msg void OnCone();
	afx_msg void OnSphere();
	afx_msg void OnTorus();
	afx_msg void OnExtsurf();
	afx_msg void OnRevsurf();
	afx_msg void OnRuledsurf();
	afx_msg void OnPipe();
	afx_msg void OnTranslate();
	afx_msg void OnRotate();
	afx_msg void OnScale();
	afx_msg void OnReflect();
	afx_msg void OnEditDeleteall();
	afx_msg void OnUpdateEditDeleteall(CCmdUI* pCmdUI);
	afx_msg void OnEditDeleteselected();
	afx_msg void OnUpdateEditDeleteselected(CCmdUI* pCmdUI);
	afx_msg void OnHelix();
	afx_msg void OnOffsetsurface();
	afx_msg void OnOffsetcurve();
	afx_msg void OnUsersurface();
	afx_msg void OnUpdateUsersurface(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CADSURFDOC_H__738EE27E_383E_4C83_93DB_0C149EBE624B__INCLUDED_)
