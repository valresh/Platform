#ifndef GLOBALAPI_H
#define GLOBALAPI_H

namespace cross {

//GDI FUNCTION
//WINGDIAPI HBRUSH  WINAPI CreateSolidBrush( __in COLORREF color);
// HBRUSH brush = ::CreateSolidBrush(back);\


// ::FillRect(hDC, &txtrc, brush);
// FillRect(
//     __in HDC hDC,
//     __in CONST RECT *lprc,
//     __in HBRUSH hbr);


//WINGDIAPI BOOL WINAPI DeleteObject( __in HGDIOBJ ho);
// ::DeleteObject(brush);



// ::DrawText( hDC, tmptext, lstrlen(tmptext), &rc, align|DT_NOPREFIX );
//    ::SetRectEmpty( &rect );





// WINUSERAPI
//     int
//         WINAPI
//         DrawTextW(
//             __in HDC hdc,
//             __inout_ecount(cchText) LPCWSTR lpchText,
//             __in int cchText,
//             __inout LPRECT lprc,
//             __in UINT format);
// ::DrawTextW( hDC, tmptext, lstrlen(tmptext), &rc, align|DT_NOPREFIX );


//    ::SetRectEmpty( &rect );

}

#endif // GLOBALAPI_H
