#pragma once

namespace ns_rsu
{
    enum eVersion
    {
        НеЗадана,// не задана
        JASTP,
        Коксование,// Коксование Уфа
        АВТ_4,// АВТ-4 Самара
        ГО, // Гидроочистка Самара
        ВБ,// Висбрекинг Самара
        УКПГ,// Переработка газа
        Реформинг,// Реформинг газа
        АВТ_4_Яр,// АВТ-4 Ярославль
        МТАЭ,// МТАЭ Новокузнецк
        Окисление_ИПБ,// Окисление изопропилбензола Новокузнецк
        ГФУ,//ГФУ Тобольск
        NH3_KEM, // Аммиак Кемерово
        УПВ_1_КНПЗ, // УПВ-1 Куйбышев
        УПВ_2_КНПЗ, // УПВ-2 Куйбышев
        Реформинг_КНПЗ,  // Реформинг Куйбышев
        УПВ_МСК, // УПВ Москва
        DM_OMSK, //Компрессора Омск
        YAR_24_7,//24_7 Ярославль
        NH3_CHE,//Аммиак Череповец
        AVT6_MSK, //АВТ-6 Москва  
        AVTO_TOMSK,
        NH3_RSH,			// 93 - Аммиак Россошь
        UPV_1_HBSK,		//94 - УПВ-1 Хабавровск
        GKGO_HBSK,		// 95 - 
        KUS_HBSK,			// 96 -	
        HBSK,				// 3 проекта в одном
        PET_UFA,			// 97 - 
        AT_PRP,			// 98 - 
        PPF_POM,			// 99 - Полипропилен "Полиом"
        HT_UNPZ,			// 100 - Гидроочистка УНПЗ 
        KU1_UNPZ,			// 101 - Висбрекинг УНПЗ
        CPP_POM,			// 102 -
        MK2_YAR,			// 103 - 
        VT_NZMP,			// 104 - 
        PARK_NZMP,		// 105 -
        DBO10_TOBOLSK,	// Тобольск
        DM_KO_KT1_1,		// 
        DM_KO_OMSK_2,		// Омск
        GF_2_TOBOLSK,
        AOP_TOBOLSK,
        BITUM_MSK,			// 111 Москва
        PSA_MSK,			// 112 Москва
        KC_2_NV,			// 106 - 
        PHU_2_NV,			// 107 - 
        TU_4_NV,			// 108 - 
        DKS_NV,				// 109 -
        KC_3_NV,			// 110 -
        DBO2_TBK,			// 113
        DBO3_TBK,			// 114 - 
        PP_TSK,				// 115
        MAU_3_NV,			// 116
        MAU_4_NV,			// 117
        UPG_1_BZ,			// 118
        UPG_2_BZ,			// 119
        AVT_EM,				// 120
        PVD_UNH,			// 121
        UPV20_UNPZ,			// 122
        UPV_NOVOIL,			// 123
        PK_YAR,				// 124 
        C300_YAR,			// 125
        C400_YAR,			// 126
        LG35_8_KF,			// 127
        BIT_OMSK,			// 128
        NH3_PR,				// 129
        UKR_PRP,			// 130
        DGP_TBK,			// 131
        UUG_MSK,			// 132
        C500_YAR,			// 133
        MAH_TBK,			// 137 
        _35_11_OMSK,		// 138 
        URALKALIY_BKPRU2,	// 139
        IBIP_TBK,			// 141  
        EXDR_TSK,			// 142
        EP60_PM,			// 147
        RSG_PM,				// 148
        MTBE_PM,			// 149
        C4_11_14_TBK,		// 151
        C6H6_KST,			// 152
        PP_TBK,				// 155
        H2_TSK,				// 156
        TEA_TSK,			// 157
        AGFU_UFA,			// 158
        GOBKK_MSK,			// 159
        LCH_24_MSK,			// 160
        UILN_MSK,			// 161
        LAB_KF,				// 169
        UPO_KF,				// 170
        CRBA_PR,			// 171
        TAME_MSK,			// 172
        ATVB_MSK,			// 173
        L_35_5_UFA,			// 176
        NAK_MND,			// 177
        AT4_YAR,			// 178
        AVT3_POLYGON,		// 179
        GRS_MSK,			// 180
        UREA_BRZK,			// 181
        UDK_1_GAZPROM,		// 182
        UPG1_GGPZ,			// 183
        UPG2_GGPZ,			// 184
        UCGIK_NTK_UBGPZ,			// 185
        DKS_NTKR_UBGPZ,			// 186
        UPG_UBGPZ,			// 187
        UGOSH_UBGPZ,			// 188
        PARK_NKNH,			// 189
        UPG1_NGPZ,			// 190
        UPG2_NGPZ,			// 191
        _1307_NKNH,			// 192
        _1319_NKNH,			// 193
        H2O2_VP,			// 194
        L35_11_600_OMSK,		// 195
        UNU_DZ,			// 196
        AK_DZ,			// 197
        DOTF_PM,			// 198
        C8H8_PM,			// 199
        _1510_NKNH,			// 200
        _2104_NKNH,			// 201
        _2106_NKNH,			// 202
        BK2_TBK,			// 203
        BK2_1_TBK,			// 204
        M_2_TBK,			// 205
        BK8_10_1_TBK,		// 206
        BK8_1_10_2_TBK,		// 207
        B6_9_TBK,			// 208
        B3_TBK,			// 209
        B3_2_TBK,			// 210
        B1_2_TBK,			// 211
        _36_3_OZSM,			// 212
        _37_1_OZSM,			// 213
        EXDR_UFA,			// 214
        HTH_POLYGON,		// 215
        GB_201_NKNH,		// 216
        NTGR_NKNH,			// 217
        VTGR_NKNH,			// 218
        REF_C_NKNH,			// 219
        PVD_NOVOIL,			// 220
        _22_4_NOVOIL,		// 221
        GFU_UNH,			// 222
        UPV_UNH,			// 223
        ASB_MNPZ,			// 224
        USN4_1_NNPO,		// 225
        USN4_2_NNPO,		// 226
        UOUVK_BZ,			// 227
        RHPNO_MNPZ,			// 228
        ENSN_MNPZ,			// 229
        _75_24_MNPZ,		// 230
        SRU_MNPZ,			// 231
        SNMVG_MNPZ,			// 232
        CVK_MNPZ,			// 233_CVK_MNPZ
        _4007_MNPZ,			// 234_4007_MNPZ
        BRDT_MNPZ,			// 235_BRDT_MNPZ
        MOS_MNPZ,			// 236_MOS_MNPZ
        BOS_MNPZ,			// 237_BOS_MNPZ
        L35_11600_KF,		// 238_L35_11600_KF
        L35_6_YAR,			// 239_L35_6_YAR
        GFU2_OMSK,          //240_GFU2_OMSK
        _36_1_1_OZSM,			// 241_36_1_1_OZSM
        _36_1_3_OZSM,			// 242_36_1_3_OZSM
        _39_1_2_OZSM,			// 243_39_1_2_OZSM
        _39_6_4_OZSM,			// 244_39_6_4_OZSM
        UPG1_2_VP,			// 245_UPG1_2_VP
        UDK_GP,			// 246_UDK_GP
        RCOL_SI,			// 247_RCOL_SI
        COH2_PM,			// 248_COH2_PM
        UOSG_YAR,			// 249_UOSG_YAR
        AM2_NAK,			// 250_AM2_NAK
        ASSB_ONPZ,			// 251_ASSB_ONPZ
        PSG_ONPZ,			// 252_PSG_ONPZ
        SGK_ONPZ,			// 253_SGK_ONPZ
        T210_ONPZ,			// 254_T210_ONPZ
        T1100_ONPZ,			// 255_T1100_ONPZ
        T146_ONPZ,			// 256_T146_ONPZ
        T1163_ONPZ,			// 257_T1163_ONPZ
        T1165P_ONPZ,			// 258_T1165P_ONPZ
        BERTH_ONPZ,			// 259_BERTH_ONPZ
        H2SO4_YAR,			// 260_H2SO4_YAR
        GFU_YAR,			// 261_GFU_YAR
        ROSK_YAR,			// 262_ROSK_YAR
        EDU_GPN,			// 263_EDU_GPN
        SULF_OZSM,			// 264_SULF_OZSM
        PARK2_NKNH,			// 265_PARK2_NKNH
        GBK_NKNH,			// 266_GBK_NKNH
        EB_NKNH,			// 267_EB_NKNH
        _1322_NKNH,			// 268_1322_NKNH
        CHWP_MNPZ,			// 269_CHWP_MNPZ
        CWB_MNPZ,			// 270_CWB_MNPZ
        NGU_MNPZ,			// 271_NGU_MNPZ
        AVT10_ONPZ,			// 272_AVT10_ONPZ
        BON_ONPZ,			// 273_BON_ONPZ
        UPS_1_2_ONPZ,			// 274_UPS_1_2_ONPZ
        UPS_3_ONPZ,			// 275_UPS_3_ONPZ
        _2024_35_11_OMSK,			// 138_2024_35_11_OMSK
        MFCU_1_AGCC,			// 276_MFCU_1_AGCC
        MFCU_2_AGCC,			// 277_MFCU_2_AGCC
        MFCU_3_AGCC,			// 278_MFCU_3_AGCC
        L35_11_1000_KF,			// 279_L35_11_1000_KF
        DMW_NAK,			// 280_DMW_NAK
        ITH_NAK,			// 281_ITH_NAK
        UPV_ONPZ,			// 282_UPV_ONPZ
        UGK_ONPZ,			// 283_UGK_ONPZ
        L24_7_ONPZ,			// 284_L24_7_ONPZ
        L24_5_MNPZ,			// 285_L24_5_MNPZ
        UPS_NK,			// 286_UPS_NK
        COL2_SI,			// 287_COL2_SI
        ISO2_ONPZ,			// 288_ISO2_ONPZ
        UREA_EHSZ,			// 289_UREA_EHSZ
        C6H12_NKNH,			// 290_C6H12_NKNH
        GRAN_EHSZ,			// 291_GRAN_EHSZ
        SKLAD_EHSZ,			// 292_SKLAD_EHSZ
        RSG_KST,			// 294_RSG_KST
        PPP_KST,			// 295_PPP_KST
        EP360_KST,			// 293_EP360_KST
        EP300_TNH,			// 296_EP300_TNH
        EP300_2_TNH,			// 297_EP300_2_TNH
        EP300_3_TNH,			// 298_EP300_3_TNH
        EP300_4_TNH,			// 299_EP300_4_TNH
        EP300_5_TNH,			// 300_EP300_5_TNH
        EP300_6_TNH,			// 301_EP300_6_TNH
        EP300_7_TNH,			// 302_EP300_7_TNH
        EP300_8_TNH,			// 303_EP300_8_TNH
        EP300_9_TNH,			// 304_EP300_9_TNH
        EP300_10_TNH,			// 305_EP300_10_TNH
        EP300_11_TNH,			// 306_EP300_11_TNH
        EP300_12_TNH,			// 307_EP300_12_TNH
        EP300_13_TNH,			// 308_EP300_13_TNH
        EP300_14_TNH,			// 309_EP300_14_TNH
        EP300_15_TNH,			// 310_EP300_15_TNH
        EP300_16_TNH,			// 311_EP300_16_TNH
        EP300_17_TNH,			// 312_EP300_17_TNH
        EP300_18_TNH,			// 313_EP300_18_TNH
        EP300_19_TNH,			// 314_EP300_19_TNH
        LOS_TNH,			// 315_LOS_TNH
        PEU1_AGCC,			// 316_PEU1_AGCC
        LAO_AGCC,			// 317_LAO_AGCC
        UNO_AGCC,			// 318_UNO_AGCC
        VSB_KF,			// 319_VSB_KF
        PARKI_SNE_AGCC,			// 320_PARKI_SNE_AGCC
        T7230_60_AGCC,			// 321_T7230_60_AGCC
        T6400_AGCC,			// 322_T6400_AGCC
        KOS_VOS_AGCC,			// 323_KOS_VOS_AGCC
        EDU2_GPN,			// 324_EDU2_GPN
        MFCU_2613_AGCC,			// 325_MFCU_2613_AGCC
        PEU2_AGCC,			// 326_PEU2_AGCC
        CIPR_SBR,			// 900_CIPR_SBR
        GODT_ONPZ,			// 327_GODT_ONPZ
        GOBKK_ONPZ,			// 328_GOBKK_ONPZ
        IZO2_ONPZ,			// 329_IZO2_ONPZ
    };
}
