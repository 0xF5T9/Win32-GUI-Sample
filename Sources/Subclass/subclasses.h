/**
 * @file subclasses.h
 * @brief Header file containing classes that subclass WinAPI controls for custom drawing or enhance the control message response.
 */

#ifndef SUBCLASSES_H
#define SUBCLASSES_H

/**
 * @class KeyToggle
 *
 * @brief Provides an efficient alternative to GetKeyState() for capturing keydown messages only once instead of continuously while holding the key.
 *
 * @note This class helps to avoid continuous processing of keydown messages and is useful for scenarios where capturing the initial key press is sufficient.
 * @note Readmore: https://stackoverflow.com/questions/10790502/c-getkeystate-has-to-run-once
 */
class KeyToggle
{
private:
    int virtualKey; // Virtual-Key Code. [https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes]
    bool isActive;  // Indicate whether the key is currently pressed or not.

public:
    /**
     * @brief Default constructor.
     *
     * @param virtualKeycode The Virtual-Key Code associated with the key.
     */
    KeyToggle(int virtualKeycode);

    /**
     * @brief Bool operator.
     *
     * @return Returns true if the key associated with this KeyToggle instance is currently pressed, false otherwise.
     */
    operator bool();
};

/**
 * @struct MyStandardButtonInitializeConfig
 *
 * @brief This configuration structure is used for the initialization of the class MyStandardButton.
 */
struct MyStandardButtonInitializeConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pAnimationManager  Pointer to the initialized animation manager.
     * @param pAnimationTimer    Pointer to the initialized animation timer.
     * @param pTransitionLibrary Pointer to the initialized transition library.
     * @param pMyD2D1Engine      Pointer to the initialized MyD2D1Engine object.
     */
    MyStandardButtonInitializeConfig(IUIAnimationManager *&pAnimationManager, IUIAnimationTimer *&pAnimationTimer, IUIAnimationTransitionLibrary *&pTransitionLibrary, MyD2D1Engine *pMyD2D1Engine)
        : pAnimationManager(pAnimationManager), pAnimationTimer(pAnimationTimer), pTransitionLibrary(pTransitionLibrary), pMyD2D1Engine(pMyD2D1Engine) {}

public:
    IUIAnimationManager *&pAnimationManager;
    IUIAnimationTimer *&pAnimationTimer;
    IUIAnimationTransitionLibrary *&pTransitionLibrary;
    MyD2D1Engine *pMyD2D1Engine;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pAnimationManager && this->pAnimationTimer && this->pTransitionLibrary && this->pMyD2D1Engine && this->pMyD2D1Engine->isReady());
    }
};
/**
 * @struct MyStandardButtonSharedAttributesConfig
 *
 * @brief This configuration structure is used to set the shared attributes of the class MyStandardButton.
 */
struct MyStandardButtonSharedAttributesConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pColorStandardButtonDefault       Pointer to the standard button default color.
     * @param pColorStandardButtonHover         Pointer to the standard button hover color.
     * @param pColorStandardButtonDown          Pointer to the standard button down color.
     * @param pColorStandardButtonBorderDefault Pointer to the standard button border default color.
     * @param pColorStandardButtonBorderHover   Pointer to the standard button border hover color.
     * @param pColorStandardButtonBorderDown    Pointer to the standard button border down color.
     * @param pColorTextDefault                 Pointer to the standard button text default color.
     * @param pColorTextHighlight               Pointer to the standard button text highlight color.
     * @param pColorBackground                  Pointer to the standard button background color.
     * @param pColorFocus                       Pointer to the standard button focus color.
     * @param pTextFormat                       Pointer to the standard button text format.
     */
    MyStandardButtonSharedAttributesConfig(MyColor *pColorStandardButtonDefault, MyColor *pColorStandardButtonHover, MyColor *pColorStandardButtonDown,
                                           MyColor *pColorStandardButtonBorderDefault, MyColor *pColorStandardButtonBorderHover, MyColor *pColorStandardButtonBorderDown,
                                           MyColor *pColorTextDefault, MyColor *pColorTextHighlight, MyColor *pColorBackground, MyColor *pColorFocus, MyDWTextFormat *pTextFormat)
        : pColorStandardButtonDefault(pColorStandardButtonDefault), pColorStandardButtonHover(pColorStandardButtonHover), pColorStandardButtonDown(pColorStandardButtonDown),
          pColorStandardButtonBorderDefault(pColorStandardButtonBorderDefault), pColorStandardButtonBorderHover(pColorStandardButtonBorderHover), pColorStandardButtonBorderDown(pColorStandardButtonBorderDown),
          pColorTextDefault(pColorTextDefault), pColorTextHighlight(pColorTextHighlight), pColorBackground(pColorBackground), pColorFocus(pColorFocus), pTextFormat(pTextFormat) {}

public:
    MyColor *pColorStandardButtonDefault;
    MyColor *pColorStandardButtonHover;
    MyColor *pColorStandardButtonDown;
    MyColor *pColorStandardButtonBorderDefault;
    MyColor *pColorStandardButtonBorderHover;
    MyColor *pColorStandardButtonBorderDown;
    MyColor *pColorTextDefault;
    MyColor *pColorTextHighlight;
    MyColor *pColorBackground;
    MyColor *pColorFocus;
    MyDWTextFormat *pTextFormat;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pColorStandardButtonDefault && this->pColorStandardButtonHover && this->pColorStandardButtonDown &&
                this->pColorStandardButtonBorderDefault && this->pColorStandardButtonBorderHover && this->pColorStandardButtonBorderDown &&
                this->pColorTextDefault && this->pColorTextHighlight && this->pColorBackground && this->pColorFocus && this->pTextFormat);
    }
};
/**
 * @class MyStandardButton
 *
 * @brief This class subclasses a button control and handles its drawing behavior.
 * @brief Standard variant: Rounded corners, border, text. All the buttons using this class share the same characteristics.
 *
 * Usage:
 *
 * 1. Initialize the class:
 *    - Initialize the class using MyStandardButton::initialize() function.
 *
 * 2. Set the shared properties of the class:
 *    - Shared properties are variables that are shared by all instances of the class.
 *    - Set the shared properties for the class using MyStandardButton::setSharedProperties() function.
 *
 * 3. Create and subclass the button control:
 *    - Create the button window.
 *    - Create a MyStandardButton object and perform subclassing using setSubclass() function.
 *    - Each MyStandardButton instance can only handle a single button control.
 */
class MyStandardButton
{
public:
    /**
     * @brief Destructor.
     */
    ~MyStandardButton();

private:
    // Shared attributes (Initialization variables).
    inline static KeyToggle returnKeyToggle = KeyToggle(VK_RETURN);              // ENTER key-down state capture object.
    inline static IUIAnimationManager **ppAnimationManager = nullptr;            // Pointer to the pointer that point to initialized animation manager.
    inline static IUIAnimationTimer **ppAnimationTimer = nullptr;                // Pointer to the pointer that point to initialized animation timer.
    inline static IUIAnimationTransitionLibrary **ppTransitionLibrary = nullptr; // Pointer to the pointer that point to initialized transition library.
    inline static MyD2D1Engine *pMyD2D1Engine = nullptr;                         // Pointer to the initialized MyD2D1Engine object.
    inline static bool isInitialized = false;                                    // Indicate whether the class is initialized.
    inline static bool isSharedAttributesSet = false;                            // Indicate whether the shared attributes are set.
    inline static bool isReady = false;                                          // Indicate whether the class is ready to use.

    // Shared attributes (Drawing variables).
    inline static MyColor *pColorStandardButtonDefault = nullptr;       // Pointer to the standard button default color.
    inline static MyColor *pColorStandardButtonHover = nullptr;         // Pointer to the standard button hover color.
    inline static MyColor *pColorStandardButtonDown = nullptr;          // Pointer to the standard button down color.
    inline static MyColor *pColorStandardButtonBorderDefault = nullptr; // Pointer to the standard button border default color.
    inline static MyColor *pColorStandardButtonBorderHover = nullptr;   // Pointer to the standard button border hover color.
    inline static MyColor *pColorStandardButtonBorderDown = nullptr;    // Pointer to the standard button border down color.
    inline static MyColor *pColorTextDefault = nullptr;                 // Pointer to the standard button text default color.
    inline static MyColor *pColorTextHighlight = nullptr;               // Pointer to the standard button text highlight color.
    inline static MyColor *pColorBackground = nullptr;                  // Pointer to the standard button background color.
    inline static MyColor *pColorFocus = nullptr;                       // Pointer to the standard button focus color.
    inline static MyDWTextFormat *pTextFormat = nullptr;                // Pointer to the standard button text format.

    // Shared attributes (Animation variables).
    inline static const UINT_PTR IDT_ANIMATION_INVALIDATE = 1;   // Animation invalidation timer ID.
    inline static const DOUBLE HOVER_ANIMATION_DURATION = 0.150; // Hover state animation duration.
    inline static const DOUBLE DOWN_ANIMATION_DURATION = 0.100;  // Down state animation duration.
    inline static const DOUBLE FLASH_ANIMATION_DURATION = 0.150; // Flash state animation duration.

    // Shared attributes (Direct2D variables).
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushFocus = nullptr;         // D2D1 solid color brush of the focus color.
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushTextDefault = nullptr;   // D2D1 solid color brush of the text default color.
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushTextHighlight = nullptr; // D2D1 solid color brush of the text highlight color.

    // Non-shared attributes (Animation variables).
    enum class ButtonAnimationState
    {
        Default,
        Hover,
        Down,
        Flash
    } currentAnimationState = ButtonAnimationState::Default;                                  // The current animation state.
    IUIAnimationVariable *pAnimationVariableButtonRGB[3] = {nullptr, nullptr, nullptr};       // The RGB values of the button.
    IUIAnimationVariable *pAnimationVariableButtonBorderRGB[3] = {nullptr, nullptr, nullptr}; // The RGB values of the button border.
    bool isHoverState = false;                                                                // Indicate whether the button is hovered.
    bool isDownState = false;                                                                 // Indicate whether the button is pressed down.

    // Non-shared attributes (Direct2D variables).
    ID2D1DCRenderTarget *pD2D1DCRenderTarget = nullptr; // D2D1 DC render target.

private:
    /// [HELPER FUNCTIONS]

    /**
     * @brief (Re)create the associated D2D1 device resources.
     *
     * @param hWnd                 Handle to the window.
     * @param recreateRenderTarget Specifies whether to recreate the render target.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createD2D1DeviceResources(HWND hWnd, bool recreateRenderTarget);

    /**
     * @brief Create animation variable and set its basic parameters.
     *
     * @param pAnimationVariable Reference to the animation variable pointer.
     * @param initialValue       Specifies the animation variable initial value.
     * @param lowerBound         Specifies the animation variable lower bound value.
     * @param upperBound         Specifies the animation variable upper bound value.
     * @param roundingMode       Specifies the animation variable rounding mode.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode = UI_ANIMATION_ROUNDING_NEAREST);

public:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if successfully installed subclass callback for the window, false otherwise.
     */
    bool installSubclass(HWND hWnd);

    /**
     * @brief Starts the animation for the window.
     *
     * @param hWnd           Handle to the window.
     * @param animationState The window animation state.
     *
     * @return Returns true if the animation started successfully, false otherwise.
     */
    bool startAnimation(HWND hWnd, MyStandardButton::ButtonAnimationState animationState);

    /**
     * @brief Subclass callback function for the window.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyStandardButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    /// [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the class.
     *
     * @param initializeConfig Initialization configuration structure.
     *
     * @return Returns true if successfully initialized the class, false otherwise.
     */
    static bool initialize(MyStandardButtonInitializeConfig initializeConfig);

    /**
     * @brief Set the shared attributes of the class.
     *
     * @param configSharedAttributes Shared attributes configuration structure.
     */
    static bool setSharedAttributes(MyStandardButtonSharedAttributesConfig configSharedAttributes);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Release D2D1 shared resources.
     */
    static void releaseD2D1SharedResources();

    /**
     * @brief Check if the window is subclassed by the class MyStandardButton.
     *
     * @param hWnd       Handle to the window.
     * @param ppSubclass Pointer to the pointer that will receive the subclass instance. (Optional)
     *
     * @return Returns true if the window is subclassed by the class MyStandardButton, false otherwise.
     *
     * @note The returned void pointer must be casted to the correct class type before using.
     */
    static bool isSubclassed(HWND hWnd, void **ppSubclass = nullptr);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Associate the subclass instance to the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if successfully associated the subclass instance to the window, false otherwise.
     */
    bool setSubclass(HWND hWnd);

    /**
     * @bried Refresh the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if the window is successfully refreshed, false otherwise.
     */
    bool refresh(HWND hWnd);
};

/**
 * @struct MyImageButtonInitializeConfig
 *
 * @brief This configuration structure is used for the initialization of the class MyImageButton.
 */
struct MyImageButtonInitializeConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pAnimationManager  Pointer to the initialized animation manager.
     * @param pAnimationTimer    Pointer to the initialized animation timer.
     * @param pTransitionLibrary Pointer to the initialized transition library.
     * @param pMyD2D1Engine      Pointer to the initialized MyD2D1Engine object.
     */
    MyImageButtonInitializeConfig(IUIAnimationManager *&pAnimationManager, IUIAnimationTimer *&pAnimationTimer, IUIAnimationTransitionLibrary *&pTransitionLibrary, MyD2D1Engine *pMyD2D1Engine)
        : pAnimationManager(pAnimationManager), pAnimationTimer(pAnimationTimer), pTransitionLibrary(pTransitionLibrary), pMyD2D1Engine(pMyD2D1Engine) {}

public:
    IUIAnimationManager *&pAnimationManager;
    IUIAnimationTimer *&pAnimationTimer;
    IUIAnimationTransitionLibrary *&pTransitionLibrary;
    MyD2D1Engine *pMyD2D1Engine;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pAnimationManager && this->pAnimationTimer && this->pTransitionLibrary && this->pMyD2D1Engine && this->pMyD2D1Engine->isReady());
    }
};
/**
 * @struct MyImageButtonSharedAttributesConfig
 *
 * @brief This configuration structure is used to set the shared attributes of the class MyImageButton.
 */
struct MyImageButtonSharedAttributesConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pColorFocus Pointer to the image button focus color.
     */
    MyImageButtonSharedAttributesConfig(MyColor *pColorFocus) : pColorFocus(pColorFocus) {}

public:
    MyColor *pColorFocus;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pColorFocus);
    }
};
/**
 * @struct MyImageButtonNonSharedAttributesConfig
 *
 * @brief This configuration structure is used to set the non-shared attributes of the class MyImageButton.
 */
struct MyImageButtonNonSharedAttributesConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pImageDefault           Pointer to the image button default image.
     * @param pImageHover             Pointer to the image button hover image.
     * @param pImageDown              Pointer to the image button down image.
     * @param pColorBackgroundDefault Pointer to the image button default background color.
     * @param pColorBackgroundHover   Pointer to the image button hover background color.
     * @param pColorBackgroundDown    Pointer to the image button down background color.
     * @param imagePosX               The image relative x position.
     * @param imagePosY               The image relative y position.
     * @param imageWidth              The image width.
     * @param imageHeight             The image height.
     * @param centering               Indicate whether to centering the image.
     * @param skipHoverAnimationState Indicate whether to skip the hover state animation.
     * @param skipDownAnimationState  Indicate whether to skip the down state animation.
     */
    MyImageButtonNonSharedAttributesConfig(std::unique_ptr<MyD2D1Image> *pImageDefault, std::unique_ptr<MyD2D1Image> *pImageHover, std::unique_ptr<MyD2D1Image> *pImageDown,
                                           MyColor *pColorBackgroundDefault, MyColor *pColorBackgroundHover, MyColor *pColorBackgroundDown,
                                           FLOAT imagePosX = 0, FLOAT imagePosY = 0, FLOAT imageWidth = 0, FLOAT imageHeight = 0, bool centering = false,
                                           bool skipHoverAnimationState = false, bool skipDownAnimationState = false)
        : pImageDefault(pImageDefault), pImageHover(pImageHover), pImageDown(pImageDown),
          pColorBackgroundDefault(pColorBackgroundDefault), pColorBackgroundHover(pColorBackgroundHover), pColorBackgroundDown(pColorBackgroundDown),
          imagePosX(imagePosX), imagePosY(imagePosY), imageWidth(imageWidth), imageHeight(imageHeight), centering(centering),
          skipHoverAnimationState(skipHoverAnimationState), skipDownAnimationState(skipDownAnimationState) {}

public:
    std::unique_ptr<MyD2D1Image> *pImageDefault;
    std::unique_ptr<MyD2D1Image> *pImageHover;
    std::unique_ptr<MyD2D1Image> *pImageDown;
    MyColor *pColorBackgroundDefault;
    MyColor *pColorBackgroundHover;
    MyColor *pColorBackgroundDown;
    FLOAT imagePosX;
    FLOAT imagePosY;
    FLOAT imageWidth;
    FLOAT imageHeight;
    bool centering;
    bool skipHoverAnimationState;
    bool skipDownAnimationState;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pImageDefault && this->pImageHover && this->pImageDown && this->pColorBackgroundDefault && this->pColorBackgroundHover && this->pColorBackgroundDown);
    }
};
/**
 * @class MyImageButton
 *
 * @brief This class subclasses a button control and handles its drawing behavior.
 * @brief Image variant: No text or border, only image. All the buttons using this class have their own unique characteristics, such as image and background color.
 *
 * Usage:
 *
 * 1. Initialize the class:
 *    - Initialize the class using MyImageButton::initialize() function.
 *
 * 2. Set the shared properties of the class:
 *    - Shared properties are variables that are shared by all instances of the class.
 *    - Set the shared properties for the class using MyImageButton::setSharedProperties() function.
 *
 * 3. Create and subclass the button control:
 *    - Create the button window.
 *    - Prepare a configuration structure MyImageButtonNonSharedAttributesConfig.
 *    - Create a MyImageButton object and perform subclassing using setSubclass() function.
 *    - Each MyImageButton instance can only handle a single button control.
 */
class MyImageButton
{
public:
    /**
     * @brief Destructor.
     */
    ~MyImageButton();

private:
    // Shared attributes (Initialization variables).
    inline static KeyToggle returnKeyToggle = KeyToggle(VK_RETURN);              // ENTER key-down state capture object.
    inline static IUIAnimationManager **ppAnimationManager = nullptr;            // Pointer to the pointer that point to initialized animation manager.
    inline static IUIAnimationTimer **ppAnimationTimer = nullptr;                // Pointer to the pointer that point to initialized animation timer.
    inline static IUIAnimationTransitionLibrary **ppTransitionLibrary = nullptr; // Pointer to the pointer that point to initialized transition library.
    inline static MyD2D1Engine *pMyD2D1Engine = nullptr;                         // Pointer to the initialized MyD2D1Engine object.
    inline static bool isInitialized = false;                                    // Indicate whether the class is initialized.
    inline static bool isSharedPropertiesSet = false;                            // Indicate whether the shared attributes are set.
    inline static bool isReady = false;                                          // Indicate whether the class is ready to use.

    // Shared attributes (Drawing variables).
    inline static MyColor *pColorFocus = nullptr; // Pointer to the image button focus color.

    // Shared attributes (Animation variables).
    inline static const UINT_PTR IDT_ANIMATION_INVALIDATE = 1;   // Animation invalidation timer ID.
    inline static const DOUBLE HOVER_ANIMATION_DURATION = 0.150; // Hover state animation duration.
    inline static const DOUBLE DOWN_ANIMATION_DURATION = 0.100;  // Down state animation duration.
    inline static const DOUBLE FLASH_ANIMATION_DURATION = 0.150; // Flash state animation duration.

    // Shared attributes (Direct2D variables).
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushFocus = nullptr; // D2D1 solid color brush of the focus color.

    // Non-shared attributes (Drawing variables).
    std::unique_ptr<MyD2D1Image> *pImageDefault = nullptr; // Pointer to the image button default image.
    std::unique_ptr<MyD2D1Image> *pImageHover = nullptr;   // Pointer to the image button hover image.
    std::unique_ptr<MyD2D1Image> *pImageDown = nullptr;    // Pointer to the image button down image.
    MyColor *pColorBackgroundDefault = nullptr;            // Pointer to the image button default background color.
    MyColor *pColorBackgroundHover = nullptr;              // Pointer to the image button hover background color.
    MyColor *pColorBackgroundDown = nullptr;               // Pointer to the image button down background color.
    FLOAT imagePosX = 0;                                   // The image relative x position.
    FLOAT imagePosY = 0;                                   // The image relative y position.
    FLOAT imageWidth = 0;                                  // The image width.
    FLOAT imageHeight = 0;                                 // The image height.
    bool isCentering = false;                              // Indicate whether to centering the image.
    bool skipHoverAnimationState = false;                  // Indicate whether to skip the hover state animation.
    bool skipDownAnimationState = false;                   // Indicate whether to skip the down state animation.

    // Non-shared attributes (Animation variables).
    enum class ButtonAnimationState
    {
        Default,
        Hover,
        Down,
        Flash
    } currentAnimationState = ButtonAnimationState::Default;                                // The current animation state.
    IUIAnimationVariable *pAnimationVariableBackgroundRGB[3] = {nullptr, nullptr, nullptr}; // The RGB values of the background.
    IUIAnimationVariable *pAnimationVariableDefaultImageOpacity = nullptr;                  // The opacity value of the default image.
    IUIAnimationVariable *pAnimationVariableHoverImageOpacity = nullptr;                    // The opacity value of the hover image.
    IUIAnimationVariable *pAnimationVariableDownImageOpacity = nullptr;                     // The opacity value of the down image.
    bool isHoverState = false;                                                              // Indicate whether the button is hovered.
    bool isDownState = false;                                                               // Indicate whether the button is pressed down.

    // Non-shared attributes (Direct2D variables).
    ID2D1DCRenderTarget *pD2D1DCRenderTarget = nullptr; // D2D1 DC render target.
    ID2D1Bitmap *pD2D1BitmapDefaultImage = nullptr;     // D2D1 bitmap of the default image.
    ID2D1Bitmap *pD2D1BitmapHoverImage = nullptr;       // D2D1 bitmap of the hover image.
    ID2D1Bitmap *pD2D1BitmapDownImage = nullptr;        // D2D1 bitmap of the down image.

private:
    /// [HELPER FUNCTIONS]

    /**
     * @brief (Re)create the associated D2D1 device resources.
     *
     * @param hWnd                 Handle to the window.
     * @param recreateRenderTarget Specifies whether to recreate the render target.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createD2D1DeviceResources(HWND hWnd, bool recreateRenderTarget);

    /**
     * @brief Create animation variable and set its basic parameters.
     *
     * @param pAnimationVariable Reference to the animation variable pointer.
     * @param initialValue       Specifies the animation variable initial value.
     * @param lowerBound         Specifies the animation variable lower bound value.
     * @param upperBound         Specifies the animation variable upper bound value.
     * @param roundingMode       Specifies the animation variable rounding mode.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode = UI_ANIMATION_ROUNDING_NEAREST);

public:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if successfully installed subclass callback for the window, false otherwise.
     */
    bool installSubclass(HWND hWnd);

    /**
     * @brief Starts the animation for the window.
     *
     * @param hWnd           Handle to the window.
     * @param animationState The window animation state.
     *
     * @return Returns true if the animation started successfully, false otherwise.
     */
    bool startAnimation(HWND hWnd, MyImageButton::ButtonAnimationState animationState);

    /**
     * @brief Subclass callback function for the window.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyImageButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    /// [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the class.
     *
     * @param initializeConfig Initialization configuration structure.
     *
     * @return Returns true if successfully initialized the class, false otherwise.
     */
    static bool initialize(MyImageButtonInitializeConfig initializeConfig);

    /**
     * @brief Set the shared attributes of the class.
     *
     * @param configSharedAttributes Shared attributes configuration structure.
     */
    static bool setSharedAttributes(MyImageButtonSharedAttributesConfig configSharedAttributes);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Release D2D1 shared resources.
     */
    static void releaseD2D1SharedResources();

    /**
     * @brief Check if the window is subclassed by the class MyImageButton.
     *
     * @param hWnd       Handle to the window.
     * @param ppSubclass Pointer to the pointer that will receive the subclass instance. (Optional)
     *
     * @return Returns true if the window is subclassed by the class MyImageButton, false otherwise.
     *
     * @note The returned void pointer must be casted to the correct class type before using.
     */
    static bool isSubclassed(HWND hWnd, void **ppSubclass = nullptr);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Associate the subclass instance to the window.
     *
     * @param hWnd                      Handle to the window.
     * @param configNonSharedAttributes Non-shared properties configuration structure.
     *
     * @return Returns true if successfully associated the subclass instance to the window, false otherwise.
     */
    bool setSubclass(HWND hWnd, MyImageButtonNonSharedAttributesConfig configNonSharedAttributes);

    /**
     * @bried Refresh the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if the window is successfully refreshed, false otherwise.
     */
    bool refresh(HWND hWnd);
};

/**
 * @struct MyRadioButtonInitializeConfig
 *
 * @brief This configuration structure is used for the initialization of the class MyRadioButton.
 */
struct MyRadioButtonInitializeConfig
{
    /**
     * @brief Constructor.
     *
     * @param pAnimationManager  Pointer to the initialized animation manager.
     * @param pAnimationTimer    Pointer to the initialized animation timer.
     * @param pTransitionLibrary Pointer to the initialized transition library.
     * @param pMyD2D1Engine      Pointer to the initialized MyD2D1Engine object.
     */
    MyRadioButtonInitializeConfig(IUIAnimationManager *&pAnimationManager, IUIAnimationTimer *&pAnimationTimer, IUIAnimationTransitionLibrary *&pTransitionLibrary, MyD2D1Engine *pMyD2D1Engine)
        : pAnimationManager(pAnimationManager), pAnimationTimer(pAnimationTimer), pTransitionLibrary(pTransitionLibrary), pMyD2D1Engine(pMyD2D1Engine) {}

public:
    IUIAnimationManager *&pAnimationManager;
    IUIAnimationTimer *&pAnimationTimer;
    IUIAnimationTransitionLibrary *&pTransitionLibrary;
    MyD2D1Engine *pMyD2D1Engine;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pAnimationManager && this->pAnimationTimer && this->pTransitionLibrary && this->pMyD2D1Engine && this->pMyD2D1Engine->isReady());
    }
};
/**
 * @struct MyRadioButtonSharedAttributesConfig
 *
 * @brief This configuration structure is used to set the shared attributes of the class MyRadioButton.
 */
struct MyRadioButtonSharedAttributesConfig
{
public:
    /**
     * @brief Constructor.
     *
     * @param pColorRadioButtonPrimaryDefault           Pointer to the radio button primary default color.
     * @param pColorRadioButtonPrimaryHover             Pointer to the radio button primary hover color.
     * @param pColorRadioButtonPrimaryDown              Pointer to the radio button primary down color.
     * @param pColorRadioButtonSecondaryDefault         Pointer to the radio button secondary default color.
     * @param pColorRadioButtonSecondaryHover           Pointer to the radio button secondary hover color.
     * @param pColorRadioButtonSecondaryDown            Pointer to the radio button secondary down color.
     * @param pColorRadioButtonBorderDefault            Pointer to the radio button border default color.
     * @param pColorRadioButtonBorderHover              Pointer to the radio button border hover color.
     * @param pColorRadioButtonBorderDown               Pointer to the radio button border down color.
     * @param pColorSelectedRadioButtonPrimaryDefault   Pointer to the selected radio button primary default color.
     * @param pColorSelectedRadioButtonPrimaryHover     Pointer to the selected radio button primary hover color.
     * @param pColorSelectedRadioButtonPrimaryDown      Pointer to the selected radio button primary down color.
     * @param pColorSelectedRadioButtonSecondaryDefault Pointer to the selected radio button secondary default color.
     * @param pColorSelectedRadioButtonSecondaryHover   Pointer to the selected radio button secondary hover color.
     * @param pColorSelectedRadioButtonSecondaryDown    Pointer to the selected radio button secondary down color.
     * @param pColorSelectedRadioButtonBorderDefault    Pointer to the selected radio button border default color.
     * @param pColorSelectedRadioButtonBorderHover      Pointer to the selected radio button border hover color.
     * @param pColorSelectedRadioButtonBorderDown       Pointer to the selected radio button border down color.
     * @param pColorTextDefault                         Pointer to the radio button text default color.
     * @param pColorTextHighlight                       Pointer to the radio button text highlight color.
     * @param pColorBackground                          Pointer to the radio button background color.
     * @param pColorFocus                               Pointer to the radio button focus color.
     * @param pTextFormat                               Pointer to the radio button text format.
     */
    MyRadioButtonSharedAttributesConfig(MyColor *pColorRadioButtonPrimaryDefault, MyColor *pColorRadioButtonPrimaryHover, MyColor *pColorRadioButtonPrimaryDown,
                                        MyColor *pColorRadioButtonSecondaryDefault, MyColor *pColorRadioButtonSecondaryHover, MyColor *pColorRadioButtonSecondaryDown,
                                        MyColor *pColorRadioButtonBorderDefault, MyColor *pColorRadioButtonBorderHover, MyColor *pColorRadioButtonBorderDown,
                                        MyColor *pColorSelectedRadioButtonPrimaryDefault, MyColor *pColorSelectedRadioButtonPrimaryHover, MyColor *pColorSelectedRadioButtonPrimaryDown,
                                        MyColor *pColorSelectedRadioButtonSecondaryDefault, MyColor *pColorSelectedRadioButtonSecondaryHover, MyColor *pColorSelectedRadioButtonSecondaryDown,
                                        MyColor *pColorSelectedRadioButtonBorderDefault, MyColor *pColorSelectedRadioButtonBorderHover, MyColor *pColorSelectedRadioButtonBorderDown,
                                        MyColor *pColorTextDefault, MyColor *pColorTextHighlight, MyColor *pColorBackground, MyColor *pColorFocus, MyDWTextFormat *pTextFormat)
        : pColorRadioButtonPrimaryDefault(pColorRadioButtonPrimaryDefault), pColorRadioButtonPrimaryHover(pColorRadioButtonPrimaryHover), pColorRadioButtonPrimaryDown(pColorRadioButtonPrimaryDown),
          pColorRadioButtonSecondaryDefault(pColorRadioButtonSecondaryDefault), pColorRadioButtonSecondaryHover(pColorRadioButtonSecondaryHover), pColorRadioButtonSecondaryDown(pColorRadioButtonSecondaryDown),
          pColorRadioButtonBorderDefault(pColorRadioButtonBorderDefault), pColorRadioButtonBorderHover(pColorRadioButtonBorderHover), pColorRadioButtonBorderDown(pColorRadioButtonBorderDown),
          pColorSelectedRadioButtonPrimaryDefault(pColorSelectedRadioButtonPrimaryDefault), pColorSelectedRadioButtonPrimaryHover(pColorSelectedRadioButtonPrimaryHover), pColorSelectedRadioButtonPrimaryDown(pColorSelectedRadioButtonPrimaryDown),
          pColorSelectedRadioButtonSecondaryDefault(pColorSelectedRadioButtonSecondaryDefault), pColorSelectedRadioButtonSecondaryHover(pColorSelectedRadioButtonSecondaryHover), pColorSelectedRadioButtonSecondaryDown(pColorSelectedRadioButtonSecondaryDown),
          pColorSelectedRadioButtonBorderDefault(pColorSelectedRadioButtonBorderDefault), pColorSelectedRadioButtonBorderHover(pColorSelectedRadioButtonBorderHover), pColorSelectedRadioButtonBorderDown(pColorSelectedRadioButtonBorderDown),
          pColorTextDefault(pColorTextDefault), pColorTextHighlight(pColorTextHighlight), pColorBackground(pColorBackground), pColorFocus(pColorFocus), pTextFormat(pTextFormat) {}

public:
    MyColor *pColorRadioButtonPrimaryDefault;
    MyColor *pColorRadioButtonPrimaryHover;
    MyColor *pColorRadioButtonPrimaryDown;
    MyColor *pColorRadioButtonSecondaryDefault;
    MyColor *pColorRadioButtonSecondaryHover;
    MyColor *pColorRadioButtonSecondaryDown;
    MyColor *pColorRadioButtonBorderDefault;
    MyColor *pColorRadioButtonBorderHover;
    MyColor *pColorRadioButtonBorderDown;
    MyColor *pColorSelectedRadioButtonPrimaryDefault;
    MyColor *pColorSelectedRadioButtonPrimaryHover;
    MyColor *pColorSelectedRadioButtonPrimaryDown;
    MyColor *pColorSelectedRadioButtonSecondaryDefault;
    MyColor *pColorSelectedRadioButtonSecondaryHover;
    MyColor *pColorSelectedRadioButtonSecondaryDown;
    MyColor *pColorSelectedRadioButtonBorderDefault;
    MyColor *pColorSelectedRadioButtonBorderHover;
    MyColor *pColorSelectedRadioButtonBorderDown;
    MyColor *pColorTextDefault;
    MyColor *pColorTextHighlight;
    MyColor *pColorBackground;
    MyColor *pColorFocus;
    MyDWTextFormat *pTextFormat;

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pColorRadioButtonPrimaryDefault && this->pColorRadioButtonPrimaryHover && this->pColorRadioButtonPrimaryDown &&
                this->pColorRadioButtonSecondaryDefault && this->pColorRadioButtonSecondaryHover && this->pColorRadioButtonSecondaryDown &&
                this->pColorRadioButtonBorderDefault && this->pColorRadioButtonBorderHover && this->pColorRadioButtonBorderDown &&
                this->pColorSelectedRadioButtonPrimaryDefault && this->pColorSelectedRadioButtonPrimaryHover && this->pColorSelectedRadioButtonPrimaryDown &&
                this->pColorSelectedRadioButtonSecondaryDefault && this->pColorSelectedRadioButtonSecondaryHover && this->pColorSelectedRadioButtonSecondaryDown &&
                this->pColorSelectedRadioButtonBorderDefault && this->pColorSelectedRadioButtonBorderHover && this->pColorSelectedRadioButtonBorderDown &&
                this->pColorTextDefault && this->pColorTextHighlight &&
                this->pColorBackground && this->pColorFocus && this->pTextFormat);
    }
};
/**
 * @class MyRadioButton
 *
 * @brief This class subclasses a button control and handles its drawing behavior.
 * @brief Radio variant: Classic radio style. All the buttons using this class share the same characteristics.
 *
 * Usage:
 *
 * 1. Initialize the class:
 *    - Initialize the class using MyRadioButton::initialize() function.
 *
 * 2. Set the shared properties of the class:
 *    - Shared properties are variables that are shared by all instances of the class.
 *    - Set the shared properties for the class using MyRadioButton::setSharedProperties() function.
 *
 * 3. Create and subclass the button control:
 *    - Create the button window.
 *    - Create a MyRadioButton object and perform subclassing using setSubclass() function.
 *    - Each MyRadioButton instance can only handle a single button control.
 *
 * 4. Add the button to a radio group:
 *    - After the button is subclassed successfully, add the button to a radio group (class MyRadioGroup).
 *    - The radio group will manage the radio button's selection state.
 */
class MyRadioButton
{
    friend class MyRadioGroup; // Make class MyRadioGroup a friend. Allow it to access the class member directly.
public:
    /**
     * @brief Destructor.
     */
    ~MyRadioButton();

private:
    // Shared attributes (Initialization variables).
    inline static KeyToggle returnKeyToggle = KeyToggle(VK_RETURN);              // ENTER key-down state capture object.
    inline static IUIAnimationManager **ppAnimationManager = nullptr;            // Pointer to the pointer that point to initialized animation manager.
    inline static IUIAnimationTimer **ppAnimationTimer = nullptr;                // Pointer to the pointer that point to initialized animation timer.
    inline static IUIAnimationTransitionLibrary **ppTransitionLibrary = nullptr; // Pointer to the pointer that point to initialized transition library.
    inline static MyD2D1Engine *pMyD2D1Engine = nullptr;                         // Pointer to the initialized MyD2D1Engine object.
    inline static bool isInitialized = false;                                    // Indicate whether the class is initialized.
    inline static bool isSharedAttributesSet = false;                            // Indicate whether the shared attributes are set.
    inline static bool isReady = false;                                          // Indicate whether the class is ready to use.

    // Shared attributes (Drawing variables).
    inline static MyColor *pColorRadioButtonPrimaryDefault = nullptr;           // Pointer to the radio button primary default color.
    inline static MyColor *pColorRadioButtonPrimaryHover = nullptr;             // Pointer to the radio button primary hover color.
    inline static MyColor *pColorRadioButtonPrimaryDown = nullptr;              // Pointer to the radio button primary down color.
    inline static MyColor *pColorRadioButtonSecondaryDefault = nullptr;         // Pointer to the radio button secondary default color.
    inline static MyColor *pColorRadioButtonSecondaryHover = nullptr;           // Pointer to the radio button secondary hover color.
    inline static MyColor *pColorRadioButtonSecondaryDown = nullptr;            // Pointer to the radio button secondary down color.
    inline static MyColor *pColorRadioButtonBorderDefault = nullptr;            // Pointer to the radio button border default color.
    inline static MyColor *pColorRadioButtonBorderHover = nullptr;              // Pointer to the radio button border hover color.
    inline static MyColor *pColorRadioButtonBorderDown = nullptr;               // Pointer to the radio button border down color.
    inline static MyColor *pColorSelectedRadioButtonPrimaryDefault = nullptr;   // Pointer to the selected radio button primary default color.
    inline static MyColor *pColorSelectedRadioButtonPrimaryHover = nullptr;     // Pointer to the selected radio button primary hover color.
    inline static MyColor *pColorSelectedRadioButtonPrimaryDown = nullptr;      // Pointer to the selected radio button primary down color.
    inline static MyColor *pColorSelectedRadioButtonSecondaryDefault = nullptr; // Pointer to the selected radio button secondary default color.
    inline static MyColor *pColorSelectedRadioButtonSecondaryHover = nullptr;   // Pointer to the selected radio button secondary hover color.
    inline static MyColor *pColorSelectedRadioButtonSecondaryDown = nullptr;    // Pointer to the selected radio button secondary down color.
    inline static MyColor *pColorSelectedRadioButtonBorderDefault = nullptr;    // Pointer to the selected radio button border default color.
    inline static MyColor *pColorSelectedRadioButtonBorderHover = nullptr;      // Pointer to the selected radio button border hover color.
    inline static MyColor *pColorSelectedRadioButtonBorderDown = nullptr;       // Pointer to the selected radio button border down color.
    inline static MyColor *pColorTextDefault = nullptr;                         // Pointer to the radio button text default color.
    inline static MyColor *pColorTextHighlight = nullptr;                       // Pointer to the radio button text highlight color.
    inline static MyColor *pColorBackground = nullptr;                          // Pointer to the radio button background color.
    inline static MyColor *pColorFocus = nullptr;                               // Pointer to the radio button focus color.
    inline static MyDWTextFormat *pTextFormat = nullptr;                        // Pointer to the radio button text format.

    // Shared attributes (Animation variables).
    inline static const UINT_PTR IDT_ANIMATION_INVALIDATE = 1;   // Animation invalidation timer ID.
    inline static const DOUBLE HOVER_ANIMATION_DURATION = 0.150; // Hover state animation duration.
    inline static const DOUBLE DOWN_ANIMATION_DURATION = 0.150;  // Down state animation duration.
    inline static const DOUBLE FLASH_ANIMATION_DURATION = 0.150; // Flash state animation duration.

    // Shared attributes (Direct2D variables).
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushFocus = nullptr;         // D2D1 solid color brush of the focus color.
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushTextDefault = nullptr;   // D2D1 solid color brush of the default text color.
    inline static ID2D1SolidColorBrush *pD2D1SolidColorBrushTextHighlight = nullptr; // D2D1 solid color brush of the highlight text color.

    // Non-shared attributes (Animation variables).
    enum ButtonAnimationState
    {
        Default,
        Hover,
        Down,
        Selected,
        SelectedHover,
        SelectedDown,
        SelectedFlash
    } currentAnimationState = ButtonAnimationState::Default;                                     // The current animation state.
    IUIAnimationVariable *pAnimationVariableButtonPrimaryRGB[3] = {nullptr, nullptr, nullptr};   // The RGB values of the button (primary).
    IUIAnimationVariable *pAnimationVariableButtonSecondaryRGB[3] = {nullptr, nullptr, nullptr}; // The RGB values of the button (secondary).
    IUIAnimationVariable *pAnimationVariableButtonBorderRGB[3] = {nullptr, nullptr, nullptr};    // The RGB values of the button border.
    bool isHoverState = false;                                                                   // Indicate whether the button is hovered.
    bool isDownState = false;                                                                    // Indicate whether the button is pressed down.
    bool isSelected = false;                                                                     // Indicate whether the button is selected.

    // Non-shared attributes (Direct2D variables).
    ID2D1DCRenderTarget *pD2D1DCRenderTarget = nullptr; // D2D1 DC render target.

private:
    MyRadioGroup *pRadioGroup = nullptr; // Pointer to the radio group the button belongs to. This value will be set by the radio group when the button is added to the group.

private:
    /// [HELPER FUNCTIONS]

    /**
     * @brief (Re)create the associated D2D1 device resources.
     *
     * @param hWnd                 Handle to the window.
     * @param recreateRenderTarget Specifies whether to recreate the render target.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createD2D1DeviceResources(HWND hWnd, bool recreateRenderTarget);

    /**
     * @brief Create animation variable and set its basic parameters.
     *
     * @param pAnimationVariable Reference to the animation variable pointer.
     * @param initialValue       Specifies the animation variable initial value.
     * @param lowerBound         Specifies the animation variable lower bound value.
     * @param upperBound         Specifies the animation variable upper bound value.
     * @param roundingMode       Specifies the animation variable rounding mode.
     *
     * @return Returns true if all the operations are successfully performed, false otherwise.
     */
    bool createAnimationVariable(IUIAnimationVariable *&pAnimationVariable, DOUBLE initialValue, DOUBLE lowerBound, DOUBLE upperBound, UI_ANIMATION_ROUNDING_MODE roundingMode = UI_ANIMATION_ROUNDING_NEAREST);

public:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if successfully installed subclass callback for the window, false otherwise.
     */
    bool installSubclass(HWND hWnd);

    /**
     * @brief Starts the animation for the window.
     *
     * @param hWnd           Handle to the window.
     * @param animationState The button animation state.
     *
     * @return Returns true if the animation started successfully, false otherwise.
     */
    bool startAnimation(HWND hWnd, MyRadioButton::ButtonAnimationState animationState);

    /**
     * @brief Subclass callback function for the window.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyRadioButton(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    /// [UN/INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the class.
     *
     * @param initializeConfig Initialization configuration structure.
     *
     * @return Returns true if successfully initialized the class, false otherwise.
     */
    static bool initialize(MyRadioButtonInitializeConfig initializeConfig);

    /**
     * @brief Set the shared attributes of the class.
     *
     * @param configSharedAttributes Shared attributes configuration structure.
     */
    static bool setSharedAttributes(MyRadioButtonSharedAttributesConfig configSharedAttributes);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Release D2D1 shared resources.
     */
    static void releaseD2D1SharedResources();

    /**
     * @brief Check if the window is subclassed by the class MyRadioButton.
     *
     * @param hWnd       Handle to the window.
     * @param ppSubclass Pointer to the pointer that will receive the subclass instance. (Optional)
     *
     * @return Returns true if the window is subclassed by the class MyRadioButton, false otherwise.
     *
     * @note The returned void pointer must be casted to the correct class type before using.
     */
    static bool isSubclassed(HWND hWnd, void **ppSubclass = nullptr);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Associate the subclass instance to the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if successfully associated the subclass instance to the window, false otherwise.
     */
    bool setSubclass(HWND hWnd);

    /**
     * @brief Update the button selection state.
     *
     * @param hWnd  Handle to the window.
     * @param state Specifies the new button selection state. (0 - deselected | 1 - selected)
     */
    void updateSelectionState(HWND hWnd, bool state);

    /**
     * @bried Refresh the window.
     *
     * @param hWnd Handle to the window.
     *
     * @return Returns true if the window is successfully refreshed, false otherwise.
     */
    bool refresh(HWND hWnd);
};

/**
 * @class MyRadioGroup
 *
 * @brief Represents a radio group and manages the selection state of the radio buttons.
 *
 * Usage:
 *
 * 1. Add radio buttons to the group:
 *    - To add radio buttons to the group, use the addRadioButton() function.
 *
 * 2. Use the radio group:
 *    - To get current radio group state, use the getRadioState() function.
 *      (The radio group's state indicates which button in the group is selected.)
 *    - The radio group state updates when a button from its group is clicked,
 *      but the radio group state can be updated programmatically using the updateRadioState() function.
 */
class MyRadioGroup
{
private:
    /**
     * @brief Map associating radio button IDs with pointers to subclass instances (class MyRadioButton).
     *        Key: The ID of the radio button.
     *        Value: Pointer to the subclass instance that subclasses the button (class MyRadioButton).
     */
    std::map<INT, std::pair<HWND, MyRadioButton *>> mapButtons;
    INT currentSelectedButtonID = 0; // This indicates which button in the radio group is selected.

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Add a radio button to the radio group.
     * @note The radio button window must be a managed window and initialized.
     *
     * @param pWindow Pointer to the radio button window (MyWindow).
     *
     * @return Returns true if the button is successfully added to the radio group, false otherwise.
     */
    bool addRadioButton(MyWindow *pWindow);

    /**
     * @brief Get the current state of the radio group.
     *
     * @return Returns the ID of the currently selected button in the group.
     */
    INT getRadioState();

    /**
     * @brief Update the state of the radio group.
     *
     * @param buttonID ID of the button that will be selected.
     *
     * @return Returns true if the radio group state is successfully updated, false otherwise.
     */
    bool updateRadioState(INT buttonID);
};

/**
 * @struct MyEditInitializeConfig
 *
 * @brief This configuration structure is used for the initialization of the class MyEdit.
 */
struct MyEditInitializeConfig
{
    KeyToggle *pEnterKeyToggle = nullptr; // Pointer to the ENTER key-down state capture object.
    HWND *pMainWindowHandle = nullptr;    // Pointer to the main window handle.

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pEnterKeyToggle && this->pMainWindowHandle);
    }
};
/**
 * @struct MyEditNonSharedPropertiesConfig
 *
 * @brief This configuration structure is used to set the non-shared properties of the class MyEdit.
 */
struct MyEditNonSharedPropertiesConfig
{
    INT hWndStaticID = 0;                 // The ID of the editbox-associated static window.
    INT associatedWMCommandMessageID = 0; // The ID of the WM_COMMAND that the editbox triggers when the enter key is pressed.
    bool skipTabStop = false;             // Specifies whether to skip the tab stop event.

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->hWndStaticID);
    }
};
/**
 * @class MyEdit
 *
 * @brief This class subclasses a editbox control and handles its behavior.
 *
 * Usage:
 *
 * 1. Initialize the class:
 *    - Initialize the class using MyEdit::initialize() function.
 *
 * 2. Create the static window control:
 *    - Prepare a configuration structure MyEditNonSharedPropertiesConfig.
 *    - Create a MyEdit object and create the static window using the hWndStatic class member.
 *    - The static window will be destroyed automatically along with the editbox.
 *
 * 3. Create and subclass the editbox control:
 *    - Create the editbox window and perform subclassing using setSubclass() function.
 *    - Note that for the editbox's border to draw correctly, the editbox positions and dimensions should be:
 *      X: Static window pos x + 2
 *      Y: Static window pos y + 2
 *      W: Static window width - 4
 *      H: Static window height - 4
 *    - Each MyEdit instance can only handle a single editbox control.
 */
class MyEdit
{
private:
    // Initialization variables.
    inline static KeyToggle *pEnterKeyToggle = nullptr; // Pointer to the ENTER key-down state capture object.
    inline static HWND *pMainWindowHandle = nullptr;    // Pointer to the main window handle.
    inline static bool isInitialized = false;           // Indicate whether the class is initialized.
    inline static bool isReady = false;                 // Indicate whether the class is ready to use.

    // Non-shared properties.
    HWND hWndStatic = nullptr;            // Handle to the editbox-associated static window. (This static window is responsible for border drawing)
    HWND hWndEdit = nullptr;              // Handle to the editbox window.
    INT associatedWMCommandMessageID = 0; // The ID of the WM_COMMAND that the editbox triggers when the enter key is pressed.
    bool skipTabStop = false;             // Indicates whether to skip the tab stop event.

private:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the editbox.
     *
     * @param hWnd Handle to the editbox.
     *
     * @return Returns true if successfully installed subclass callback for the editbox, false otherwise.
     */
    bool installEditSubclass(HWND hWnd);

    /**
     * @brief Install subclass callback for the editbox-associated static window.
     *
     * @param hWnd Handle to the editbox-associated static window.
     *
     * @return Returns true if successfully installed subclass callback for the editbox, false otherwise.
     */
    bool installStaticSubclass(HWND hWnd);

private:
    /// [SUBCLASS CALLBACK FUNCTIONS]

    /**
     * @brief Subclass callback function for the editbox.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyEdit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    /**
     * @brief Subclass callback function for the editbox.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyEditStatic(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    /// [INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the class.
     *
     * @param initializeConfig Initialization configuration structure.
     *
     * @return Returns true if successfully initialized the class, false otherwise.
     */
    static bool initialize(MyEditInitializeConfig initializeConfig);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the window is subclassed by the class MyEdit.
     *
     * @param hWnd           Handle to the window.
     * @param isStaticHandle Specifies whether the window handle is the editbox-associated static window handle.
     * @param ppSubclass     Pointer to the pointer that will receive the subclass instance. (Optional)
     *
     * @return Returns true if the window is subclassed by the class MyEdit, false otherwise.
     *
     * @note The returned void pointer must be casted to the correct class type before using.
     */
    static bool isSubclassed(HWND hWnd, bool isStaticHandle, void **ppSubclass = nullptr);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Get the editbox-associated static window handle.
     *
     * @return Returns the editbox-associated static window handle.
     */
    HWND getStaticHandle();

    /**
     * @brief Get the editbox-associated static window handle (Reference).
     *
     * @return Returns the editbox-associated static window handle (Reference).
     */
    HWND &getStaticHandleRef();

    /**
     * @brief Install subclass callback for the editbox.
     *
     * @param hWnd Handle to the editbox.
     *
     * @return Returns true if successfully installed subclass callback for the editbox, false otherwise.
     */
    bool setSubclass(HWND hWnd, MyEditNonSharedPropertiesConfig nonSharedPropertiesConfig);
};

/**
 * @struct MyComboboxSharedPropertiesConfig
 *
 * @brief This configuration structure is used to set the shared properties of the class MyDDLCombobox.
 */
struct MyDDLComboboxSharedPropertiesConfig
{
    MyColor *pColorDDLCombobox = nullptr;                       // Pointer to the drop-down list combobox color.
    MyColor *pColorDDLComboboxBorder = nullptr;                 // Pointer to the drop-down list combobox border color.
    MyColor *pColorDDLComboboxItemBackground = nullptr;         // Pointer to the drop-down list combobox item default state background color color.
    MyColor *pColorDDLComboboxSelectedItemBackground = nullptr; // Pointer to the drop-down list combobox item selected state background color color.
    MyColor *pColorDDLComboboxDropdownlistBorder = nullptr;     // Pointer to the drop-down list combobox item drop down list border color.
    MyColor *pColorText = nullptr;                              // Pointer to the drop-down list combobox text color.
    MyColor *pColorBackground = nullptr;                        // Pointer to the drop-down list combobox background color.
    MyColor *pColorFocus = nullptr;                             // Pointer to the drop-down list combobox focus color.
    MyFont *pFontDefault = nullptr;                             // Pointer to the drop-down list combobox font.

    /**
     * @brief Check if the configuration structure is valid.
     *
     * @return Returns true if the configuration structure is valid, false otherwise.
     */
    bool isValid()
    {
        return (this->pColorDDLCombobox && this->pColorDDLComboboxBorder && this->pColorDDLComboboxItemBackground &&
                this->pColorDDLComboboxSelectedItemBackground && this->pColorDDLComboboxDropdownlistBorder &&
                this->pColorText && this->pColorBackground && this->pColorFocus && this->pFontDefault);
    }
};
/**
 * @class MyDDLCombobox
 *
 * @brief This class subclasses a drop-down list combobox and handles its drawing behavior.
 *
 * Usage:
 *
 * 1. Set the shared properties of the class:
 *    - Shared properties are variables that are shared by all instances of the class.
 *    - Set the shared properties for the class using MyDDLCombobox::setSharedProperties() function.
 *
 * 2. Create and subclass the drop-down list combobox control:
 *    - Create the drop-down list combobox window.
 *    - Create a MyDDLCombobox object and perform subclassing using setSubclass() function.
 *    - Each MyDDLCombobox instance can only handle a single drop-down list combobox control.
 */
class MyDDLCombobox
{
private:
    // Initialization variables.
    inline static bool isSharedPropertiesSet = false; // Indicate whether the shared attributes are set.
    inline static bool isReady = false;               // Indicate whether the class is ready to use.

    // Shared properties.
    inline static MyColor *pColorDDLCombobox = nullptr;                       // Pointer to the drop-down list combobox color.
    inline static MyColor *pColorDDLComboboxBorder = nullptr;                 // Pointer to the drop-down list combobox border color.
    inline static MyColor *pColorDDLComboboxItemBackground = nullptr;         // Pointer to the drop-down list combobox item default state background color color.
    inline static MyColor *pColorDDLComboboxSelectedItemBackground = nullptr; // Pointer to the drop-down list combobox item selected state background color color.
    inline static MyColor *pColorDDLComboboxDropdownlistBorder = nullptr;     // Pointer to the drop-down list combobox item drop down list border color.
    inline static MyColor *pColorText = nullptr;                              // Pointer to the drop-down list combobox text color.
    inline static MyColor *pColorBackground = nullptr;                        // Pointer to the drop-down list combobox background color.
    inline static MyColor *pColorFocus = nullptr;                             // Pointer to the drop-down list combobox focus color.
    inline static MyFont *pFontDefault = nullptr;                             // Pointer to the drop-down list combobox font.

    // Non-shared properties.
    HWND hWndDDLCombobox = nullptr;             // Handle to the drop-down list combobox window.
    HWND hWndDDLComboboxDropdownlist = nullptr; // Handle to the drop-down list combobox drop-down list window.

private:
    /// [INTERNAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the DDL combobox.
     *
     * @param hWnd Handle to the DDL combobox.
     *
     * @return Returns true if successfully installed subclass callback for the DDL combobox, false otherwise.
     */
    bool installSubclassDDLCombobox(HWND hWnd);

    /**
     * @brief Install subclass callback for the drop-down list.
     *
     * @param hWnd Handle to the drop-down list.
     *
     * @return Returns true if successfully installed subclass callback for the drop-down list, false otherwise.
     */
    bool installSubclassDropdownlist(HWND hWnd);

private:
    /// [SUBCLASS CALLBACK FUNCTIONS]

    /**
     * @brief Subclass callback function for the DDL combobox.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyDDLCombobox(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    /**
     * @brief Subclass callback function for the combobox drop-down list window.
     *
     * @param hWnd        The handle to the window.
     * @param uMsg        The message identifier.
     * @param wParam      The first message parameter.
     * @param lParam      The second message parameter.
     * @param uIdSubclass The subclass ID.
     * @param dwRefData   DWORD_PTR to reference data.
     */
    static LRESULT CALLBACK scMyDDLComboboxDropdownlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

public:
    /// [INITIALIZATION FUNCTIONS]

    /**
     * @brief Set the shared properties of the class.
     *
     * @param configSharedProperties Shared properties configuration structure.
     */
    static bool setSharedProperties(MyDDLComboboxSharedPropertiesConfig configSharedProperties);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Check if the window is subclassed by the class MyDDLCombobox.
     *
     * @param hWnd       Handle to the window.
     * @param ppSubclass Pointer to the pointer that will receive the subclass instance. (Optional)
     *
     * @return Returns true if the window is subclassed by the class MyDDLCombobox, false otherwise.
     *
     * @note The returned void pointer must be casted to the correct class type before using.
     */
    static bool isSubclassed(HWND hWnd, void **ppSubclass = nullptr);

public:
    /// [GENERAL FUNCTIONS]

    /**
     * @brief Install subclass callback for the DDL combobox.
     *
     * @param hWnd Handle to the DDL combobox.
     *
     * @return Returns true if successfully installed subclass callback for the DDL combobox, false otherwise.
     */
    bool setSubclass(HWND hWnd, INT comboboxHeight);
};

#endif // SUBCLASSES_H