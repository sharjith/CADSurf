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

// GLDisplayContext.h: interface for the CGLDisplayContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLDISPLAYCONTEXT_H__D04FD53F_4852_4D6C_957A_44E8AE0C54DB__INCLUDED_)
#define AFX_GLDISPLAYCONTEXT_H__D04FD53F_4852_4D6C_957A_44E8AE0C54DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListOfCGLObject.h"
#include "ListIteratorOfListOfCGLObject.h"
#include "GLObject.h"
#include "BoundingBox.h"
#include "GLView.h"

class CGLView;

class CGLDisplayContext  
{
public:
	CGLDisplayContext();
	CGLDisplayContext(CDocument* pDoc);
	virtual ~CGLDisplayContext();
	void Display(const CGLObject*);
	void Erase(const CGLObject*);
	void DisplayAll();
	void DisplaySelected();
	void EraseAll();
	void EraseSelected();
	void DeleteAll();
	void DeleteSelected();
	void Render(const GLDisplayMode& = GLWIREFRAME);
	bool Select(CGLView* aView, const int& x, const int& y);
	bool MultiSelect(CGLView* aView, const int& x, const int& y);
	bool SweepSelect(CGLView* aView, const CRect& swRect);
	bool IsEmpty() const;
	bool IsDisplayed(const CGLObject*) const;
	bool IsErased(const CGLObject*) const;
	bool IsSelected(const CGLObject*) const;
	bool IsAllDisplayed() const;
	bool IsAllErased() const;
	bool HasSelected() const;
	void InitSelected() const;
	bool MoreSelected() const;
	void NextSelected() const;
	CGLObject* CurrentSelected() const;
	CBoundingBox GetBoundingBox() const { return viewBox; }
	void SetSensitivity(const int& s) { mouseSensitivity = s; }
private:
	void AddToSelection(const CGLObject* O);
	void RemoveSelection(const CGLObject* O);
	void EmptySelection();
private:
	CListOfCGLObject* display;
	CListOfCGLObject* erase;
	CListOfCGLObject* select;
	mutable CListIteratorOfListOfCGLObject selListIter;
	CDocument* ptrDoc;
	CBoundingBox viewBox;
	int mouseSensitivity; //for selection;
};

#endif // !defined(AFX_GLDISPLAYCONTEXT_H__D04FD53F_4852_4D6C_957A_44E8AE0C54DB__INCLUDED_)
