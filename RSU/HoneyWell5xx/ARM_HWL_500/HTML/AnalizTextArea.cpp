// #include "StdAfx.h"
// #include "HTML.h"
// #include "Tag.h"
// //
// #undef  STD_VAR
// #define STD_VAR CHTML& html, CTag& a, CStyle& style, char* value
// typedef void (*LPFuncs)( STD_VAR );
// //
// void textClass( STD_VAR )
//   {
//       //класс без точки - это класс из css
//       char *pp1 = strchr(value, ' ');
//       while(pp1)
//         {
//           char *pp2 = strchr(++pp1, ' ');
//           if(pp2)
//           {
//             *pp2 = '\0';
//             if(*pp1 && strchr(pp1, '.')==0)
//             {
//                strcpy_s(style.m_sStyleClass, pp1);
//                *pp2 = ' ';
//                break;
//             }
//             *pp2 = ' ';
//           }
//           else
//           {
//             if(*pp1 && strchr(pp1, '.')==0)
//             {
//                strcpy_s(style.m_sStyleClass, pp1);
//                break;
//             }
//           }
//           pp1 = pp2;
//         }
//   }
// //
// void textStyle( STD_VAR )
//   {
// #ifdef _DEBUG
//   const char *id = style.NameID( html );
//   if(_stricmp(id, "CM_FB_C1DESC")==0)
//     AAA();
// #endif
//   a.AnalizStyle( html, value );
//   }
// //
// void textID( STD_VAR )
//   {
//   style.NameID( html, value );
//   //
//   if ( lstrcmpi( value, "alphaNum" ) == 0 )
//     style.mClass = clsTitleBarR;// Нужен L
//   }
// //
// void textTabIndex( STD_VAR )
//   {
//   ASSERT( ::IsInt(value) );
//   style.nTabIndex = atoi(value);
//   }
// //
// void textGlobalScripts( STD_VAR )
//   {
//   // Не обрабатываем
//   }
// //
// void textTitle( STD_VAR )
//   {
//    strcpy_s(style.m_szTitle, value);
//   }
// //
// void textHdxProperties( STD_VAR )
//   {
//   a.AnalizHdx( html, value );
//   }
// //
// void textHDX_LOCK( STD_VAR )
//   {
//   ASSERT( lstrcmp( value, "-1" ) == 0 );
//   }
// //    { "ondatachange"           , divOnDataChange           },
// void textDisabled          ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// void textDefaultValue      ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// void textNewValue          ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// void textOnKeyUp           ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
// void textOnDataChange      ( STD_VAR ){/*Встречается в sysdtltpsdevctl_fp*/}
// void textPrintDisplayValue ( STD_VAR ){}

// static bool findSubrutine(char** ptr, char* search)
// {
// 	if (!_strnicmp(*ptr, search, strlen(search)))
// 	{
// 		*ptr += strlen(search);
// 		return true;
// 	}
// 	return false;
// }

// void textOnUpdate          ( STD_VAR )
// {
// 	//if (!IsKhabarovsk()) return;
// 	char* ptr = value;
// 	//if (!_strnicmp("VBScript", ptr, 8))
// 	{
//         if(_strnicmp(ptr, "call ", sizeof("call ")-1)==0)
//         {
// 		  ptr += sizeof("call ")-1;
//         }
// 		while(*ptr)
// 		{
// 			if (findSubrutine(&ptr, "Cda_CEXXX_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCda_CEXXX_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_ConAlpTP_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_ConAlpTP_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "LK2B_Cda_CEXXX_RecPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usLK2B_Cda_CEXXX_RecPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "UZ_Cda_Graph_AlpError_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usUZ_Cda_Graph_AlpError_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "All_ILBPGraph_AlpError_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usAll_ILBPGraph_AlpError_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Interlock_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Interlock_OnUpdate_me_);

// 			/*else if (findSubrutine(&ptr, "CDA_ExecState_Alp_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_ExecState_Alp_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_RegCtlValves_RedTag_BadCtlFl_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_RegCtlValves_RedTag_BadCtlFl_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Graph_AlpError_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Graph_AlpError_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_RegCntlValve_AlpVal_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_RegCntlValve_AlpVal_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Devctl_Hialm_RedTag_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Devctl_Hialm_RedTag_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Graph_AlpError_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Graph_AlpError_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Device3States_AlpGPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Device3States_AlpGPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Discrepancy_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Discrepancy_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Graph_AlpError_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Graph_AlpError_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_DigStateCColor_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_DigStateCColor_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_OnOffValves_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_OnOffValves_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Numeric_AlpPVFormat_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Numeric_AlpPVFormat_OnUpdate_me_);
//                 */
// 			else if (findSubrutine(&ptr, "CDA_RegCntl_Mode_Hialm_RedTag_EUDesc_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_RegCntl_Mode_Hialm_RedTag_EUDesc_OnUpdate_me_);
//             /*
// 			else if (findSubrutine(&ptr, "Scd_Numeric_AlpPVFormat_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Numeric_AlpPVFormat_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_RegCntl_Mode_Hialm_EUDesc_OnUpdate(me,1)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_RegCntl_Mode_Hialm_EUDesc_OnUpdate_me_1_);
// 			else if (findSubrutine(&ptr, "CDA_DigState_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_DigState_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_DigStateButton_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_DigStateButton_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_DigStateCColor_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_DigStateCColor_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_OnOffValves_AlpGPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_OnOffValves_AlpGPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Device3States_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Device3States_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_DigState_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_DigState_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_DigStateButton_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_DigStateButton_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_DigStateColor_Khabarovsk_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Bypass_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Bypass_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Enclav_onupdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Enclav_onupdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Interlock_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Interlock_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_libKHABAROVSK_Bypass_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_libKHABAROVSK_Bypass_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_libKHABAROVSK_Interlock_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_libKHABAROVSK_Interlock_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Description_onupdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Description_onupdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_InitAnalog_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_InitAnalog_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_Actuador_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_Actuador_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_InitDigital_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_InitDigital_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_OnOffValves_Khabarovsk_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_OnOffValves_Khabarovsk_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_RegCntlValve_Khabarovsk_AlpVal_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_RegCntlValve_Khabarovsk_AlpVal_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_RegCntlValve_AlpVal_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_RegCntlValve_AlpVal_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "Scd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usScd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate_me_);
// 			else if (findSubrutine(&ptr, "CDA_Damper_AlpVal_OnUpdate(me)"))
// 				a.m_vctrUpdateScripts.push_back(usCDA_Damper_AlpVal_OnUpdate_me_);*/
// 			else
// 				ptr++;
// 		}
// 	}
// 	//a.mUpdate
//     if(Version != LG35_8_KF)
//     {
//     if(strlen(value)>2)
//       {
//       char *id = style.NameID( html );
//         {
//         char* pValue = value;
//         if(value[0]=='\'')
//            pValue++;
//         size_t len= strlen(pValue);
//         if(pValue[len-1]=='\'')
//            pValue[len-1] = 0;
//         a.m_ScriptTextId = style.AddTexts( html, pValue );
//         a.m_ScriptTexteType = eventOnUpdate;
//         }
//       }
//     }
// }
// void textSHORTCUTMENUCUSTOM( STD_VAR ){}
// void textSHORTCUTMENUFILE  ( STD_VAR ){}
// //
// void AnalizTextArea( CTag* pTag, ANALIZ )
//   {
//   //
//   struct SLocal { char* name; LPFuncs func; };
//   _static SLocal list[] =
//     {
//     { "class"             , textClass              },
//     { "title"             , textTitle              },
//     { "style"             , textStyle              },
//     { "id"                , textID                 },
//     { "tabIndex"          , textTabIndex           },
//     { "hdxproperties"     , textHdxProperties      },
//     { "globalscripts"     , textGlobalScripts      },
//     { "disabled"          , textDisabled           },
//     { "defaultValue"      , textDefaultValue       },
//     { "newValue"          , textNewValue           },
//     { "onkeyup"           , textOnKeyUp            },
//     { "onupdate"          , textOnUpdate           },
//     { "ondatachange"      , textOnDataChange       },
//     { "HDX_LOCK"          , textHDX_LOCK           },
//     { "SHORTCUTMENUCUSTOM", textSHORTCUTMENUCUSTOM },
//     { "SHORTCUTMENUFILE"  , textSHORTCUTMENUFILE   },
//     { "printDisplayValue" , textPrintDisplayValue  },
//     }InitLocal;
//   //
//   CStyle& style = pTag->Style(html);
//   int n = 0;
//   while ( teg[n].name )
//     {
//     SLocal* find = FindLocal( teg[n].name );
//     ASSERT( find );
//     if ( find != NULL )
//     (*find->func)(html,*pTag,style,teg[n].value);
//     n++;
//     }
//   //
//   CStyle& styl = pTag->Style(html);
//   styl.m_dwFlags |= HDX_PRESENT;
//   styl.AddFlag( TYPE_TEXT );
//   //
//   }
// //
