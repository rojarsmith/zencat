/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <simulator/mainBase.hpp>
#include <platform/hal/simulator/sdl2/HALSDL2.hpp>
#include <common/TouchGFXInit.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>
#include <simulator/video/DirectFrameBufferVideoController.hpp>
#include <simulator/video/SoftwareMJPEGDecoder.hpp>
#include <videos/VideoDatabase.hpp>
#include <touchgfx/widgets/canvas/CWRVectorRenderer.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/VectorFontRendererImpl.hpp>
#include <string.h>

#ifdef __GNUC__
#define fopen_s(pFile, filename, mode) (((*(pFile)) = fopen((filename), (mode))) == NULL)
#endif
touchgfx::LCD24bpp lcd;

uint32_t lineBuffer[10000];
SoftwareMJPEGDecoder *mjpegDecoders[1];
DirectFrameBufferVideoController<1, Bitmap::RGB888> controller;

VideoController& VideoController::getInstance()
{
    return controller;
}

void setupVideoDecoder(touchgfx::HAL& hal)
{
    for(int i = 0; i < 1; i++)
    {
        mjpegDecoders[i] = new SoftwareMJPEGDecoder((uint8_t*)lineBuffer);
        controller.addDecoder(*mjpegDecoders[i], i);
    }
}

void setupVideo(const char* videoFileName, const uint8_t** videoBuffer, uint32_t videoLength)
{
    FILE *file = 0;
    fopen_s(&file, videoFileName, "rb");
    assert(file && "Unable to setupVideo.");
    *videoBuffer = (uint8_t*) malloc(videoLength + 1);
    fread(const_cast<uint8_t*>(*videoBuffer), 1, videoLength, file);
    fclose(file);
}

namespace touchgfx
{
    VectorRenderer* VectorRenderer::getInstance()
    {
        static CWRVectorRendererRGB888 renderer;
        return &renderer;
    }
}

VectorFontRendererImpl vectorFontRendererImpl;

void setupSimulator(int argc, char** argv, touchgfx::HAL& hal)
{
    // Simulate hardware running at 60Hz generating a vsync every 16.6667 ms
    static_cast<touchgfx::HALSDL2&>(hal).setVsyncInterval(16.6667f);
    static_cast<touchgfx::HALSDL2&>(hal).setWindowTitle("MyApplication");

    // Initialize SDL
    bool sdl_init_result = static_cast<touchgfx::HALSDL2&>(hal).sdl_init(argc, argv);
    assert(sdl_init_result && "Error during SDL initialization");
    HAL::lcd().setVectorFontRenderer(&vectorFontRendererImpl);
}

touchgfx::LCD& setupLCD()
{
    return lcd;
}
