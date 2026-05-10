#include "Api.h"
#include <stdlib.h>

#include <vector>
#include <type_traits>
#include <cmath>
//#include <algorithm>
// #include "HTML/tag.h"
// #include "HTML/HTML.h"
// #include "HTML/Dsd.h"
// #include "HTML/Bin.h"
// #include "HTML/HTML.h"
// #include "ViewMnemo.h"

//#include <winnt.h>//LPCTSTR
// Global storage for the callback


// Example data to render (could be replaced with your actual rendering logic)
static std::vector<std::tuple<int, int, int, int, uint32_t>> s_linesToDraw;


HVIEWMNEMO API CreateViewMnemo(const char* htmlFileContent) {

    // // //get div tag
    // auto mnemo = new CViewMnemo();
    // auto tag = new CTag();


    // CDsd dsd;
    // CBin bin(dsd);
    // CHTML html;

    //  htmlFileContent;

    //  char* ptr =  const_cast<char*>(htmlFileContent);
    // //Parser( CHTML& html, char* szBuffer, char*& ptr, CBin* bin, SCRIPT_SOURSE_DATA *pSriptData, CTag **pTag )

    //  int  a = 0;

    // auto  bReturn = tag->Parser( html,  const_cast<char*> (htmlFileContent), ptr, &bin );


        void* ptr;
    //    std::string str(htmlFileContent);
    //    mnemo->Build(htmlFileContent);






    return ptr;
}

//void API Render(HVIEWMNEMO hViewMnemo, DrawLineDelegate drawLine)
//{
//    // Пример — рисуем крест
//    drawLine(10, 10, 100, 100, 0xFFFF0000); // красная линия
//    drawLine(100, 10, 10, 100, 0xFFFF0000);
//
//}


// void API Render(HVIEWMNEMO hViewMnemo, IRenderContext renderContext) {
//     CViewMnemo* mnem = reinterpret_cast<CViewMnemo*> (hViewMnemo);
//    // mnem->Draw(renderContext); // parametru
// }


void API OnMouseDown(HVIEWMNEMO hViewMnemo) {

    // auto mnem = reinterpret_cast<CViewMnemo*> (hViewMnemo);
    // //	mnem.lbuttonDown();
}



// Render a frame (called from C#)
void API RenderFrame(int width, int height) {



    

    if (!m_RenderCallbacks._DrawLineCallback) return;


    // RECT rect = style.FullRect();
    // CMyBrush brush( hDC, 0x555555 );
    // brush.Rect( rect );
    // CMyFont font( hDC, style.m_hFont, 0xffffff );
    // ::DrawText( hDC, "OI", 2, &rect, DT_CENTER | DT_VCENTER );


    // Example: Draw a border around the canvas
    m_RenderCallbacks._DrawLineCallback(0, 0, width, 0, 0xFFFFFFFF);         // White top border
    m_RenderCallbacks._DrawLineCallback(width, 0, width, height, 0xFFFFFFFF); // Right border
    m_RenderCallbacks._DrawLineCallback(0, height, width, height, 0xFFFFFFFF); // Bottom border
    m_RenderCallbacks._DrawLineCallback(0, 0, 0, height, 0xFFFFFFFF);         // Left border

    // Draw all stored lines
    for (const auto& line : s_linesToDraw) {
        m_RenderCallbacks._DrawLineCallback(
            std::get<0>(line), // x1
            std::get<1>(line), // y1
            std::get<2>(line), // x2
            std::get<3>(line), // y2
            std::get<4>(line)  // color (ARGB)
            );
    }
    
    // Example: Draw a rotating line (dynamic rendering)
    static float angle = 0.0f;
    int cx = width / 2;
    int cy = height / 2;

    using namespace std;

    int length = min(width, height) / 3;
    int endX = cx + length * std::cos(angle);
    int endY = cy + length * std::sin(angle);
    m_RenderCallbacks._DrawLineCallback(cx, cy, endX, endY, 0xFF0000FF); // Blue rotating line
    angle += 0.05f;
}



void InitializeCallbacks(RenderCallbacks _renderCallbacks)
{
    m_RenderCallbacks = _renderCallbacks;
}

void API InitializeFilesCallbacks(GraphisFilesCallbacks _graphisFilesCallback)
{
    m_GraphisFilesCallback = _graphisFilesCallback;
}


void API TestRenderDIV()
{
     //RECT rect = style.FullRect();
    WrapperRectF rect;

    rect.X = 10;
    rect.Y = 10;
    rect.height = 100;
    rect.width = 100;

    s_linesToDraw.push_back({ 10, 10, 100, 100, 0xFFFF0000 }); // Red line
    s_linesToDraw.push_back({ 100, 10, 10, 100, 0xFF00FF00 }); // Green line

    if (!m_RenderCallbacks._DrawLineCallback) return;


    // RECT rect = style.FullRect();
    // CMyBrush brush( hDC, 0x555555 );
    // brush.Rect( rect );
    // CMyFont font( hDC, style.m_hFont, 0xffffff );
    // ::DrawText( hDC, "OI", 2, &rect, DT_CENTER | DT_VCENTER );


    // Example: Draw a border around the canvas
    m_RenderCallbacks._DrawLineCallback(0, 0, 200, 0, 0xFFFFFFFF);         // White top border
    m_RenderCallbacks._DrawLineCallback(200, 0, 200, 200, 0xFFFFFFFF); // Right border
    m_RenderCallbacks._DrawLineCallback(0, 200, 200, 200, 0xFF0000FF); // Bottom border
    m_RenderCallbacks._DrawLineCallback(0, 0, 0, 200, 0xFFFFFFFF);         // Left border

    // Draw all stored lines
    for (const auto& line : s_linesToDraw) {
        m_RenderCallbacks._DrawLineCallback(
            std::get<0>(line), // x1
            std::get<1>(line), // y1
            std::get<2>(line), // x2
            std::get<3>(line), // y2
            std::get<4>(line)  // color (ARGB)
        );
    }

    // Example: Draw a rotating line (dynamic rendering)
    static float angle = 0.0f;
    int cx = 200 / 2;
    int cy = 200 / 2;

    using namespace std;

    int length = min(200, 200) / 3;
    int endX = cx + length * std::cos(angle);
    int endY = cy + length * std::sin(angle);
    m_RenderCallbacks._DrawLineCallback(cx, cy, endX, endY, 0xFF0000FF); // Blue rotating line
    angle += 0.05f;
}
