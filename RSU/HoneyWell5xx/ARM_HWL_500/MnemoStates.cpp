#include "StdAfx.h"
#include "MnemoStates.h"
#include <commproc.h>
#include <shdata.h>

//CMnemoStates::CMnemoStates(char *OperName, KHoneyPipeClient &mmPipe) :
//mPipe(mmPipe)
CMnemoStates::CMnemoStates(char *OperName) :
mPipe("MnemoStates")
,m_OperName(OperName)
,m_countManemoData(0)
{
    //ищем имена мнемосхем
    memset(m_ManemoData, 0, sizeof m_ManemoData);
    DWORD dwSize;
    char fullname[MAX_PATH];
    sprintf_s(fullname, "%s%s.scn", GetDirScript(), OperName);
    char* szBuffer1 = ::FileToBuff( fullname, dwSize );
    if ( szBuffer1 )
      {
      char *ref1=szBuffer1;
      while(ref1=strstr(ref1, "<include="))
        {
         ref1 += sizeof("<include=")-1;
         if( strncmp(ref1, "\"Клавиатура HoneywellBase\"", sizeof("\"Клавиатура HoneywellBase\"")-1) 
             &&
             (strncmp(ref1, "\"Клавиатура", sizeof("\"Клавиатура")-1) == 0)
           )
          {
          char filename[MAX_PATH/2];
          char *ref2 = strchr(ref1+2,'"');
          if (ref2 && ((ref2-ref1) < sizeof filename-1))
            {
             *ref2=0;
             strcpy_s(filename, ref1+1);
             sprintf_s(fullname, "%s%s.scn", GetDirScript(), filename);
             char* szBuffer2 = ::FileToBuff( fullname, dwSize );
             ref1= szBuffer2;
             while(ref1=strstr(ref1, "\xA<Q "))
              {
               do{
               ref1++;
               } while(*ref1==' ');
               ref2 = strchr(ref1, '\xA');
               if(ref2)
               {
               *ref2=0;
               char *ref4;
               if( (ref4=strstr(ref1, "p4=\"")) && strstr(ref1, "p7=\"") && (ref1=strstr(ref1, "par1=")) && strstr(ref1, "par3=") )
                 {
                  int ind = atoi(ref1+sizeof("par1=")-1);
                  if((ind>=0) && (ind < int(_countof(m_ManemoData))))
                  {
                  ref4 += sizeof("p4=\"")-1;
                  char *ref3 = strchr(ref4, '"');
                  if(ref3<ref1)
                    {
                     char mnemoname[MAX_PATH/2];
                     strncpy_s(mnemoname, _countof(mnemoname)-1, ref4, ref3-ref4);
                     mnemoname[ref1-ref4]=0;
                     m_ManemoData[ind].mnemoState = 0;
                     m_ManemoData[ind].mnemoName = mnemoname;
                     m_countManemoData++;
                    }
                  }
                 }
               *ref2='\xA';
               ref1=ref2;
               }
               else break;
              }
             ::BuffFree( szBuffer2 );
            }
          break;
          }
        }
      ::BuffFree( szBuffer1 );
      }
    //
    for(int ii=0; ii< _countof(m_ManemoData); ii++)
     {
        if(m_ManemoData[ii].mnemoName.size())
          m_ManemoData[ii].m_nNumber = mPipe.AddMnemo(m_ManemoData[ii].mnemoName.c_str());	
     }
    mPipe.End();
}

CMnemoStates::~CMnemoStates(void)
{
}

void CMnemoStates::OnTimer(void)
{
    mPipe.Refresh();

    for(int ii=0; ii< _countof(m_ManemoData); ii++)
     {
        if(m_ManemoData[ii].mnemoName.size() && (m_ManemoData[ii].m_nNumber!=-1))
          {
	      CShOneMnemoHoney* sh = (CShOneMnemoHoney*)mPipe.Sh(m_ManemoData[ii].m_nNumber);
		  if (sh->RetCode==0)
            {
			KKK();
            SAlarmsOnMnemo2 strAlarms;
            ZeroMemory(&strAlarms, sizeof(strAlarms));
			
            for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
			if (sh->count1[i]!=0)
				{
					strAlarms.bBlueNotAck=true;
					break;
				}
			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
				if (sh->count2[i]!=0)
				{
					strAlarms.bRedNotAck=true;
					break;
				}
			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
				if (sh->count3[i]!=0)
				{
					strAlarms.bYellowNotAck=true;
					break;
				}
			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
				if (sh->state1[i])
				{
					strAlarms.bBlueBlink=true;
					break;
				}
			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
				if (sh->state2[i])
				{
					strAlarms.bRedBlink=true;
					break;
				}
			for (int i=0;i<COneMnemoHoney::COUNT_UNIT;i++)
				if (sh->state3[i])
				{
					strAlarms.bYellowBlink=true;
					break;
				}
            
            m_ManemoData[ii].mnemoState = 0;
            if( strAlarms.bRedBlink )
               m_ManemoData[ii].mnemoState |= 0x01;
            if( strAlarms.bRedNotAck )
               m_ManemoData[ii].mnemoState |= 0x02;
            if( strAlarms.bYellowBlink )
               m_ManemoData[ii].mnemoState |= 0x04;
            if( strAlarms.bYellowNotAck)
               m_ManemoData[ii].mnemoState |= 0x08;
            }
        }
     }
}