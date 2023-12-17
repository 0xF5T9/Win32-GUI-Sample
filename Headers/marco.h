/**
 * @file marco.h
 * @brief Define global marcos.
 */

#ifndef MARCO_H
#define MARCO_H

/**********************
 * Window identifiers *
 **********************/

// Non-client windows:

#define IDC_NONCLIENT_CLOSE_BUTTON 101
#define IDC_NONCLIENT_MAXIMIZE_BUTTON 102
#define IDC_NONCLIENT_MINIMIZE_BUTTON 103
#define IDC_NONCLIENT_CAPTIONTITLE_STATIC 104

// Client windows:

#define IDC_DC_CONTAINER 200 // DC - Default container.
#define IDC_DC_SCROLLBAR 201
#define IDC_DC_HEADING1 202
#define IDC_DC_STANDARDBUTTON1 203
#define IDC_DC_STANDARDBUTTON2 204
#define IDC_DC_STANDARDBUTTON3 205
#define IDC_DC_HEADING2 206
#define IDC_DC_RADIOBUTTON1 207
#define IDC_DC_RADIOBUTTON2 208
#define IDC_DC_RADIOBUTTON3 209
#define IDC_DC_HEADING3 210
#define IDC_DC_IMAGEBUTTON1 211
#define IDC_DC_IMAGEBUTTON2 212
#define IDC_DC_IMAGEBUTTON3 213
#define IDC_DC_IMAGEBUTTON4 214
#define IDC_DC_HEADING4 215
#define IDC_DC_EDITBOXNORMALBORDER 216
#define IDC_DC_EDITBOXNORMAL 217
#define IDC_DC_EDITBOXPASSWORDBORDER 218
#define IDC_DC_EDITBOXPASSWORD 219
#define IDC_DC_EDITBOXMULTILINEBORDER 220
#define IDC_DC_EDITBOXMULTILINE 221
#define IDC_DC_EDITBOXNORMALNOTE 222
#define IDC_DC_EDITBOXPASSWORDNOTE 223
#define IDC_DC_EDITBOXMULTILINENOTE 224
#define IDC_DC_HEADING5 225
#define IDC_DC_COMBOBOXEMPTY 226
#define IDC_DC_COMBOBOXNORMAL 227
#define IDC_DC_HEADING6 228
#define IDC_DC_OPENLOGFILE 229
#define IDC_DC_OPENLOGFILENOTE 230
#define IDC_DC_SELECTFILE 231
#define IDC_DC_SELECTFILENOTE 232
#define IDC_DC_COMBOBOXSELECTTHEME 233
#define IDC_DC_COMBOBOXSELECTTHEMENOTE 234
#define IDC_DC_COMBOBOXSELECTFONT 235
#define IDC_DC_COMBOBOXSELECTFONTNOTE 236

/***********************
 * My WinAPI constants *
 ***********************/

#define WINDOW_CAPTIONBAR_DEFAULTHEIGHT 37 // Default window caption bar height (pixel)
#define WINDOW_BORDER_DEFAULTWIDTH 1       // Default window borders width      (pixel)
#define WINDOW_SCROLLBAR_DEFAULTWIDTH 18   // Default window scrollbars width   (pixel)

// My defined window styles.
// - WS_POPUP: Allows flexible window appearance customization.
// - WS_THICKFRAME: Provides a drop-shadow effect and enables window snapping functionality.
// - WS_CAPTION: Enables window minimize and maximize animations.
// - WS_MINIMIZEBOX: Enables window minimize functionality.
// - WS_MAXIMIZEBOX: Enables window maximize functionality.
// - WS_CLIPCHILDREN: Ensures child windows are clipped to the parent window boundaries, preventing drawing outside of the parent's area.
// Note: The non-client caption bar from WS_CAPTION and size borders from WS_THICKFRAME will be ignored and not visible.
//       The application will draw and handle its own caption bar and size border events manually.
#define WS_MYSTYLE (WS_POPUP |       \
                    WS_THICKFRAME |  \
                    WS_CAPTION |     \
                    WS_MINIMIZEBOX | \
                    WS_MAXIMIZEBOX | \
                    WS_CLIPCHILDREN)

#endif // MARCO_H