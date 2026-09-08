/*-------------------------------------------------------------------------
RTC library

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/Rtc)

-------------------------------------------------------------------------
This file is part of the Makuna/Rtc library.

Rtc is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

Rtc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with Rtc.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/

#include "RtcLocaleEn.h"
#include "RtcTimeZone.h"
#include <cctype>

// due to how works, strings must be declared standalone
//
const char stringACDT[] = "ACDT";
const char stringACST[] = "ACST";
const char stringACT[] = "ACT";
const char stringACWST[] = "ACWST";
const char stringADT[] = "ADT";
const char stringAEDT[] = "AEDT";
const char stringAEST[] = "AEST";
const char stringAET[] = "AET";
const char stringAFT[] = "AFT";
const char stringAKDT[] = "AKDT";
const char stringAKST[] = "AKST";
const char stringALMT[] = "ALMT";
const char stringAMST[] = "AMST";
const char stringAMT[] = "AMT";
const char stringANAT[] = "ANAT";
const char stringAQTT[] = "AQTT";
const char stringART[] = "ART";
const char stringAST[] = "AST";
const char stringAWST[] = "AWST";
const char stringAZOST[] = "AZOST";
const char stringAZOT[] = "AZOT";
const char stringAZT[] = "AZT";
const char stringBNT[] = "BNT";
const char stringBIOT[] = "BIOT";
const char stringBIT[] = "BIT";
const char stringBOT[] = "BOT";
const char stringBRST[] = "BRST";
const char stringBRT[] = "BRT";
const char stringBST[] = "BST";
const char stringBTT[] = "BTT";
const char stringCAT[] = "CAT";
const char stringCCT[] = "CCT";
const char stringCDT[] = "CDT";
const char stringCEST[] = "CEST";
const char stringCET[] = "CET";
const char stringCHADT[] = "CHADT";
const char stringCHAST[] = "CHAST";
const char stringCHOT[] = "CHOT";
const char stringCHOST[] = "CHOST";
const char stringCHST[] = "CHST";
const char stringCHUT[] = "CHUT";
const char stringCIST[] = "CIST";
const char stringCKT[] = "CKT";
const char stringCLST[] = "CLST";
const char stringCLT[] = "CLT";
const char stringCOST[] = "COST";
const char stringCOT[] = "COT";
const char stringCST[] = "CST";
const char stringCT[] = "CT";
const char stringCVT[] = "CVT";
const char stringCWST[] = "CWST";
const char stringCXT[] = "CXT";
const char stringDAVT[] = "DAVT";
const char stringDDUT[] = "DDUT";
const char stringDFT[] = "DFT";
const char stringEASST[] = "EASST";
const char stringEAST[] = "EAST";
const char stringEAT[] = "EAT";
const char stringECT[] = "ECT";
const char stringEDT[] = "EDT";
const char stringEEST[] = "EEST";
const char stringEET[] = "EET";
const char stringEGST[] = "EGST";
const char stringEGT[] = "EGT";
const char stringEST[] = "EST";
const char stringET[] = "ET";
const char stringFET[] = "FET";
const char stringFJT[] = "FJT";
const char stringFKST[] = "FKST";
const char stringFKT[] = "FKT";
const char stringFNT[] = "FNT";
const char stringGALT[] = "GALT";
const char stringGAMT[] = "GAMT";
const char stringGET[] = "GET";
const char stringGFT[] = "GFT";
const char stringGILT[] = "GILT";
const char stringGIT[] = "GIT";
const char stringGMT[] = "GMT";
const char stringGST[] = "GST";
const char stringGYT[] = "GYT";
const char stringHDT[] = "HDT";
const char stringHAEC[] = "HAEC";
const char stringHST[] = "HST";
const char stringHKT[] = "HKT";
const char stringHMT[] = "HMT";
const char stringHOVST[] = "HOVST";
const char stringHOVT[] = "HOVT";
const char stringICT[] = "ICT";
const char stringIDLW[] = "IDLW";
const char stringIDT[] = "IDT";
const char stringIOT[] = "IOT";
const char stringIRDT[] = "IRDT";
const char stringIRKT[] = "IRKT";
const char stringIRST[] = "IRST";
const char stringIST[] = "IST";
const char stringJST[] = "JST";
const char stringKALT[] = "KALT";
const char stringKGT[] = "KGT";
const char stringKOST[] = "KOST";
const char stringKRAT[] = "KRAT";
const char stringKST[] = "KST";
const char stringLHST[] = "LHST";
const char stringLINT[] = "LINT";
const char stringMAGT[] = "MAGT";
const char stringMART[] = "MART";
const char stringMAWT[] = "MAWT";
const char stringMDT[] = "MDT";
const char stringMET[] = "MET";
const char stringMEST[] = "MEST";
const char stringMHT[] = "MHT";
const char stringMIST[] = "MIST";
const char stringMIT[] = "MIT";
const char stringMMT[] = "MMT";
const char stringMSK[] = "MSK";
const char stringMST[] = "MST";
const char stringMUT[] = "MUT";
const char stringMVT[] = "MVT";
const char stringMYT[] = "MYT";
const char stringNCT[] = "NCT";
const char stringNDT[] = "NDT";
const char stringNFT[] = "NFT";
const char stringNOVT[] = "NOVT";
const char stringNPT[] = "NPT";
const char stringNST[] = "NST";
const char stringNT[] = "NT";
const char stringNUT[] = "NUT";
const char stringNZDT[] = "NZDT";
const char stringNZST[] = "NZST";
const char stringOMST[] = "OMST";
const char stringORAT[] = "ORAT";
const char stringPDT[] = "PDT";
const char stringPET[] = "PET";
const char stringPETT[] = "PETT";
const char stringPGT[] = "PGT";
const char stringPHOT[] = "PHOT";
const char stringPHT[] = "PHT";
const char stringPHST[] = "PHST";
const char stringPKT[] = "PKT";
const char stringPMDT[] = "PMDT";
const char stringPMST[] = "PMST";
const char stringPONT[] = "PONT";
const char stringPST[] = "PST";
const char stringPWT[] = "PWT";
const char stringPYST[] = "PYST";
const char stringPYT[] = "PYT";
const char stringRET[] = "RET";
const char stringROTT[] = "ROTT";
const char stringSAKT[] = "SAKT";
const char stringSAMT[] = "SAMT";
const char stringSAST[] = "SAST";
const char stringSBT[] = "SBT";
const char stringSCT[] = "SCT";
const char stringSDT[] = "SDT";
const char stringSGT[] = "SGT";
const char stringSLST[] = "SLST";
const char stringSRET[] = "SRET";
const char stringSRT[] = "SRT";
const char stringSST[] = "SST";
const char stringSYOT[] = "SYOT";
const char stringTAHT[] = "TAHT";
const char stringTHA[] = "THA";
const char stringTFT[] = "TFT";
const char stringTJT[] = "TJT";
const char stringTKT[] = "TKT";
const char stringTLT[] = "TLT";
const char stringTMT[] = "TMT";
const char stringTRT[] = "TRT";
const char stringTOT[] = "TOT";
const char stringTVT[] = "TVT";
const char stringULAST[] = "ULAST";
const char stringULAT[] = "ULAT";
const char stringUTC[] = "UTC";
const char stringUYST[] = "UYST";
const char stringUYT[] = "UYT";
const char stringUZT[] = "UZT";
const char stringVET[] = "VET";
const char stringVLAT[] = "VLAT";
const char stringVOLT[] = "VOLT";
const char stringVOST[] = "VOST";
const char stringVUT[] = "VUT";
const char stringWAKT[] = "WAKT";
const char stringWAST[] = "WAST";
const char stringWAT[] = "WAT";
const char stringWEST[] = "WEST";
const char stringWET[] = "WET";
const char stringWIB[] = "WIB";
const char stringWIT[] = "WIT";
const char stringWITA[] = "WITA";
const char stringWGST[] = "WGST";
const char stringWGT[] = "WGT";
const char stringWST[] = "WST";
const char stringYAKT[] = "YAKT";
const char stringYEKT[] = "YEKT";

// collection in alphabetic order of abbreviation
// duplicates commented out randomly (why are there duplicates in a standard?!)
// IF YOU MODIFY THIS to your locale, please goto github and add an issue
//
const RtcTimeZone c_tzEn[] = {
  { .abbr = stringACDT,  .offset = (10 * 60) + 30   }, // Australian Central Daylight Saving Time
  { .abbr = stringACST,  .offset = (9 * 60) + 30    }, // Australian Central Standard Time
  { .abbr = stringACT,   .offset = -5 * 60          }, // Acre Time
                                                      //	{ stringACT, 8 * 60 + 0 },    // ASEAN Common Time (proposed)
  { .abbr = stringACWST, .offset = (8 * 60) + 45    }, // Australian Central Western Standard Time (unofficial)
  { .abbr = stringADT,   .offset = -3 * 60          }, // Atlantic Daylight Time
  { .abbr = stringAEDT,  .offset = 11 * 60          }, // Australian Eastern Daylight Saving Time
  { .abbr = stringAEST,  .offset = 10 * 60          }, // Australian Eastern Standard Time
  { .abbr = stringAET,   .offset = 10 * 60          }, // Australian Eastern Time
  { .abbr = stringAFT,   .offset = (4 * 60) + 30    }, // Afghanistan Time
  { .abbr = stringAKDT,  .offset = -8 * 60          }, // Alaska Daylight Time
  { .abbr = stringAKST,  .offset = -9 * 60          }, // Alaska Standard Time
  { .abbr = stringALMT,  .offset = 6 * 60           }, // Alma-Ata Time[1]
  { .abbr = stringAMST,  .offset = -3 * 60          }, // Amazon Summer Time (Brazil)[2]
  { .abbr = stringAMT,   .offset = -4 * 60          }, // Amazon Time (Brazil)[3]
                                                      //	{ stringAMT, 4 * 60 },    // Armenia Time
  { .abbr = stringANAT,  .offset = 12 * 60          }, // Anadyr Time[4]
  { .abbr = stringAQTT,  .offset = 5 * 60           }, // Aqtobe Time[5]
  { .abbr = stringART,   .offset = -3 * 60          }, // Argentina Time
                                                      //	{ stringAST, 3 * 60 },    // Arabia Standard Time
  { .abbr = stringAST,   .offset = -4 * 60          }, // Atlantic Standard Time
  { .abbr = stringAWST,  .offset = 8 * 60           }, // Australian Western Standard Time
  { .abbr = stringAZOST, .offset = 0                }, // Azores Summer Time
  { .abbr = stringAZOT,  .offset = -1 * 60          }, // Azores Standard Time
  { .abbr = stringAZT,   .offset = 4 * 60           }, // Azerbaijan Time
  { .abbr = stringBNT,   .offset = 8 * 60           }, // Brunei Time
  { .abbr = stringBIOT,  .offset = 6 * 60           }, // British Indian Ocean Time
  { .abbr = stringBIT,   .offset = -12 * 60         }, // Baker Island Time
  { .abbr = stringBOT,   .offset = -4 * 60          }, // Bolivia Time
  { .abbr = stringBRST,  .offset = -2 * 60          }, // Bras�lia Summer Time
  { .abbr = stringBRT,   .offset = -3 * 60          }, // Bras�lia Time
                                                      //	{ stringBST, 6 * 60 },    // Bangladesh Standard Time
                                                      //	{ stringBST, 11 * 60 },    // Bougainville Standard Time[6]
  { .abbr = stringBST,   .offset = 1 * 60           }, // British Summer Time (British Standard Time from Mar 1968 to Oct 1971)
  { .abbr = stringBTT,   .offset = 6 * 60           }, // Bhutan Time
  { .abbr = stringCAT,   .offset = 2 * 60           }, // Central Africa Time
  { .abbr = stringCCT,   .offset = (6 * 60) + 30    }, // Cocos Islands Time
  { .abbr = stringCDT,   .offset = -5 * 60          }, // Central Daylight Time (North America)
                                                      //	{ stringCDT, -4 * 60 },    // Cuba Daylight Time[7]
  { .abbr = stringCEST,  .offset = 2 * 60           }, // Central European Summer Time
  { .abbr = stringCET,   .offset = 1 * 60           }, // Central European Time
  { .abbr = stringCHADT, .offset = (13 * 60) + 45   }, // Chatham Daylight Time
  { .abbr = stringCHAST, .offset = (12 * 60) + 45   }, // Chatham Standard Time
  { .abbr = stringCHOT,  .offset = 8 * 60           }, // Choibalsan Standard Time
  { .abbr = stringCHOST, .offset = 9 * 60           }, // Choibalsan Summer Time
  { .abbr = stringCHST,  .offset = 10 * 60          }, // Chamorro Standard Time
  { .abbr = stringCHUT,  .offset = 10 * 60          }, // Chuuk Time
  { .abbr = stringCIST,  .offset = -8 * 60          }, // Clipperton Island Standard Time
  { .abbr = stringCKT,   .offset = -10 * 60         }, // Cook Island Time
  { .abbr = stringCLST,  .offset = -3 * 60          }, // Chile Summer Time
  { .abbr = stringCLT,   .offset = -4 * 60          }, // Chile Standard Time
  { .abbr = stringCOST,  .offset = -4 * 60          }, // Colombia Summer Time
  { .abbr = stringCOT,   .offset = -5 * 60          }, // Colombia Time
  { .abbr = stringCST,   .offset = -6 * 60          }, // Central Standard Time (North America)
                                                      //	{ stringCST, 8 * 60 },    // China Standard Time
                                                      //	{ stringCST, -5 * 60 },    // Cuba Standard Time
  { .abbr = stringCT,    .offset = -6 * 60          }, // Central Time
  { .abbr = stringCVT,   .offset = -1 * 60          }, // Cape Verde Time
  { .abbr = stringCWST,  .offset = (8 * 60) + 45    }, // Central Western Standard Time (Australia) unofficial
  { .abbr = stringCXT,   .offset = 7 * 60           }, // Christmas Island Time
  { .abbr = stringDAVT,  .offset = 7 * 60           }, // Davis Time
  { .abbr = stringDDUT,  .offset = 10 * 60          }, // Dumont d'Urville Time
  { .abbr = stringDFT,   .offset = 1 * 60           }, // AIX-specific equivalent of Central European Time[NB 1]
  { .abbr = stringEASST, .offset = -5 * 60          }, // Easter Island Summer Time
  { .abbr = stringEAST,  .offset = -6 * 60          }, // Easter Island Standard Time
  { .abbr = stringEAT,   .offset = 3 * 60           }, // East Africa Time
  { .abbr = stringECT,   .offset = -4 * 60          }, // Eastern Caribbean Time (does not recognise DST)
                                                      //	{ stringECT, -5 * 60 },    // Ecuador Time
  { .abbr = stringEDT,   .offset = -4 * 60          }, // Eastern Daylight Time (North America)
  { .abbr = stringEEST,  .offset = 3 * 60           }, // Eastern European Summer Time
  { .abbr = stringEET,   .offset = 2 * 60           }, // Eastern European Time
  { .abbr = stringEGST,  .offset = 0                }, // Eastern Greenland Summer Time
  { .abbr = stringEGT,   .offset = -1 * 60          }, // Eastern Greenland Time
  { .abbr = stringEST,   .offset = -5 * 60          }, // Eastern Standard Time (North America)
  { .abbr = stringET,    .offset = -5 * 60          }, // Eastern Time (North America)
  { .abbr = stringFET,   .offset = 3 * 60           }, // Further-eastern European Time
  { .abbr = stringFJT,   .offset = 12 * 60          }, // Fiji Time
  { .abbr = stringFKST,  .offset = -3 * 60          }, // Falkland Islands Summer Time
  { .abbr = stringFKT,   .offset = -4 * 60          }, // Falkland Islands Time
  { .abbr = stringFNT,   .offset = -2 * 60          }, // Fernando de Noronha Time
  { .abbr = stringGALT,  .offset = -6 * 60          }, // Gal�pagos Time
  { .abbr = stringGAMT,  .offset = -9 * 60          }, // Gambier Islands Time
  { .abbr = stringGET,   .offset = 4 * 60           }, // Georgia Standard Time
  { .abbr = stringGFT,   .offset = -3 * 60          }, // French Guiana Time
  { .abbr = stringGILT,  .offset = 12 * 60          }, // Gilbert Island Time
  { .abbr = stringGIT,   .offset = -9 * 60          }, // Gambier Island Time
  { .abbr = stringGMT,   .offset = 0                }, // Greenwich Mean Time
                                                      //	{ stringGST, -2 * 60 },    // South Georgia and the South Sandwich Islands Time
  { .abbr = stringGST,   .offset = 4 * 60           }, // Gulf Standard Time
  { .abbr = stringGYT,   .offset = -4 * 60          }, // Guyana Time
  { .abbr = stringHDT,   .offset = -9 * 60          }, // Hawaii�Aleutian Daylight Time
  { .abbr = stringHAEC,  .offset = 2 * 60           }, // Heure Avanc�e d'Europe Centrale French-language name for CEST
  { .abbr = stringHST,   .offset = -10 * 60         }, // Hawaii�Aleutian Standard Time
  { .abbr = stringHKT,   .offset = 8 * 60           }, // Hong Kong Time
  { .abbr = stringHMT,   .offset = 5 * 60           }, // Heard and McDonald Islands Time
  { .abbr = stringHOVST, .offset = 8 * 60           }, // Hovd Summer Time (not used from 2017-present)
  { .abbr = stringHOVT,  .offset = 7 * 60           }, // Hovd Time
  { .abbr = stringICT,   .offset = 7 * 60           }, // Indochina Time
  { .abbr = stringIDLW,  .offset = -12 * 60         }, // International Date Line West time zone
  { .abbr = stringIDT,   .offset = 3 * 60           }, // Israel Daylight Time
  { .abbr = stringIOT,   .offset = 3 * 60           }, // Indian Ocean Time
  { .abbr = stringIRDT,  .offset = (4 * 60) + 30    }, // Iran Daylight Time
  { .abbr = stringIRKT,  .offset = 8 * 60           }, // Irkutsk Time
  { .abbr = stringIRST,  .offset = (3 * 60) + 30    }, // Iran Standard Time
                                                      //	{ stringIST, 5 * 60 + 30 },    // Indian Standard Time
                                                      //	{ stringIST, 1 * 60 },    // Irish Standard Time[8]
  { .abbr = stringIST,   .offset = 2 * 60           }, // Israel Standard Time
  { .abbr = stringJST,   .offset = 9 * 60           }, // Japan Standard Time
  { .abbr = stringKALT,  .offset = 2 * 60           }, // Kaliningrad Time
  { .abbr = stringKGT,   .offset = 6 * 60           }, // Kyrgyzstan Time
  { .abbr = stringKOST,  .offset = 11 * 60          }, // Kosrae Time
  { .abbr = stringKRAT,  .offset = 7 * 60           }, // Krasnoyarsk Time
  { .abbr = stringKST,   .offset = 9 * 60           }, // Korea Standard Time
  { .abbr = stringLHST,  .offset = (10 * 60) + 30   }, // Lord Howe Standard Time
  { .abbr = stringLHST,  .offset = 11 * 60          }, // Lord Howe Summer Time
  { .abbr = stringLINT,  .offset = 14 * 60          }, // Line Islands Time
  { .abbr = stringMAGT,  .offset = 12 * 60          }, // Magadan Time
  { .abbr = stringMART,  .offset = -((9 * 60) + 30) }, // Marquesas Islands Time
  { .abbr = stringMAWT,  .offset = 5 * 60           }, // Mawson Station Time
  { .abbr = stringMDT,   .offset = -6 * 60          }, // Mountain Daylight Time (North America)
  { .abbr = stringMET,   .offset = 1 * 60           }, // Middle European Time (same zone as CET)
  { .abbr = stringMEST,  .offset = 2 * 60           }, // Middle European Summer Time (same zone as CEST)
  { .abbr = stringMHT,   .offset = 12 * 60          }, // Marshall Islands Time
  { .abbr = stringMIST,  .offset = 11 * 60          }, // Macquarie Island Station Time
  { .abbr = stringMIT,   .offset = -((9 * 60) + 30) }, // Marquesas Islands Time
  { .abbr = stringMMT,   .offset = (6 * 60) + 30    }, // Myanmar Standard Time
  { .abbr = stringMSK,   .offset = 3 * 60           }, // Moscow Time
  { .abbr = stringMST,   .offset = 8 * 60           }, // Malaysia Standard Time
  { .abbr = stringMST,   .offset = -7 * 60          }, // Mountain Standard Time (North America)
  { .abbr = stringMUT,   .offset = 4 * 60           }, // Mauritius Time
  { .abbr = stringMVT,   .offset = 5 * 60           }, // Maldives Time
  { .abbr = stringMYT,   .offset = 8 * 60           }, // Malaysia Time
  { .abbr = stringNCT,   .offset = 11 * 60          }, // New Caledonia Time
  { .abbr = stringNDT,   .offset = -((2 * 60) + 30) }, // Newfoundland Daylight Time
  { .abbr = stringNFT,   .offset = 11 * 60          }, // Norfolk Island Time
  { .abbr = stringNOVT,  .offset = 7 * 60           }, // Novosibirsk Time [9]
  { .abbr = stringNPT,   .offset = (5 * 60) + 45    }, // Nepal Time
  { .abbr = stringNST,   .offset = -((3 * 60) + 30) }, // Newfoundland Standard Time
  { .abbr = stringNT,    .offset = -((3 * 60) + 30) }, // Newfoundland Time
  { .abbr = stringNUT,   .offset = -11 * 60         }, // Niue Time
  { .abbr = stringNZDT,  .offset = 13 * 60          }, // New Zealand Daylight Time
  { .abbr = stringNZST,  .offset = 12 * 60          }, // New Zealand Standard Time
  { .abbr = stringOMST,  .offset = 6 * 60           }, // Omsk Time
  { .abbr = stringORAT,  .offset = 5 * 60           }, // Oral Time
  { .abbr = stringPDT,   .offset = -7 * 60          }, // Pacific Daylight Time (North America)
  { .abbr = stringPET,   .offset = -5 * 60          }, // Peru Time
  { .abbr = stringPETT,  .offset = 12 * 60          }, // Kamchatka Time
  { .abbr = stringPGT,   .offset = 10 * 60          }, // Papua New Guinea Time
  { .abbr = stringPHOT,  .offset = 13 * 60          }, // Phoenix Island Time
  { .abbr = stringPHT,   .offset = 8 * 60           }, // Philippine Time
  { .abbr = stringPHST,  .offset = 8 * 60           }, // Philippine Standard Time
  { .abbr = stringPKT,   .offset = 5 * 60           }, // Pakistan Standard Time
  { .abbr = stringPMDT,  .offset = -2 * 60          }, // Saint Pierre and Miquelon Daylight Time
  { .abbr = stringPMST,  .offset = -3 * 60          }, // Saint Pierre and Miquelon Standard Time
  { .abbr = stringPONT,  .offset = 11 * 60          }, // Pohnpei Standard Time
  { .abbr = stringPST,   .offset = -8 * 60          }, // Pacific Standard Time (North America)
  { .abbr = stringPWT,   .offset = 9 * 60           }, // Palau Time[10]
  { .abbr = stringPYST,  .offset = -3 * 60          }, // Paraguay Summer Time[11]
  { .abbr = stringPYT,   .offset = -4 * 60          }, // Paraguay Time[12]
  { .abbr = stringRET,   .offset = 4 * 60           }, // R�union Time
  { .abbr = stringROTT,  .offset = -3 * 60          }, // Rothera Research Station Time
  { .abbr = stringSAKT,  .offset = 11 * 60          }, // Sakhalin Island Time
  { .abbr = stringSAMT,  .offset = 4 * 60           }, // Samara Time
  { .abbr = stringSAST,  .offset = 2 * 60           }, // South African Standard Time
  { .abbr = stringSBT,   .offset = 11 * 60          }, // Solomon Islands Time
  { .abbr = stringSCT,   .offset = 4 * 60           }, // Seychelles Time
  { .abbr = stringSDT,   .offset = -10 * 60         }, // Samoa Daylight Time
  { .abbr = stringSGT,   .offset = 8 * 60           }, // Singapore Time
  { .abbr = stringSLST,  .offset = (5 * 60) + 30    }, // Sri Lanka Standard Time
  { .abbr = stringSRET,  .offset = 11 * 60          }, // Srednekolymsk Time
  { .abbr = stringSRT,   .offset = -3 * 60          }, // Suriname Time
                                                      //	{ stringSST, -11 * 60 },    // Samoa Standard Time
  { .abbr = stringSST,   .offset = 8 * 60           }, // Singapore Standard Time
  { .abbr = stringSYOT,  .offset = 3 * 60           }, // Showa Station Time
  { .abbr = stringTAHT,  .offset = -10 * 60         }, // Tahiti Time
  { .abbr = stringTHA,   .offset = 7 * 60           }, // Thailand Standard Time
  { .abbr = stringTFT,   .offset = 5 * 60           }, // French Southern and Antarctic Time[13]
  { .abbr = stringTJT,   .offset = 5 * 60           }, // Tajikistan Time
  { .abbr = stringTKT,   .offset = 13 * 60          }, // Tokelau Time
  { .abbr = stringTLT,   .offset = 9 * 60           }, // Timor Leste Time
  { .abbr = stringTMT,   .offset = 5 * 60           }, // Turkmenistan Time
  { .abbr = stringTRT,   .offset = 3 * 60           }, // Turkey Time
  { .abbr = stringTOT,   .offset = 13 * 60          }, // Tonga Time
  { .abbr = stringTVT,   .offset = 12 * 60          }, // Tuvalu Time
  { .abbr = stringULAST, .offset = 9 * 60           }, // Ulaanbaatar Summer Time
  { .abbr = stringULAT,  .offset = 8 * 60           }, // Ulaanbaatar Standard Time
  { .abbr = stringUTC,   .offset = 0                }, // Coordinated Universal Time
  { .abbr = stringUYST,  .offset = -2 * 60          }, // Uruguay Summer Time
  { .abbr = stringUYT,   .offset = -3 * 60          }, // Uruguay Standard Time
  { .abbr = stringUZT,   .offset = 5 * 60           }, // Uzbekistan Time
  { .abbr = stringVET,   .offset = -4 * 60          }, // Venezuelan Standard Time
  { .abbr = stringVLAT,  .offset = 10 * 60          }, // Vladivostok Time
  { .abbr = stringVOLT,  .offset = 3 * 60           }, // Volgograd Time
  { .abbr = stringVOST,  .offset = 6 * 60           }, // Vostok Station Time
  { .abbr = stringVUT,   .offset = 11 * 60          }, // Vanuatu Time
  { .abbr = stringWAKT,  .offset = 12 * 60          }, // Wake Island Time
  { .abbr = stringWAST,  .offset = 2 * 60           }, // West Africa Summer Time
  { .abbr = stringWAT,   .offset = 1 * 60           }, // West Africa Time
  { .abbr = stringWEST,  .offset = 1 * 60           }, // Western European Summer Time
  { .abbr = stringWET,   .offset = 0                }, // Western European Time
  { .abbr = stringWIB,   .offset = 7 * 60           }, // Western Indonesian Time
  { .abbr = stringWIT,   .offset = 9 * 60           }, // Eastern Indonesian Time
  { .abbr = stringWITA,  .offset = 8 * 60           }, // Central Indonesia Time
  { .abbr = stringWGST,  .offset = -2 * 60          }, // West Greenland Summer Time[14]
  { .abbr = stringWGT,   .offset = -3 * 60          }, // West Greenland Time[15]
  { .abbr = stringWST,   .offset = 8 * 60           }, // Western Standard Time
  { .abbr = stringYAKT,  .offset = 9 * 60           }, // Yakutsk Time
  { .abbr = stringYEKT,  .offset = 5 * 60           }  // Yekaterinburg Time
};

uint8_t RtcLocaleEn::CharsToMonth(const char* monthChars, size_t count) {
  uint8_t month = 0;

  // works for both standard abbreviations and full month names
  //
  if (count >= 3) {
    switch (tolower(monthChars[0])) {
      case 'j':
        if (tolower(monthChars[1]) == 'a') month = 1;
        else if (tolower(monthChars[2]) == 'n') month = 6;
        else month = 7;
        break;
      case 'f': month = 2; break;
      case 'a': month = tolower(monthChars[1]) == 'p' ? 4 : 8; break;
      case 'm': month = tolower(monthChars[2]) == 'r' ? 3 : 5; break;
      case 's': month = 9; break;
      case 'o': month = 10; break;
      case 'n': month = 11; break;
      case 'd': month = 12; break;
    }
  }
  return month;
}

size_t RtcLocaleEn::TimeZoneMinutesFromAbbreviation(int32_t* minutes, const char* abbr) {
  RtcTimeZone entry{};

  *minutes = 0;

  size_t result = RtcTimeZone::BinarySearchProgmemTable(&entry, abbr, c_tzEn, sizeof(c_tzEn)/sizeof(c_tzEn[0]));

  if (result) {
    // found
    *minutes = entry.offset;
  }

  return result;
}
