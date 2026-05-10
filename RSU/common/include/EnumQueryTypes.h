#pragma once
typedef enum  QueryTypes
{
  qUpdateSound = 7 ,// Снятие звуковой сигнализации

  id_Model            = 0x00000000,//для передачи сообщений оператору
  id_Rsu_Centum       = 0x01000000,
  id_Rsu_RsLogix      = 0x02000000,
  id_Rsu_DeltaV       = 0x03000000,
  id_Rsu_Intouch      = 0x04000000,
  id_Rsu_ArchestrA    = 0x05000000,
  id_Rsu_HoneyWell    = 0x06000000,
  id_Rsu_AbbItxa800   = 0x07000000,
  id_Rsu_HoneyWell4xx = 0x08000000,
  id_Rsu_RsView       = 0x09000000,
  id_Rsu_MetsoDna     = 0x0A000000,
  id_Rsu_HoneyWell5xx = 0x0B000000,
  id_Rsu_Siemens2     = 0x0C000000,//самописный ARM-2
  id_Rsu_Flex         = 0x0D000000,
  id_Rsu_SuerAP       = 0x0E000000,
  id_Rsu_NFT          = 0x0F000000,
  id_Rsu_TeCon        = 0x10000000,
  id_Rsu_SupCon       = 0x11000000,
  id_Rsu_AStud        = 0x12000000,//Astra
} QueryTypes;
