/**
 * @file subclass.h
 * @brief Header file containing user-defined types that related to subclassing windows.
 */

#ifndef SUBCLASS_H
#define SUBCLASS_H

/*************************
 * Animation state enums *
 *************************/

enum class DefaultAnimationState
{
    Default
};
enum class ButtonAnimationState
{
    Default,
    Hover,
    Active,
    Focus,
    FocusLost
};
enum class RadioButtonAnimationState
{
    Default,
    Hover,
    Down,
    Selected,
    SelectedHover,
    SelectedDown,
    Focus,
    FocusLost
};
enum class EditAnimationState
{
    Default,
    Selected
};
enum class DDLComboboxAnimationState
{
    Default,
    Active,
    Focus,
    FocusLost
};
enum class ScrollbarAnimationState
{
    Default,
    Hover,
    Dragging
};

/*************************************
 * Subclass configuration structures *
 *************************************/

/**
 * @brief Contains information that determines the behavior of a subclassed window.
 * @note The effect of a setting can vary based on the subclass type.
 */
struct SubclassConfig
{
public:
    MyColor *pBackground = nullptr;          // If applicable, the subclass class will use this color to render the window background.
    bool skipHoverAnimationState = false;    // Skip all hover-related animation states.
    bool skipActiveAnimationState = false;   // Skip all down-related animation states.
    bool skipSelectedAnimationState = false; // Skip all selected-related animation states.
    bool ignoreTextScalingLimits = true;     // Ignore text format scaling limits. Enabled by default.
    FLOAT textLowerBoundSizeLimit = 20.0f;   // Lower bound text size limit.
    FLOAT textUpperBoundSizeLimit = 100.0f;  // Upper bound text size limit.
};

/**
 * @brief Contains information that determines the behavior of a subclassed window (MyImageButtonSubclass).
 */
struct MyImageButtonSubclassConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Default constructor.
     */
    MyImageButtonSubclassConfig();

    /**
     * @brief Constructor.
     * @param pImageDefault Specifies pointer to the image button default image.
     * @param pImageHover Specifies pointer to the image button hover image.
     * @param pImageActive Specifies pointer to the image button active image.
     * @param pColorBackgroundDefault Specifies pointer to the image button default background color.
     * @param pColorBackgroundHover Specifies pointer to the image button hover background color.
     * @param pColorBackgroundActive Specifies pointer to the image button active background color.
     * @param imagePosX Specifies the image relative x position.
     * @param imagePosY Specifies the image relative y position.
     * @param imageWidth Specifies the image width.
     * @param imageHeight Specifies the image height.
     */
    MyImageButtonSubclassConfig(MyWICBitmap *pImageDefault, MyWICBitmap *pImageHover, MyWICBitmap *pImageActive, MyColor *pColorBackgroundDefault, MyColor *pColorBackgroundHover, MyColor *pColorBackgroundActive, FLOAT imagePosX, FLOAT imagePosY, FLOAT imageWidth, FLOAT imageHeight);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    MyWICBitmap *pImageDefault = nullptr;       // Pointer to the image button default image.
    MyWICBitmap *pImageHover = nullptr;         // Pointer to the image button hover image.
    MyWICBitmap *pImageActive = nullptr;        // Pointer to the image button active image.
    MyColor *pColorBackgroundDefault = nullptr; // Pointer to the image button default background color.
    MyColor *pColorBackgroundHover = nullptr;   // Pointer to the image button hover background color.
    MyColor *pColorBackgroundActive = nullptr;  // Pointer to the image button active background color.
    FLOAT imagePosX = 0;                        // The image relative x position. (optional)
    FLOAT imagePosY = 0;                        // The image relative y position. (optional)
    FLOAT imageWidth = 0;                       // The image width.
    FLOAT imageHeight = 0;                      // The image height.
    bool alwaysDrawDefaultImage = true;         // Indicate whether to always draw the default image. (optional)
    bool centering = false;                     // Indicate whether to center the image. (optional)
};

/**
 * @brief Contains information that determines the behavior of a subclassed window (MyEditboxSubclass).
 */
struct MyEditboxSubclassConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Default constructor.
     */
    MyEditboxSubclassConfig();

    /**
     * @brief Constructor.
     * @param staticWindowID Specifies the ID of the associated-static window.
     */
    MyEditboxSubclassConfig(HMENU staticWindowID);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    HMENU staticWindowID = 0;                       // The ID of the associated-static window.
    INT associatedWMCommandMessageID = 0;           // The associated WM_COMMAND message ID that will be sent by the editbox on the enter key press. (optional)
    std::wstring editboxWindowFontFamilyName = L""; // The font family name for the editbox window. If not set, default font will be used. (optional)
    FLOAT fontSize = 0;                             // Font size in pixels, internal leading included. If not specified (0), the font size will be calculated automatically. (optional)
};

/**
 * @brief Contains information that determines the behavior of a subclassed window (MyStandardTextSubclass).
 */
struct MyStandardTextSubclassConfig
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Default constructor.
     */
    MyStandardTextSubclassConfig();

    /**
     * @brief Constructor.
     * @param centerMode Specifies the centering mode. (0 - No centering, 1 - Center horizontally, 2 - Center vertically, 3 - Center horizontally and vertically.)
     * @param posX Specifies the relative x position.
     * @param posY Specifies the relative Y position.
     * @param fontWeight Specifies the font weight. (https://learn.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_font_weight)
     * @param fontStyle Specifies the font style. (https://learn.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_font_style)
     * @param fontScale Specifies the font scale, the default is at maximum 1.0f (100%), the minimum scale is 0.1f (10%).
     * @param pTextColor Specifies the pointer to the text color object. If nullptr, use the default text color.
     */
    MyStandardTextSubclassConfig(int centerMode, int posX, int posY, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, float fontScale, MyColor *pTextColor);

    // [VALIDATE FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     * @return Returns true if the configuration structure is valid, otherwise false.
     */
    bool isValid();

public:
    int centerMode = 0;                                        // The centering mode. (0 - No centering, 1 - Center horizontally, 2 - Center vertically, 3 - Center horizontally and vertically.)
    int posX = 0;                                              // The text relative X position.
    int posY = 0;                                              // The text relative Y position.
    DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL; // The font weight. (https://learn.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_font_weight)
    DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL;    // The font style. (https://learn.microsoft.com/en-us/windows/win32/api/dwrite/ne-dwrite-dwrite_font_style)
    float fontScale = 1.0f;                                    // The font scale, the default is at maximum 1.0f (100%), the minimum scale is 0.1f (10%).
    MyColor *pTextColor = nullptr;                             // Pointer to the text color object. If nullptr, use the default text color.
};

/********************
 * Subclass classes *
 ********************/

/**
 * @brief Initialize the subclass classes.
 * @note Call `InitializeSubclassClasses()` to initialize the subclass classes before any subclass class object creation.
 * @param pGraphic Reference to the smart pointer that point to the graphic engine.
 * @param pUIManager Reference to the snart pointer that point to the UI manager.
 * @param appWindow Reference to the application main window handle.
 * @return Returns true if all the operations are successfully performed, false otherwise.
 */
bool InitializeSubclassClasses(std::unique_ptr<MyGraphicEngine> *pGraphic, std::unique_ptr<UIManager> *pUIManager, HWND &appWindow);

/**
 * @brief Uninitialize the subclass classes.
 * @note Call `UninitializeSubclassClasses()` to uninitialize the subclass classes after all the subclass class objects are destroyed to ensure proper cleanup.
 * @return Returns true if all the operations are successfully performed, false otherwise.
 */
bool UninitializeSubclassClasses();

/**
 * @brief The subclass abstract base class. The derived classes access graphic-related functions and resources via this class.
 * @note Call `InitializeSubclassClasses()` to initialize the subclass classes before any subclass class object creation.
 * @note Call `UninitializeSubclassClasses()` to uninitialize the subclass classes after all the subclass class objects are destroyed to ensure proper cleanup.
 */
class MySubclass
{
    friend bool InitializeSubclassClasses(std::unique_ptr<MyGraphicEngine> *, std::unique_ptr<UIManager> *, HWND &);
    friend bool UninitializeSubclassClasses();

public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Constructor.
     */
    MySubclass();

    /**
     * @brief Destructor.
     * @note The destructor is made virtual to make this class abstract.
     */
    virtual ~MySubclass() = 0;

protected:
    // [GRAPHIC FUNCTIONS]
    // The derived classes access graphic-related functions and resources via this class.

    /**
     * @brief Get the pointer to the graphic engine.
     * @note The graphic engine is used to perform graphic-related operations.
     * @return Returns a pointer to the graphic engine.
     */
    MyGraphicEngine *graphics();

    /**
     * @brief Get the pointer to the UI manager.
     * @note The UI manager is used to access graphic-related resources.
     * @return Returns a pointer to the UI manager.
     */
    UIManager *objects();

public:
    SubclassConfig config;                   // Configuration settings for customizing the behavior of the associated window.
    inline static UINT64 totalInstances = 0; // Indicate the total number of MySubclass instances. [DEBUG]

protected:
    inline static const UINT_PTR IDT_ANIMATION_INVALIDATE = 1;                              // Animation invalidation timer ID.
    inline static bool isInitialized = false;                                               // Indicate whether the subclass classes are initialized.
    inline static HWND *pAppWindow = nullptr;                                               // Pointer to the application main window handle.
    std::unique_ptr<ID2D1DCRenderTarget *, ID2D1DCRenderTargetDeleter> pD2D1DCRenderTarget; // Direct2D render target.
    bool isAssociated = false;                                                              // Indicate whether the subclass object is associated to a window.

private:
    inline static std::unique_ptr<MyGraphicEngine> *ppGraphic = nullptr; // Pointer to pointer that point to the graphic engine.
    inline static std::unique_ptr<UIManager> *ppUIElements = nullptr;    // Pointer to pointer that point to the UI manager.
};

/**
 * @brief Standard button subclass class.
 */
class MyStandardButtonSubclass : public MySubclass
{
public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release the shared device resources of the class.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(ButtonAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyStandardButtonSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(ButtonAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
    // Button-related variables.
    HWND buttonWindow = nullptr; // Handle to the button window.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.150f;                                                                             // Default state animation duration.
    FLOAT hoverAnimationDuration = 0.150f;                                                                               // Hover state animation duration.
    FLOAT activeAnimationDuration = 0.100f;                                                                              // Active state animation duration.
    FLOAT focusAnimationDuration = 0.100f;                                                                               // Focus state animation duration.
    ButtonAnimationState currentAnimationState = ButtonAnimationState::Default;                                          // Indicate the current animation state.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableFocusBorderOpacity;           // Animation variable: Focus border opacity.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonRGB;       // Animation variable: Button color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonBorderRGB; // Animation variable: Button border color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonTextRGB;   // Animation variable: Button text color (RGB).
    bool isHoverState = false;                                                                                           // Indicate whether the window is hovered.
    bool isActiveState = false;                                                                                          // Indicate whether the window is active.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    std::unique_ptr<IDWriteTextFormat *, IDWriteTextFormatDeleter> pTextFormat; // Text format. (Non-shared resource)
};

/**
 * @brief Image button subclass class.
 */
class MyImageButtonSubclass : public MySubclass
{
public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release all shared device resources.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(ButtonAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @param pConfig Pointer to the configuration structure.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd, MyImageButtonSubclassConfig *pConfig);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyImageButtonSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(ButtonAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    // Configuration variables.
    MyImageButtonSubclassConfig imageConfig;

private:
    // Button-related variables.
    HWND buttonWindow = nullptr; // Handle to the button window.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.150f;                                                                           // Default state animation duration.
    FLOAT hoverAnimationDuration = 0.150f;                                                                             // Hover state animation duration.
    FLOAT activeAnimationDuration = 0.100f;                                                                            // Active state animation duration.
    FLOAT focusAnimationDuration = 0.100f;                                                                             // Focus state animation duration.
    ButtonAnimationState currentAnimationState = ButtonAnimationState::Default;                                        // Indicate the current animation state.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableFocusBorderOpacity;         // Animation variable: Focus border opacity.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableDefaultImageOpacity;        // Animation variable: Default state image opacity.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableHoverImageOpacity;          // Animation variable: Hover state image opacity.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableActiveImageOpacity;         // Animation variable: Active state image opacity.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableBackgroundRGB; // Animation variable: Background color (RGB).
    bool isHoverState = false;                                                                                         // Indicate whether the window is hovered.
    bool isActiveState = false;                                                                                        // Indicate whether the window is active.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    std::unique_ptr<ID2D1Bitmap *, ID2D1BitmapDeleter> pD2D1BitmapDefaultImage; // Default state image bitmap. (Non-shared resource)
    std::unique_ptr<ID2D1Bitmap *, ID2D1BitmapDeleter> pD2D1BitmapHoverImage;   // Hover state image bitmap.   (Non-shared resource)
    std::unique_ptr<ID2D1Bitmap *, ID2D1BitmapDeleter> pD2D1BitmapActiveImage;  // Active state image bitmap.  (Non-shared resource)
};

/**
 * @brief Radio button subclass class.
 */
class MyRadioButtonSubclass : public MySubclass
{
    friend class MyRadioGroup; // Make class MyRadioGroup a friend. Allow it to access the class member directly.
public:
    // [RADIO FUNCTIONS]

    /**
     * @brief Update the button selection state.
     * @param state Specifies the new button selection state. (0 - deselected | 1 - selected)
     */
    void updateSelectionState(bool state);

    // [UTILITY FUNCTIONS]

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release the shared device resources of the class.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(RadioButtonAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyRadioButtonSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(RadioButtonAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
    // Radio-related variables.
    HWND buttonWindow = nullptr;         // Handle to the button window.
    MyRadioGroup *pRadioGroup = nullptr; // Pointer to the radio group the button belongs to. This value will be set by the radio group when the button is added to the group.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.150f;                                                                                // Default state animation duration.
    FLOAT hoverAnimationDuration = 0.150f;                                                                                  // Hover state animation duration.
    FLOAT activeAnimationDuration = 0.100f;                                                                                 // Active state animation duration.
    FLOAT focusAnimationDuration = 0.100f;                                                                                  // Focus state animation duration.
    RadioButtonAnimationState currentAnimationState = RadioButtonAnimationState::Default;                                   // Indicate the current animation state.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableFocusBorderOpacity;              // Animation variable: Focus border opacity.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonPrimaryRGB;   // Animation variable: Button primary color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonSecondaryRGB; // Animation variable: Button secondary color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonBorderRGB;    // Animation variable: Button border color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableButtonTextRGB;      // Animation variable: Button text color (RGB).
    bool isHoverState = false;                                                                                              // Indicate whether the window is hovered.
    bool isActiveState = false;                                                                                             // Indicate whether the window is active.
    bool isSelected = false;                                                                                                // Indicate whether the window is selected.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    std::unique_ptr<IDWriteTextFormat *, IDWriteTextFormatDeleter> pTextFormat; // Text format. (Non-shared resource)
};

/**
 * @brief Radio group responsible for managing a collection of radio buttons,
 *        ensuring that only one button in the group can be selected at a time.
 * @note Each object represents a single group.
 */
class MyRadioGroup
{
public:
    // [RADIO GROUP FUNCTIONS]

    /**
     * @brief Add a radio button to the radio group.
     * @param hWnd Handle to the window that is subclassed by `MyRadioButtonSubclass` class.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool addRadioButton(HWND hWnd);

    /**
     * @brief Remove a radio button from the radio group.
     * @param hWnd Handle to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool removeRadioButton(HWND hWnd);

    /**
     * @brief Get the current state of the radio group.
     * @return Returns the ID of the currently selected button in the group.
     * @note If no button is selected, the return value is zero.
     */
    int getRadioState();

    /**
     * @brief Update the state of the radio group.
     * @param buttonID ID of the button that will be selected.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool updateRadioState(INT buttonID);

    /**
     * @brief Reset the radio group.
     */
    void resetRadioGroup();

private:
    // Map associating radio button IDs with pointers to subclass objects (class MyRadioButtonSubclass).
    // Key: The ID of the radio button.
    // Value: Pointer to the subclass object that subclasses the button (class MyRadioButtonSubclass).
    std::map<int, std::pair<HWND, MyRadioButtonSubclass *>> mapButtons;
    int currentSelectedButtonID = 0; // This indicates which button in the radio group is selected.
};

/**
 * @brief Editbox subclass class.
 */
class MyEditboxSubclass : public MySubclass
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Destructor.
     */
    ~MyEditboxSubclass();

    // [UTILITY FUNCTIONS]

    /**
     * @brief Get the window associated static window handle.
     * @return Returns the window associated static window handle.
     */
    HWND getStaticHandle();

    /**
     * @brief Get the window associated static window handle. (Reference)
     * @return Returns the window associated static window handle. (Reference)
     */
    HWND &getStaticHandleRef();

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release all shared device resources.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(EditAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @param pConfig Pointer to the configuration structure.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd, MyEditboxSubclassConfig *pConfig);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyEditboxSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(EditAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    // Configuration variables.
    MyEditboxSubclassConfig editConfig;

private:
    // Editbox-related variables.
    HWND staticWindow = nullptr;                      // Handle to the editbox-associated static window. (This static window is responsible for border drawing)
    HWND editboxWindow = nullptr;                     // Handle to the editbox window.
    CHARFORMAT2 editDefaultCharFormat = {0};          // Editbox window character formats.
    std::unique_ptr<HFONT, HFONTDeleter> editboxFont; // Editbox window font.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.150f;                                                                              // Default state animation duration.
    FLOAT selectedAnimationDuration = 0.150f;                                                                             // Selected state animation duration.
    EditAnimationState currentAnimationState = EditAnimationState::Default;                                               // Indicate the current animation state.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableEditboxBorderRGB; // Animation variable: Editbox border color (RGB).

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    inline static std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSharedD2D1SolidColorBrushEditbox;           // Editbox solid color bush.            (Shared resource)
    inline static std::unique_ptr<ID2D1SolidColorBrush *, ID2D1SolidColorBrushDeleter> pSharedD2D1SolidColorBrushEditboxBackground; // Editbox background solid color bush. (Shared resource)
};

/**
 * @brief Drop-down list combobox subclass class.
 */
class MyDDLComboboxSubclass : public MySubclass
{
public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    /**
     * @brief Release the shared device resources of the class.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(DDLComboboxAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd, INT comboboxHeight);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyDDLComboboxSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @param comboboxHeight Specifies the combobox height on the first time creating device resources.
     *                       so the font height can be calculated correctly.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false, int comboboxHeight = 0);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(DDLComboboxAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureCombobox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureComboboxDDL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
    // DDL Combobox-related variables.
    HWND comboboxWindow = nullptr;                // Handle to the drop-down list combobox window.
    HWND ddlWindow = nullptr;                     // Handle to the drop-down list combobox drop-down list window.
    std::unique_ptr<HFONT, HFONTDeleter> ddlFont; // Drop-down list font.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.100f;                                                                               // Default state animation duration.
    FLOAT activeAnimationDuration = 0.100f;                                                                                // Active state animation duration.
    FLOAT focusAnimationDuration = 0.100f;                                                                                 // Focus state animation duration.
    DDLComboboxAnimationState currentAnimationState = DDLComboboxAnimationState::Default;                                  // Indicate the current animation state.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableFocusBorderOpacity;             // Animation variable: Focus border opacity.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableComboboxRGB;       // Animation variable: Combobox color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableComboboxBorderRGB; // Animation variable: Combobox border color (RGB).
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableComboboxTextRGB;   // Animation variable: Combobox text color (RGB).
    bool isActive = false;                                                                                                 // Indicate whether the window is active.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    std::unique_ptr<IDWriteTextFormat *, IDWriteTextFormatDeleter> pTextFormat;              // Text format. (Non-shared resource)
    inline static std::unique_ptr<ID2D1Bitmap *, ID2D1BitmapDeleter> pSharedD2D1BitmapArrow; // Arrow image bitmap. (Shared resource)
};

/**
 * @brief Scrollbar (Vertical) subclass class.
 */
class MyVerticalScrollbarSubclass : public MySubclass
{
public:
    // [CONSTRUCTOR/DESTRUCTOR/..]

    /**
     * @brief Destructor.
     */
    ~MyVerticalScrollbarSubclass();

    // [UTILITY FUNCTIONS]

    /**
     * @brief Scroll the associated container window by amount.
     * @param scrollAmount Number of pixels to scroll. Positive number for downward direction, negative number for upward direction.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool scrollWindowByAmount(INT scrollAmount);

    /**
     * @brief Scroll the associated container window by pos.
     * @param scrollPos The target scroll position.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool scrollWindowByPos(INT scrollPos);

    /**
     * @brief Scroll the associated container window by amount. (Smooth)
     * @param scrollAmount Number of pixels to scroll. Positive number for downward direction, negative number for upward direction.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool scrollWindowByAmountSmooth(INT scrollAmount);

    /**
     * @brief Scroll the associated container window by pos. (Smooth)
     * @param scrollPos The target scroll position.
     * @param milliseconds The duration of the smooth scrolling animation in milliseconds.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool scrollWindowByPosSmooth(INT scrollPos, FLOAT milliseconds);

    /**
     * @brief Get the scrollbar scroll informations.
     * @param scrollInfo Reference to the SCROLLINFO struct that will receive the scroll information. [OUT]
     * @param pCurrentPos Pointer to the variable that will receive the current scroll position. [OUT] (optional)
     * @param pMinScrollPos Pointer to the variable that will receive the min scroll position. [OUT] (optional)
     * @param pMaxScrollPos Pointer to the variable that will receive the max scroll position. [OUT] (optional)
     * @param pMaxUpwardScrollAmount Pointer to the variable that will receive the max upward scroll amount. [OUT] (optional)
     * @param pMaxDownwardScrollAmount Pointer to the variable that will receive the max downward scroll amount. [OUT] (optional)
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool getScrollInfo(SCROLLINFO &scrollInfo, int *pCurrentPos = nullptr, int *pMinScrollPos = nullptr, int *pMaxScrollPos = nullptr, int *pMaxUpwardScrollAmount = nullptr, int *pMaxDownwardScrollAmount = nullptr);

    /**
     * @brief Get the window associated static window handle.
     * @return Returns the window associated static window handle.
     */
    HWND getStaticHandle();

    /**
     * @brief Get the window associated static window handle. (Reference)
     * @return Returns the window associated static window handle. (Reference)
     */
    HWND &getStaticHandleRef();

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release the shared device resources of the class.
     */
    static void releaseSharedDeviceResources();

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Set the animation duration for a specific animation state.
     * @param animateState The animation state.
     * @param milliseconds Duration in milliseconds.
     */
    void setAnimationDuration(ScrollbarAnimationState animateState, FLOAT milliseconds);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @param pContainer Handle to the container window that will be scrolled.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd, MyContainer *pContainer);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyVerticalScrollbarSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [ANIMATION FUNCTIONS]

    /**
     * @brief Start animation for the window.
     * @param animationState The animation state to be played.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool startAnimation(ScrollbarAnimationState animationState);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureScrollbar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedureStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
    // Scrollbar-related variables.
    inline static const UINT_PTR IDT_ANIMATION_SCROLLBAR = 2; // Animation scrollbar timer ID.
    inline static const UINT SMOOTH_SCROLL_SPEED = 150;       // Smooth scroll speed. (Recommended: 150)
    HWND scrollbarWindow = nullptr;                           // Handle to the scrollbar window.
    HWND staticWindow = nullptr;                              // Handle to the scrollbar-associated static window that represents the scrollbar appearance.
    MyContainer *pContainer = nullptr;                        // Handle to the container window that will be scrolled.
    int initialThumbPos = 0;                                  // The initial position of the scrollbar thumb when the dragging starts.
    int initialClickPos = 0;                                  // The initial position of the mouse cursor when the dragging starts.
    int lastScrollPos = 0;                                    // Indicate the last scroll position.
    bool scrollInProgress = false;                            // Indicate whether scrolling animation is in progress.

    // Animation-related variables.
    FLOAT defaultAnimationDuration = 0.100f;                                                                               // Default state animation duration.
    FLOAT hoverAnimationDuration = 0.100f;                                                                                 // Hover state animation duration.
    FLOAT draggingAnimationDuration = 0.100f;                                                                              // Dragging state animation duration.
    ScrollbarAnimationState currentAnimationState = ScrollbarAnimationState::Default;                                      // Indicate the current animation state.
    std::vector<std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter>> pAnimationVariableScrollbarThumbRGB; // Animation variable: Scrollbar thumb color (RGB).
    bool isThumbHoverState = false;                                                                                        // Indicate whether the scrollbar thumb is hovered.
    bool isThumbDragging = false;                                                                                          // Indicate whether the scrollbar thumb is being dragged.
    std::unique_ptr<IUIAnimationVariable *, IUIAnimationVariableDeleter> pAnimationVariableLastScrollPos;                  // Animation variable: Last scroll position.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    // ...
};

/**
 * @brief Standard text subclass class.
 */
class MyStandardTextSubclass : public MySubclass
{
public:
    // [UTILITY FUNCTIONS]

    /**
     * @brief Refresh the window appearance.
     * @note Refresh the window to recreate its device resources and update its animation variable values.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool refresh();

    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Release the shared device resources of the class.
     */
    static void releaseSharedDeviceResources();

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Associate the subclass object with a window.
     * @note Each object can only be associated with a single window.
     * @param hWnd Handle to the window.
     * @param pConfig Pointer to the configuration structure.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool setWindow(HWND hWnd, MyStandardTextSubclassConfig *pConfig);

    /**
     * @brief Get a pointer to the associated subclass object of a window.
     * @param hWnd Handle to the window.
     * @return Returns a pointer to the associated subclass object of a window. Otherwise, it returns nullptr if the window is not subclassed by this class.
     */
    static MyStandardTextSubclass *getSubclassPointer(HWND hWnd);

private:
    // [DIRECT2D FUNCTIONS]

    /**
     * @brief Create the shared device resources of the class.
     * @note Shared resources are the resources used by all instances of the subclass class.
     * @note The function won't create new shared resources if they already exist.
     * @note If the shared resource needs updating, call the releaseSharedDeviceResources() function beforehand.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createSharedDeviceResources();

    /**
     * @brief Create the device resources for the window.
     * @note Any existing device resources will be automatically released beforehand. (This doesn't include shared resources unless specified.)
     * @note The class shared device resources will be created if they do not exist yet.
     * @param recreateSharedResources Specifies whether to recreate the shared resources.
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createDeviceResources(bool recreateSharedResources = false);

    // [SUBCLASS FUNCTIONS]

    /**
     * @brief Subclass callback function for the window.
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK subclassProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    // Configuration variables.
    MyStandardTextSubclassConfig textConfig;

private:
    // Text-related variables.
    HWND textWindow = nullptr; // Handle to the text window.

    // Direct2D-related variables.
    // Shared resources: These are resources that are utilized by all instances of the class.
    // Non-shared resources: These are resources that are exclusive and associated with a specific instance.
    std::unique_ptr<IDWriteTextFormat *, IDWriteTextFormatDeleter> pTextFormat; // Text format. (Non-shared resource)
};

#endif // SUBCLASS_H