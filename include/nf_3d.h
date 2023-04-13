// SPDX-License-Identifier: MIT
//
// Copyright (c) 2009-2014 Cesar Rincon "NightFox"
//
// NightFox LIB - Include de funciones 3D
// http://www.nightfoxandco.com/
// Version 20140413

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NF_3D_H__
#define __NF_3D_H__

#include <nds.h>





// Funcion NF_Set3D();
void NF_Set3D(u8 screen, u8 mode);
// Inicia el modo 3D seleccionado en la pantalla deseada


// Funcion NF_InitOpenGL();
void NF_InitOpenGL(void);
// Inicializa el OpenGL para la libreria



// Funcion NF_GetTextureSize();
extern u16 NF_GetTextureSize(u16 textel);
// Devuelve el tamaño del textel





#endif

#ifdef __cplusplus
}
#endif
