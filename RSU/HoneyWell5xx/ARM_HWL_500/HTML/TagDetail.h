// #pragma once
// #include "tag.h"

// #include "../Native/UniPolsky.h"
// //
// struct SItemWindow : public SItemValue
//   {
//   int m_nDot;
//   int m_nEdit;
//   // 0 - разрешено менять параметр
//   // 1 - запрещено менять параметр
//   // 2 - ошибка при попытке изменить
//   //
//   char name [32];
//   char value[32];
//   char formt[16];
//   bool m_bLefts;
//   bool m_bAlarm;
//   bool m_bCombo;
//   SValueDef* mPr;
//   //
//   void PaintItem( KHoneyPipeClient& pipe, CMyFont& font );
//   bool FillItem( KHoneyPipeClient& pipe, int nTag, const char* alias );
//   };
// //
// class CTagDetail : public CTag, public SUniBuffer
//   {
//   SUniPolsky mPol;
//   public:
//     CTagDetail(void);
//     void Matrix( LOCALS );
//     void Analiz( ANALIZ );
//     void Kerdyk( KERDYK );
//   };
