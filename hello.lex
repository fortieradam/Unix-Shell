%{#include <stdio.h>
#include "y.tab.h"
%}
%%
[0-9]+		return NUMBER;
hello		return HELLO;
bye			return BYE;
on|off		return STATE;
cd			return CD;
\n 			/* ignore end of line */
%%
