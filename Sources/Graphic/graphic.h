/**
 * @file graphic.h
 * @brief Header file containing classes that related to graphic engine.
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

/**
 * @brief My Direct2D engine class (Singleton).
 */
class MyD2D1Engine
{
public:
    /**
     * @brief Destructor.
     */
    ~MyD2D1Engine();

private:
    bool isInitialized = false; // The initialization status of the D2D1 engine.

public:
    ID2D1Factory *pFactory = nullptr; // The Direct2D factory.

public:
    // [INITIALIZATION FUNCTIONS]

    /**
     * @brief Initialize the D2D1 engine.
     *
     * @return Returns true if successfully initialized the D2D1 engine, otherwise false.
     */
    bool initialize();

    /**
     * @brief Uninitialize the D2D1 engine.
     *
     * @return Returns true if successfully uninitialized the D2D1 engine, otherwise false.
     */
    bool uninitialize();

public:
    // [GENERAL FUNCTIONS]

    /**
     * @brief Check if the D2D1 engine is ready to use.
     */
    bool isReady() const;

    /**
     * @brief Create a Direct2D device context render target.
     *
     * @param hDC                 The handle to the GDI device context.
     * @param rect                The dimensions of the GDI device context.
     * @param disableRTLRendering Disable the right-to-left rendering. (Avoids mirrored rendering)
     *
     * @return Returns the pointer to the Direct2D device context render target, otherwise nullptr.
     */
    ID2D1DCRenderTarget *createDCRenderTarget(HDC &hDC, RECT &rect, bool disableRTLRendering = true);
};

#endif // GRAPHIC_H