/*****************************************************************************
 * vlc_module.c: vlc python binding module
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: $
 *
 * Authors: Olivier Aubert <oaubert at bat710.univ-lyon1.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/* We need to access some internal features of VLC (for vlc_object) */
#define __VLC__


#include "vlcglue.h"

/**************************************************************************
 * VLC Module
 **************************************************************************/

#ifndef vlcMODINIT_FUNC /* declarations for DLL import/export */
#define vlcMODINIT_FUNC void
#endif

static PyMethodDef vlc_methods[] = {
    { NULL }  /* Sentinel */
};

/* Module globals */
PyObject* MediaControl_InternalException          = NULL;
PyObject* MediaControl_PositionKeyNotSupported    = NULL;
PyObject *MediaControl_PositionOriginNotSupported = NULL;
PyObject* MediaControl_InvalidPosition            = NULL;
PyObject *MediaControl_PlaylistException          = NULL;

vlcMODINIT_FUNC
initvlc( void )
{
    PyObject* p_module;

    PyPosition_Type.tp_new = PyType_GenericNew;
    PyPosition_Type.tp_alloc = PyType_GenericAlloc;

    vlcInput_Type.tp_new = PyType_GenericNew;
    vlcInput_Type.tp_alloc = PyType_GenericAlloc;

    p_module = Py_InitModule3( "vlc", vlc_methods,
                               "VLC media player embedding module." );

    if( !p_module )
      return;

    if( PyType_Ready( &PyPosition_Type ) < 0 )
        return;
    if( PyType_Ready( &MediaControl_Type ) < 0 )
        return;
    if( PyType_Ready( &vlcObject_Type ) < 0 )
        return;
    if( PyType_Ready( &vlcInstance_Type ) < 0 )
        return;
    if( PyType_Ready( &vlcInput_Type ) < 0 )
        return;

    /* Exceptions */
    MediaControl_InternalException =
            PyErr_NewException( "vlc.InternalException", NULL, NULL );
    Py_INCREF( MediaControl_InternalException );
    PyModule_AddObject( p_module, "InternalException",
                        MediaControl_InternalException );

    MediaControl_PositionKeyNotSupported =
            PyErr_NewException( "vlc.PositionKeyNotSupported", NULL, NULL );
    Py_INCREF( MediaControl_PositionKeyNotSupported );
    PyModule_AddObject( p_module, "PositionKeyNotSupported",
                        MediaControl_PositionKeyNotSupported );

    MediaControl_PositionOriginNotSupported=
            PyErr_NewException( "vlc.InvalidPosition", NULL, NULL );
    Py_INCREF( MediaControl_PositionOriginNotSupported );
    PyModule_AddObject( p_module, "PositionOriginNotSupported",
                        MediaControl_PositionOriginNotSupported );

    MediaControl_InvalidPosition =
            PyErr_NewException( "vlc.InvalidPosition", NULL, NULL );
    Py_INCREF( MediaControl_InvalidPosition );
    PyModule_AddObject( p_module, "InvalidPosition",
                        MediaControl_InvalidPosition );

    MediaControl_PlaylistException =
            PyErr_NewException( "vlc.PlaylistException", NULL, NULL );
    Py_INCREF( MediaControl_PlaylistException );
    PyModule_AddObject( p_module, "PlaylistException",
                        MediaControl_PlaylistException );

    /* Exceptions */
    vlcInstance_Exception =
        PyErr_NewException( "vlc.InstanceException", NULL, NULL );
    Py_INCREF( vlcInstance_Exception );
    PyModule_AddObject( p_module, "InstanceException",
                        vlcInstance_Exception );

    /* Types */
    Py_INCREF( &PyPosition_Type );
    PyModule_AddObject( p_module, "Position",
                        ( PyObject * )&PyPosition_Type );

    Py_INCREF( &MediaControl_Type );
    PyModule_AddObject( p_module, "MediaControl",
                        ( PyObject * )&MediaControl_Type );

    Py_INCREF( &vlcObject_Type );
    PyModule_AddObject( p_module, "Object",
                        ( PyObject * )&vlcObject_Type );
    Py_INCREF( &vlcInstance_Type );
    PyModule_AddObject( p_module, "Instance",
                        ( PyObject * )&vlcInstance_Type );
    Py_INCREF( &vlcInput_Type );
    PyModule_AddObject( p_module, "Input",
                        ( PyObject * )&vlcInput_Type );

    /* Constants */
    PyModule_AddIntConstant( p_module, "AbsolutePosition",
                             mediacontrol_AbsolutePosition );
    PyModule_AddIntConstant( p_module, "RelativePosition",
                             mediacontrol_RelativePosition );
    PyModule_AddIntConstant( p_module, "ModuloPosition",
                             mediacontrol_ModuloPosition );

    PyModule_AddIntConstant( p_module, "ByteCount",
                             mediacontrol_ByteCount );
    PyModule_AddIntConstant( p_module, "SampleCount",
                             mediacontrol_SampleCount );
    PyModule_AddIntConstant( p_module, "MediaTime",
                             mediacontrol_MediaTime );
    PyModule_AddIntConstant( p_module, "PlayingStatus",
                             mediacontrol_PlayingStatus );
    PyModule_AddIntConstant( p_module, "PauseStatus",
                             mediacontrol_PauseStatus );
    PyModule_AddIntConstant( p_module, "ForwardStatus",
                             mediacontrol_ForwardStatus );
    PyModule_AddIntConstant( p_module, "BackwardStatus",
                             mediacontrol_BackwardStatus );
    PyModule_AddIntConstant( p_module, "InitStatus",
                             mediacontrol_InitStatus );
    PyModule_AddIntConstant( p_module, "EndStatus",
                             mediacontrol_EndStatus );
    PyModule_AddIntConstant( p_module, "UndefinedStatus",
                             mediacontrol_UndefinedStatus );
}


/* Make libpostproc happy... */
void * fast_memcpy( void * to, const void * from, size_t len )
{
  return memcpy( to, from, len );
}

/* Horrible hack... Please do not look.  Temporary workaround for the
   forward declaration mess of python types (cf vlcglue.h). If we do a
   separate compilation, we have to declare some types as extern. But
   the recommended way to forward declare types in python is
   static... I am sorting the mess but in the meantime, this will
   produce a working python module.
*/
#include "vlc_mediacontrol.c"
#include "vlc_position.c"
#include "vlc_instance.c"
#include "vlc_input.c"
#include "vlc_object.c"
