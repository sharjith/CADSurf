// GLObject.cpp: implementation of the CGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CGLObject::glObjCount = 0;

CGLObject::CGLObject()
{
    displayMode = GLWIREFRAME;

    //matl
    itsMaterial = CGLMaterial(ALUMINIUM);
    ambiMat[0] = itsMaterial.GetAmbient().GetRed();
    ambiMat[1] = itsMaterial.GetAmbient().GetGreen();
    ambiMat[2] = itsMaterial.GetAmbient().GetBlue();
    ambiMat[3] = itsMaterial.GetAmbientCoeff();

    diffMat[0] = itsMaterial.GetDiffuse().GetRed();
    diffMat[1] = itsMaterial.GetDiffuse().GetGreen();
    diffMat[2] = itsMaterial.GetDiffuse().GetBlue();
    diffMat[3] = itsMaterial.GetDiffuseCoeff();

    specMat[0] = itsMaterial.GetSpecular().GetRed();
    specMat[1] = itsMaterial.GetSpecular().GetGreen();
    specMat[2] = itsMaterial.GetSpecular().GetBlue();
    specMat[3] = itsMaterial.GetSpecularCoeff();

    emisMat[0] = 0.0f;
    emisMat[1] = 0.0f;
    emisMat[2] = 0.0f;
    emisMat[3] = 0.0f;

    coloMat[0] = ambiMat[0]*1.2f;
    coloMat[1] = ambiMat[1]*1.2f;
    coloMat[2] = ambiMat[2]*1.2f;
    coloMat[3] = ambiMat[3]*1.2f;

    shine = itsMaterial.GetShininessCoeff();
    //end matl
    
    itsRed = 205;
    itsGreen = 205;
    itsBlue = 25;
    itsTransparency = 255;
    itsBox.SetLimits(-1, 1, -1, 1, -1, 1);
    glObjCount++;
    MakeHidden(false);
    firstTime = true;
}

CGLObject::~CGLObject()
{
    glObjCount--;
}

GLObjectType CGLObject::GLObjType() const
{
    return glObjType;
}

bool CGLObject::IsOfType(const GLObjectType& type) const
{
    GLObjectType typ = type;
    bool b = (glObjType == typ);
    return b;
}

void CGLObject::SetDisplayMode(const GLDisplayMode& mode)
{
    displayMode = mode;
    DefineDisplay();
}

GLDisplayMode CGLObject::GetDisplayMode() const
{
    return displayMode;
}

void CGLObject::SetMaterial(const GLMaterialName& matName)
{
    SetMaterial(CGLMaterial(matName));
}

void CGLObject::SetMaterial(const CGLMaterial& mat)
{
    itsMaterial = mat;
    ambiMat[0] = itsMaterial.GetAmbient().GetRed();
    ambiMat[1] = itsMaterial.GetAmbient().GetGreen();
    ambiMat[2] = itsMaterial.GetAmbient().GetBlue();
    ambiMat[3] = itsMaterial.GetAmbientCoeff();

    diffMat[0] = itsMaterial.GetDiffuse().GetRed();
    diffMat[1] = itsMaterial.GetDiffuse().GetGreen();
    diffMat[2] = itsMaterial.GetDiffuse().GetBlue();
    diffMat[3] = itsMaterial.GetDiffuseCoeff();

    specMat[0] = itsMaterial.GetSpecular().GetRed();
    specMat[1] = itsMaterial.GetSpecular().GetGreen();
    specMat[2] = itsMaterial.GetSpecular().GetBlue();
    specMat[3] = itsMaterial.GetSpecularCoeff();

    emisMat[0] = 0.0f;
    emisMat[1] = 0.0f;
    emisMat[2] = 0.0f;
    emisMat[3] = 0.0f;

    coloMat[0] = ambiMat[0]*1.2f;
    coloMat[1] = ambiMat[1]*1.2f;
    coloMat[2] = ambiMat[2]*1.2f;
    coloMat[3] = ambiMat[3]*1.2f;

    shine = itsMaterial.GetShininessCoeff();
    firstTime = true;
}

CGLMaterial CGLObject::GetMaterial() const
{
    return itsMaterial;
}

void CGLObject::ApplyMaterial()
{
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambiMat);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffMat);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffMat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);
    glEnable(GL_LIGHT0);
}

void CGLObject::SetColor(const GLubyte& red, const GLubyte& green, const GLubyte& blue)
{
    itsRed = red;
    itsGreen = green;
    itsBlue = blue;
    firstTime = true;
    DefineDisplay();
}

void CGLObject::GetColor(GLubyte* colVect) const
{
    colVect = new GLubyte[3];
    colVect[0] = itsRed;
    colVect[1] = itsGreen;
    colVect[2] = itsBlue;
}

void CGLObject::SetBoundingBox(const CBoundingBox& B)
{
    itsBox = B;
}

CBoundingBox CGLObject::GetBoundingBox() const
{
    return itsBox;
}


//helper function to draw torus removes 'glut' dependency
void drawTorus(int numMajor, int numMinor, float majorRadius, float minorRadius)
{
    double majorStep = 2.0 * PI / numMajor;
    double minorStep = 2.0 * PI / numMinor;
    int i, j;

    for (i = 0; i < numMajor; ++i) {
	double a0 = i * majorStep;
	double a1 = a0 + majorStep;
	GLdouble x0 = cos(a0);
	GLdouble y0 = sin(a0);
	GLdouble x1 = cos(a1);
	GLdouble y1 = sin(a1);

	glBegin(GL_TRIANGLE_STRIP);
	for (j = 0; j <= numMinor; ++j) {
	    double b = j * minorStep;
	    GLdouble c = cos(b);
	    GLdouble r = minorRadius * c + majorRadius;
	    GLdouble z = minorRadius * sin(b);

	    glNormal3d(x0 * c, y0 * c, z / minorRadius);
	    glTexCoord2d(i / (GLdouble) numMajor, j / (GLdouble) numMinor);
	    glVertex3d(x0 * r, y0 * r, z);

	    glNormal3d(x1 * c, y1 * c, z / minorRadius);
	    glTexCoord2d((i + 1) / (GLdouble) numMajor, j / (GLdouble) numMinor);
	    glVertex3d(x1 * r, y1 * r, z);
	}
	glEnd();
    }
}

