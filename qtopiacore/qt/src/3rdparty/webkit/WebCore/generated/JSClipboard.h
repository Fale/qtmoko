/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSClipboard_h
#define JSClipboard_h

#include "JSDOMBinding.h"
#include <runtime/JSGlobalObject.h>
#include <runtime/ObjectPrototype.h>

namespace WebCore {

class Clipboard;

class JSClipboard : public DOMObject {
    typedef DOMObject Base;
public:
    JSClipboard(PassRefPtr<JSC::Structure>, PassRefPtr<Clipboard>);
    virtual ~JSClipboard();
    static JSC::JSObject* createPrototype(JSC::ExecState*);
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::PropertySlot&);
    virtual void put(JSC::ExecState*, const JSC::Identifier& propertyName, JSC::JSValuePtr, JSC::PutPropertySlot&);
    virtual const JSC::ClassInfo* classInfo() const { return &s_info; }
    static const JSC::ClassInfo s_info;

    static PassRefPtr<JSC::Structure> createStructure(JSC::JSValuePtr prototype)
    {
        return JSC::Structure::create(prototype, JSC::TypeInfo(JSC::ObjectType));
    }

    static JSC::JSValuePtr getConstructor(JSC::ExecState*);

    // Custom attributes
    JSC::JSValuePtr types(JSC::ExecState*) const;

    // Custom functions
    JSC::JSValuePtr clearData(JSC::ExecState*, const JSC::ArgList&);
    JSC::JSValuePtr getData(JSC::ExecState*, const JSC::ArgList&);
    JSC::JSValuePtr setData(JSC::ExecState*, const JSC::ArgList&);
    JSC::JSValuePtr setDragImage(JSC::ExecState*, const JSC::ArgList&);
    Clipboard* impl() const { return m_impl.get(); }

private:
    RefPtr<Clipboard> m_impl;
};

JSC::JSValuePtr toJS(JSC::ExecState*, Clipboard*);
Clipboard* toClipboard(JSC::JSValuePtr);

class JSClipboardPrototype : public JSC::JSObject {
public:
    static JSC::JSObject* self(JSC::ExecState*);
    virtual const JSC::ClassInfo* classInfo() const { return &s_info; }
    static const JSC::ClassInfo s_info;
    virtual bool getOwnPropertySlot(JSC::ExecState*, const JSC::Identifier&, JSC::PropertySlot&);
    static PassRefPtr<JSC::Structure> createStructure(JSC::JSValuePtr prototype)
    {
        return JSC::Structure::create(prototype, JSC::TypeInfo(JSC::ObjectType));
    }
    JSClipboardPrototype(PassRefPtr<JSC::Structure> structure) : JSC::JSObject(structure) { }
};

// Functions

JSC::JSValuePtr jsClipboardPrototypeFunctionClearData(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsClipboardPrototypeFunctionGetData(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsClipboardPrototypeFunctionSetData(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
JSC::JSValuePtr jsClipboardPrototypeFunctionSetDragImage(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr, const JSC::ArgList&);
// Attributes

JSC::JSValuePtr jsClipboardDropEffect(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSClipboardDropEffect(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsClipboardEffectAllowed(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
void setJSClipboardEffectAllowed(JSC::ExecState*, JSC::JSObject*, JSC::JSValuePtr);
JSC::JSValuePtr jsClipboardTypes(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);
JSC::JSValuePtr jsClipboardConstructor(JSC::ExecState*, const JSC::Identifier&, const JSC::PropertySlot&);

} // namespace WebCore

#endif