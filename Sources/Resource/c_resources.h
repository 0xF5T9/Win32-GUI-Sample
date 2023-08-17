/**
 * @file c_resource.h
 * @brief Define application constants and include project resource header.
*/

#ifndef C_RESOURCES_H
#define C_RESOURCES_H

#define NOMINMAX    // Defines NOMINMAX to prevent the Windows header <Windows.h> from defining its own min and max macros.
#define OEMRESOURCE // Enable OEM resource marcos.

#include "../Resource/resource.h" // Project resource header.

// Timer identifiers.
// #define IDT_ACTIVE_CHECK 1 // Reserved and likely to be removed in the future.

// Window identifiers. (non-client)
#define IDC_NONCLIENT_CLOSE_BUTTON        101
#define IDC_NONCLIENT_MAXIMIZE_BUTTON     102
#define IDC_NONCLIENT_MINIMIZE_BUTTON     103
#define IDC_NONCLIENT_CAPTIONTITLE_STATIC 104

// Window identifiers. (client)
#define IDC_MAINCONTENT_CONTAINER           500
#define IDC_MAINCONTENT_CONTAINER_SCROLLBAR 501

// Container identifiers: MainContent
#define IDC_MAINCONTENT_SAMPLEBUTTONS_STATIC     502
#define IDC_MAINCONTENT_STANDARDBUTTON_BUTTON    503
#define IDC_MAINCONTENT_RADIO_TEST_FIRST_BUTTON  504
#define IDC_MAINCONTENT_RADIO_TEST_SECOND_BUTTON 505
#define IDC_MAINCONTENT_RADIO_TEST_THIRD_BUTTON  506

#define IDC_MAINCONTENT_SAMPLEEDITBOXES_STATIC      509
#define IDC_MAINCONTENT_STANDARDEDITBOXNOTE_STATIC  510
#define IDC_MAINCONTENT_STANDARDEDITBOX_STATIC      511
#define IDC_MAINCONTENT_STANDARDEDITBOX_EDIT        512
#define IDC_MAINCONTENT_STANDARDEDITBOX_BUTTON      513
#define IDC_MAINCONTENT_PASSWORDEDITBOXNOTE_STATIC  514
#define IDC_MAINCONTENT_PASSWORDEDITBOX_STATIC      515
#define IDC_MAINCONTENT_PASSWORDEDITBOX_EDIT        516
#define IDC_MAINCONTENT_PASSWORDEDITBOX_BUTTON      517
#define IDC_MAINCONTENT_MULTILINEEDITBOXNOTE_STATIC 518
#define IDC_MAINCONTENT_MULTILINEEDITBOX_STATIC     519
#define IDC_MAINCONTENT_MULTILINEEDITBOX_EDIT       520
#define IDC_MAINCONTENT_MULTILINEEDITBOX_BUTTON     521

#define IDC_MAINCONTENT_SAMPLECOMBOBOXES_STATIC 522
#define IDC_MAINCONTENT_SELECTTHEMENOTE_STATIC  523
#define IDC_MAINCONTENT_SELECTTHEME_COMBOBOX    524

// Miscellaneous
#define WINDOW_CAPTIONBAR_DEFAULTHEIGHT 37     // Default window caption bar height (pixel)
#define WINDOW_BORDER_DEFAULTWIDTH      1      // Default window borders width      (pixel)
#define WINDOW_SCROLLBAR_DEFAULTWIDTH   18     // Default window scrollbars width   (pixel) 
#define WINDOW_CONTAINER_DEFAULTPADDING 10     // Default container's contents padding, both vertical and horizontal (pixel)
#define WINDOW_ANIMATION_INVALIDATETICK 16     // Animation invalidate tick (~60IPS)
#define MAX_CLASS_NAME_LENGTH           256    // Maximum class name length (character)

// My defined window styles.
// - WS_POPUP: Allows flexible appearance customization, with all content drawn in the client area, including the caption bar and borders.
// - WS_THICKFRAME: Provides a drop-shadow effect and enables window snapping functionality.
// - WS_CAPTION: Enables window minimize and maximize animations.
// - WS_MINIMIZEBOX: Enables window minimize functionality.
// - WS_MAXIMIZEBOX: Enables window maximize functionality.
// - WS_CLIPCHILDREN: Ensures child windows are clipped to the parent window boundaries, preventing drawing outside of the parent's area.
// Note: The non-client caption bar from WS_CAPTION and size borders from WS_THICKFRAME will be ignored and not visible.
//       The application will draw and handle its own caption bar and size borders.
#define WS_MYSTYLE (    WS_POPUP          | \
                        WS_THICKFRAME     | \
                        WS_CAPTION        | \
                        WS_MINIMIZEBOX    | \
                        WS_MAXIMIZEBOX    | \
                        WS_CLIPCHILDREN)

#endif // C_RESOURCES_H