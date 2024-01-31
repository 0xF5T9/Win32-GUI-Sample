/**
 * @file forward_declarations.h
 * @brief Global forward declarations for types, functions, etc.
 */

#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

/*********
 * Enums *
 *********/

enum class MyLogType;

/***********
 * Structs *
 ***********/

struct SubclassConfig;
struct MyImageButtonSubclassConfig;
struct MyEditboxSubclassConfig;
struct MyStandardTextSubclassConfig;

/***********
 * Classes *
 ***********/

class MySubclass;
class MyGraphicEngine;
class MyRadioGroup;
class MyContainer;

/*************
 * Functions *
 *************/

LRESULT CALLBACK ApplicationWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK DefaultContainerProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR idSubclass, DWORD_PTR refData);

#endif // FORWARD_DECLARATIONS_H