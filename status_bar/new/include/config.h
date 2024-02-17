#include "components.h"
#include "components/alsa.h"
#include "components/datetime.h"
#include "components/keyboard_indicator.h"
#include "components/keyboard_layout.h"

static const Arg args[] = {
    {keyboard_layout, &(KeyboardLayoutOptions){Country}},
    {keyboard_indicator, " | ?c ?n"},
/*    {alsa,
     &(AlsaOptions){"default", 0, "Master", 1, " | %d%%", " | %d%%(muted)"}},
        {alsa,
     &(AlsaOptions){"default", 0, "Speaker", 1, " %d%%", " %d%%(muted)"}},
*/
    {datetime, &(DatetimeOptions){" | %X %x", 1000}},
};
