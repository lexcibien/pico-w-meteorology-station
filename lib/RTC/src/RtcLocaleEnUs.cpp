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

#include "RtcLocaleEnUs.h"
#include "RtcTimeZone.h"
#include <cctype>

// due to how works, strings must be declared standalone
//
const char stringADT[] = "ADT";
const char stringAKDT[] = "AKDT";
const char stringAKST[] = "AKST";
const char stringAST[] = "AST";
const char stringCDT[] = "CDT";
const char stringCST[] = "CST";
const char stringEDT[] = "EDT";
const char stringEST[] = "EST";
const char stringGMT[] = "GMT";
const char stringHDT[] = "HDT";
const char stringHST[] = "HST";
const char stringMDT[] = "MDT";
const char stringMST[] = "MST";
const char stringPDT[] = "PDT";
const char stringPST[] = "PST";
const char stringUTC[] = "UTC";

// collection in alphabetic order of abbreviation
//
const RtcTimeZone c_tzEnUs[] = {
  { .abbr=stringADT,  .offset=-3 * 60  }, // Atlantic Daylight Time
  { .abbr=stringAKDT, .offset=-8 * 60  }, // Alaska Daylight Time
  { .abbr=stringAKST, .offset=-9 * 60  }, // Alaska Standard Time
  { .abbr=stringAST,  .offset=-4 * 60  }, // Atlantic Standard Time
  { .abbr=stringCDT,  .offset=-5 * 60  }, // Central Daylight Time
  { .abbr=stringCST,  .offset=-6 * 60  }, // Central Standard Time
  { .abbr=stringEDT,  .offset=-4 * 60  }, // Eastern Daylight Time
  { .abbr=stringEST,  .offset=-5 * 60  }, // Eastern Standard Time
  { .abbr=stringGMT,  .offset=0        }, // Greenwich Mean Time
  { .abbr=stringHDT,  .offset=-9 * 60  }, // Hawaii�Aleutian Daylight Time
  { .abbr=stringHST,  .offset=-10 * 60 }, // Hawaii�Aleutian Standard Time
  { .abbr=stringMDT,  .offset=-6 * 60  }, // Mountain Daylight Time
  { .abbr=stringMST,  .offset=-7 * 60  }, // Mountain Standard Time
  { .abbr=stringPDT,  .offset=-7 * 60  }, // Pacific Daylight Time
  { .abbr=stringPST,  .offset=-8 * 60  }, // Pacific Standard Time
  { .abbr=stringUTC,  .offset=0        }, // Cordinated Universal Time
};

uint8_t RtcLocaleEnUs::CharsToMonth(const char* monthChars, size_t count) {
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

size_t RtcLocaleEnUs::TimeZoneMinutesFromAbbreviation(int32_t* minutes, const char* abbr) {
  RtcTimeZone entry{};

  *minutes = 0;

  size_t result = RtcTimeZone::BinarySearchProgmemTable(&entry, abbr, c_tzEnUs, sizeof(c_tzEnUs) / sizeof(c_tzEnUs[0]));

  if (result) {
    // found
    *minutes = entry.offset;
  }

  return result;
}
