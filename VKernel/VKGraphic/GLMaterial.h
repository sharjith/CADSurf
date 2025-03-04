/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
/***************************************************************************
 *            GLMaterial.h
 *
 *  Thu Jun 29 22:53:56 2006
 *  Copyright  2006  N. Sharjith
 *  sharjith@gmail.com
 ****************************************************************************/


#ifndef _GLMATERIAL_H
#define _GLMATERIAL_H

#include "VKGraphic.h"

#include "GLTypeEnums.h"
#include "GLColor.h"
#include "MMath.h"

class VKGRAPHIC_API CGLMaterial
{
    public:
	CGLMaterial();
	CGLMaterial(const GLMaterialName& mat);
	void SetName(const GLMaterialName& mat);
	void SetAmbient(const CGLColor&);
	void SetDiffuse(const CGLColor&);
	void SetSpecular(const CGLColor&);
	void SetAmbientCoeff(const float&);
	void SetDiffuseCoeff(const float&);
	void SetSpecularCoeff(const float&);
	void SetShininessCoeff(const float&);
	CGLColor GetAmbient()	const;
	CGLColor GetDiffuse()	const;
	CGLColor GetSpecular()	const;
	float	 GetAmbientCoeff()	const;
	float	 GetDiffuseCoeff()	const;
	float	 GetSpecularCoeff()	const;
	float	 GetShininessCoeff()	const;
    private:
	void Init(const GLMaterialName& mat);
	GLMaterialName myMaterialName;
	CGLColor myAmbient;
	CGLColor myDiffuse;
	CGLColor mySpecular;
	float myAmbientCoeff;
	float myDiffuseCoeff;
	float mySpecularCoeff;
	float myShininessCoeff;
};

#endif /* _GLMATERIAL_H */
