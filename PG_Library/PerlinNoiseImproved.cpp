#include "pch.h"
#include "PerlinNoiseImproved.h"

#include "Math.h"
#include "Constants.h"
#include "Allocation.h"
#include "PerlinUtils.h"

float MutationValue(float x)
{
	//return (x + 1) / 2;					     	// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
	return (x + 0.7f) / 1.5f;                         // Magic value to obtain [-0.2,1.13] 
}


float ZgomotulPerlin_Îmbunătățit3D(float x, float y, float z)
{
	int xi = ParteaÎntreagăd(x),
		yi = ParteaÎntreagăd(y),
		zi = ParteaÎntreagăd(z);

	x -= xi;
	y -= yi;
	z -= zi;

	xi = xi & 255;
	yi = yi & 255;
	zi = zi & 255;

	int a  = g_TABELA_HASH_KEN_PERLIN[xi];
	int b  = g_TABELA_HASH_KEN_PERLIN[xi + 1];
	int	aa = g_TABELA_HASH_KEN_PERLIN[a + yi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		]
	int	ab = g_TABELA_HASH_KEN_PERLIN[a + yi + 1];	// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1 ]
	int	ba = g_TABELA_HASH_KEN_PERLIN[b + yi];		// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		]
	int	bb = g_TABELA_HASH_KEN_PERLIN[b + yi + 1];	// g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1 ]

	int aaa = g_TABELA_HASH_KEN_PERLIN[aa + zi];	 // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		] + zi	]
	int baa = g_TABELA_HASH_KEN_PERLIN[ba + zi];	 // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		] + zi	]
	int aba = g_TABELA_HASH_KEN_PERLIN[ab + zi];	 // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1 ] + zi	]
	int bba = g_TABELA_HASH_KEN_PERLIN[bb + zi];	 // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1 ] + zi	]
	int aab = g_TABELA_HASH_KEN_PERLIN[aa + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi		] + zi+1]
	int abb = g_TABELA_HASH_KEN_PERLIN[ab + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi		] + zi+1]
	int bab = g_TABELA_HASH_KEN_PERLIN[ba + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi	 ] + yi + 1	] + zi+1]
	int bbb = g_TABELA_HASH_KEN_PERLIN[bb + zi + 1]; // g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[g_HASH_TABLE_KEN_PERLIN[xi + 1] + yi + 1	] + zi+1]

	float g000 = Gradient3D(aaa, x, y, z);
	float g001 = Gradient3D(baa, x - 1, y, z);
	float g002 = Gradient3D(aba, x, y - 1, z);
	float g003 = Gradient3D(bba, x - 1, y - 1, z);
	float g004 = Gradient3D(aab, x, y, z - 1);
	float g005 = Gradient3D(bab, x - 1, y, z - 1);
	float g006 = Gradient3D(abb, x, y - 1, z - 1);
	float g007 = Gradient3D(bbb, x - 1, y - 1, z - 1);

	float u = CurbaEstomparef(x),
		  v = CurbaEstomparef(y),
		  w = CurbaEstomparef(z);

	float l01 = InterpolareLiniarăf(g000, g001, u);
	float l02 = InterpolareLiniarăf(g002, g003, u);
	float l03 = InterpolareLiniarăf(g004, g005, u);
	float l04 = InterpolareLiniarăf(g006, g007, u);

	float l11 = InterpolareLiniarăf(l01, l02, v);
	float l12 = InterpolareLiniarăf(l03, l04, v);

	float l20 = InterpolareLiniarăf(l11, l12, w);

	return l20;
}

float ZgomotulPerlin_Îmbunătățit2D(float x, float y)
{
	/// I. Definirea unei rețele de vectori de gradient aleatori
	// Se rotunjesc parametrii de intrare la partea întreagă pentru a indexa în tabelă
	int xÎntreg = ParteaÎntreagăd(x),
		yÎntreg = ParteaÎntreagăd(y);

	// Se ia partea fracționară pentru calcularea produsului scalar
	x -= xÎntreg;
	y -= yÎntreg;

	// Se restrâng valorile la 0 ... 255 deoarece tabela de hash are 256 de elemente
	xÎntreg = xÎntreg & 255;
	yÎntreg = yÎntreg & 255;

	/// II. Calcularea punctelor de gradient
	// Coordonatele pentru colțurile pătratului (cubului in 3D)
	int colțStânga		= g_TABELA_HASH_KEN_PERLIN[xÎntreg		  ] + yÎntreg,
	    colțDreapta		= g_TABELA_HASH_KEN_PERLIN[xÎntreg	   + 1] + yÎntreg,
	    colțJosStânga	= g_TABELA_HASH_KEN_PERLIN[colțStânga     ],
	    colțSusStânga	= g_TABELA_HASH_KEN_PERLIN[colțStânga  + 1],
	    colțJosDreapta	= g_TABELA_HASH_KEN_PERLIN[colțDreapta    ],
	    colstSusDreapta = g_TABELA_HASH_KEN_PERLIN[colțDreapta + 1];

	// Se calculează punctele de gradient, sunt 4 puncte (8 în 3D)
	float gradient1 = Gradient2D(g_TABELA_HASH_KEN_PERLIN[colțJosStânga]	, x	   , y	  ),
		  gradient2 = Gradient2D(g_TABELA_HASH_KEN_PERLIN[colțJosDreapta]	, x - 1, y	  ),
		  gradient3 = Gradient2D(g_TABELA_HASH_KEN_PERLIN[colțSusStânga]	, x	   , y - 1),
		  gradient4 = Gradient2D(g_TABELA_HASH_KEN_PERLIN[colstSusDreapta]	, x - 1, y - 1);

	// III. Interpolare între valori
	// Se evaluează valoarea curbei de estompare pentru fiecare parametru de intrare
	float estompare1 = CurbaEstomparef(x),
		  estompare2 = CurbaEstomparef(y);

	// Interpolarea pe contribuțiile de zgomot
	// Prima serie de interpolări
	float zgomot1 = InterpolareLiniarăf(gradient1, gradient2, estompare1);
	float zgomot2 = InterpolareLiniarăf(gradient3, gradient4, estompare1);
	// A două și ultima interpolarea
	float zgomotFinal = InterpolareLiniarăf(zgomot1, zgomot2, estompare2);

	return zgomotFinal;
}

float ZgomotulPerlin_Îmbunătățit1D(float x)
{
	int xi = ParteaÎntreagăd(x);
	
	x -= xi;

	xi = xi & 255;

	int	caa = g_TABELA_HASH_KEN_PERLIN[g_TABELA_HASH_KEN_PERLIN[xi]];
	int	cba = g_TABELA_HASH_KEN_PERLIN[g_TABELA_HASH_KEN_PERLIN[xi + 1]];

	float g000 = Gradient1D(g_TABELA_HASH_KEN_PERLIN[caa], x);
	float g001 = Gradient1D(g_TABELA_HASH_KEN_PERLIN[cba], x - 1);

	float fcx = CurbaEstomparef(x);

	float i01 = InterpolareLiniarăf(g000, g001, fcx);

	return i01;
}